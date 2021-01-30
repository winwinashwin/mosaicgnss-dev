#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>

#include "pcap_reader.hpp"
#include "macro_logger.h"

void tcpStream(buffer_t data, const uint16_t port)
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    LOG_INFO("Server active on port %d", port);
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    LOG_INFO("Streaming data ...");
    for (const auto &pkt : data)
        send(new_socket, &pkt, sizeof(pkt), 0);
}

int main(int argc, char **argv)
{
    uint16_t port;
    switch (argc)
    {
    case 3:
    {
        if (sscanf(argv[2], "%hx", &port) != 1)
        {
            LOG_ERROR("Cannot identify port.");
            return EXIT_FAILURE;
        };
        break;
    }
    default:
    {
        printf("Usage: %s path-to-pcap-file port\n", argv[0]);
        return EXIT_FAILURE;
    }
    }

    buffer_t data;
    pcapReader::PcapDevice device(data);
    device.connect(argv[1]);
    LOG_INFO("Reading ...");
    while (device.isConnected() && device.read() == pcapReader::READ_SUCCESS)
        ;
    device.disconnect();

    tcpStream(data, 9999);
}