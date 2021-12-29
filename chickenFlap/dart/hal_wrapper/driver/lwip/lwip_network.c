#include "../../../../../chickenFlap/dart/hal_wrapper/driver/lwip/lwip_network.h"

#ifdef HAL_ENABLE_NETWORK_LWIP

void hal_network_init() {

}

void hal_network_update() {
#ifndef HAL_ENABLE_FREERTOS
	MX_LWIP_Process();
#endif
}

void hal_network_setStatic(const char* ip) {
	// dummy
	DART_NOT_USED(ip);
}

network_udp_socket_t* hal_network_create_udp_socket(uint16_t port) {
	network_udp_socket_t* socket = (network_udp_socket_t*)mem_malloc(sizeof(network_udp_socket_t));

	// Ensure malloc worked
	DART_NOT_NULL_RETURN(socket, DART_ERROR_INTERNAL, NULL);

	// Assign HAL object
	socket->hal = udp_new();
	DART_NOT_NULL_RETURN(socket->hal, DART_ERROR_INTERNAL, NULL);

	// Bind UDP to port
	udp_bind((struct udp_pcb*)socket->hal, IP_ADDR_ANY, port);

	return socket;
}

network_udp_socket_t* hal_network_create_udp_client() {
	network_udp_socket_t* socket = (network_udp_socket_t*)mem_malloc(sizeof(network_udp_socket_t));

	// Ensure malloc worked
	DART_NOT_NULL_RETURN(socket, DART_ERROR_INTERNAL, NULL);

	// Assign HAL object
	socket->hal = udp_new();
	DART_NOT_NULL_RETURN(socket->hal, DART_ERROR_INTERNAL, NULL);

	return socket;
}


void hal_network_destroy_udp_socket(network_udp_socket_t* socket) {
	mem_free(socket);
}

void hal_network_udp_receive(network_udp_socket_t* socket, SocketReceiveCallback callback, void* arg) {
	DART_NOT_NULL(socket, DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(callback, DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(socket->hal, DART_ERROR_INVALID_VALUE);

	// Create the callback wrapper structure
	lwip_callback_wrapper_t* wrapper = (lwip_callback_wrapper_t*)mem_malloc(sizeof(lwip_callback_wrapper_t));
	DART_NOT_NULL(wrapper, DART_ERROR_INTERNAL);

	wrapper->socket = socket;
	wrapper->callback = callback;
	wrapper->arg = arg;

	udp_recv(socket->hal, lwip_udp_receive, (void*)wrapper);
}

void hal_network_udp_send(network_udp_socket_t* socket, const network_ipv4_addr_t* addr, uint16_t port, const uint8_t* data, size_t dataSize) {
	DART_NOT_NULL(socket, DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(addr, DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(data, DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(socket->hal, DART_ERROR_INVALID_VALUE);

	struct pbuf* buf = pbuf_alloc(PBUF_TRANSPORT, dataSize, PBUF_RAM);
	DART_NOT_NULL(buf, DART_ERROR_INTERNAL);

	memcpy(buf->payload, data, dataSize);

	ip_addr_t networkAddr = lwip_convert_from_ipv4_addr(addr);
	udp_sendto(socket->hal, buf, &networkAddr, port);
	pbuf_free(buf);
}

network_ipv4_addr_t lwip_convert_to_ipv4_addr(const ip_addr_t* addr) {
	network_ipv4_addr_t ret;
	memset(&ret, 0, sizeof(ret));

	DART_NOT_NULL_RETURN(addr, DART_ERROR_INVALID_VALUE, ret);


#if LWIP_IPV6
	ret.a0 = ip4_addr1(&addr->u_addr.ip4);
	ret.a1 = ip4_addr2(&addr->u_addr.ip4);
	ret.a2 = ip4_addr3(&addr->u_addr.ip4);
	ret.a3 = ip4_addr4(&addr->u_addr.ip4);
#else
	ret.a0 = ip4_addr1(addr);
	ret.a1 = ip4_addr2(addr);
	ret.a2 = ip4_addr3(addr);
	ret.a3 = ip4_addr4(addr);
#endif
	return ret;
}

ip_addr_t lwip_convert_from_ipv4_addr(const network_ipv4_addr_t* addr) {
	ip_addr_t ret;
	memset(&ret, 0, sizeof(ret));

	DART_NOT_NULL_RETURN(addr, DART_ERROR_INVALID_VALUE, ret);

#if LWIP_IPV6
	IP4_ADDR(&ret.u_addr.ip4, addr->a0, addr->a1, addr->a2, addr->a3);
#else
	IP4_ADDR(&ret, addr->a0, addr->a1, addr->a2, addr->a3);
#endif
	return ret;
}

void lwip_udp_receive(void* arg, struct udp_pcb* pcb, struct pbuf* p, const ip_addr_t* addr, uint16_t port) {
	if (p == NULL)
		return;

	DART_NOT_NULL(arg, DART_ERROR_INTERNAL);
	DART_NOT_NULL(pcb, DART_ERROR_INTERNAL);
	DART_NOT_NULL(addr, DART_ERROR_INTERNAL);

	// Translate LWIP to our definitions
	lwip_callback_wrapper_t* wrapper = (lwip_callback_wrapper_t*)arg;
	network_ipv4_addr_t networkAddr = lwip_convert_to_ipv4_addr(addr);

	// Call callback saved in the wrapper
	wrapper->callback(wrapper->socket, p->payload, p->len, &networkAddr, port, wrapper->arg);

	pbuf_free(p);
}

#endif
