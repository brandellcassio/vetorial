#ifndef VECTORMODEL_H
#define VECTORMODEL_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Vocabulary.h"

typedef struct {
	float *coord;
	float module;
} vector;

class VectorModel{

	private:
		Vocabulary vocabulary;
		vector * vectors;
		int size_docs;
		int size_terms;
		int query_coord[100];
		int query_size;
		void initVectors();
		vector createVector(char words[][100], int size_words);
		float * calculateSimilarity(vector query);
		int * generateRank(float similarities[]);
		void quick(int idx[], float similarities[], int init, int end);
	public:
		VectorModel();
		VectorModel(int size_docs, int size_terms);
		~VectorModel();
		void readFile();
		int* search(char  words[][100], int size_words);
		Vocabulary * getVocabulary();
};
#endif