#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <math.h>

#define CHARW ( ((unsigned char)-1)+1 )
#include "rnamegen.h"

#define SEPARATOR ','
#define NAMEDEF "data/names.csv"

size_t loadcsv(FILE* f, char ***values){
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

	char *namedata = NULL,
	     *indata = NULL,
	     *outdata = NULL;
	size_t count = 1;
	size_t generations = 0;
	bool silent = false;
	
	if(argc > 1)
		for(int i = 1; i < argc; i++){
			size_t l;
			for(l = 0; argv[i][l]; ++l);
			if(l == 2 && argv[i][0] == '-'){
				switch(argv[i][1]){
					case 'f': // which csv file to use for starting names
						if(argc == i - 1){
							printf("%s: option %s expects an argument\n", argv[0],argv[i]);
							return 1;
						}
						namedata = argv[++i];
					break;
					case 'n': // number of names to produce 
						if(argc == i - 1){
							printf("%s: option %s expects an argument\n", argv[0],argv[i]);
							return 1;
						}
						count = atoi(argv[++i]);
					break;
					case 'i': // input file to use
						if(argc == i - 1){
							printf("%s: option %s expects an argument\n", argv[0],argv[i]);
							return 1;
						}
						indata = argv[++i];
					break;
					case 'o': // output file to write to
						if(argc == i - 1){
							printf("%s: option %s expects an argument\n", argv[0],argv[i]);
							return 1;
						}
						outdata = argv[++i];
					break;
					case 'g': // number of evaluative generations to produce
						if(argc == i - 1){
							printf("%s: option %s expects an argument\n", argv[0],argv[i]);
							return 1;
						}
						generations = atoi(argv[++i]);
					break;
					case 's':
						silent = true;
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
	
	FILE *namefile, *datafile;
	stats data;
	
	if(!namedata && !indata){
		namefile = fopen(NAMEDEF, "r");
		size_t n = loadcsv(namefile, &names);
		data.analyze(names, n);
	}
	else{
		if(namedata){
			namefile = fopen(namedata, "r");
			size_t n = loadcsv(namefile, &names);
			data.analyze(names, n);
		}
		if(indata){
			datafile = fopen(indata, "r");
			char **csvalues;
			size_t n = loadcsv(datafile, &csvalues);
			if(n != CHARW + 2 * CHARW * CHARW + 2){
				//we are expecting to read exactly char max value + 2 * charmax^2 + 2 numeric values
				//2 times the width because for each character we store the weight of the
				//character itself, plus preceeding and succeeding values for each other character
				//and plus 2 to store the total word and letter count

				//loading any other amount is a sign of error!

				fprintf(stderr, "error loading the file: invalid number of values\n");
				return 2;
			}
			size_t di = 0;
			for(size_t i = 0; i < CHARW; i++){
				data.ldata[i].weight += atoi(csvalues[di++]);
				for(size_t j = 0; j < CHARW; j++)
					data.ldata[i].preceede[j] += atoi(csvalues[di++]);
				for(size_t j = 0; j < CHARW; j++)
					data.ldata[i].succeede[j] += atoi(csvalues[di++]);
			}
			data.ltotal = atoi(csvalues[di++]);
			data.cnames = atoi(csvalues[di++]);
		}
	}
	if(outdata){
		FILE *outfile = fopen(outdata, "w");
		for(size_t i = 0; i < CHARW; i++){
			fprintf(outfile, "%ld, ", data.ldata[i].weight);
			for(size_t j = 0; j < CHARW; j++)
				fprintf(outfile, "%ld, ", data.ldata[i].preceede[j]);
			for(size_t j = 0; j < CHARW; j++)
				fprintf(outfile, "%ld, ", data.ldata[i].succeede[j]);
		}
		fprintf(outfile, "%ld, %ld", data.ltotal, data.cnames);
	}
	if(!silent)
		for(size_t i = 0; i < count; ++i){
			printf("%s\n", data.assemble());
		}
	return 0;
}
