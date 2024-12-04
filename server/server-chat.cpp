#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

int main() {
	WSADATA wsaData;
	SOCKET server_fd;
	struct sockaddr_in server_address;
	int address_len = sizeof(server_address);

	// Initialize Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cerr << "WSAStartup failed\n";
		return -1;
	}

	// Create Socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == INVALID_SOCKET) {
		cerr << "Socket creation failed: " << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}

	// Set up the address structure
	server_address.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &(server_address.sin_addr));
	server_address.sin_port = (htons(12345));

	if (bind(server_fd, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
		cerr << "Bind failed: " << WSAGetLastError() << endl;
		closesocket(server_fd);
		WSACleanup();
		return -1;
	}

	if (listen(server_fd, SOMAXCONN) == SOCKET_ERROR) {
		cerr << "Listen failed: " << WSAGetLastError() << endl;
		closesocket(server_fd);
		WSACleanup();
		return -1;
	}

	cout << "Server is running on 127.0.0.1:12345. Waiting for connections...\n";

	
	SOCKET new_socket = accept(server_fd, (struct sockaddr*)&server_address, &address_len);
	if (new_socket == INVALID_SOCKET) {
		cerr << "Accept failed: " << WSAGetLastError() << endl;
		closesocket(server_fd);
		WSACleanup();
		return -1;
	}

	cout << "Client connected!\n";

	char buffer[1024];

	// Message exchange loop
	while (true) {
		memset(buffer, 0, sizeof(buffer)); // Clear the buffer

		// Receive message from client 
		int bytes_received = recv(new_socket, buffer, sizeof(buffer) - 1, 0);
		if (bytes_received == SOCKET_ERROR) {
			cerr << "Receive failed: " << WSAGetLastError() << '\n';
			break;
		}
		if (bytes_received == 0) {
			cout << "Client disconnected.\n";
			break;
		}

		cout << "Client: " << buffer << endl;

		// Send a response back to the client
		string response;
		cout << "Enter message to send: \n";
		getline(cin, response);
		send(new_socket, response.c_str(), response.size(), 0);
	}

	// Close sockets and clean up
	closesocket(server_fd);
	WSACleanup();
	return 0;
}