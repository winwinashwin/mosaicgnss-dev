#include "pcap_reader.hpp"
#include "streamers.hpp"
#include "macrologger.hpp"

#include <cstring>

int main(int argc, char **argv)
{
    uint16_t port;
    char *streamType, *file;

    switch (argc)
    {
    case 4:
    {
        streamType = argv[1];
        if (sscanf(argv[2], "%hd", &port) != 1)
        {
            LOG_ERROR("Cannot identify port.");
            return EXIT_FAILURE;
        };
        LOG_DEBUG("Read port: %d", port);
        file = argv[3];
        break;
    }
    default:
    {
        printf("Usage: %s [tcp|udp] port path-to-pcap-file\n", argv[0]);
        return EXIT_FAILURE;
    }
    }

    buffer_t data;
    pcapReader::PcapDevice device(data);
    
    if (!device.connect(file)) {
        LOG_ERROR("Unable to connect to file %s", file);
        return EXIT_FAILURE;
    }

    LOG_INFO("Reading ...");
    while (device.isConnected() && device.read() == pcapReader::READ_SUCCESS)
        ;
    device.disconnect();

    if (strcmp(streamType, "tcp") == 0)
        streamers::TCP::stream(data, port);
    else if (strcmp(streamType, "udp") == 0)
        streamers::UDP::stream(data, port);
    else
    {
        LOG_ERROR("Invalid stream type. Expected tcp or udp");
        return EXIT_FAILURE;
    }
}