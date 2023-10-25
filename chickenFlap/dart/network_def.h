#ifndef DART_NETWORK_DEF_H_
#define DART_NETWORK_DEF_H_

#include "../../chickenFlap/dart/standard_include.h"

typedef struct {
	void* hal;
} network_udp_socket_t;

/**
 * Structure for a IPv4 address. Format is "a0.a1.a2.a3".
 */
typedef struct {
	uint8_t a0;
	uint8_t a1;
	uint8_t a2;
	uint8_t a3;
} network_ipv4_addr_t;

/**
 * Defines the callback that is called when a button was pressed.
 */
typedef void (*SocketReceiveCallback)(network_udp_socket_t* socket, const uint8_t* data, size_t dataSize, const network_ipv4_addr_t* addr, uint16_t port, void* arg);

#endif
