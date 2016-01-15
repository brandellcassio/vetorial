#include "Indexer.h"

int Indexer::isNumber(char * str){
	int i;
	for (i = 0; i < strlen(str);i++){
		if (str[i] > 57 || str[i] < 48)
				return 0;
	}

	return 1;
}

void Indexer::readFiles(char *path){
	FILE *file;
	char name[20], *substr;
	char buffer[TAMBUFF];
	int indFiles;
	int did;
	int contDocs = 0, c1 = 0, c2 = 0;
	//flags
	unsigned isRN, isAB, isTI;
	//hashing
	//hash *table;
	Vocabulary vocal;
	for (indFiles = 74; indFiles < 80; indFiles++){
			sprintf(name, "%s%d",path,indFiles);
			file = fopen(name, "r");
			isAB = isTI = 0;
			if (file){

				while(fscanf(file, "%s", buffer) != EOF){
					
					if(!strcmp("RN", buffer)){
					
						fscanf(file, "%s", buffer);
						if(isNumber(buffer)){
							isAB = isTI = 0;
							contDocs++;
							did = atoi(buffer);
							//printf("%s\n", buffer);
						}
					}
		
					if(!strcmp("TI", buffer) && !isTI){
						isTI = 1;
						c2++;
						fscanf(file, "%s", buffer);
						while(strcmp("SO", buffer)){
							normalizestr(buffer);
							lowerstr(buffer);
							//printf("%s\n", buffer);
							if(strlen(buffer))
								vocal.addWord(buffer, did);
							fscanf(file, "%s", buffer);
						}
					
					}

						
					if ((!strcmp("EX", buffer) || !strcmp("AB", buffer) /* doc 00124 || !strcmp("EØ", buffer)*/) && !isAB){
						isAB = 1;
						fscanf(file, "%s", buffer);
						//printf("is here\n");
						c1++;
						while(strcmp("RF", buffer) && strcmp("PN", buffer)){
							lowerstr(buffer);
							normalizestr(buffer);
							if(strlen(buffer))
								vocal.addWord(buffer,did);
							fscanf(file, "%s", buffer);
						}
					}
				}

				//printf("abs %d titles %d docs %d\n", contDocs, c2, c1);
				fclose(file);
				
			}
			else
				printf("error on open file \"%s\n\"", name);	
			
	}
	vocal.idfcalculing(contDocs);
	vocal.dumpToFile(contDocs);

}

void Indexer::normalizestr(char *str){
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

void Indexer::lowerstr(char *str){
	for (int i = 0; i < strlen(str); i++){
			if (str[i] >= 65 && str[i] <= 90)
					str[i] += 32;
	}
	
}

