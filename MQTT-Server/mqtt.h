#ifndef MQTT_H
#define MQTT_H

#define CONNECT    0x10  //the header of packet for CONNECT 
#define CONNACK    0x20
#define PUBLISH    0x30
#define PUBACK     0x40
#define SUBSCRIBE  0x80
#define SUBACK     0x90

#define REGISTER   0x50
#define LOGIN      0x60

void mqtt_connect(int fd, const char *packet);
void mqtt_subscribe(int fd, const char *packet);
void mqtt_publish(int fd, const char *packet);
void pack_pub_message(int fd, const char *topic, const char *msg, int msgLen);
void mqtt_register(int fd, const char *packet);
void mqtt_login(int fd, const char *packet);


#endif