#ifndef POSTLIST_H
#define POSTLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct postlstnode{
	int did;
	int tf;
	struct postlstnode * next;
}pnode;


typedef struct {
	pnode *head;
	double idf;
	int nid;	
} postlist;


class Postlist{
	private:
		int size;
		postlist *plist;
		void realloc();
		void createModel(int totalDocs,int allterms);
		int searchDocument(postlist *list, int did);
		int containsDocument(postlist list, int did);
	public:
		Postlist():size(200){
			plist = (postlist *) malloc(sizeof(postlist) * size);
			memset(plist, 0, sizeof(plist));
		}
		~Postlist(){
			//free(plist);
		}
		void addDocument(int did, int idx);
		void idfCalc(int totaldocs, int totalterms);
		void dumpToFile(int totalDocs,int allids);
		void readFile();
		float getIDFbyPos(int idx);		
};

#endif