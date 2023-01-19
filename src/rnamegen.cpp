#include "rnamegen.h"

#define SEPARATOR ','

#define FSELECTOR sqrt(this->ldata[i].weight * this->ldata[i].preceede['\0'])
#define LSELECTOR sqrt(prev * curr * (i?1:nsize/namelen))

#define random(x, y) (rand() % ((y) - (x) + 1) + (x))
#define abs(x) ((x)>=0?(x):-(x))

#define CHARW ( ((unsigned char)-1)+1 )

stats::stats(){
	this->ltotal = 0;
	this->cnames = 0;
	this->ldata = new letterdata[CHARW];
	for(size_t i = 0; i < CHARW; i++){
		this->ldata[i].weight = 0;
		this->ldata[i].succeede = new size_t[CHARW];
		this->ldata[i].preceede = new size_t[CHARW];
		for(size_t j = 0; j < CHARW; j++)this->ldata[i].succeede[j] = 0;
		for(size_t j = 0; j < CHARW; j++)this->ldata[i].preceede[j] = 0;
	}

}

stats::~stats(){
//	const size_t CHARW = ((unsigned char)-1)+1;
	for(size_t i = 0; i < CHARW; i++){
		delete [] this->ldata[i].succeede;
		delete [] this->ldata[i].preceede;
	}
	delete [] this->ldata;
}

void stats::analyze(char **sample, size_t size){
	//analyze letters in the names by which letter preceeds and succeedes it
//	const size_t CHARW = ((unsigned char)-1)+1;
	//TODO: check if this->ldata is already allocated, if not only then do this
	this->ltotal = 0;
	this->cnames = size;
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
//	size_t CHARW = ((unsigned char)-1)+1;
	size_t nsize = 0;
	const double namelen = (double)this->ltotal / this->cnames;
//	size_t prev = '\0';
	for(size_t i = 0; i < CHARW; i++)sum+=(FSELECTOR);
	struct word *f, *c = new struct word;
	c->next = NULL;
	c->prev = NULL;
	f = c;
	long r = random(1, sum);
	for(size_t i = 0; i < CHARW && r > 0; i++){
		r -= (FSELECTOR);
		c->letter = i;
	}
	while(c->letter){
		c->next = new struct word;
		c->next->prev = c;
		c = c->next;
		c->next = NULL;
		long lsum = 0;
		for(size_t i = 0; i < CHARW; i++){
			const size_t prev = this->ldata[(size_t)c->prev->letter].succeede[i];
			const size_t curr = this->ldata[i].weight;

			lsum+=(LSELECTOR);
		}
		long r = random(1, lsum);
		for(size_t i = 0; i < CHARW && r > 0; i++){
			c->letter = i;
			const size_t prev = this->ldata[(size_t)c->prev->letter].succeede[i];
			const size_t curr = this->ldata[i].weight;
			r -= (LSELECTOR);
		}
		nsize++;
	}
	char *out = new char[nsize + 1];
	out[nsize] = '\0';
	c = f;
	for(size_t i = 0; i < nsize; i++){
		out[i] = c->letter;
		if(c->next){
			c = c->next;
			delete c->prev;
		}
	}
	delete c;
	return out;
}

double stats::evaluate(char *target){
	const double namelen = (double)this->ltotal / this->cnames;
	double out = 0;
	size_t i;
	for(i = 0; target[i]; i++){
		size_t prev, next = (size_t)target[i+1], curr=(size_t)target[i];
		if(i == 0)prev = 0;
		else prev = (size_t)target[i-1];
		out += this->ldata[prev].succeede[curr] * this->ldata[next].preceede[curr] * this->ldata[curr].weight;
	}
	out /= namelen / abs(i - namelen);
	return out;
}
