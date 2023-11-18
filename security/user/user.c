#include "printf.h"
#include "types.h"
#include "user.h"
#include "auth.h"
#include "password.h"
#include "fat_filelib.h"
#include "string.h"
#include "kheap.h"
#include "timer.h"
// // Constants
// #define MAX_USERNAME_LENGTH 50
// #define MAX_PASSWORD_LENGTH 64  // Adjust as needed
// #define MAX_EMAIL_LENGTH 100
// #define MAX_NAME_LENGTH 100
// #define MAX_QUESTION_COUNT 20
// #define MAX_QUESTION_LENGTH 100
// #define MAX_ANSWER_LENGTH 100
// #define MAX_SALT_LENGTH 32  // Adjust as needed
// #define MAX_PATH_LENGTH 256
// #define MAX_INVALID_LOGIN_COUNT 10
// #define SHA256_DIGEST_LENGTH 2566
// // User structure
// typedef struct {
//     char username[MAX_USERNAME_LENGTH];
//     unsigned char password_hash[SHA256_DIGEST_LENGTH];
//     char salt[MAX_SALT_LENGTH];
//     // Additional information
//     int user_id;
//     char email[MAX_EMAIL_LENGTH];
//     char full_name[MAX_NAME_LENGTH];
//     // Date/time information
//     // struct tm registration_date;
//     // struct tm last_login_time;
//     // Account status: 0 for active, 1 for suspended, 2 for deleted
//     int account_status;
//     // Security questions and answers
//     char security_questions[MAX_QUESTION_COUNT][MAX_QUESTION_LENGTH];
//     char security_answers[MAX_QUESTION_COUNT][MAX_ANSWER_LENGTH];
//     // New fields
//     int needs_key;  // 1 or 0
//     char key_path[MAX_PATH_LENGTH];
//     char home_dir[MAX_PATH_LENGTH];
//     char shell_path[MAX_PATH_LENGTH];
//     char config_file_path[MAX_PATH_LENGTH];
//     // Invalid login counter
//     int invalid_login_count;
// } User;




// Function to get a user based on user_id
int get_user_by_id(int user_id, User* result_user) {
    FILE* user_file;

    // Open the user file in binary mode
    user_file = fopen("/sec/user", "rb");
    if (user_file == NULL) {
        printf("Error opening the user file.\n");
        return 1;
    }

    fseek(user_file, 0, SEEK_SET);

    while (fread(result_user, sizeof(User), 1, user_file) == 1) {
        if (result_user->user_id == user_id) {
            fclose(user_file);
            return 0;  // User found
        }
    }

    // Cleanup: Close the user file
    fclose(user_file);

    return 2;  // User not found
}

// Function to remove a user based on user_id
int remove_user(int user_id) {
    FILE* user_file;
    FILE* temp_file;

    // Open the user file in binary mode
    user_file = fopen("/sec/user", "rb");
    if (user_file == NULL) {
        printf("Error opening the user file.\n");
        return 1;
    }

    // Create a temporary file
    temp_file = fopen("/sec/temp_user", "wb");
    if (temp_file == NULL) {
        printf("Error creating the temporary file.\n");
        fclose(user_file);
        return 2;
    }

    User current_user;

    while (fread(&current_user, sizeof(User), 1, user_file) == 1) {
        // Skip the user to be removed
        if (current_user.user_id == user_id) {
            continue;
        }

        // Write the other users to the temporary file
        fwrite(&current_user, sizeof(User), 1, temp_file);
    }

    // Cleanup: Close the files
    fclose(user_file);
    fclose(temp_file);

    // Remove the original user file
    if (remove("/sec/user") != 0) {
        printf("Error removing the original user file.\n");
        return 3;
    }

    // Rename the temporary file to the original user file
    if (rename("/sec/temp_user", "/sec/user") != 0) {
        printf("Error renaming the temporary file.\n");
        return 4;
    }

    printf("User with ID %d removed successfully.\n", user_id);

    return 0;  // Success
}

