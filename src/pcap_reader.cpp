#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <net/ethernet.h>
#include <chrono>
#include <thread>

#include "pcap_reader.hpp"
#include "macrologger.hpp"

using namespace pcapReader;

PcapDevice::PcapDevice(buffer_t &buffer) : m_dataBuff{buffer}
{
}

PcapDevice::~PcapDevice()
{
    disconnect();
}

bool PcapDevice::connect(const char *device)
{
    if (isConnected())
        return true;

    if ((m_device = pcap_open_offline(device, m_errBuff)) == nullptr)
        return false;

    m_deviceName = (char *)device;
    pcap_compile(m_device, &m_pktFilter, "tcp dst port 3001", 1, PCAP_NETMASK_UNKNOWN);

    LOG_INFO("Connected to %s", m_deviceName);
    return true;
}

void PcapDevice::disconnect()
{
    if (isConnected())
    {
        pcap_close(m_device);
        m_device = nullptr;
        LOG_INFO("Disconnected from %s", m_deviceName);
    }
}

bool PcapDevice::isConnected() const
{
    return m_device;
}

ReadResult PcapDevice::read()
{
    if (!isConnected())
        return READ_ERROR;

    struct pcap_pkthdr *header;
    const u_char *pktData;
    int result;

    result = pcap_next_ex(m_device, &header, &pktData);

    if (result >= 0)
    {
        auto ipHdr = reinterpret_cast<const iphdr *>(pktData + sizeof(struct ethhdr));
        uint32_t ipHdrLen = ipHdr->ihl * 4u;

        switch (ipHdr->protocol)
        {
        case 6:
        {
            if (header->len == 54)
            {
                return READ_SUCCESS;
            }
            bool storePkt = true;

            if (!m_lastPkt.empty())
            {
                auto tcpHdr = reinterpret_cast<const tcphdr *>(pktData + ipHdrLen + sizeof(struct ethhdr));

                auto lastIpHdr = reinterpret_cast<const iphdr *>(&(m_lastPkt[0]));
                uint32_t lastIpHdrLen = lastIpHdr->ihl * 4u;
                auto lastTcpHdr = reinterpret_cast<const tcphdr *>(&(m_lastPkt[0]) + lastIpHdrLen);
                uint16_t lastLen = ntohs(static_cast<uint16_t>(lastIpHdr->tot_len));
                uint16_t newLen = ntohs(static_cast<uint16_t>(ipHdr->tot_len));
                uint32_t lastSeq = ntohl(lastTcpHdr->seq);
                uint32_t newSeq = ntohl(tcpHdr->seq);

                if (newSeq != lastSeq)
                {
                    uint32_t dataOffset = lastTcpHdr->doff * 4;
                    m_dataBuff.insert(m_dataBuff.end(), m_lastPkt.begin() + lastIpHdrLen + dataOffset,
                                      m_lastPkt.end());
                }
                else if (newLen <= lastLen)
                {
                    storePkt = false;
                }
            }

            if (storePkt)
            {
                m_lastPkt.clear();
                m_lastPkt.insert(m_lastPkt.end(), pktData + sizeof(struct ethhdr), pktData + header->len);
            }
            break;
        }

        default:
        {
            LOG_ERROR("Skipping protocol: %d", result);
            return READ_ERROR;
        }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        return READ_SUCCESS;
    }
    else if (result == -2)
    {
        LOG_INFO("Done reading from %s", m_deviceName);
        if (!m_lastPkt.empty())
        {
            auto lastIpHdr = reinterpret_cast<const iphdr *>(&(m_lastPkt[0]));
            uint32_t ipHdrLength = lastIpHdr->ihl * 4u;

            auto lastTcpHdr = reinterpret_cast<const tcphdr *>(&(m_lastPkt[0]) + ipHdrLength);
            uint32_t dataOffset = lastTcpHdr->doff * 4u;

            m_dataBuff.insert(m_dataBuff.end(),
                              m_lastPkt.begin() + ipHdrLength + dataOffset,
                              m_lastPkt.end());

            m_lastPkt.clear();
        }
        disconnect();
        return READ_SUCCESS;
    }
    else
    {
        LOG_ERROR("Error reading data");
        return READ_ERROR;
    }
}