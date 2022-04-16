#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MINLEN 4
#define MAXLEN 9

#define ATOMS 3
#define ATOMMIN 2
#define ATOMMAX 4

#define VOWEL 1
#define CONSONANT 2
#define VOWELS {'a', 'e', 'i', 'o', 'u'}

void atomize(char **target, int len){
	//TODO: generate atoms according to following rules:
	//each atom must be at least ATOMMIN and at most ATOMMAX long
	//each atom must contain at least one vowel
	//no more than two consecutive consonants can be present, and no more than one vowel
	for(int i = 0; i < len; i++){
		int tvow = 0, vowl = 0, cons = 0;
		int size = rand() % (ATOMMIN - ATOMMAX) + ATOMMIN;
//		printf("%d\n",size);
		target[i] = (char*)malloc((size + 1) * sizeof(char));
		target[i][size] = '\0';
		const char vowels[5] = VOWELS;
		for(int j = 0; j < size; j++){
			do{
				target[i][j] = (double)rand() / RAND_MAX * ('z' - 'a') + 'a';
				int f = 0;
				for(int l = 0; l < 5; l++){
					if(target[i][j] == vowels[l]){
						vowl++;
						tvow++;
						cons = 0;
						f = 1;
						break;
					}
				}
				if(!f){
					cons++;
					vowl = 0;
				}
				if(j == size - 1 && !tvow){
				target[i][j] = vowels[rand() % 5];
				}
			}while(vowl > VOWEL || cons > CONSONANT);
		}
	}
}

void rnamegen(char *target, int len){
	int size = 0;
	char *atoms[ATOMS];
	atomize(atoms, ATOMS);
	while(true){
		int atom = rand() % ATOMS;
		int asize;
		for(asize = 0; atoms[atom][asize]; asize++);
		if(asize + size > len)break;
		for(int i = 0; atoms[atom][i]; i++){
			target[size++] = atoms[atom][i];
		}
	}
	target[size+1] = '\0';
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

