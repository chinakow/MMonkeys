/*
 * main.c -- A test to see how long it really would take to randomly
 * create even a simple string.
 *
 * Created by: Jon McLaughlin jon@jpmrc.com
 *
 * Created: October 20, 2012
 *
 * Since it is so simple, this application is public domain.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

#define STRING "Monkey"
#define MAX_COUNT_REACHED -2

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
	attempt[num] = '\0';

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
		free(attempt);
		attempt = randomStrOfLen(numChars);
		count++;
		if(count > 500000000) {
			free(target);
			free(attempt);
			fprintf(stderr, "%20s:%4d: Bailing after 500M attempts\n", __FILE__, __LINE__);
			return MAX_COUNT_REACHED;
		}
	}
	printResults(target, attempt, count);
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
		printInterval(start, end, string, curStrLen);
		if (rv == -1) {
			fprintf(stderr, "%20s:%4d: did not find string\n", __FILE__, __LINE__);
			return -1;
		}
		if (curStrLen < maxStrLen) {
			curStrLen++;
			sleep(2);
			start = end = time(NULL);
			rv = iterateAndCompare(string, curStrLen);
			end = time(NULL);
			if (rv == MAX_COUNT_REACHED) {
				fprintf(stderr, "%20s:%4d: Maximum count reached without success after %ld seconds\n", __FILE__, __LINE__, end - start);
				return -1;
			}
		} else {
			fprintf(stderr, "%20s:%4d: End of test.\n", __FILE__, __LINE__);
			break;
		}
	}
	return 0;
}















