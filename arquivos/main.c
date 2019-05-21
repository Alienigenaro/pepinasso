//*******************************************************///
//*******************************************************///
//*****Caio Cesar Aguiar Alarcon*************************///
//*****N°7241109*****************************************///
//*******************************************************///
//*******************************************************///
//*******************************************************///
//*******************************************************///

#include "biblioteca.h"
int main(int argc, char * argv[]){
	int option, n ,i;
	//FILE * fpbin;
	char FileName[MAX_STRING];
	char fieldName[MAX_STRING];
	char ToSearch[MAX_STRING];
	unsigned char * aux;
	scanf("%d",&option);//Faz a leitura do número referente à opção.


	switch(option){//Encaminha o fluxo de execução de acordo com a opção selecionada.
		case 1:
			scanf("%s",FileName);
			csv_to_bin(FileName);
		break;
		case 2:
			scanf("%s",FileName);
			dump_file(FileName);
		break;
		case 3:
			scanf("%s",FileName);
			scanf("%s",fieldName);
			scanf(" %[^\n]",ToSearch);
			find_register(FileName, fieldName, ToSearch);
		break;
		case 4:
			scanf("%s",FileName);
			scanf(" %s",ToSearch);
			find(FileName, ToSearch);
		break;
		
		case 5:
			scanf("%s",FileName);
			scanf(" %d",&n);
			for(i=0;i<n;i++){
				scanf("%s",fieldName);
				scanf(" %[^\n]",ToSearch);
				aux =(unsigned char*) removeReg(FileName,fieldName, ToSearch);
			}
			//fpbin = fopen(FileName, "rb+");
			//fwrite(aux,416000,1,fpbin);
			for(i = 0; i < 416000; i += sizeof(unsigned char)) {
				printf("%02X ", aux[i]);
				if((i + 1) % 16 == 0)printf("\n");
			}
			//fclose(fpbin);
			free(aux);
		break;
		
		default:
		printf("na\n");

	}

	 return 0;
}



