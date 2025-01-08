#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>

#define PORT 1884 // Replace with the server port if it's different
#define IP "18.185.92.160"  // Replace with the server IP if it's different
void *recv_handle(void *arg){
    char msg[128] = {0};
    int fd = *(int *)arg;
    while(1){
        memset(msg, 0, 128);
        if(recv(fd, msg, 128, 0) == 0){
            break;
        }
        printf("receive message from server: %s\n", msg);
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_info;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(1);
    }

    // Configure server address information
    memset(&server_info, 0, sizeof(server_info));
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(PORT);

    // Convert and set the server IP address
    if (inet_pton(AF_INET, IP, &server_info.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        exit(2);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_info, sizeof(server_info)) == -1) {
        perror("connect");
        close(sockfd);
        exit(3);
    }

    printf("Connected to the server successfully\n");

    pthread_t tid;
    pthread_create(&tid, NULL, recv_handle, &sockfd);

    //char msg[1024] = {0};
    // const char *json = "{\"username\":\"0001\", \"password\":\"11111\"}";
    // msg[0] = 0x60;
    // memcpy(msg+1, json, strlen(json));
    // send(sockfd, msg, strlen(json) + 1, 0);

    // char msg[1024] = {0};     
    // const char *json = "{\"username\":\"0001\", \"password\":\"11112\"}";
    // msg[0] = 0x60;
    // memcpy(msg+1, json, strlen(json));
    // send(sockfd, msg, strlen(json) + 1, 0);

    char msg[1024] = {0};
    const char *json = "{\"username\":\"0003\", \"password\":\"1111\"}";
    msg[0] = 0x60;
    memcpy(msg+1, json, strlen(json));
    send(sockfd, msg, strlen(json) + 1, 0);

    while(1);
    // Close the connection
    //close(sockfd);
    return 0;
}
