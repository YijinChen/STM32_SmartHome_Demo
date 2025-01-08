#ifndef DATABASE_H
#define DATABASE_H

void init_sqlite3();
int sqlite3_user_exist(const char *name);
void database_insert_user(const char *username, const char *password);
int check_password(const char *username, const char *password);


#endif