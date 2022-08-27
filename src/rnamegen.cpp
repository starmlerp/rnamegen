#include "rnamegen.h"

#define SEPARATOR ','

#define FSELECTOR sqrt(this->ldata[i].weight * this->ldata[i].preceede['\0'])
#define LSELECTOR sqrt(prev * curr * (i?1:nsize/namelen))

#define random(x, y) (rand() % ((y) - (x) + 1) + (x))
#define abs(x) ((x)>=0?(x):-(x))

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
	char **names = new char*[n];
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
	for(int i = 0; i < n; i++)names[i] = new char[m[i] + 1];
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

void stats::analyze(char **sample, size_t size){
	//analyze letters in the names by which letter preceeds and succeedes it
	const size_t charwidth = ((unsigned char)-1)+1;
	//TODO: check if this->ldata is already allocated, if not only then do this
	this->ltotal = 0;
	this->cnames = size;
	this->ldata = new letterdata[charwidth];
	for(size_t i = 0; i < charwidth; i++){
		this->ldata[i].weight = 0;
		this->ldata[i].succeede = new size_t[charwidth];
		this->ldata[i].preceede = new size_t[charwidth];
		for(size_t j = 0; j < charwidth; j++)this->ldata[i].succeede[j] = 0;
		for(size_t j = 0; j < charwidth; j++)this->ldata[i].preceede[j] = 0;
	}

	for(size_t i = 0; i < size; i++){
		int c = -1;
		do{
			c++;
			this->ldata[(size_t)sample[i][c]].weight++;
			if(sample[i][c])this->ldata[(size_t)sample[i][c]].succeede[(size_t)sample[i][c+1]]++;
			if(!c){
				this->ldata[(size_t)sample[i][c]].preceede['\0']++;
			}
			else this->ldata[(size_t)sample[i][c]].preceede[(size_t)sample[i][c-1]]++;
			this->ltotal++;
		}while(sample[i][c] != '\0');
	}
}

char *stats::assemble(){
	//assemble a random word based on the letter data
	long sum = 0;
	size_t charwidth = ((unsigned char)-1)+1, nsize = 0;
	const double namelen = (double)this->ltotal / this->cnames;
//	size_t prev = '\0';
	for(size_t i = 0; i < charwidth; i++)sum+=(FSELECTOR);
	struct word *f, *c = (struct word*)malloc(sizeof(struct word));
	c->next = NULL;
	c->prev = NULL;
	f = c;
	long r = random(1, sum);
	for(size_t i = 0; i < charwidth && r > 0; i++){
		r -= (FSELECTOR);
		c->letter = i;
	}
	while(c->letter){
		c->next = (struct word*)malloc(sizeof(struct word));
		c->next->prev = c;
		c = c->next;
		c->next = NULL;
		long lsum = 0;
//		prev = c->prev->letter;
		for(size_t i = 0; i < charwidth; i++){
			const size_t prev = this->ldata[(size_t)c->prev->letter].succeede[i];
			const size_t curr = this->ldata[i].weight;

			lsum+=(LSELECTOR);
		}
		long r = random(1, lsum);
		for(size_t i = 0; i < charwidth && r > 0; i++){
			c->letter = i;
			const size_t prev = this->ldata[(size_t)c->prev->letter].succeede[i];
			const size_t curr = this->ldata[i].weight;
			r -= (LSELECTOR);
		}
		nsize++;
	}
	char *out = (char*)malloc(nsize * sizeof(char));
	c = f;
	for(size_t i = 0; i < nsize+1; i++){
		out[i] = c->letter;
		struct word *h = c;
		c = c->next;
		free(h);
	}
	return out;
}

double stats::evaluate(char *target){
	const double namelen = (double)this->ltotal / this->cnames;
	double out = 0;
	size_t i;
	for(i = 0; target[i]; i++){
		char prev, next = target[i+1], curr=target[i];
		if(i == 0)prev ='\0';
		else prev = target[i-1];
		out += this->ldata[prev].succeede[curr] * this->ldata[next].preceede[curr] * this->ldata[curr].weight;
	}
	out /= namelen / abs(i - namelen);
	return out;
}
