#include "../../chickenFlap/dart/network.h"

void dartNetwork_init() {
	hal_network_init();
}

void dartNetwork_update() {
	hal_network_update();
}

void dartNetwork_setStatic(const char* ip) {
	DART_NOT_NULL(ip, DART_ERROR_INVALID_VALUE);
}

network_udp_socket_t* dartNetwork_create_udp_socket(uint16_t port) {
	DART_ASSERT_RETURN(port >= 1, DART_ERROR_INVALID_VALUE, NULL);
	return hal_network_create_udp_socket(port);
}

network_udp_socket_t* dartNetwork_create_udp_client() {
    return hal_network_create_udp_client();
}

void dartNetwork_destroy_udp_socket(network_udp_socket_t* socket) {
	if (socket != NULL)
		hal_network_destroy_udp_socket(socket);
}

void dartNetwork_udp_receive(network_udp_socket_t* socket, SocketReceiveCallback callback, void* arg) {
	DART_NOT_NULL(socket, DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(callback, DART_ERROR_INVALID_VALUE);
	hal_network_udp_receive(socket, callback, arg);
}

void dartNetwork_udp_send(network_udp_socket_t* socket, const network_ipv4_addr_t* addr, uint16_t port, const uint8_t* data, size_t dataSize) {
	DART_NOT_NULL(socket, DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(addr, DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(data, DART_ERROR_INVALID_VALUE);
	hal_network_udp_send(socket, addr, port, data, dataSize);
}

bool dartNetwork_ipv4_addr_equal(const network_ipv4_addr_t* a, const network_ipv4_addr_t* b) {
	if (a == b)
		return true;
	if (a == NULL || b == NULL)
		return false;
	return a->a0 == b->a0 && a->a1 == b->a1 && a->a2 == b->a2 && a->a3 == b->a3;
}
