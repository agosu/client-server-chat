#include <winsock2.h>
#include <stdio.h>

int main(int argc, char **argv) {
     WSADATA wsaData;
     SOCKET SendingSocket;
     SOCKADDR_IN ServerAddr;
     char sendbuf[1024] = "This is a test string from client";
     char recvbuff[1024];

     // Initialize Winsock version 2.2
     WSAStartup(MAKEWORD(2,2), &wsaData);
     printf("Client: Winsock DLL status is %s.\n", wsaData.szSystemStatus);

     // Create a new socket to make a client connection.
     // AF_INET = 2, The Internet Protocol version 4 (IPv4) address family, TCP protocol
     SendingSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

     if (SendingSocket == INVALID_SOCKET) {
          printf("Client: socket() failed! Error code: %ld\n", WSAGetLastError());
          WSACleanup();
          return -1;
     }

     ServerAddr.sin_family = AF_INET;
     ServerAddr.sin_port = htons(7170);
     ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

     int connection_code = connect(SendingSocket, (SOCKADDR *) &ServerAddr, sizeof(ServerAddr));
     if (connection_code != 0) {
          printf("Client: connect() failed! Error code: %ld\n", WSAGetLastError());
          closesocket(SendingSocket);
          WSACleanup();
          return -1;
     } else {
          printf("Client: connect() is OK, got connected...\n");
          printf("Client: Ready for sending and/or receiving data...\n");
     }

     // Sends some data to server/receiver...
     scanf("%s", sendbuf);
     int bytes_sent = send(SendingSocket, sendbuf, strlen(sendbuf), 0);
     if (bytes_sent == SOCKET_ERROR) {
        printf("Client: send() error %ld.\n", WSAGetLastError());
     } else {
        printf("Client: send() is OK - bytes sent: %ld\n", bytes_sent);
     }

     int BytesReceived = recv(SendingSocket, recvbuff, sizeof(recvbuff), 0);
     for(int i=0; i < BytesReceived; i++) {
                printf("%c", recvbuff[i]);
     }
     printf("\n");
        
     if (shutdown(SendingSocket, SD_SEND) != 0) {
        printf("Client: Well, there is something wrong with the shutdown(). The error code: %ld\n", WSAGetLastError());
     } else {
        printf("Client: shutdown() looks OK...\n");
     }
        
     if(closesocket(SendingSocket) != 0) {
        printf("Client: Cannot close \"SendingSocket\" socket. Error code: %ld\n", WSAGetLastError());
     } else {
        printf("Client: Closing \"SendingSocket\" socket...\n");
    }

     if (WSACleanup() != 0) {
        printf("Client: WSACleanup() failed!...\n");
     }

     return 0;
}