#ifndef QUERY_H
#define QUERY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "VectorModel.h"

#define MAX_WRDS 100
#define MAX_CHR 100
#define QRY_NUMBER 100

typedef struct {
	int num;
	char words[MAX_WRDS][MAX_CHR];
	int word_size;
	int rank_size;
	int *relevants;
}query;

class Query{
	private:
		query queries[QRY_NUMBER];
		void normalizestr(char *str);
		void lowerstr(char *str);
		double map(int idx, int *result);
		double precisonAtTen(int idx, int * result);
		int binarySearch(int element, int v[], int size);
	public:
		Query();
		~Query();
		void loadQuery(char *path);
		void calculate(VectorModel *vm);
};
#endif