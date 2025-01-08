#include "mqtt.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include "link.h"
#include <string.h>
#include <json-c/json.h>
#include "database.h"

void mqtt_connect(int fd, const char *packet){
    unsigned char replyData[64] = {0};
    printf("[CONNECT]\n");
    //the second byte of the received message denotes the "rest length"
    //printf("length of rest packet: %d\n", packet[1]); 
    //printf("name of the protocol: %s\n", &packet[4]);
    
    //reply to client
    replyData[0] = CONNACK;
    replyData[1] = 0x02;
    replyData[2] = 0x00;
    replyData[3] = 0x00;

    if(send(fd, replyData, 4, 0) == -1){
        perror("send");
    }
}

void mqtt_subscribe(int fd, const char *packet){
    printf("[SUBSCRIBE]\n");
    char IdenMSB = packet[2];
    char IdenLSB = packet[3];
    char topic[32] = {0};
    char replyData[16] = {0};

    //printf("packet identity MSB: %x LSB: %x\n", IdenMSB, IdenLSB);

    short topicLen = ((short)packet[4] << 8) + packet[5];
    memcpy(topic, &packet[6], topicLen);
    printf("topicLen: %d, topic: %s\n", topicLen, topic);
    replyData[0] = SUBACK;
    replyData[1] = 0x03;
    replyData[2] = IdenMSB;
    replyData[3] = IdenLSB;
    replyData[4] = 0x00;
    if(send(fd, replyData, 5, 0) == -1){
        perror("send");
    }

    //add topic to list
    add_link(fd, topic);
}

void mqtt_publish(int fd, const char *packet){
    printf("[PUBLISH]\n");
    int remainLen = packet[1];
    short topicLen = ((short)packet[2] << 8) + packet[3]; 
    char topic[32] = {0};
    char msg[256] = {0};

    memcpy(topic, &packet[4], topicLen);
    int msgLen = remainLen - 2 - topicLen;
    memcpy(msg, &packet[8], msgLen);
    printf("topic: %s; msg: %s\n", topic, msg);

    send_topic_message(topic, msg, msgLen);
}

void pack_pub_message(int fd, const char *topic, const char *msg, int msgLen){
    char replyData[256] = {0};
    int index = 0;
    int topicLen = strlen(topic);
    replyData[index++] = PUBLISH;
    replyData[index++] = 2 + topicLen + 1 + msgLen;
    replyData[index++] = 0;
    replyData[index++] = topicLen;
    memcpy(&replyData[index], topic, topicLen);
    index += topicLen;
    replyData[index++] = 0x00;
    memcpy(&replyData[index], msg, msgLen);

    if(send(fd, replyData, 5 + topicLen + msgLen, 0) == -1){
        perror("send");
    }
}

void mqtt_register(int fd, const char *packet){
    //convert char to json
    struct json_object *obj = json_tokener_parse(packet + 1);
    if(obj == NULL){
        printf("json_tokener_parse() error\n");
        return;
    }

    //get username and password
    struct json_object *value;
    json_object_object_get_ex(obj, "username", &value);
    const char *username = json_object_get_string(value);
    json_object_object_get_ex(obj, "password", &value);
    const char *password = json_object_get_string(value);

    //check if the username and password alreay exist
    if(sqlite3_user_exist(username)){
        //user exist
        //printf("user already exists\n");
        struct json_object *replyObj = json_object_new_object();
        json_object_object_add(replyObj, "cmd", json_object_new_string("register"));
        json_object_object_add(replyObj, "result", json_object_new_string("exist"));
        const char *sendStr = json_object_to_json_string(replyObj);

        if(send(fd, sendStr, strlen(sendStr), 0) == -1){
            perror("send");
        }
        json_object_put(replyObj);
    }
    else{
        //user does not exist
        //printf("user doesn't exist\n");
        database_insert_user(username, password);

        struct json_object *replyObj = json_object_new_object();
        json_object_object_add(replyObj, "cmd", json_object_new_string("register"));
        json_object_object_add(replyObj, "result", json_object_new_string("success"));
        const char *sendStr = json_object_to_json_string(replyObj);

        if(send(fd, sendStr, strlen(sendStr), 0) == -1){
            perror("send");
        }
        json_object_put(replyObj);
    }
    json_object_put(obj);  //free the memory of obj
}

void mqtt_login(int fd, const char *packet){
    struct json_object *obj = json_tokener_parse(packet + 1);
    if(obj == NULL){
        printf("json_tokener_parse() error\n");
        return;
    }

    //get username and password
    struct json_object *value;
    json_object_object_get_ex(obj, "username", &value);
    const char *username = json_object_get_string(value);
    json_object_object_get_ex(obj, "password", &value);
    const char *password = json_object_get_string(value);


    //check if the username and password alreay exist
    if(sqlite3_user_exist(username)){
        //user exist
        //printf("user already exists\n");
        if(check_password(username, password)){  //password is correct
            struct json_object *replyObj = json_object_new_object();
            json_object_object_add(replyObj, "cmd", json_object_new_string("login"));
            json_object_object_add(replyObj, "result", json_object_new_string("success"));
            const char *sendStr = json_object_to_json_string(replyObj);

            if(send(fd, sendStr, strlen(sendStr), 0) == -1){
                perror("send");
            }
            json_object_put(replyObj);
            return;
        }
        else{   //if password is not correct
        struct json_object *replyObj = json_object_new_object();
        json_object_object_add(replyObj, "cmd", json_object_new_string("login"));
        json_object_object_add(replyObj, "result", json_object_new_string("passworderror"));
        const char *sendStr = json_object_to_json_string(replyObj);

        if(send(fd, sendStr, strlen(sendStr), 0) == -1){
            perror("send");
        }
        json_object_put(replyObj);
        return;
        }
    }
    else{
        //user does not exist
        //printf("user doesn't exist\n");
        struct json_object *replyObj = json_object_new_object();
        json_object_object_add(replyObj, "cmd", json_object_new_string("login"));
        json_object_object_add(replyObj, "result", json_object_new_string("notexist"));
        const char *sendStr = json_object_to_json_string(replyObj);

        if(send(fd, sendStr, strlen(sendStr), 0) == -1){
            perror("send");
        }
        json_object_put(replyObj);
        return;
    }
    json_object_put(obj);
}