// Function to update an existing user based on user_id
int update_user(int user_id, User* updated_user) {
    FILE* user_file;

    // Open the user file in binary mode
    user_file = fopen("/sec/user", "rb+");
    if (user_file == NULL) {
        printf("Error opening the user file.\n");
        return 1;
    }

    fseek(user_file, 0, SEEK_SET);

    User current_user;

    while (fread(&current_user, sizeof(User), 1, user_file) == 1) {
        // Find the user to update
        if (current_user.user_id == user_id) {
            // Update the user with new information
            fseek(user_file, -sizeof(User), SEEK_CUR);
            fwrite(updated_user, sizeof(User), 1, user_file);

            fclose(user_file);

            printf("User with ID %d updated successfully.\n", user_id);

            return 0;  // Success
        }
    }

    // Cleanup: Close the user file
    fclose(user_file);

    printf("User with ID %d not found.\n", user_id);

    return 2;  // User not found
}

// Function to validate user credentials
int validate_credentials(const char* username, const char* password) {
    User check_user;

    // Get the user based on the username
    if (get_user(username, &check_user) != 0) {
        printf("User not found. Couldn't validate user\n");
        return 1;  // User not found
    }

    // Hash the provided password with the stored salt
    unsigned char provided_password_hash[SHA256_DIGEST_LENGTH];
    char salted_password[MAX_PASSWORD_LENGTH + MAX_SALT_LENGTH];

    snprintf(salted_password, sizeof(salted_password), "%s%s", password, check_user.salt);
    printf("here %d",__LINE__);
    strcpy(provided_password_hash,SHA256Hash(salted_password));
    if (provided_password_hash == NULL) {
        printf("Error hashing the provided password.\n");
        return 2;  // Error hashing provided password
    }
    

    // Compare hashed passwords
    if (memcmp(check_user.password_hash, provided_password_hash, SHA256_DIGEST_LENGTH) == 0) {
        printf("Username and password are correct.\n");

        // Reset invalid login counter on successful login
        check_user.invalid_login_count = 0;
        
        // Update last login time
        // time_t current_time = time(NULL);
        // check_user.last_login_time = *localtime(&current_time);

        // Update the user in the file
        FILE* user_file = fopen("/sec/user", "rb+");
        if (user_file == NULL) {
            printf("Error opening the user file.\n");
            return 4;  // Error opening user file
        }

        fseek(user_file, -sizeof(User), SEEK_CUR);
        fwrite(&check_user, sizeof(User), 1, user_file);

        fclose(user_file);
        free(provided_password_hash);
        return 0;  // Username and password are correct
    } else {
        printf("Incorrect password.\n");

        // Increment invalid login counter
        check_user.invalid_login_count++;

        // Update the user in the file
        FILE* user_file = fopen("/sec/user", "rb+");
        if (user_file == NULL) {
            printf("Error opening the user file.\n");
            return 5;  // Error opening user file
        }

        fseek(user_file, -sizeof(User), SEEK_CUR);
        fwrite(&check_user, sizeof(User), 1, user_file);

        fclose(user_file);

        // Check if the invalid login counter exceeds the limit
        if (check_user.invalid_login_count > MAX_INVALID_LOGIN_COUNT) {
            // Call the query_user function to ask security questions
            query_user(&check_user);
        }

        return 3;  // Incorrect password
    }
}

