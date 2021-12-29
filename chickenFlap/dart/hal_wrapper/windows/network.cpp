#include "../../../../chickenFlap/dart/hal_wrapper/windows/network.h"

#ifdef USE_WINDOWS
#include <WinSock2.h>
#include <windows.h>

#define WINDOWS_MAX_NUMBER_OF_UDP_SOCKETS 8

network_udp_socket_t* windows_udp_sockets[WINDOWS_MAX_NUMBER_OF_UDP_SOCKETS];
SocketReceiveCallback windows_receive_callbacks[WINDOWS_MAX_NUMBER_OF_UDP_SOCKETS];
void* windows_receive_callback_args[WINDOWS_MAX_NUMBER_OF_UDP_SOCKETS];
char windows_network_buffer[4096];

struct hal_udp_socket {
	SOCKET socket;
};

network_ipv4_addr_t windows_convert_to_ipv4_addr(const in_addr* addr) {
	network_ipv4_addr_t ret;
	ret.a0 = addr->S_un.S_un_b.s_b1;
	ret.a1 = addr->S_un.S_un_b.s_b2;
	ret.a2 = addr->S_un.S_un_b.s_b3;
	ret.a3 = addr->S_un.S_un_b.s_b4;
	return ret;
}

in_addr windows_convert_from_ipv4_addr(const network_ipv4_addr_t* addr) {
	in_addr ret;
	ret.S_un.S_un_b.s_b1 = addr->a0;
	ret.S_un.S_un_b.s_b2 = addr->a1;
	ret.S_un.S_un_b.s_b3 = addr->a2;
	ret.S_un.S_un_b.s_b4 = addr->a3;
	return ret;
}

void hal_network_init() {
	WSADATA wsaData;
	DART_ASSERT(WSAStartup(MAKEWORD(2, 2), &wsaData) == 0, DART_ERROR_HAL);

	memset(windows_udp_sockets, 0, sizeof(windows_udp_sockets));
}

void hal_network_update() {
	for (int i = 0; i < WINDOWS_MAX_NUMBER_OF_UDP_SOCKETS; i++) {
		network_udp_socket_t* socket = windows_udp_sockets[i];
		SocketReceiveCallback callback = windows_receive_callbacks[i];
		void* arg = windows_receive_callback_args[i];
		if (socket == NULL || callback == NULL)
			continue;

		SOCKADDR_IN from;
		int fromLen = sizeof(from);
		int received = recvfrom((SOCKET)socket->hal, windows_network_buffer, sizeof(windows_network_buffer), 0, (sockaddr*)&from, &fromLen);
		if (received <= 0) {
			int error = WSAGetLastError();
			continue;
		}

		network_ipv4_addr_t addr = windows_convert_to_ipv4_addr(&from.sin_addr);
		callback(socket, (uint8_t*)windows_network_buffer, received, &addr, ntohs(from.sin_port), arg);
	}
}

void hal_network_setStatic(const char* ip) {
	// dummy
}

network_udp_socket_t* hal_network_create_udp_socket(uint16_t port) {
	// Create socket
	SOCKET s;
	s = socket(AF_INET, SOCK_DGRAM, 0);
	DART_ASSERT_RETURN(s != INVALID_SOCKET, DART_ERROR_HAL, NULL);

	// Bind the socket
	if (port > 0) {
        SOCKADDR_IN addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;
        long result = bind(s, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
        DART_ASSERT_RETURN(result != SOCKET_ERROR, DART_ERROR_HAL, NULL);
	}

	// Set read timeout
	DWORD timeout = 1;
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

	// Set non blocking
	u_long nonBlocking = 1;
	ioctlsocket(s, FIONBIO, &nonBlocking);

	// Create wrapper around socket
	network_udp_socket_t* udp_socket = new network_udp_socket_t();
	udp_socket->hal = (void*)s;

	for (int i = 0; i < WINDOWS_MAX_NUMBER_OF_UDP_SOCKETS; i++) {
		if (windows_udp_sockets[i] == NULL) {
			windows_udp_sockets[i] = udp_socket;
			break;
		}
	}
	return udp_socket;
}

network_udp_socket_t* hal_network_create_udp_client() {
    return hal_network_create_udp_socket(0);
}

void hal_network_destroy_udp_socket(network_udp_socket_t* socket) {
	delete socket;
}

void hal_network_udp_receive(network_udp_socket_t* socket, SocketReceiveCallback callback, void* arg) {
	for (int i = 0; i < WINDOWS_MAX_NUMBER_OF_UDP_SOCKETS; i++) {
		if (windows_udp_sockets[i] == socket) {
			windows_receive_callbacks[i] = callback;
			windows_receive_callback_args[i] = arg;
		}
	}
}

void hal_network_udp_send(network_udp_socket_t* socket, const network_ipv4_addr_t* addr, uint16_t port, const uint8_t* data, size_t dataSize) {
	SOCKADDR_IN a;
	a.sin_family = AF_INET;
	a.sin_port = htons(port);
	a.sin_addr = windows_convert_from_ipv4_addr(addr);

	sendto((SOCKET)socket->hal, (const char*)data, dataSize, 0, (SOCKADDR*)&a, sizeof(a));
}
#endif
