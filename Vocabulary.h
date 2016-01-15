#ifndef VOCABULARY_H
#define VOCABULARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Postlist.h"

#define TAM_STR 105
#define INIT_VALUE 700
#define FNV_PRIME_32 16777619
#define FNV_OFFSET_32 2166136261U

typedef struct{
	int id;
	char word[TAM_STR];	
} word;

typedef struct node{
	word data;
	struct node * next;
} node;


typedef struct {
	node * head;
	int size;
} hash;

class Vocabulary {
	private:
		int vocal_size;
		int local_id;
		hash *table;
		Postlist list;
		unsigned int fnhash(char *s);
		void initTable();
		void rehash();
	public:
		Vocabulary():vocal_size(INIT_VALUE), local_id(0), list(){
			initTable();
		}
				
		int addWord(char * str, int docid);
		int findWord(char *str);
		int getSize();
		int verifySize();
		void idfcalculing(int totaldocs);
		void dumpToFile(int totalDocs);	
		void readFile();
		Postlist * getlist();	
};





#endif