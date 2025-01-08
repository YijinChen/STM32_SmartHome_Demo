#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "main.h"
#include "epoll.h"
#include "link.h"
#include <sqlite3.h>
#include "database.h"

TopicLink *head = NULL;
sqlite3 *ppDb = NULL;

int main(){
    int ret;

    //initialize link for saving topic and corresponding fd
    init_link();

    init_sqlite3();

    //create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        perror("socket");
        exit(1);
    }

    //let the socket can be reuse to avoid potential error of quick restart
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    //bind info
    struct sockaddr_in server_info;
    memset(&server_info, 0, sizeof(server_info));
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(PORT);
    server_info.sin_addr.s_addr = inet_addr(IP);

    ret = bind(sockfd, (struct sockaddr *)&server_info, sizeof(server_info));
    if(ret == -1){
        perror("bind");
        exit(2);
    }

    //listen the connection
    ret = listen(sockfd, 10);
    if(ret == -1){
        perror("listen");
        exit(3);
    }

    epoll_handle(sockfd);
    printf("waiting the connection from the client...\n");

    // struct sockaddr_in client_info;
    // int len = sizeof(client_info);
    // int fd = accept(sockfd, (struct sockaddr *)&client_info, &len);
    // if(fd == -1){
    //     perror("accept");
    //     exit(4);
    // }

    //printf("accept the connection from client %d\n", fd);
    return 0;
}