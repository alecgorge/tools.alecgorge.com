#include <stdio.h>

void *load(const char *Filename);

float data[100][100];
int i = 0;
int j = 0;

int main(void) {
	char fname[1000];
	int count = 0;

	printf("Enter a filename: ");
	scanf("%s", fname);

	if(load(fname) == NULL) {
		return 1;
	}

	for(int x = 0; x < i; x++) {
		for(int y = 0; y < j + 1; y++) {
			if(count >= 10) {
				return 0;
			}

			printf("val[%d][%d]: %f\n", x, y, data[x][y]);
			count++;
		}
	}

	return 0;
}

void *load(const char *Filename) {
	int done = 0;
	char c;
	FILE *fhandle = fopen(Filename, "r+");

	if(fhandle == NULL) {
		printf("You suck. File doesn't exist.\n");
		return NULL:
	}

	for(i = 0; done == 0; i++) {
		for(j = 0; /* no condition required */ ; j++) {
			fscanf(fhandle, "%f", &data[i][j]);

			c = fgetc(fhandle);
			if(c == '\n' || c == '\r' || c == EOF) {
				if(c == EOF) {
					done = 1;
				}

				break;
			}
		}
	}

	fclose(fhandle);

	return &data[0][0];
}
