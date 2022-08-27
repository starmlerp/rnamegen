#ifndef _RNAMEGEN_H_
#define _RNAMEGEN_H_

#include <cstdio>
#include <ctime>
#include <cmath>

struct letterdata{
	size_t weight, *preceede, *succeede;
};

class stats{
	struct letterdata *ldata;
	size_t ltotal;
	size_t cnames;

	public:
	void analyze(char **sample, size_t size);
	double evaluate(char *target);
	char *assemble();
};

struct word{
	char letter;
	struct word *next, *prev;
};

int load(FILE* f, char ***out);

#endif
