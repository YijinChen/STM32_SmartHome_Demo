#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "epoll.h"
#include "mqtt.h"
#include "link.h"

int parse_type(unsigned char byte){
    return byte & 0xf0;  //get bit0-4
}
void epoll_listen(int epfd, int sockfd){
    struct epoll_event events[MAX_EPOLL_EVENT] = {0};
    struct epoll_event ev;
    int readyNum = 0, ret;
    struct sockaddr_in client_info;
    int len = sizeof(client_info);
    char message[128] = {0};

    while(1){
        readyNum = epoll_wait(epfd, events, MAX_EPOLL_EVENT, -1); //listen
        if(readyNum == -1){
            perror("epoll_wait");
            continue;
        }
        for(int i = 0; i < readyNum; i++){
            if(events[i].data.fd == sockfd){   //received connection
                int fd = accept(sockfd, (struct sockaddr *)&client_info, &len);
                if(fd == -1){
                    perror("accept");
                    exit(4);
                }

                printf("accept the connection from client %d\n", fd);

                ev.data.fd = fd;
                ev.events = EPOLLIN;

                if(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1){
                    perror("epoll_ctl");
                }
            }
            else{    //if received data
                memset(message, 0, sizeof(message));
                ret = recv(events[i].data.fd, message, sizeof(message), 0);
                if(ret == -1){
                    perror("recv");
                }
                else if(ret == 0){
                    printf("client %d quit\n", events[i].data.fd);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, &events[i]);
                    delete_link(events[i].data.fd);
                }
                else{    //received data
                    //printf received message byte by byte using hexadecimal
                    // for(int i = 0; i < 5; i++){
                    //     printf("%x ", message[i]);
                    // } 
                    // printf("\n");
                    int type = parse_type(message[0]);
                    switch(type){
                        case CONNECT:
                        mqtt_connect(events[i].data.fd, message);
                        break;
                        case PUBLISH:
                        mqtt_publish(events[i].data.fd, message);
                        break;
                        case SUBSCRIBE:
                        mqtt_subscribe(events[i].data.fd, message);
                        break;
                        case REGISTER:
                        mqtt_register(events[i].data.fd, message);
                        break;
                        case LOGIN:
                        mqtt_login(events[i].data.fd, message);
                        break;
                    }

                }
            }
        }
    }
}


void epoll_handle(int sockfd){
    //create epoll object
    int epfd = epoll_create(1);
    if(epfd == -1){
        perror("epoll_create");
        exit(1);
    }

    //add sockfd into epoll
    struct epoll_event ev;  //save sockfd into an epoll event
    ev.data.fd = sockfd;
    ev.events = EPOLLIN;
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
    if(ret == -1){
        perror("epoll_ctl");
        exit(2);
    }
    epoll_listen(epfd, sockfd);
}