#ifndef DART_NETWORK_H_
#define DART_NETWORK_H_

#include "../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"
#include "../../chickenFlap/dart/network_def.h"
#include "../../chickenFlap/dart/util/error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes the network module.
 */
void dartNetwork_init();
/**
 * Updates the network module.
 */
void dartNetwork_update();

/**
 * Sets an static ip address.
 * @param ip
 */
void dartNetwork_setStatic(const char* ip);

/**
 * Creates a new UDP socket listening on the given port.
 */
network_udp_socket_t* dartNetwork_create_udp_socket(uint16_t port);

/**
 * Creates a new UDP socket client.
 */
network_udp_socket_t* dartNetwork_create_udp_client();
/**
 * Destroys a UDP created by dartNetwork_create_udp_socket.
 */
void dartNetwork_destroy_udp_socket(network_udp_socket_t* socket);
/**
 * Sets a receive callback on a UDP socket. The given argument arg is saved and given to the callback on each call.
 */
void dartNetwork_udp_receive(network_udp_socket_t* socket, SocketReceiveCallback callback, void* arg);
/**
 * Sends a UDP packet using the given socket, to the given address and the given port.
 */
void dartNetwork_udp_send(network_udp_socket_t* socket, const network_ipv4_addr_t* addr, uint16_t port, const uint8_t* data, size_t dataSize);

/**
 * Returns true when to IPV4 addresses are equal and false otherwise.
 */
bool dartNetwork_ipv4_addr_equal(const network_ipv4_addr_t* a, const network_ipv4_addr_t* b);

void hal_network_init();
void hal_network_update();
void hal_network_setStatic(const char* ip);
network_udp_socket_t* hal_network_create_udp_socket(uint16_t port);
network_udp_socket_t* hal_network_create_udp_client();
void hal_network_destroy_udp_socket(network_udp_socket_t* socket);
void hal_network_udp_receive(network_udp_socket_t* socket, SocketReceiveCallback callback, void* arg);
void hal_network_udp_send(network_udp_socket_t* socket, const network_ipv4_addr_t* addr, uint16_t port, const uint8_t* data, size_t dataSize);

#ifdef __cplusplus
}
#endif

#endif
