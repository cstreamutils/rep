#include <stdio.h>

#ifndef REP_H
#define REP_H
#include "rep.h"
#endif


/* find the offset of the string to out where the end of the
   string will be after replacement */
int find_offset(int start, int end, int rep_length)
{
	return ((end - start) > rep_length) ? end -start : rep_length;
}

ssize_t init_regex(struct regex* regx, char *pattern, int flags)
{
	int ret;
	regx->flags = flags;
	ret = regcomp(&regx->pattern, pattern, flags);
	return ret;
}

int remove_and_replace_from_string(char *string, char *replacement, int start, int end, int rep_length, int str_len)
{
	int i, j, offset, replaced;
	char copy[str_len];
	strcpy(copy, string);
	offset = find_offset(start, end, rep_length);
	for (i = 0, replaced = 0, j = 0; string[i + offset] != '\0'; i++) {
		if (replaced < rep_length) {
			string[start + i] = replacement[i];
			replaced++;
		} else {
			string[start + i] = copy[end + j++];
		}
	
	};
	return i - j;
}

int main(int argc, char **argv)
{
	char in_buf[BUF_SIZE];
	int ret, rep_size;
	ssize_t num_read, num_written;
	struct regex regx;

	if (argc != 3) {
		fprintf(stderr, "usage: rep <regex to replace> <text to replace with>");
		return 1;
	}

	if (isatty(STDIN_FILENO)) {
		fprintf(stderr, "no data in input stream");
		return 1;
	}

	ret = init_regex(&regx, argv[1], 0);
	for (rep_size = 0; argv[2][rep_size] != '\0'; rep_size++);

	while ((num_read = read(STDIN_FILENO, in_buf, BUF_SIZE)) != 0) {
		while ((ret = regexec(&regx.pattern, in_buf, 1, &regx.match, regx.flags)) == 0) {
			ret = remove_and_replace_from_string(in_buf, argv[2], regx.match.rm_so, regx.match.rm_eo, rep_size, num_read);
			num_read += ret;
		}
		write(STDOUT_FILENO, in_buf, num_read);
	}
}
