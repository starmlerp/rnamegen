#ifndef _RNAMEGEN_H_
#define _RNAMEGEN_H_

#include <cstdio>
#include <ctime>
#include <cmath>
#include <climits>

struct letterdata{
	size_t weight, *preceede, *succeede;
};

class stats{
	struct word{
		char letter;
		struct word *next, *prev;
	};
	public:
	struct letterdata *ldata;
	size_t ltotal;
	size_t cnames;
	void analyze(char **sample, size_t size);
	double evaluate(char *target);
	char *assemble();
	 stats();
	~stats();
};

#endif
