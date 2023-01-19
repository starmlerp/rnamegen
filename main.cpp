#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <math.h>

#include "rnamegen.h"

#define SEPARATOR ','

int loadcsv(FILE* f, char ***values){
	rewind(f);
	int inp, n = 1;
	enum{
		SEEK, // searching through the word for the comma
		SPAC  // searching through the spaces for the start of the word
	}state = SPAC;
	//obtaining an calculating the size of the file
	fseek(f, 0, SEEK_END);
	size_t fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	fsize -= ftell(f);
	char *buffer = new char[fsize];
	for(size_t i = 0; i < fsize; i++){
		buffer[i]=fgetc(f);
		if(buffer[i] == ',')n++; // increment N if we come by a comma
	}
	*values = new char*[n];
	size_t ival = 0;
	for(size_t i = 0; i < fsize; i++){
		switch(state){
			case SEEK:
				if(buffer[i] == ',' || buffer[i] == '\n'){
					state = SPAC;
					buffer[i] = '\0';
				}
			break;
			case SPAC:
				if(buffer[i] != ' ' && buffer[i] != '\n'){
					//NOTE: this may break if multiple immediatelly successive commas are present
					state = SEEK;
					(*values)[ival++] = &buffer[i];
				}
			break;
		}
	}
	return n;
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
					case 'g': // number of evaluative generations to produce
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
	int n = loadcsv(fopen(namedata, "r"), &names);
	stats data;
	data.analyze(names, n);
	for(size_t i = 0; i < n; ++i){
		double out = data.evaluate(names[i]);
		printf("%s: %f\n", names[i], out);
	}
	return 0;
}
