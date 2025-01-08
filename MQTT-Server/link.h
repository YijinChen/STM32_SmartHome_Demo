#ifndef LINK_H
#define LINK_H

typedef struct TopicLink{
    int fd;
    char topic[16];
    struct TopicLink *next;
}TopicLink;

void init_link();
void add_link(int fd, const char *topic);
void send_topic_message(const char *topic, const char *msg, int msgLen);
void delete_link(int fd);
#endif