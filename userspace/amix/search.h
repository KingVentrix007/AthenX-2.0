#ifndef __SEARCH__H
#define __SEARCH__H

typedef struct MatchInfo {
    int line_number;
    int char_index;
} MatchInfo;
MatchInfo maches[1000];
int search_file(const char *file_path, const char *search_text, int case_sensitive);
void search_first_match(const char *file_path, const char *search_text, int case_sensitive);
int csearch_file(const char *file_path, const char *search_text, int case_sensitive);



#endif