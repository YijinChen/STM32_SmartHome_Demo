#include "link.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mqtt.h"

extern TopicLink *head;

void init_link(){
    head = (TopicLink *)malloc(sizeof(TopicLink));
    if(head == NULL){
        perror("malloc");
        return;
    }

    head->next = NULL;
}

void add_link(int fd, const char *topic){
    TopicLink *p = head;
    while(p->next){
        p = p->next;
    }
    TopicLink *newNode = (TopicLink *)malloc(sizeof(TopicLink));
    if(newNode == NULL){
        perror("malloc");
        return;
    }

    newNode->fd = fd;
    strcpy(newNode->topic, topic);
    newNode->next = NULL;
    p->next = newNode;
}

void send_topic_message(const char *topic, const char *msg, int msgLen){
    TopicLink *p = head->next;
    while(p){
        if(!strcmp(p->topic, topic)){
            pack_pub_message(p->fd, topic, msg, msgLen);
        }
        p = p->next;
    }
}

void delete_link(int fd){
    TopicLink *p = head->next;

    while(p->next){
        if(p->next->fd == fd){
            TopicLink *delNode = p->next;
            p->next = delNode->next;
            free(delNode);
        }
        else{
            p = p->next;
        }
    }
}


