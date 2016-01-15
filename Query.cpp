#include "Query.h"
#include <time.h>

Query::Query(){

}
Query::~Query(){
	
}

void Query::loadQuery(char *path){
	char totalpath[MAX_CHR];
	FILE * orig;
	char buff[MAX_CHR];	
	int i,	size;

	sprintf(totalpath, "%squery", path);
	orig = fopen(totalpath, "r");
	if(orig){
		for (i = 0; i < QRY_NUMBER;i++){
			size = 0;
			//QN
			fscanf(orig, "%s", buff);
			printf("%s ", buff);

			fscanf(orig, "%s", buff);
			printf("%s ", buff);
			
			queries[i].num = i + 1;
			//QU
			fscanf(orig, "%s", buff);
			fscanf(orig, "%s", buff);
			while(strcmp(buff, "NR")){
				normalizestr(buff);
				lowerstr(buff);
				strcpy(queries[i].words[size++], buff);
				
				fscanf(orig, "%s", buff);
			}

			//if(!i)
			//printf("\n");
			queries[i].word_size = size;
			//NR
			fscanf(orig, "%d", &queries[i].rank_size);
			queries[i].relevants = (int *) malloc(sizeof(int)*queries[i].rank_size);

			fscanf(orig, "%s", buff);
			for (int j = 0; j < queries[i].rank_size; j++){
				fscanf(orig, "%d", &queries[i].relevants[j]);
				fscanf(orig, "%s", buff);
				//if (!i)
				//	printf("%d ", queries[i].relevants[j]);
			}

			
			
		}
		fclose(orig);
	}
}

void Query::normalizestr(char *str){
	int i, j = 0;
	 while(j < strlen(str)){
		if(((str[j] >= 33 && str[j] <= 47) ||
	   		(str[j] >= 58 && str[j] <= 64) ||
	   		(str[j] >= 91 && str[j] <= 96) ||
	   		(str[j] >= 33 && str[j] <= 47) ||
	   		(str[j] >= 123 && str[j] <= 126)) && str[j] != '-'){
			for (i = j + 1;i < strlen(str);i++)
				str[i - 1] = str[i];
			str[i - 1] = '\0';
			j--;
		}
		j++;
	}
	
}

void Query::lowerstr(char *str){
	for (int i = 0; i < strlen(str); i++){
			if (str[i] >= 65 && str[i] <= 90)
					str[i] += 32;
	}
	
}

void Query::calculate(VectorModel *vm){
	int i, *result, j;
	clock_t begin, end;
	double time_spent, pmap, p10, geralmap = 0, geralp10 = 0;
	double time_spentTotal = 0.0;
	for (i = 0; i < QRY_NUMBER;i++){
		begin = clock();
		result = vm->search(queries[i].words, queries[i].word_size);
		end = clock();	
		time_spent = (double)(end - begin)/CLOCKS_PER_SEC;
		printf("Tempo gasto na consulta %d: %.3lf s ",queries[i].num, time_spent);
		time_spentTotal+= time_spent;
		printf("\n");
		printf("TOP 10\n");
		for (j = 0; j < 10;j++){
			printf("%d: %d\n",j + 1,result[j]);
		}

		pmap = map(i, result);
		p10 = precisonAtTen(i, result);

		printf("MAP : %.3lf, P@10 : %.3lf\n", pmap, p10);
		geralp10 += p10;
		geralmap += pmap;
	}
	
	
	printf("Tempo total gasto %.3lf s ", time_spentTotal);
	printf("MAP Geral: %.3lf, P@10 Geral: %.3lf\n", geralmap/QRY_NUMBER, geralp10/QRY_NUMBER);
}

double Query::precisonAtTen(int idx, int * result){
	int count = 0;
	
	for (int i = 0; i < 10; i++){
			if (binarySearch(result[i], queries[idx].relevants,queries[idx].rank_size)){
				count++;
			}
		}
	
	return count / 10.0;
}

double Query::map(int idx, int *result){
	int relevants = 0;
	double precisions = 0.0, total, idouble;
	for (int i = 0; relevants != queries[idx].rank_size ; i++){
		if (binarySearch(result[i], queries[idx].relevants,queries[idx].rank_size)){
			relevants++;
			idouble = i+1;
			precisions += relevants/idouble;
		}	
	}
	total = relevants;
	
	return precisions / total;
}

int Query::binarySearch(int element, int v[], int size){
	int init = 0, end = size - 1;
	int mid = (init  + end)/2;

	while(init <= end){
		
		if (element > v[mid])
			init = mid + 1;
		else if (element < v[mid])
			end = mid - 1;
		else 	
			return 1;
		mid = (init  + end)/2;		
	}

	return 0;
}