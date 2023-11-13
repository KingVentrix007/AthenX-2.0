#include "printf.h"
#include "types.h"
#include "user.h"
#include "auth.h"
#include "password.h"
#include "fat_filelib.h"
#include "string.h"
#include "kheap.h"
//! username:password:UID:GID:GECOS:home_directory:login_shell
//! username:password_hash:salt:additional_fields
int valid_user(const char *username)
{
    // char *data;
    // FILE *fp = fopen("/etc/passwd","r");
    // data = (char *)kmalloc(get_file_size(fp));
    // if(data == NULL)
    // {
    //     printf("Malloc error\n");
    //     fclose(fp);
    //     return -1;
    // }
    // char line[256];
    // while (fgets(line, sizeof(line), fp) != NULL)
    // {
    //     if(compare_username(line, username) == TRUE)
    //     {
    //         return 1;
    //     }
    //     else
    //     {

    //     }
    // }
    // return -2;

}







/**
 * Function Name: compare_username
 * Description: Splits a string at the colon (':') and compares the first part to a variable called 'username'.
 *
 * Parameters:
 *   input_string (const char*) - The input string to be split and compared.
 *   username (const char*) - The username to compare with the first part of the input string.
 *
 * Return:
 *   true if the first part of the input string matches the 'username', otherwise false.
 */
bool compare_username(const char* input_string, const char* username) {
    // Find the position of the colon (':') in the input string
    const char* colon_position = strchr(input_string, ':');
    
    // Check if the colon was found and if the username is the same as the first part
    if (colon_position != NULL) {
        size_t colon_index = colon_position - input_string;
        if (strncmp(input_string, username, colon_index) == 0) {
            return true;
        }
    }
    
    return false;
}



bool authenticatePassword(const char *password) {
    FILE *shadow_file = fopen("/etc/shadow", "r");

    if (shadow_file == NULL) {
        printf("Failed to open /etc/shadow.\n");
        return false;
    }

    char line[256]; // Adjust the buffer size as needed.
    
    while (fgets(line, sizeof(line), shadow_file) != NULL) {
        // Extract the username, stored hash, and stored salt manually.
        char *username = strtok(line, ":");
        char *stored_hash = strtok(NULL, ":");
        char *stored_salt = strtok(NULL, ":");
        
        // Hash the provided password with the stored salt
        uint8_t salted_password[256]; // Adjust the size as needed.
        snprintf(salted_password, sizeof(salted_password), "%s:%s", password, stored_salt);
        uint64_t *hashed_password = SHA256Hash((uint8_t *)salted_password, strlen(salted_password));
        
        // Compare the computed hash with the stored hash
        if (memcmp(hashed_password, stored_hash, sizeof(uint64_t)) == 0) {
            fclose(shadow_file);
            return true; // Password authentication succeeded.
        }
    }

    fclose(shadow_file);
    return false; // Password authentication failed.
}

// Function to add a new user
bool addUser(const char *username, const char *password, int access_level, const char *shell, const char *home) {
    // Generate a random salt (replace with your secure salt generation).
    int salt = 12345; // Example salt value (should be generated securely).

    // Generate the salt string from the salt value (assumed to be of type int).
    char salt_str[20]; // Adjust the size as needed.
    snprintf(salt_str, sizeof(salt_str), "%d", salt);

    // Hash the password with the salt
    uint8_t salted_password[256]; // Adjust the size as needed.
    snprintf(salted_password, sizeof(salted_password), "%s:%s", password, salt_str);
    uint64_t *hashed_password = SHA256Hash((uint8_t *)salted_password, strlen(salted_password));

    // Open /etc/passwd for appending in binary mode
    FILE *passwd_file = fopen("/etc/passwd", "ab");
    FILE *shadow_file = fopen("/sec/shadow", "ab");
 
    if (passwd_file == NULL || shadow_file == NULL) {
        printf("Failed to open system files.\n");
        return false;
    }

    // Convert access_level to a string
    char access_level_str[20]; // Adjust the size as needed.
    snprintf(access_level_str, sizeof(access_level_str), "%d", access_level);

    // Create user entry strings for /etc/passwd and /etc/shadow
    char passwd_entry[256]; // Adjust the buffer size as needed.
    printf("home->%s\n",home);
    snprintf(passwd_entry, sizeof(passwd_entry), "%s:%s:%s:%s:%s:%s:%s\n", username, password, access_level_str, access_level_str, "GECOS", home, shell);
    printf("entry->%s\n",passwd_entry);
    char shadow_entry[256]; // Adjust the buffer size as needed.
    snprintf(shadow_entry, sizeof(shadow_entry), "%s:%s:%s:additional_fields\n", username, hashed_password, salt_str);

    // Write the user's information to /etc/passwd and /etc/shadow
    fwrite(passwd_entry, 1, strlen(passwd_entry), passwd_file);
    fwrite(shadow_entry, 1, strlen(shadow_entry), shadow_file);

    fclose(passwd_file);
    fclose(shadow_file);

    free(hashed_password); // Free the memory allocated for the password hash.
    if(fl_is_dir(home) != 1)
    {
        printf("home->%s",home);
        fl_createdirectory(home);
    }
    return true; // User addition succeeded.
}


