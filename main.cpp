#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <math.h>

#include "rnamegen.h"

#define SEPARATOR ','

int load(FILE* f, char ***out){
	rewind(f);
	int inp, n = 1;
	enum{
		FC, //file size countup
		WC, //word size countup
		RD  //read the words
	};
	do{
		inp = fgetc(f);
		if(inp == EOF)break;
		if(inp == SEPARATOR){
			n++;
			inp = fgetc(f);
			while(inp == ' ' || inp == '\n')inp = fgetc(f);
		}
	}while(inp != EOF);
	
	rewind(f);
	inp = fgetc(f);
	char **names = (char**)malloc(n * sizeof(char*));
	*out = names;
	int m[n];
	for(int i = 0; i < n; i++)m[i] = 0;
	n = 1;

	do{
		if(inp == EOF)break;
		if(inp == SEPARATOR || inp == '\n'){
			n++;
			inp = fgetc(f);
			while(inp == ' ' || inp == '\n')inp = fgetc(f);
		}
		else{
			m[n-1]++;
			inp = fgetc(f);
		}
	}while(inp != EOF);
	
	rewind(f);
	inp = fgetc(f);
	for(int i = 0; i < n; i++)names[i] = (char*)malloc((m[i] + 1) * sizeof(char));
	for(int i = 0; i < n; i++)names[i][m[i]] = '\0';
	n = 0;
	int l = 0;
	do{
		if(inp == EOF || inp == '\n')break;
		if(inp == SEPARATOR){
			n++;
			l = 0;
		inp = fgetc(f);
			while(inp == ' ' || inp == '\n' )inp = fgetc(f);
		}
		else{
			names[n][l++] = inp;
			inp = fgetc(f);
		}
	}while(inp != EOF);
	return n+1;
	//TODO: use a for loop and a switch case instead of running while loop three times
}

//TODO: implement functions to load and save letter data in a file, as well as reimplement the analyzer function in order to make it possible for it to take the existing letter data, and append to it
int main(int argc, char *argv[]){
	char **names = NULL;

	char namedefault[50] = "data/names.csv", *namedata = namedefault;
	size_t count = 8;
	size_t generations = 0;
	
	if(argc > 1)
		for(int i = 1; i < argc; i++){
			size_t l;
			for(l = 0; argv[i][l]; ++l);
			if(l == 2 && argv[i][0] == '-'){
				if(argc == i+1){
					printf("%s: option expects an argument\n", argv[0]);
					return 1;
				}
				switch(argv[i][1]){
					case 'f': // which csv file to use for starting names
						namedata = argv[++i];
					break;
					case 'n': // number of names to produce 
						count = atoi(argv[++i]);
					break;
					case 'g':
						generations = atoi(argv[++i]);
					break;
					default:
						goto prterr;
					break;
				}
			}
			else{
prterr:
				printf("%s: invalid option \"%s\"\n", argv[0], argv[i]);
				return 2;
			}
		}
	srand(time(NULL));
	int n = load(fopen(namedata, "r"), &names);
	stats data;
	data.analyze(names, n);
	for(size_t i = 0; *names[i]; ++i){
		double out = data.evaluate(names[i]);
		printf("%s: %f\n", names[i], out);
	}
}
