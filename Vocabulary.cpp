#include "Vocabulary.h"

unsigned int Vocabulary::fnhash(char *str){

	unsigned int hash = FNV_OFFSET_32, i;
  	for(i = 0; i < strlen(str); i++){
        	hash = hash * FNV_PRIME_32;
		hash = hash ^ (str[i]);
    	}
    	return hash%vocal_size;
}

void Vocabulary::initTable(){
	table = (hash *)malloc(sizeof(hash)*vocal_size);
	int i;
	for (i = 0; i < vocal_size;i++){
			table[i].head = NULL;
			table[i].size = 0;
	}
	//init_list();
	//return table;

}

int Vocabulary::addWord(char *str, int docid){
	word w;
	node *tmp;
	int index, nid;
	
	nid = findWord(str);
	
	if(!str)
			return -1;

	if (nid){
		list.addDocument(docid, nid - 1);	
		return 0;
	}

	
	if(verifySize())
		rehash();
	
	index = fnhash(str);
	tmp = table[index].head;

	w.id = ++local_id;
	strcpy(w.word,str);
	tmp = (node *)malloc(sizeof(node));
	tmp->data = w;
	tmp->next = table[index].head;
	table[index].head = tmp;
	table[index].size++;
	list.addDocument(docid, w.id - 1);

	return 0;
}

int Vocabulary::findWord(char * str){
	if (!str) return -1;
	int i = fnhash(str);
	int c = 0;
	//printf("w:%s h: %d size %d\n", str,i, table[i].size);
	node *tmp;
	//if(i == 261)
	//	printf("this fucking str :%s, len: %ld\n", str, strlen(str));
	for (tmp = table[i].head; tmp; tmp = tmp->next){
		//printf("\twc %s\n", tmp->data.word);
		if (!strcmp(tmp->data.word, str))
			return tmp->data.id;
	}

	return 0;
}

inline int Vocabulary::getSize(){
	return local_id;
}

inline int Vocabulary::verifySize(){
	return (local_id/(vocal_size * 2)) >= 0.75;
}


void Vocabulary::rehash(){
	int i, lstsize, index;
	node *tmp, *rm, *nnode;
	word w;
	hash * ntable = (hash *)malloc(sizeof(hash) * vocal_size * 2);
	memset(ntable, 0, sizeof(ntable));
	lstsize = vocal_size;
	vocal_size *=2;
	for (i = 0; i < lstsize; i++){
		rm = table[i].head;
		if(!rm)
			continue;

		for (tmp = rm->next; tmp; tmp = tmp->next){
			w.id = rm->data.id;
			strcpy(w.word, rm->data.word);
			index = fnhash(rm->data.word);
			nnode = (node *)malloc(sizeof(node));
			nnode->data = w;
			nnode->next = ntable[index].head;
			ntable[index].head = nnode;
			ntable[index].size++;
			free(rm);
			rm = tmp;
		}
		
		w.id = rm->data.id;
		strcpy(w.word, rm->data.word);

		index = fnhash(rm->data.word);
		nnode = (node *)malloc(sizeof(node));
		nnode->data = w;
		nnode->next = ntable[index].head;
		ntable[index].head = nnode;
		ntable[index].size++;
		free(rm);
	}

	free(table);
	table = ntable;
	
}

void Vocabulary::idfcalculing(int totalDocs){
	list.idfCalc(totalDocs, local_id);
}

void Vocabulary::dumpToFile(int totalDocs){
	FILE * file;
	node * tmp;
	
	file = fopen("vocabulary.idx", "w");
	if (file){
		fprintf(file, "%d\n",vocal_size);
		for(int i = 0; i < vocal_size; i++){
			fprintf(file, "%d %d\n",i, table[i].size);
			for (tmp = table[i].head; tmp; tmp = tmp->next)	
				fprintf(file, "%s %d\n",tmp->data.word, tmp->data.id);
		}
	}
	fclose(file);
	list.dumpToFile(totalDocs, local_id);
}

void Vocabulary::readFile(){
	FILE * file = fopen("vocabulary.idx", "r");
	int size, idx, id_size, i, j;

	node * tmp;
	if (file){
		fscanf(file, "%d", &size);
		vocal_size = size;
		free(table);
		initTable();
		
		for(i = 0; i < size; i++){
			fscanf(file, "%d", &idx);
			fscanf(file, "%d", &id_size);
			
			//printf("i: %d idx: %d id_size: %d\n",i,idx,id_size);
			//	printf()
			table[i].size = id_size;

			for (j = 0; j < id_size; j++){
				tmp = (node *) malloc(sizeof(node));
				fscanf(file, "%s", tmp->data.word);
				fscanf(file, "%d", &tmp->data.id);
				tmp->next = table[i].head;
				table[i].head = tmp;
			}
		}
		fclose(file);
		list.readFile();
	}
}

Postlist * Vocabulary::getlist(){
	return &list;
}
