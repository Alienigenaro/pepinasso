//*******************************************************///
//*******************************************************///
//*****Caio Cesar Aguiar Alarcon*************************///
//*****N°7241109*****************************************///
//*******************************************************///
//*******************************************************///
//*******************************************************///
//*******************************************************///

#include "biblioteca.h"

//Função sugerida para escrever o binario na tela
void binarioNaTela2(char *nomeArquivoBinario) {

	/* Escolha essa função se você já fechou o ponteiro de arquivo 'FILE *'.
	*  Ela vai abrir de novo para leitura e depois fechar. */

	unsigned char *mb;
	unsigned long i;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela2): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar? Se você não fechou ele, pode usar a outra função, binarioNaTela1, ou pode fechar ele antes de chamar essa função!\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);
	for(i = 0; i < fl; i += sizeof(unsigned char)) {
		printf("%02X ", mb[i]);
		if((i + 1) % 16 == 0)	printf("\n");
	}
	free(mb);
	fclose(fs);
}


int main(int argc, char * argv[]){
	int option, n, i;
	char FileName[MAX_STRING];
	char fieldName[MAX_STRING];
	char ToSearch[MAX_STRING];

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
				remove_register(FileName,fieldName, ToSearch);
			}
			//binarioNaTela2(FileName);
			
		break;
		default:
		printf("na\n");

	}

	 return 0;
}



