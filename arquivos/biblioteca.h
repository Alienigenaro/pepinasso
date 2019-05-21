//*******************************************************///
//*******************************************************///
//*****Caio Cesar Aguiar Alarcon*************************///
//*****N°7241109*****************************************///
//*******************************************************///
//*******************************************************///
//*******************************************************///
//*******************************************************///

#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <stdlib.h>     //exit,free, malloc
#include <stdio.h>      //scanf,printf,sscanf,sprintf,fopen,fclose,fgets,fwrite,fread,fseek,
#include <string.h>     //strlen,strcpy,strcmp 

//Definição do caractere representante do lixo de memória.
#define LIXO '@'
#define MAX_STRING 300
#define DISK_PAGE_SIZE 16000

//Definição dos tipos abstratos de dados:
typedef char byte;

typedef struct{//Definição da página de disco vazia no tipo diskpag.
	byte data[DISK_PAGE_SIZE];
}freediskpag;

typedef struct{//Definição da página de disco de cabeçalho.
	char status;
	byte topoPilha[4];//4 Bytes para armazenar um int sem ativar o padding do campo anterior...
	char tagCampo1;
	char desCampo1[55];

	char tagCampo2;
	char desCampo2[55];

	char tagCampo3;
	char desCampo3[55];

	char tagCampo4;
	char desCampo4[55];

	char tagCampo5;
	char desCampo5[55];
	byte lixo[DISK_PAGE_SIZE - 285];
}diskpagHeader;

typedef struct{//Os campos todos são byte para evitar struct padding.
	byte removido;
	byte encadeamento[4];
	byte nroInscricao[4];
	byte nota[8];
	byte data[10];
	byte variavel[53];
}reg;

typedef struct{//Definição do tipo página de disco de dados.
	reg data[DISK_PAGE_SIZE/80];
}diskpagData;

typedef union{//Definição flexível da página de disco.
	freediskpag free;
	diskpagHeader header;
	diskpagData data;
}diskpag;

//Funcionalidades:
//Funcionalidade 1: Converte o csv em binário e salva no arquivo
void csv_to_bin(char * Name);
//Funcionalidade 2: Exibe na tela todos os registros (faz um dump dos registros formatados para a saída padrão)
void dump_file(char * aux);
//Funcionalidade 3: Faz uma busca no registro específico e imprime os resultados.
void find_register(char *binName, char * fieldName, char * ToSearch);
//Funcionalidade 4: Encontra um registro específico a partir da posição no arquivo.
void find(char *binName,char * ToSearch);

//Funcionalidade 5: Remove logicamente um registro baseando-se na abordagem dinâmica de reaproveitamento de registros logicamente removidos.
reg * removeReg(char * FileName, char * fieldName, char * ToSearch);

//Protótipo das funções auxiliares criadas:
void * newDiskPage();//Retorna uma página de disco preenchida com o caractere de lixo de memória

diskpagHeader * newHeader();	  //Retorna um cabeçalho já preenchido e indicando arquivo corrompido com rrn = -1

//Funções de abertura ou fechamento de arquivos
//Abre o arquivo para leitura ou escrita considerando o cabeçalho.
FILE * OpenBinFile(char str[],diskpagHeader ** header,const char * modo);

//Fecha o arquivo considerando o cabeçalho.
void CloseBinFile(FILE * fp, diskpagHeader * header);

//Divide a string em várias sub strings de acordo com um delimitador.
int split(char str[], char delimiter, char ** vet);	

//Imprime um registro na formatação especificada.
void printReg(reg * aux);

//Funções de leitura da estrutura r:
int insc(reg * r);			//Retorna o número de inscrição a partir de um registro especifico.
double nota(reg * r);		//Retorna a nota a partir de um registro específico.
char * data(reg * r);		//Retorna a data a partir de um registro específico.
int tc(reg * r);			//Retorna o tamanho da string da cidade a partir de um registro específico.
char * cidade(reg * r);		//Retorna a string da cidade a partir de um registro específico.
int tn(reg * r);			//Retorna o tamanho da string do nome a partir de um registro específico.
char * nome(reg * r);		//Retorna a string do nome da cidade a partir de um registro específico.
void * malloc_A(int size);

#endif

