#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define MAX_STRING_LENGTH 30
#define ASCII_SIZE	256


int stat [MAX_STRING_LENGTH];
int stat2 [ASCII_SIZE];


int main(int argc, char *argv[]) {
	int i = 0;
	int rc = 0;
	int sum = 0;
	int line_num = 1;
	char *line = NULL;
	size_t length = 0;
	FILE *rfile = NULL;

	if (argc == 1) {
		printf("usage: ./stat <filename>\n");
		exit(0);
	}

	// Open argv[1] file
	rfile = fopen((char *) argv[1], "rb");
	if (rfile == NULL) {
		perror(argv[1]);
		exit(0);
	}

	// initialize stat
	memset(stat, 0, sizeof(stat));
	memset(stat2, 0, sizeof(stat));

	while (1) {
		char *cptr = NULL;
		char *substr = NULL;
		char *brka = NULL;
		char *sep = "{}()[],;\" \n\t^";

		// For each line,
		rc = getdelim(&line, &length, '\n', rfile);
		if (rc == -1) break;

		cptr = line;
#ifdef _IO_
		printf("[%3d] %s\n", line_num++, line);
#endif
		for (substr = strtok_r(cptr, sep, &brka); substr; substr = strtok_r(NULL, sep, &brka)) {
			length = strlen(substr);
			// update stats

			// length of the sub-string
			if (length >= 30) length = 30;
#ifdef _IO_
			printf("length: %d\n", (int)length);
#endif
			stat[length-1]++;

			// number of the character in the sub-string
			for (int i = 0 ; i < length ; i++) {
				if (*cptr < 256 && *cptr > 1) {
					stat2[*cptr]++;
#ifdef _IO_
					printf("# of %c(%d): %d\n", *cptr, *cptr, stat2[*cptr]);
#endif
				}
				cptr++;
			}
			cptr++;
			if (*cptr == '\0') break;
		}
	}

	// sum
	sum = 0;
	for (i = 0 ; i < 30 ; i++) {
		sum += stat[i];
	}
	// print out distributions
	printf("*** print out distributions *** \n");
	printf("  #ch  freq \n");
	for (i = 0 ; i < 30 ; i++) {
		int j = 0;
		int num_star = stat[i]*80/sum;
		printf("[%3d]: %4d \t", i+1, stat[i]);
		for (j = 0 ; j < num_star ; j++)
			printf("*");
		printf("\n");
	}
	
	for (char c = 'A'; c <= 'Z'; c++) {
    printf("%c: %d\n", c, stat2[c] + stat2[c + 32]); // c + 32는 소문자로 변환
	}

	if (line != NULL) free(line);
	// Close the file
	fclose(rfile);

	return 0;
}
