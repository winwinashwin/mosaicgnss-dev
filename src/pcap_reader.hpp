#pragma once

#include <pcap/pcap.h>
#include <vector>

using buffer_t = std::vector<uint8_t>;

namespace pcapReader
{
    enum ReadResult
    {
        /// Data read successfully
        READ_SUCCESS = 0,
        READ_INSUFFICIENT_DATA = 1,
        READ_TIMEOUT = 2,
        READ_INTERRUPTED = 3,
        READ_ERROR = -1,
        /// Unable to parse data, parsing error
        READ_PARSE_FAILED = -2

    };

    class PcapDevice
    {
    private:
        static const size_t BUFFSIZE = 100;

        buffer_t &m_dataBuff;

        pcap_t *m_device{nullptr};
        bpf_program m_pktFilter{};
        char m_errBuff[BUFFSIZE]{};
        char *m_deviceName;
        buffer_t m_lastPkt;

    public:
        PcapDevice(buffer_t &buffer);
        bool connect(const char *device);
        void disconnect();
        bool isConnected() const;

        ReadResult read();

        ~PcapDevice();
    };
} // namespace pcapReader