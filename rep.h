#include <unistd.h>
#include <regex.h>
#include <string.h>
#include <unistd.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

struct regex {
	regex_t pattern;
	int flags;
	regmatch_t match;
};

int find_offset(int, int, int);
ssize_t init_regex(struct regex*, char*, int);
int remove_and_replace_from_string(char*, char*, int, int, int, int);
