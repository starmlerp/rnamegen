#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MINLEN 4
#define MAXLEN 11

#define VOWEL 1
#define CONSONANT 2
#define VOWELS {'a', 'e', 'i', 'o', 'u'}

void rnamegen(char *target, int len){
	int vowl=0;
	int cons=0;
	const char vowels[5] = VOWELS;
	for(int j = 0; j < len - 1; j++){
		do{
			target[j] = (double)rand() / RAND_MAX * ('z' - 'a') + 'a';
			int f = 0;
			for(int l = 0; l < 5; l++){
				if(target[j] == vowels[l]){
					vowl++;
					cons = 0;
					f = 1;
					break;
				}
			}
			if(!f){
				cons++;
				vowl = 0;
			}
		}while(vowl > VOWEL || cons > CONSONANT);
	}
	target[len] = '\0';
}

int main(int argc, char **argv){
	int c = 8;
	switch(argc){
		case 2:
			c = atoi(argv[1]);
		case 1:
			srand(time(0));
			for(int i = 0; i < c; i++){
				int len = (double)rand() / RAND_MAX * (MAXLEN - MINLEN) + MINLEN;
				char out[len];
				rnamegen(out, len);
				printf("%s\n", out);
			}
			return 0;
		default:
			printf("usage: %s [N]\n", argv[0]);
		return 0;
	}
}