bool validateUserCredentials(const char *username, const char *password) {
    // Open /etc/shadow for reading
    FILE *shadow_file = fopen("/etc/shadow", "r");

    if (shadow_file == NULL) {
        printf("Failed to open /etc/shadow.\n");
        return false;
    }

    char line[400]; // Adjust the buffer size as needed.
    
    while (fgets(line, sizeof(line), shadow_file) != NULL) {
        // Extract the username, stored hash, and stored salt manually.
        char *stored_username = strtok(line, ":");
        char *stored_hash = strtok(NULL, ":");
        char *stored_salt = strtok(NULL, ":");
        
        if (strcmp(username, stored_username) == 0) {
            // Hash the provided password with the stored salt
            uint8_t salted_password[256]; // Adjust the size as needed.
            snprintf(salted_password, sizeof(salted_password), "%s:%s", password, stored_salt);
            uint64_t *hashed_password = SHA256Hash((uint8_t *)salted_password, strlen(salted_password));

            // Compare the computed hash with the stored hash
            if (strcmp(hashed_password, stored_hash) == 0) {
                // printf("Hashedpassword: %s\n",hashed_password);
                // printf("stored hass   : %s\n",stored_hash);
                fclose(shadow_file);
                free(hashed_password);
                return true; // User credentials are valid.
            }
            
            free(hashed_password);
        }
    }

    fclose(shadow_file);
    return false; // User credentials are not valid.
}


bool getUserInfo(const char *username, UserInfo *user) {
    // Open /etc/passwd for reading
     // Open /etc/passwd for reading
    FILE *passwd_file = fopen("/etc/passwd", "rb");
       size_t fs_s = get_file_size(passwd_file);
    char *buf = (char *)kmalloc(fs_s);
    fl_fread(buf,sizeof(char),fs_s,passwd_file);

    printf("buf = %s",buf);
    printf("\t\n");
    kfree(buf);
    if (passwd_file == NULL) {
        printf("Failed to open /etc/passwd.\n");
        return false;
    }

    char line[256]; // Adjust the buffer size as needed.
    
    while (fgets(line, sizeof(line), passwd_file) != NULL) {
        printf("LINE: %s\n", line);
        // Extract the username and other information manually.
        char *stored_username = strtok(line, ":");
        // You can extract other fields here as needed.
        
        if (strcmp(username, stored_username) == 0) {
            
            // Parse and store user information in the struct
            strncpy(user->username, stored_username, sizeof(user->username));
            user->access_level = atoi(strtok(NULL, ":"));
            strncpy(user->gecos, strtok(NULL, ":"), sizeof(user->gecos));
            strncpy(user->home_directory, strtok(NULL, ":"), sizeof(user->home_directory));
            strncpy(user->login_shell, strtok(NULL, ":"), sizeof(user->login_shell));
            printf("\npath (%s)\n",user->home_directory);
            fclose(passwd_file);
            return true; // User information found.
        }
    }
    
    fclose(passwd_file);
    return false; // User information not found.
}