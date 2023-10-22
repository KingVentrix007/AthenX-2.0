#ifndef __SHELL__H
#define __SHELL__H
#define MAX_TOKENS 100
#define MAX_TOKEN_LENGTH 50
int shell(char *buf);
int tokenizeCommand(const char* buffer, char tokens[][MAX_TOKEN_LENGTH]);

#endif