// Function to query user with security questions
void query_user(User* user) {
    printf("Answer security questions to reset the invalid login counter:\n");

    // for (int i = 0; i < MAX_QUESTION_COUNT; ++i) {
    //     char answer[MAX_ANSWER_LENGTH];

    //     printf("Question %d: %s\n", i + 1, user->security_questions[i]);
    //     printf("Your Answer: ");
    //     // f_fgets(answer, MAX_ANSWER_LENGTH, stdin);

    //     // Remove newline character if present
    //     size_t length = strlen(answer);
    //     if (length > 0 && answer[length - 1] == '\n') {
    //         answer[length - 1] = '\0';
    //     }

    //     // Compare the provided answer with the stored answer
    //     if (strcmp(answer, user->security_answers[i]) != 0) {
    //         printf("Incorrect answer. Please try again later.\n");
    //         return;
    //     }
    // }

    // // If all answers are correct, reset the invalid login counter
    // user->invalid_login_count = 0;

    // // Update the user in the file
    // FILE* user_file = fopen("/sec/user", "rb+");
    // if (user_file == NULL) {
    //     printf("Error opening the user file.\n");
    //     return;  // Error opening user file
    // }

    // fseek(user_file, -sizeof(User), SEEK_CUR);
    // fwrite(user, sizeof(User), 1, user_file);

    // fclose(user_file);

    // printf("Security questions answered correctly. Invalid login counter reset.\n");
}
// Function to get a user based on username
int get_user(const char* username, User* result_user) {
    FILE* user_file;

    // Open the user file in binary mode
    user_file = fopen("/sec/user", "rb");
    if (user_file == NULL) {
        printf("Error opening the user file.\n");
        return 1;
    }

    fseek(user_file, 0, SEEK_SET);
    int ret = fl_fread(result_user, sizeof(User), 1, user_file);
    while (ret != 0 && ret != -1) {
        printf("username->%s\n",result_user->username);
        if (strcmp(result_user->username, username) == 0) {
            fclose(user_file);
            return 0;  // User found
        }
        ret = fl_fread(result_user, sizeof(User), 1, user_file);
    }

    // Cleanup: Close the user file
    fclose(user_file);

    return 2;  // User not found
}

// Function to add a new user
int add_user(User* new_user) {
    FILE* user_file;

    // Open the user file in binary mode
    user_file = fopen("/sec/user", "ab+");
    if (user_file == NULL) {
        printf("Error opening the user file.\n");
        return 1;
    }

    // Write the new user to the file
    fseek(user_file, 0, SEEK_END);
    fwrite(new_user, sizeof(User), 1, user_file);

    // Cleanup: Close the user file
    fclose(user_file);

    printf("User '%s' added successfully.\n", new_user->username);
    print_user_info(new_user);
    return 0;  // Success
}




User create_user(const char* full_name, const char* username, const char* home_dir,
                 const char* shell_path, int needs_key, const char* email,
                 const char* config_file_path, const char* password) {
    User new_user;

    // Set user information
    strncpy(new_user.full_name, full_name, MAX_NAME_LENGTH - 1);
    strncpy(new_user.username, username, MAX_USERNAME_LENGTH - 1);
    strncpy(new_user.home_dir, home_dir, MAX_PATH_LENGTH - 1);
    strncpy(new_user.shell_path, shell_path, MAX_PATH_LENGTH - 1);
    new_user.needs_key = needs_key;
    strncpy(new_user.email, email, MAX_EMAIL_LENGTH - 1);
    strncpy(new_user.config_file_path, config_file_path, MAX_PATH_LENGTH - 1);

    // Hash the password and set the salt
    // (Replace SHA256Hash with your actual password hashing implementation)
    const char* hashed_password = SHA256Hash(password);
    strncpy(new_user.salt, "4356", MAX_SALT_LENGTH - 1);  // Replace with actual salt
    strncpy(new_user.password_hash, hashed_password, SHA256_DIGEST_LENGTH);

    // Set other default values
    new_user.user_id = 0;  // You might want to generate a unique user ID
    new_user.account_status = 0;  // Default account status is active
    new_user.invalid_login_count = 0;

    return new_user;
}

void print_user_info(const User* user) {
    printf("User Information:\n");
    printf("Username: %s\n", user->username);
    printf("Full Name: %s\n", user->full_name);
    printf("Home Directory: %s\n", user->home_dir);
    printf("Shell Path: %s\n", user->shell_path);
    printf("Needs Key: %d\n", user->needs_key);
    printf("Email: %s\n", user->email);
    printf("Config File Path: %s\n", user->config_file_path);
    printf("User ID: %d\n", user->user_id);
    printf("Account Status: %d\n", user->account_status);
    printf("Invalid Login Count: %d\n", user->invalid_login_count);

    // You can add more fields as needed

    printf("\n");
}