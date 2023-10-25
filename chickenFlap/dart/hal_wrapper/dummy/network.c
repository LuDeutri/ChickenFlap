#include "../../../../chickenFlap/dart/hal_wrapper/dummy/network.h"

#ifdef HAL_DUMMY_NETWORK

void hal_network_init() {

}

void hal_network_update() {

}

network_udp_socket_t* hal_network_create_udp_socket(uint16_t port) {
	return NULL;
}

void hal_network_destroy_udp_socket(network_udp_socket_t* socket) {

}

void hal_network_udp_receive(network_udp_socket_t* socket, SocketReceiveCallback callback, void* arg) {

}

void hal_network_udp_send(network_udp_socket_t* socket, const network_ipv4_addr_t* addr, uint16_t port, const uint8_t* data, size_t dataSize) {

}
#endif
