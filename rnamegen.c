#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MINLEN 4
#define MAXLEN 11

struct letterdata{
	char letter;
	int preceede[255], succeede[255];
};

char **load(FILE* f){
	//separate name file by commas
}

struct letterdata *analyze(char **sample){
	//analyze letters in the names by which letter preceeds and succeedes it
}

char *assemble(struct letterdata *letters){
	//assemble a random word based on the letter data
}

int main(int argc, char *argv[]){
	
}
