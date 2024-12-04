#include <iostream>
#include <string>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>


using namespace std;
int main() {
	WSADATA wsaData;
	SOCKET client_fd;
	struct sockaddr_in server_address;

	// Intialize Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cerr << "WSAStartup failed\n";
		return -1;
	}

	// Create socket
	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_fd == INVALID_SOCKET) {
		cerr << "Socket creation failed: " << WSAGetLastError() << '\n';
		WSACleanup();
		return -1;
	}

	// Set up the server address structure 
	server_address.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &(server_address.sin_addr));
	server_address.sin_port = htons(12345);

	// Connect to the server 
	if (connect(client_fd, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
		cerr << "Connection failed: " << WSAGetLastError() << endl;
		closesocket(client_fd);
		WSACleanup();
		return -1;
	}

	cout << "Connected to the server\n";

	char buffer[1024];
	while (true) {
		// Send message to server
		string message;
		cout << "Enter message to send: ";
		getline(cin, message); 
		send(client_fd, message.c_str(), message.size(), 0); 

		// Receive response from server
		memset(buffer, 0, sizeof(buffer)); 
		int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
		if (bytes_received == SOCKET_ERROR) {
			cerr << "Receive failed: " << WSAGetLastError() << endl;
			break;
		}
		if (bytes_received == 0) {
			cout << "Server disconnected.\n";
			break;
		}

		cout << "Server: " << buffer << endl; 
	}

	// Close socket and clean up
	closesocket(client_fd);
	WSACleanup();
	return 0;
}