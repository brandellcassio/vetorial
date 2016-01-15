#include "Query.h"

int main(int argc, char * argv[]){
	Query query;
	VectorModel model;
	model.readFile();
	printf("Load files completed. . .\n");
	query.loadQuery(argv[1]);
	printf("Load query completed. . .\n");
	query.calculate(&model);
}