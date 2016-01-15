#include "VectorModel.h"

VectorModel::VectorModel(){

}

VectorModel::~VectorModel(){

}

VectorModel::VectorModel(int size_docs, int size_terms){
	this->size_docs = size_docs;
	this->size_terms = size_terms;
	initVectors();
}

void VectorModel::initVectors(){
	vectors = (vector *)malloc(sizeof(vector) * size_docs);
	memset(vectors, 0, sizeof(vectors));
	for (int i = 0; i < size_docs;i++){
		vectors[i].coord = (float *)malloc(sizeof(float) * size_terms);
		memset(vectors[i].coord, 0, sizeof(vectors[i].coord));
	}
}

void VectorModel::readFile(){
	FILE * file = fopen("model_vector.idx", "r");
	int i, j;
	
	if (file){
		fscanf(file, "%d", &size_docs);
		fscanf(file, "%d", &size_terms);
		initVectors();

		for(i = 0; i < size_docs; i++){
			for (j = 0; j < size_terms; j++){
				fscanf(file,"%f", &vectors[i].coord[j]);
			}
			fscanf(file, "%f", &vectors[i].module);
	
		}
		fclose(file);
	}

	vocabulary.readFile();
}

int * VectorModel::search(char words[][100], int size_words){
	float * similarities;
	vector query = createVector(words, size_words);
	similarities = calculateSimilarity(query);
	return generateRank(similarities);
}


vector VectorModel::createVector(char words[][100], int size_words){
	int i, coord;
	query_size = 0;
	vector v;
	v.coord = (float *) malloc(sizeof(float) * size_terms);
	memset(v.coord, 0, sizeof(v.coord));
	//printf("words: ");
	for (i = 0; i < size_words;i++){
		coord = vocabulary.findWord(words[i]);
		if(coord){
			coord--;
			if(!v.coord[coord])
				query_coord[query_size++] = coord;
			
			v.coord[coord]+= vocabulary.getlist()->getIDFbyPos(coord);
		//	printf("coord: %d with value: %.3f\n", coord, v.coord[coord]);
		}
	}
	

	return v;
}
float * VectorModel::calculateSimilarity(vector query){
	float *similarities = (float *) malloc(sizeof(float)* size_docs);
	float sum;

	for (int i = 0; i < size_docs; i++){
		sum = 0;
		for(int j = 0; j < query_size; j++){
			int k = query_coord[j];
			sum += query.coord[k] * vectors[i].coord[k];
		}
		similarities[i] = sum/vectors[i].module; 
	}

    return similarities;
}

int * VectorModel::generateRank(float similarities[]){
	int * docs= (int *) malloc(size_docs * sizeof(int));
	
	for (int i = 0; i < size_docs;i++){
		docs[i] = i + 1;
	}

	//selection sort
/*	for (int i = 0; i < size_docs - 1; i++){
		for (int j = i; j < size_docs; j++){
				if(similarities[i] < similarities[j]){
						
				}
		}
	}*/
	quick(docs, similarities, 0, size_docs - 1);
	
	return docs;
}

void VectorModel::quick(int idx[], float similarities[], int init, int end){
   int i = init;
   int j = end;
   float pivot;
   int aux_id;
   float aux_sim;
   if (i < j){
	pivot  = similarities[(i+j)/2];
	do {
        while (similarities[i] > pivot) i++;
        while (similarities[j] < pivot) j--;
            
	    if (i <= j){
	  		//similarities
			aux_sim = similarities[i];
			similarities[i] = similarities[j];
			similarities[j] = aux_sim;
			
			//docs
			aux_id = idx[i];
			idx[i] = idx[j];
			idx[j] = aux_id;
	       	i++;
	       	j--;
	    }
  
	}while( i <= j);

    quick(idx,similarities, i, end);
    quick(idx,similarities, init, j);
         	

   }
}

Vocabulary * VectorModel::getVocabulary(){
	return &vocabulary;
}