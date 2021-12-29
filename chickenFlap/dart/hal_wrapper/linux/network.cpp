#include "../../../../chickenFlap/dart/hal_wrapper/linux/network.h"

#ifdef USE_LINUX

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct hal_udp_socket {
    int fd;
};

#define LINUX_MAX_NUMBER_OF_UDP_SOCKETS 8

network_udp_socket_t* linux_udp_sockets[LINUX_MAX_NUMBER_OF_UDP_SOCKETS];
SocketReceiveCallback linux_receive_callbacks[LINUX_MAX_NUMBER_OF_UDP_SOCKETS];
void* linux_receive_callback_args[LINUX_MAX_NUMBER_OF_UDP_SOCKETS];
char linux_network_buffer[4096];

void hal_network_init() {

}

void hal_network_update() {
    for (int i = 0; i < LINUX_MAX_NUMBER_OF_UDP_SOCKETS; i++) {
        network_udp_socket_t* socket = linux_udp_sockets[i];
        SocketReceiveCallback callback = linux_receive_callbacks[i];
        void* arg = linux_receive_callback_args[i];
        if (socket == NULL || callback == NULL)
            continue;

        // See if we receive any data on the socket
        struct sockaddr_in from;
        socklen_t fromLen = sizeof(from);
        int received = recvfrom(((hal_udp_socket*)socket->hal)->fd, linux_network_buffer, sizeof(linux_network_buffer), 0, (sockaddr*)&from, &fromLen);
        if (received <= 0)
            continue;

        // Converter IP
        network_ipv4_addr_t address;
        address.a0 = (ntohl(from.sin_addr.s_addr) >> 24) & 0xFF;
        address.a1 = (ntohl(from.sin_addr.s_addr) >> 16) & 0xFF;
        address.a2 = (ntohl(from.sin_addr.s_addr) >> 8) & 0xFF;
        address.a3 = ntohl(from.sin_addr.s_addr) & 0xFF;

        callback(socket, (uint8_t*)linux_network_buffer, received, &address, ntohs(from.sin_port), arg);
    }
}

void hal_network_setStatic(const char* ip) {
	// dummy
}

network_udp_socket_t* hal_network_create_udp_socket(uint16_t port) {
    network_udp_socket_t* s = (network_udp_socket_t*)(malloc(sizeof(network_udp_socket_t)));
    hal_udp_socket* hal_socket = (hal_udp_socket*)malloc(sizeof(hal_udp_socket));
    s->hal = hal_socket;

    // Create socket
    hal_socket->fd = socket(AF_INET, SOCK_DGRAM, 0);
    DART_ASSERT_RETURN(hal_socket->fd >= 0, DART_ERROR_HAL, NULL);

    // Bind socket
    if (port > 0) {
        struct sockaddr_in address;
        memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = htonl(INADDR_ANY);
        address.sin_port = htons(port);
        DART_ASSERT_RETURN(bind(hal_socket->fd, (const sockaddr *) &address, sizeof(address)) == 0, DART_ERROR_HAL,
                           NULL);
    }

    // Set read timeout
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 10000;
    DART_ASSERT_RETURN(setsockopt(hal_socket->fd , SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) == 0, DART_ERROR_HAL, NULL);

    // Set non blocking
    int flags = fcntl(hal_socket->fd , F_GETFL);
    DART_ASSERT_RETURN(fcntl(hal_socket->fd , F_SETFL, flags | O_NONBLOCK) == 0, DART_ERROR_HAL, NULL);

    for (int i = 0; i < LINUX_MAX_NUMBER_OF_UDP_SOCKETS; i++) {
        if (linux_udp_sockets[i] == NULL) {
            linux_udp_sockets[i] = s;
            break;
        }
    }
    return s;
}

network_udp_socket_t* hal_network_create_udp_client() {
    return hal_network_create_udp_socket(0);
}

void hal_network_destroy_udp_socket(network_udp_socket_t* socket) {
    if (socket != NULL)
        delete (hal_udp_socket*)socket->hal;
	delete socket;
}

void hal_network_udp_receive(network_udp_socket_t* socket, SocketReceiveCallback callback, void* arg) {
    for (int i = 0; i < LINUX_MAX_NUMBER_OF_UDP_SOCKETS; i++) {
        if (linux_udp_sockets[i] == socket) {
            linux_receive_callbacks[i] = callback;
            linux_receive_callback_args[i] = arg;
        }
    }
}

void hal_network_udp_send(network_udp_socket_t* socket, const network_ipv4_addr_t* addr, uint16_t port, const uint8_t* data, size_t dataSize) {
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(addr->a0 << 24 | addr->a1 << 16 | addr->a2 << 8 | addr->a3);
    address.sin_port = htons(port);

    DART_ASSERT(sendto(((hal_udp_socket*)socket->hal)->fd, data, dataSize, 0, (struct sockaddr*) &address, sizeof(address)) != -1, DART_ERROR_HAL);
}
#endif
