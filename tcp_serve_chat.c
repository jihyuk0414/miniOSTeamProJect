#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <stdlib.h>  // 추가된 헤더 파일

#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)

pthread_mutex_t socket_mutex = PTHREAD_MUTEX_INITIALIZER;
fd_set master;
SOCKET max_socket;
SOCKET socket_listen;  // 전역 변수로 이동

void *handle_client(void *arg) {
    SOCKET client_socket = *(SOCKET *)arg;
    free(arg);
    char read[1024];
    while (1) {
        int bytes_received = recv(client_socket, read, 1024, 0);
        if (bytes_received < 1) {
            printf("완료\n");
            FD_CLR(client_socket, &master);
            CLOSESOCKET(client_socket);

            return NULL;
        }
        // sleep(8);

        for (SOCKET j = 0; j <= max_socket; ++j) {

            if (FD_ISSET(j, &master)) {
                if (j != client_socket && j != socket_listen) {
                    printf("전송완료\n");
                    pthread_mutex_lock(&socket_mutex);
                    send(j, read, bytes_received, 0);
                    pthread_mutex_unlock(&socket_mutex);
                }
            }
        }

    }

    return NULL;
}

int main() {

    printf("Configuring local address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_address;
    getaddrinfo(0, "8080", &hints, &bind_address);

    printf("Creating socket...\n");
    socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
    if (!ISVALIDSOCKET(socket_listen)) {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    printf("Binding socket to local address...\n");
    if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)) {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    freeaddrinfo(bind_address);

    printf("Listening...\n");
    if (listen(socket_listen, 10) < 0) {
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    FD_ZERO(&master);
    FD_SET(socket_listen, &master);
    max_socket = socket_listen;

    printf("Waiting for connections...\n");

    while (1) {
        fd_set reads;
        reads = master;
        if (select(max_socket + 1, &reads, 0, 0, 0) < 0) {
            fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
            return 1;
        }

        for (SOCKET i = 0; i <= max_socket; ++i) {
            if (FD_ISSET(i, &reads)) {
                if (i == socket_listen) {
                    struct sockaddr_storage client_address;
                    socklen_t client_len = sizeof(client_address);
                    SOCKET socket_client = accept(socket_listen, (struct sockaddr *)&client_address, &client_len);
                    if (!ISVALIDSOCKET(socket_client)) {
                        fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
                        return 1;
                    }

                    FD_SET(socket_client, &master);
                    if (socket_client > max_socket)
                        max_socket = socket_client;

                    char address_buffer[100];
                    getnameinfo((struct sockaddr *)&client_address, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
                    printf("New connection from %s\n", address_buffer);

                    SOCKET *pclient = malloc(sizeof(SOCKET));
                    *pclient = socket_client;
                    pthread_t thread_id;


                    pthread_create(&thread_id, NULL, handle_client, pclient);
                    printf("스레드 분기");
                    pthread_detach(thread_id);
                }
            }
        }
    }


    printf("Closing listening socket...\n");
    CLOSESOCKET(socket_listen); 


    printf("Finished.\n");
    return 0;
}
