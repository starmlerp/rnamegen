#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <math.h>

#define SEPARATOR ','

#define random(x, y) (rand() % (y - x + 1) + x)

#define FSELECTOR letters.ldata[i].weight * letters.ldata[i].preceede['\0']
#define LSELECTOR letters.ldata[t].succeede[i] * letters.ldata[i].weight * letters.ldata[i].preceede[t] * (i?1:nsize/namelen)

struct letterdata{
	int weight, *preceede;
	int *succeede;
};

struct stats{
	struct letterdata *ldata;
	size_t ltotal;
	size_t cnames;
};

struct word{
	char letter;
	struct word *next;
};

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
		double test = sqrt(9);
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

void analyze(char **sample, size_t size, struct stats *outdata){
	//analyze letters in the names by which letter preceeds and succeedes it
	size_t charwidth = 1;
	for(size_t i = 0; i < 8 * sizeof(char); i++)charwidth *= 2;
	//TODO: check if outdata->ldata is already allocated, if not only then do this
	outdata->ltotal = 0;
	outdata->cnames = size;
	outdata->ldata = (struct letterdata*)malloc(charwidth * sizeof(struct letterdata));
	for(size_t i = 0; i < charwidth; i++){
		outdata->ldata[i].weight = 0;
		outdata->ldata[i].succeede = (int*)malloc(charwidth * sizeof(int));
		outdata->ldata[i].preceede = (int*)malloc(charwidth * sizeof(int));
		for(size_t j = 0; j < charwidth; j++)outdata->ldata[i].succeede[j] = 0;
		for(size_t j = 0; j < charwidth; j++)outdata->ldata[i].preceede[j] = 0;
	}

	for(size_t i = 0; i < size; i++){
		int c = -1;
		do{
			c++;
			char letter = sample[i][c];
			outdata->ldata[sample[i][c]].weight++;
			if(sample[i][c])outdata->ldata[sample[i][c]].succeede[sample[i][c+1]]++;
			if(!c){
				outdata->ldata[sample[i][c]].preceede['\0']++;
			}
			else outdata->ldata[sample[i][c]].preceede[sample[i][c-1]]++;
			outdata->ltotal++;
		}while(sample[i][c] != '\0');
	}
}

char *assemble(struct stats letters){
	//assemble a random word based on the letter data
	//TODO: make the assembler better utilize the data it has in function construction
	long sum = 0;
	size_t charwidth = 1, s = 0;
	const double namelen = (double)letters.ltotal / letters.cnames;
	for(size_t i = 0; i < 8 * sizeof(char); i++)charwidth *= 2;

	for(size_t i = 0; i < charwidth; i++)sum+=(FSELECTOR);
	struct word *f, *c = (struct word*)malloc(sizeof(struct word));
	c->next = NULL;
	f = c;
	long r = random(1, sum);
	for(size_t i = 0; i < charwidth && r > 0; i++){
		r -= (FSELECTOR);
		c->letter = i;
	}
	s++;
	while(c->letter){
		int t = c->letter;
		c->next = (struct word*)malloc(sizeof(struct word));
		c = c->next;
		c->next = NULL;
		long lsum = 0;
		size_t nsize = s-1;
		for(size_t i = 0; i < charwidth; i++)lsum+=(LSELECTOR);
		long r = random(1, lsum);
//		printf("%d/%d\n", r, lsum);
		for(size_t i = 0; i < charwidth && r > 0; i++){
			c->letter = i;
			r -= (LSELECTOR);
//			printf("%d\n", r);
		}
//		printf("%c", c->letter);
		s++;
	}
	char *out = (char*)malloc(s * sizeof(char));
	c = f;
	for(size_t i = 0; i < s; i++){
		out[i] = c->letter;
		struct word *h = c;
		c = c->next;
		free(h);
	}
	return out;
}
//TODO: implement functions to load and save letter data in a file, as well as reimplement the analyzer function in order to make it possible for it to take the existing letter data, and append to it
int main(int argc, char *argv[]){
	char **names = NULL;
	int count = 8;
	if(argc == 2)count = atoi(argv[1]);
	srand(time(NULL));
	int n = load(fopen("data/names.csv", "r"), &names);
	struct stats data;
	analyze(names, n, &data);
//	for(int i = 0; i < n; i++)printf("%s\n", names[i]);
/*
	for(int i = 0; i < 256; i++){
		if(data[i].weight)printf("%c(%d): %d\n", i, i, data[i].weight);
		for(int j = 0; j < 256; j++)if(data[i].succeede[j] || data[i].preceede[j])printf("\t%c(%d): %d:%d\n", j, j, data[i].preceede[j], data[i].succeede[j]); 
	}
*/
//	printf("%d\n", n);
	for(int i = 0; i < count; i++)printf("%s\n", assemble(data));
}
