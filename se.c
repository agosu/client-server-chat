#include <winsock2.h>
#include <stdio.h>

int main(int argc, char **argv) {
     WSADATA wsaData;
     SOCKET ListeningSocket;
     SOCKADDR_IN ServerAddr, SenderInfo;
     char recvbuff[1024];
     char sendbuf[1024];

     // Initialize Winsock version 2.2
     if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        printf("Server: WSAStartup failed with error %ld.\n", WSAGetLastError());
        return 1;
     }

     if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
          printf("Server: The dll do not support the Winsock version %u.%u!\n", LOBYTE(wsaData.wVersion),HIBYTE(wsaData.wVersion));
          WSACleanup();
          return 1;
     }

     ListeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
     if (ListeningSocket == INVALID_SOCKET) {
          printf("Server: Error at socket(), error code: %ld.\n", WSAGetLastError());
          WSACleanup();
          return 1;
     }

     ServerAddr.sin_family = AF_INET;
     ServerAddr.sin_port = htons(7170);
     ServerAddr.sin_addr.s_addr = INADDR_ANY;

     if (bind(ListeningSocket, (SOCKADDR *)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR) {
          printf("Server: bind() failed! Error code: %ld.\n", WSAGetLastError());
          closesocket(ListeningSocket);
          WSACleanup();
          return 1;
     }

     if (listen(ListeningSocket, 5) == SOCKET_ERROR) {
          printf("Server: listen(): Error listening on socket %ld.\n", WSAGetLastError());
          closesocket(ListeningSocket);
          WSACleanup();
          return 1;
     } else {
        printf("Server: listen() is OK, I'm listening for connections...\n");
    }

     ListeningSocket = accept(ListeningSocket, NULL, NULL);
     if (ListeningSocket != SOCKET_ERROR) {
         printf("\nServer: accept() is OK...\n");
         printf("Server: Client got connected, ready to receive and send data...\n");
         int BytesReceived = recv(ListeningSocket, recvbuff, sizeof(recvbuff), 0);
         if (BytesReceived > 0) {
              printf("Server: recv() looks fine....\n");
              printf("Server: Bytes received: %d\n", BytesReceived);
              printf("Server: Those bytes are: \n");
              for(int i=0; i < BytesReceived; i++) {
                printf("%c", recvbuff[i]);
              }
              printf("\n");
         } else if (BytesReceived == 0) {
              printf("Server: Connection closed!\n");
         } else {
              printf("Server: recv() failed with error code: %d\n", WSAGetLastError());
         }
    } else {
        printf("Error while accepting connection.\n");
    }

    scanf("%s", sendbuf);

    int bytes_sent = send(ListeningSocket, sendbuf, strlen(sendbuf), 0);

     if (closesocket(ListeningSocket) != 0) {
          printf("Server: Cannot close socket. Error code: %ld\n", WSAGetLastError());
     } else {
          printf("Server: Closing socket...\n");
     }

     if (WSACleanup() != 0) {
          printf("Server: WSACleanup() failed! Error code: %ld\n", WSAGetLastError());
     }
     return 0;
}