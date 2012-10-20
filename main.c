
/*
Iterate through a string until the string is randomly generated. keep
track of both the number of itterations and the time it took, bail it it
runs for more than an hour. Print out each itteration.
main
	get string
	setup timer
	itterate through string
		start with 1 char
			create substring
			Print iterations and stats
		add 1 more on each success
		if full string, exit

*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

#define STRING "Monkey"

char *randomStrOfLen(int num)
{
	char *attempt = NULL;
	int i = 0;
	attempt = malloc(num + 1);
	for (i = 0 ; i < num ; i++) {
		while (1) {
			char c = arc4random_uniform(0x7a);
			if (c == 0x20 || isalnum(c)) {
				attempt[i] = c;
				break;
			}
		}
	}

	return attempt;
}

void printResults(char *target, char *result, unsigned long count)
{
	fprintf(stderr, "%20s:%4d: result: %s target: %s iterations: %ld\n", __FILE__, __LINE__, result, target, count);
	return;
}

int iterateAndCompare(char *source, int numChars)
{
	unsigned long count = 0;
	char *attempt = NULL;
	char *target = NULL;

	count = 1;
	target = malloc(numChars + 1);
	snprintf(target, numChars + 1, "%s", source);
	attempt = randomStrOfLen(numChars);
	while (strncmp(target, attempt, numChars) != 0) {
		printResults(target, attempt, count);
		free(attempt);
		attempt = randomStrOfLen(numChars);
		count++;
		if(count > 5000000) {
			free(target);
			free(attempt);
			fprintf(stderr, "%20s:%4d: Bailing after 5M attempts\n", __FILE__, __LINE__);
			return -1;
		}
	}
	printResults(target, attempt, count);
	fprintf(stderr, "%20s:%4d: found \"%s\" after %ld iterations\n", __FILE__, __LINE__, attempt, count);
	free(target);
	free(attempt);
	return count;
}

void printInterval(time_t start, time_t end, char *string, int maxStrLen)
{
	char *subStr = NULL;
	time_t elapsed = 0;

	if (start == end) {
		elapsed = 1; /* first couple will not take long so lie */
	} else {
		elapsed = end - start;
	}

	subStr = malloc(maxStrLen + 1);
	snprintf(subStr, maxStrLen + 1, "%s", string);
	fprintf(stderr, "%20s:%4d: \"%s\" found after %ld seconds\n", __FILE__, __LINE__, subStr, elapsed);
	free(subStr);
	subStr = NULL;
	return;
}

int main(int argc, char *argv[])
{
	int	rv = 0;
	char	*string = NULL;
	int	curStrLen = 0;
	int	maxStrLen = 0;
	time_t	start = 0;
	time_t	end = 0;

	string = malloc(strlen(STRING) + 1);
	snprintf(string, strlen(STRING) + 1, "%s", STRING);
	start = end = time(NULL);
	maxStrLen = strlen(string);
	curStrLen = 1;
	rv = iterateAndCompare(string, curStrLen);
	end = time(NULL);
	while ((end - start) < 30) {
		if (rv == -1) {
			fprintf(stderr, "%20s:%4d: did not find string\n", __FILE__, __LINE__);
			return -1;
		}
		printInterval(start, end, string, curStrLen);
		if (curStrLen < maxStrLen) {
			curStrLen++;
			sleep(10);
			start = end = time(NULL);
			rv = iterateAndCompare(string, curStrLen);
			end = time(NULL);
		} else {
			fprintf(stderr, "%20s:%4d: End of test.\n", __FILE__, __LINE__);
			break;
		}
	}
	return 0;
}















