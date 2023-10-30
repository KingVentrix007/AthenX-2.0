#include "stdio.h"
#include "string.h"
#include "search.h"
#include "stdlib.h"
#include "terminal.h"
int search_file(const char *file_path, const char *search_text, int case_sensitive) {
    int match_count = 0;
    int line_number = 1;  // Initialize line number
    FILE *file = fopen(file_path, "r");

    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    char buffer[1024];
    size_t search_text_length = strlen(search_text);

    char *search_text_copy = NULL;
    if (!case_sensitive) {
        search_text_copy = strdup(search_text);
        for (int i = 0; search_text_copy[i]; i++) {
            search_text_copy[i] = tolower(search_text_copy[i]);
        }
    }

    while (fgets(buffer, sizeof(buffer), file)) {
        char *pos = buffer;

        if (!case_sensitive) {
            for (int i = 0; buffer[i]; i++) {
                buffer[i] = tolower(buffer[i]);
            }
        }

        while ((pos = strstr(pos, case_sensitive ? search_text : search_text_copy)) != NULL) {
            MatchInfo tmp;
            tmp.line_number = line_number;
            maches[match_count] = tmp;
            match_count++;
            // Print or store the line number for this match
            // printf("Match at Line %d\n", line_number);
            
            pos += search_text_length;
        }
        line_number++;  // Increment the line number for the next line
    }

    free(search_text_copy);  // Free the dynamically allocated memory
    fclose(file);
    return match_count;
}

int update_status(const char *file, size_t matchs)
{ 
     
    int x = get_x();    
    int y = get_y(); 
    // printf("\nHELLO -> x=%d y=%d \n",x,y);
    set_x(768); 
    set_y(100); 
    printf("%d matches found in %s\n", matchs,file); 
    set_x(y);   
    set_y(x); 
    // for(;;);  
} 

void search_first_match(const char *file_path, const char *search_text, int case_sensitive) {
    FILE *file = fopen(file_path, "r");

    if (file == NULL) { 
        perror("Error opening file"); 
        return;
    }

    char *buffer = malloc(get_file_size_from_pointer(file));
    if(buffer == NULL)
    {
        printf("Failed to allocate memory for file %s\n", file_path);
        return;
    }
    size_t search_text_length = strlen(search_text); 

    char *search_text_copy = NULL;
    if (!case_sensitive) {
        search_text_copy = strdup(search_text);
        for (int i = 0; search_text_copy[i]; i++) {
            search_text_copy[i] = tolower(search_text_copy[i]);
        }
    }

    int line_number = 1;

    while (fgets(buffer, sizeof(buffer), file)) {
        char *pos = buffer;
        char *match_pos = NULL;  // To store the position of the first match

        if (!case_sensitive) {
            for (int i = 0; buffer[i]; i++) {
                buffer[i] = tolower(buffer[i]);
            }
        }

        while ((pos = strstr(pos, case_sensitive ? search_text : search_text_copy)) != NULL) {
            // Print the snippet of where the first match was found
            match_pos = pos;
            break;  // Stop after the first match
        }

        if (match_pos != NULL) {
            // Print a snippet of where the first match was found
            printf("Match at Line %d, Char Index %ld: ", line_number, match_pos - buffer);
            for (int i = 0; i < 20 && match_pos[i] != '\0'; i++) {
                printf("%s",match_pos[i]);
            }
            printf('\n');
            break;  // Stop after the first match
        }

        line_number++;  // Increment the line number for the next line
    }

    free(search_text_copy);  // Free the dynamically allocated memory
    free(buffer);
    fclose(file);
}


int csearch_file(const char *file_path, const char *search_text, int case_sensitive) {
    int m = 0;
   FILE *file = fopen(file_path, "r");

    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char *buffer = malloc(get_file_size_from_pointer(file));
    if(buffer == NULL)
    {
        printf("Failed to allocate memory for file %s\n", file_path);
        fclose(file);
        return;
    }
    // printf("\n%d ",strlen(buffer));
    fread(buffer,sizeof(char),get_file_size_from_pointer(file),file);
    char* copy = strdup(buffer);
    char *token = strtok(copy, " "); // Split the text at spaces.
     while (token != NULL) {
        update_status(file_path,m);
        if(strcmp(token, search_text) == 0)
        {
            
            m++;
            set_font_c(255, 255, 0);
            set_bg_c(0,0,255);
        }
        printf("%s ", token);
        token = strtok(NULL, " "); // Get the next token.
        set_font_c(0,255,0);
        set_bg_c(0,0,0);
        update_status(file_path,m);
        
    }
    free(buffer);
    free(copy); // Free the memory allocated for the copy.
    fclose(file);
}