#include <sqlite3.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern sqlite3 *ppDb;

void init_sqlite3(){
    //create and open the database called userinfo.db
    //sqlite3 userinfo.db (open the database file)
    if(sqlite3_open("userinfo.db", &ppDb) != SQLITE_OK){ 
        printf("sqlite3_open error %s\n", sqlite3_errmsg(ppDb));
        exit(1);
    }

    char sql[128] = {0};
    //user (name of table), username (text: type of username)
    sprintf(sql, "create table if not exists user (username text, password text)");
    if(sqlite3_exec(ppDb, sql, NULL, NULL, NULL) != SQLITE_OK){
        printf("sqlite3_exec error %s\n", sqlite3_errmsg(ppDb));
        return;
    }
}

//check if the user already exists
int sqlite3_user_exist(const char *name){
    char sql[128] = {0};
    char **result;  //save the check result
    int row, column; //save the row and column of the table
    char *errmsg;
    sprintf(sql, "select username from user;");
    
    if(sqlite3_get_table(ppDb, sql, &result, &row, &column, &errmsg) != SQLITE_OK){
        printf("sqlite3_get_table() error %s\n", errmsg);
        return 1;
    }

    for (int i = 0; i < row; i++){
        if(!strcmp(result[i + column], name)){  //if the name already exist
            return 1;
        }
    }
    sqlite3_free_table(result);  //free the memory of result

    return 0;
}

void database_insert_user(const char *username, const char *password){
    char sql[128] = {0};
    sprintf(sql, "insert into user values ('%s', '%s');", username, password);
    if(sqlite3_exec(ppDb, sql, NULL, NULL, NULL) != SQLITE_OK){
        printf("sqlite3_exec error %s\n", sqlite3_errmsg(ppDb));
        return;
    }
}

int check_password(const char *username, const char *password){
        char sql[128] = {0};
    char **result;  //save the check result
    int row, column, flag = 0; //save the row and column of the table
    char *errmsg;
    sprintf(sql, "select password from user where username = '%s';", username);
    
    if(sqlite3_get_table(ppDb, sql, &result, &row, &column, &errmsg) != SQLITE_OK){
        printf("sqlite3_get_table() error %s\n", errmsg);
        return 1;
    }

    if(!strcmp(result[1], password)){ //password correct
        flag = 1;
    }

    sqlite3_free_table(result);  //free the memory of result

    return flag;
}