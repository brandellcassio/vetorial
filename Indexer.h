#ifndef INDEXER_H
#define INDEXER_H 

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "Vocabulary.h"

#define TAMBUFF 105

class Indexer{
	public:
		void readFiles(char *path);
		Indexer(){}
		~Indexer(){}
	private:
		void lowerstr(char *str);
		void normalizestr(char *str);
		int isNumber(char * str);
		
};


#endif