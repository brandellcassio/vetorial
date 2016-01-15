#include "Postlist.h"

void Postlist::addDocument(int did, int idx){
	pnode *tmp;
	
	if(idx >= size){
		//	printf("realloc one time again\n");
			realloc();
	}
	//Fprintf("idx: %d\n", idx);
	if(searchDocument(&plist[idx], did))
		return;

	tmp = (pnode *) malloc(sizeof(pnode));
	tmp->did = did;
	tmp->tf = 1;
	tmp->next = plist[idx].head;
	plist[idx].head = tmp;
	plist[idx].nid++;
			
}

int Postlist::searchDocument(postlist *list, int did){
	pnode * tmp;
	for (tmp = list->head;tmp && 
			tmp->did != did; tmp = tmp->next);
	if (tmp){
		tmp->tf++;
		return 1;
	}
	return 0;
}


void Postlist::idfCalc(int totaldocs, int totalterms){
	double idfdouble;
	for (int i=0;i<totalterms;i++){
		idfdouble = plist[i].nid;
		plist[i].idf = log(totaldocs/idfdouble);
		
		//printf("docs:%d\t alldocs:%d\tIDF:%.5lf\tallterms:%d\n",
		//plist[i].nid,totaldocs,plist[i].idf, totalterms);
	}
}

void Postlist::dumpToFile(int totalDocs, int allids){
	FILE * file = fopen("postlist.idx", "w");
	pnode * tmp;
	fprintf(file, "%d\n",allids);
	for (int i = 0; i < allids; i++){
		fprintf(file, "%d %.3lf\n",plist[i].nid, plist[i].idf);
		for (tmp = plist[i].head; tmp; tmp = tmp->next)
			fprintf(file, "%d %d\n",tmp->did, tmp->tf);
	}
	fclose(file);
	createModel(totalDocs, allids);
}
void Postlist::readFile(){
	FILE * file = fopen("postlist.idx", "r");
	pnode * tmp;
	int n_size, i, j;
	fscanf(file, "%d", &n_size);
	size = n_size;
	free(plist);
	plist = (postlist *)malloc(sizeof(postlist)*size);
	memset(plist, 0, sizeof(plist));
	
	for(i = 0; i < size; i++){
		fscanf(file, "%d", &plist[i].nid);
		fscanf(file, "%lf", &plist[i].idf);
		for (j = 0; j < plist[i].nid; j++){
			tmp = (pnode *) malloc(sizeof(pnode));
			fscanf(file, "%d", &tmp->did);
			fscanf(file, "%d", &tmp->tf);

			tmp->next = plist[i].head;
			plist[i].head = tmp->next;
		}
	}
	fclose(file);

}

void Postlist::realloc(){
	postlist * newlist = (postlist *)malloc(sizeof(postlist)*2*size);
	pnode *rm, *tmp, *create;
	int oldsize = size, i, j;
	size*=2;
	memset(newlist, 0, sizeof(newlist));
	for (i = 0; i < oldsize; i++){
		//newlist[i] = plist[i];
		newlist[i].nid = plist[i].nid;
		rm = plist[i].head;
		for (tmp = rm->next; tmp; tmp = tmp->next){
			create = (pnode *) malloc(sizeof(pnode));
			create->tf = rm->tf;
			create->did = rm->did;
			create->next = newlist[i].head;
			newlist[i].head = create;
			free(rm);
			rm = tmp;
		}

		create = (pnode *) malloc(sizeof(pnode));
		create->tf = rm->tf;
		create->did = rm->did;
		create->next = newlist[i].head;
		newlist[i].head = create;
		free(rm);

	}
	free(plist);
	plist = newlist;
}

void Postlist::createModel(int totalDocs,int allterms){
	unsigned tf; 
	double module, coord;
	
	FILE * file = fopen("model_vector.idx", "w");
	fprintf(file, "%d %d\n",totalDocs, allterms);
	for (int i = 1; i <= totalDocs; i++){
		module = 0;
		for (int j = 0; j < allterms;j++){
			tf = containsDocument(plist[j], i);
			coord = tf*plist[j].idf;
			module += pow(coord, 2);
			fprintf(file, "%.3lf ",coord); 
		}
		fprintf(file, "%.3lf\n", sqrt(module));
	}
	fclose(file);
}


int Postlist::containsDocument(postlist list, int did){
	pnode * tmp;
	for (tmp = list.head;tmp && 
			tmp->did != did; tmp = tmp->next);
	if (tmp)
		return tmp->tf;

	return 0;
}

float Postlist::getIDFbyPos(int idx){
	return (float) plist[idx].idf;
}
