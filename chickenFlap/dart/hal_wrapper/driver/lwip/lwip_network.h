#ifndef DART_HAL_WRAPPER_DRIVER_LWIP_LWIP_NETWORK_H_
#define DART_HAL_WRAPPER_DRIVER_LWIP_LWIP_NETWORK_H_

#include "../../../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"

#ifdef HAL_ENABLE_NETWORK_LWIP
#include "../../../../../chickenFlap/dart/network_def.h"
#include "../../../../../chickenFlap/dart/network.h"
#include "lwip.h"
#include "lwip/udp.h"
#include "lwip/ip4_addr.h"

#ifdef __cplusplus
extern "C" {
#endif

struct hal_udp_socket {
	struct udp_pcb* pcb;
};

typedef struct {
	SocketReceiveCallback callback;
	network_udp_socket_t* socket;
	void* arg;
} lwip_callback_wrapper_t;

network_ipv4_addr_t lwip_convert_to_ipv4_addr(const ip_addr_t* addr);
ip_addr_t lwip_convert_from_ipv4_addr(const network_ipv4_addr_t* addr);

void lwip_udp_receive(void* arg, struct udp_pcb* pcb, struct pbuf* p, const ip_addr_t* addr, uint16_t port);

#ifdef __cplusplus
}
#endif

#endif
#endif
