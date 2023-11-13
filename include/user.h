#ifndef USER_H
#define USER_H
#include "fat_filelib.h"
#include <stdbool.h>

typedef struct  {
    char username[256];
    int access_level;
    char gecos[256];
    char home_directory[256];
    char login_shell[256];
}UserInfo;



int valid_user(const char *username);
bool compare_username(const char* input_string, const char* username);
bool addUser(const char *username, const char *password, int access_level, const char *shell, const char *home);
bool validateUserCredentials(const char *username, const char *password);
bool getUserInfo(const char *username, UserInfo *user);


#endif