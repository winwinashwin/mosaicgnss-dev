#pragma once

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>

#include "macro_logger.h"

namespace streamers::UDP
{
    template <typename __Tp>
    void stream(const __Tp &data, const uint16_t port)
    {
        int sockfd;
        struct sockaddr_in servaddr, cliaddr;

        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            LOG_ERROR("socket creation failed");
            exit(EXIT_FAILURE);
        }

        memset(&servaddr, 0, sizeof(servaddr));
        memset(&cliaddr, 0, sizeof(cliaddr));

        // Filling server information
        servaddr.sin_family = AF_INET; // IPv4
        servaddr.sin_addr.s_addr = INADDR_ANY;
        servaddr.sin_port = htons(port);

        // Bind the socket with the server address
        if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        {
            LOG_ERROR("bind failed");
            exit(EXIT_FAILURE);
        }

        int len = sizeof(cliaddr);
        LOG_INFO("Server ready on port %d", port);
        LOG_INFO("Streaming data ...");
        for (const auto &pkt : data)
        {
            sendto(sockfd, (const char *)&pkt, sizeof(pkt), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
        }
    }

} // namespace streamers::UDP
namespace streamers::TCP
{
    template <typename __Tp>
    void stream(const __Tp &data, const uint16_t port)
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

        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                       &opt, sizeof(opt)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

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
} // namespace streamers::TCP