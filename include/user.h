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

// Constants
#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 64
#define MAX_EMAIL_LENGTH 100
#define MAX_NAME_LENGTH 100
#define MAX_QUESTION_COUNT 20
#define MAX_QUESTION_LENGTH 100
#define MAX_ANSWER_LENGTH 100
#define MAX_SALT_LENGTH 32
#define MAX_PATH_LENGTH 256
#define MAX_INVALID_LOGIN_COUNT 10
#define SHA256_DIGEST_LENGTH 2566

// User structure
typedef struct {
    char username[MAX_USERNAME_LENGTH];          // User's username
    unsigned char password_hash[SHA256_DIGEST_LENGTH];  // Hashed password
    char salt[MAX_SALT_LENGTH];                  // Salt used in password hashing
    int user_id;                                // Unique identifier for the user
    char email[MAX_EMAIL_LENGTH];               // User's email address
    char full_name[MAX_NAME_LENGTH];            // User's full name
    int account_status;                         // Account status: 0 for active, 1 for suspended, 2 for deleted
    char security_questions[MAX_QUESTION_COUNT][MAX_QUESTION_LENGTH];  // Security questions
    char security_answers[MAX_QUESTION_COUNT][MAX_ANSWER_LENGTH];      // Security question answers
    int needs_key;                              // Flag indicating if the user needs a key
    char key_path[MAX_PATH_LENGTH];             // Path to the user's key
    char home_dir[MAX_PATH_LENGTH];             // User's home directory path
    char shell_path[MAX_PATH_LENGTH];           // Path to the user's shell
    char config_file_path[MAX_PATH_LENGTH];     // Path to the user's configuration file
    int invalid_login_count;                    // Counter for invalid login attempts
} User;

// Function declarations
int get_user_by_id(int user_id, User* result_user);
int remove_user(int user_id);
int update_user(int user_id, User* updated_user);
int validate_credentials(const char* username, const char* password);
void query_user(User* user);
int add_user(User* new_user);
int get_user(const char* username, User* result_user);
User create_user(const char* full_name, const char* username, const char* home_dir,
                 const char* shell_path, int needs_key, const char* email,
                 const char* config_file_path, const char* password);

void print_user_info(const User* user);

#endif