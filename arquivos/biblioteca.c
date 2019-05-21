//*******************************************************///
//*******************************************************///
//*****Caio Cesar Aguiar Alarcon*************************///
//*****N°7241109*****************************************///
//*******************************************************///
//*******************************************************///
//*******************************************************///
//*******************************************************///

#include "biblioteca.h"

const reg RegLixo = (reg){//Declaração de um registro completo com o caractere definido como lixo
	LIXO,{LIXO,LIXO,LIXO,LIXO},{LIXO,LIXO,LIXO,LIXO},{LIXO,
	LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,LIXO},{LIXO,LIXO,LIXO,
	LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,LIXO},{LIXO,LIXO,LIXO,
	LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,
	LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,
	LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,
	LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,
	LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,LIXO,LIXO}
};

//***FUNCIONALIDADES******************//

//Transfere os registros de um arquivo no formato csv para um arquivo no formato binário.
void csv_to_bin(char * Name){
	char str[MAX_STRING];
	char * fields[5];
	int regCount=0,i=0;
	FILE * fpcsv;
	FILE * fpbin;
	diskpagHeader * header=0;
	diskpagData * pag;
	reg * aux;

	fpcsv = fopen(Name,"r");
	if(!fpcsv){
		printf("Falha no carregamento do arquivo.");
		exit(0);
	}


	fpbin = OpenBinFile("arquivoTrab1si.bin", &header,"wb");

	pag = (diskpagData*)newDiskPage();
	aux = pag->data;

	fgets(str, sizeof str, fpcsv);//Joga fora a primeira linha que é o cabeçalho do csv...

	while (fgets(str, sizeof str, fpcsv)) {//Efetua a leitura do csv linha por linha
		split(str,',',fields);
		aux[regCount].removido = '-';
		*((int*)aux[regCount].encadeamento) = -1;

		if(*fields[0])//Campo nroInscricao
			sscanf(fields[0],"%d",((int*)&aux[regCount].nroInscricao));
		else
			*((int*)aux[regCount].nroInscricao) = -1;

		if(*fields[1])//Campo nota
			sscanf(fields[1],"%lf",(double*)&aux[regCount].nota);
		else{
			*((double*)&aux[regCount].nota) = -1.0;
		}
		if(*fields[2]){//Campo data
			sscanf(fields[2],"%[^\n]s",aux[regCount].data);
			//printf("%d %s",*((int*)&aux[regCount].nroInscricao),aux[regCount].data);
			aux[regCount].data[10]=LIXO;//Devolve o lixo que a função sscanf removeu com o '\0'
		}
		else
			*aux[regCount].data='\0';

		//campos de tamanho variável
		i=0;
		if(*fields[3]>32){//Primeiro campo variável, cidade
			i+=5;
			sscanf(fields[3],"%[^\n]s",&aux[regCount].variavel[i]);//Lê do campo direto pra estrutura
			*((int*)&aux[regCount].variavel[0])=strlen(&aux[regCount].variavel[i])+2;//Guarda o tamanho na estruct
			aux[regCount].variavel[4]='4';
			i += strlen(fields[3])+1;
		}

		if(*fields[4]>32){//Segundao campo variável, nome da escola
			sscanf(fields[4],"%[^\n]s",&aux[regCount].variavel[i+5]);//Le do campo para a estrutura
			*((int*)&aux[regCount].variavel[i])=strlen(&aux[regCount].variavel[i+5])+2;//Tamanho na struct
			aux[regCount].variavel[i+4]='5';
		}

		if(++regCount==200){//Buffer cheio...
			regCount = 0;//Reiniciar o uso do buffer
			//salvar o buffer no disco:
			fwrite(aux,DISK_PAGE_SIZE,1,fpbin);
			//limpa o buffer
			for(i=0;i<200;i++){
				aux[i] = RegLixo;
			}
		}
	}

	fwrite(aux,regCount*80,1,fpbin);//registrar o buffer em disco

	fclose(fpcsv);	//Fecha o arquivo csv
	free(pag);   	//Libera o espaço utilizado
	CloseBinFile(fpbin, header);//Fecha o arquivo binário considerando seu cabeçalho
	printf("arquivoTrab1si.bin");
}

//Exibe todos os registros.
void dump_file(char * binName){
	FILE * fpbin;
	int i,j=0;
	diskpagHeader * _header;
	diskpagData * _data;
	reg * aux;
	_data = (diskpagData*)newDiskPage();
	aux = _data->data;

	fpbin = OpenBinFile(binName,&_header,"rb");
	j++;
	while(fread(_data,DISK_PAGE_SIZE,1,fpbin))
		for(i=0,j++;i<200;i++){
			printReg(&aux[i]);
		}

	printf("Número de páginas de disco acessadas: %d",j);
	fclose(fpbin);
	free(_data);   	//Libera o espaço utilizado
}

//Imprime todos os registros a partir de uma busca por campo.
void find_register(char *binName, char * fieldName, char * ToSearch){
	FILE * fpbin;
	int i,j=0,nreg=0,fim=0;
	char S[MAX_STRING];
	diskpagHeader * _header;
	diskpagData * _data;
	reg * aux;
	_data = (diskpagData*)newDiskPage();
	aux = _data->data;

	fpbin = OpenBinFile(binName,&_header,"rb");
	j++;
	
	while(!fim && fread(_data,DISK_PAGE_SIZE/200,200,fpbin)){
		j++;
		for(i=0;i<200;i++){
			if(!strcmp(fieldName,"data")){
				sprintf(S,"%.10s",data(&aux[i]));
				if(!strcmp(ToSearch,S)){
					printReg(&aux[i]);
					nreg++;
				}
			}

			if(!strcmp(fieldName,"nroInscricao")){
				sprintf(S,"%d",insc(&aux[i]));
				if(!strcmp(ToSearch,S)){
					printReg(&aux[i]);
					nreg++;
					fim = 1;
				}
			}

			if(!strcmp(fieldName,"nota")){
				sprintf(S,"%.1lf",nota(&aux[i]));
				if(!strcmp(ToSearch,S)){
					printReg(&aux[i]);
					nreg++;
				}
			}

			if(!strcmp(fieldName,"cidade")){
				sprintf(S,"%s",cidade(&aux[i])?cidade(&aux[i]):"");
				if(!strcmp(ToSearch,S)){
					printReg(&aux[i]);
					nreg++;
				}
			}

			if(!strcmp(fieldName,"nomeEscola")){
				sprintf(S,"%s",nome(&aux[i])?nome(&aux[i]):"");
				if(!strcmp(ToSearch,S)){
					printReg(&aux[i]);
					nreg++;
				}
			}
		}
		
		
	}
	
		
	if(nreg)
		printf("Número de páginas de disco acessadas: %d",j);
	else
		printf("Registro inexistente.");
	fclose(fpbin);
	free(_data);
}

//Busca um registro baseado em sua posição no arquivo.
void find(char *binName,char * ToSearch){
	FILE * fpbin;
	int j=0,k;
	diskpagHeader * _header;
	diskpagData * _data;
	reg * aux;
	_data = (diskpagData*)newDiskPage();
	aux = _data->data;
	
	
	fpbin = OpenBinFile(binName,&_header,"rb");
	j++;
	
	sscanf(ToSearch,"%d",&k);
	
	fseek(fpbin,k*DISK_PAGE_SIZE/200,SEEK_CUR);
	j++;
	if(!fread(_data,DISK_PAGE_SIZE/200,1,fpbin))
		printf("Registro inexistente.");
	else{
		printReg(aux);
		printf("Número de páginas de disco acessadas: %d",j);
	}
	
	fclose(fpbin);
	free(_data);
}

//***FUNCIONALIDADES TRABALHO 2*******//
int match(char * fieldName,char * ToSearch, reg * REG){
	char S[MAX_STRING];
	
	if(!strcmp(fieldName,"data")){
		sprintf(S,"\"%.10s\"",data(REG));
		if(!strcmp(ToSearch,S)){
			return 1;
		}
	}

	if(!strcmp(fieldName,"nroInscricao")){
		sprintf(S,"%d",insc(REG));
		if(!strcmp(ToSearch,S)){
			return 1;
		}
	}

	if(!strcmp(fieldName,"nota")){
		sprintf(S,"%.1lf",nota(REG));
		if(!strcmp(ToSearch,S)){
			return 1;
		}
	}

	if(!strcmp(fieldName,"cidade")){
		sprintf(S,"\"%s\"",cidade(REG)?cidade(REG):"");
		if(!strcmp(ToSearch,S)){
			return 1;
		}
	}

	if(!strcmp(fieldName,"nomeEscola")){
		sprintf(S,"\"%s\"",nome(REG)?nome(REG):"");
		if(!strcmp(ToSearch,S)){
			return 1;
		}
	}
	
	return 0;
	
}
reg * removeReg(char * FileName, char * fieldName, char * ToSearch){
	static reg * ramfile=NULL;			//Ponteiro para o arquivo na ram
	static diskpagHeader * _header=NULL;	//Ponteiro para o header
	static int regcount;
	static int * rrntopo;
	int size,i;
	FILE * fpbin;
	
	
	//verificar se arquivo está aberto já
	if(!ramfile){//Se ainda não tem arquivo
		//Encontrar o tamanho do arquivo
		fpbin = OpenBinFile(FileName,&_header,"rb+");
		
		
		//printf("%d\n",rrntopo);
		fseek(fpbin, 0, SEEK_END);		//Move para o final o filepointer
		size = ftell(fpbin);			//encontra o tamanho do arquivo
		rewind(fpbin);					//Move o filepointer para o inicio
		//Criar um vetor de registros do tamanho do arquivo
		ramfile = (reg*)malloc_A(size);
		rrntopo = ((int*)((diskpagHeader * )ramfile)->topoPilha);
		//carregar o arquivo para a ram
		regcount = fread(ramfile,80,size/80,fpbin);
		CloseBinFile(fpbin, _header);//Fecha já que tá tudo em ram
	}
	//Aqui eu tenho o arquivo em ram
	//Agora é necessário buscar e efetivar a remoção
	//Começa do 200 pois é o tamanho do cabecalho em reg
	for(i=200;i<regcount;i++){//Percorrer o arquivo reg a reg
		//Verifica se há correspondência
		//&ramfile[i] é o reg em questão
		if(match(fieldName,ToSearch, &ramfile[i])){
			//printReg(&ramfile[i]);
			ramfile[i] = RegLixo; //Preenche o registro com o lixo
			*((int*)(ramfile[i].encadeamento)) = *rrntopo;
			ramfile[i].removido = '*';
			*rrntopo = i-200;
			
			//printf("%d\n",i-200);
		}
		
		
	}
	
	return ramfile;
}

//***Funções de manipulação da struct
int insc(reg * r){			//Retorna o numero de inscrição.
	return *(int*)r->nroInscricao;
}
double nota(reg * r){		//Retorna a nota.
	return *(double*)r->nota;
}
char * data(reg * r){		//Regorna a data.
	return r->data;
}
int tc(reg * r){			//Retorna o tamanho da string que guarda o nome da cidade.
	if(r->variavel[4]=='4')
		return *((int*)&r->variavel[0])-2;
	return 0;
}
char * cidade(reg * r){		//Retorna a string que guarda o nome da cidade.
	if(tc(r)==0)return NULL;
	return ((char*)&r->variavel[5]);
}
int tn(reg * r){			//Retorna o tamanho da string que guarda o nome da escola.
    if(r->variavel[4]=='5')
		return *((int*)&r->variavel[0])-2;
	else if(r->variavel[0]==LIXO)return 0;

	if(r->variavel[*((int*)&r->variavel[0])+8]=='5')
		return r->variavel[*((int*)&r->variavel[0])+4]-2;
	return 0 ;
}
char * nome(reg * r){		//Retorna a string que guarda o nome da escola.
	if(tn(r)==0)return NULL;
	if(r->variavel[4]=='5')
		return ((char*)&r->variavel[5]);
    if(r->variavel[*((int*)&r->variavel[0])+8]=='5')
		return (char*)&r->variavel[*((int*)&r->variavel[0])+9];
	return NULL ;
}
void printReg(reg * aux){	//Imprime um registro no formato especificado.
	if(aux->removido == '*')
		return;
	if(insc(aux))
		printf("%d",insc(aux));

	if(nota(aux) != -1.0)
		printf(" %.1lf",nota(aux));

	if(*data(aux) > 32)
		printf(" %.10s",data(aux));

	if(tc(aux)!= 0){
		printf(" %d",tc(aux));
		printf(" %s",cidade(aux));
	}

	if(tn(aux)!=0){
		printf(" %d",tn(aux));
		printf(" %s",nome(aux));
	}
	printf("\n");
}


//Funções auxiliares:
//Divide uma string em várias partes de acordo com um caractere delimitador.
int split(char str[], char delimiter, char ** vet){
	int i,j=0;

	vet[j++]=str;
	for(i=0;i<MAX_STRING && str[i]!='\n';i++){
		if(str[i] == delimiter){
			str[i] = '\0';
			vet[j++]=&str[i+1];
		}
	}
	return j;
}

//Abre o arquivo binário para leitura ou escrita levando em consideração se ele se encontra corrompido.
FILE * OpenBinFile(char str[],diskpagHeader ** header, const char * modo){
	FILE * fpbin;
	*header = newHeader();
	if(modo[0]=='w'){
		fpbin = fopen(str,modo);//Abre o arquivo.bin
		//Aloca e cria o cabeçalho indicando arquivo corrompido
		fwrite(*header,DISK_PAGE_SIZE,1,fpbin);	//Escreve no arquivo
	}else{
		fpbin = fopen(str,modo);
		fread(*header,DISK_PAGE_SIZE,1,fpbin);

		if((*header)->status=='0'){
			printf("Falha no processamento do arquivo.");
			exit(0);
		}
	}
	return fpbin;
}

//Fecha o arquivo binário mas antes altera o cabecalho para não corrompido
void CloseBinFile(FILE * fp, diskpagHeader * header){
	//Altera o cabeçalho do arquivo para não corrompido e fecha o ponteiro
	header->status='1';
	fseek (fp , 0 , SEEK_SET );
	fwrite(header,DISK_PAGE_SIZE,1,fp);
	free(header);
	fclose(fp);
}

//Aloca e preenche o espaço alocado com o caractere definido em 'define LIXO'.
void * malloc_A(int size){
	void * aux;					 //Variável auxiliar.
	int i;						  //Contador.
	aux = malloc(size);			 //O espaço definido em bytes no argumento size é alocado.
	for(i=0;i<size;i++)			 //Um laço é usado para preencher o espaço alocado com o caractere definido em 'define LIXO'.
		((char*)aux)[i]=LIXO;
	return aux;					 //O endereço do espaço alocado é retornado.
}

//Aloca e retorna o espaço de uma página de disco 'limpa'
void * newDiskPage(){
	return (diskpag*)malloc_A(DISK_PAGE_SIZE);
}

//Cria uma página de disco de cabeçalho já preenchida
diskpagHeader * newHeader(){
	diskpagHeader * buffer;
	buffer = (diskpagHeader*)newDiskPage();

	//Ó A gambiarra moço! Cuidado que machuca as vista....
	*((int*)buffer->topoPilha)=-1;


	buffer->status = '0';

	buffer->tagCampo1 = '1';
	strcpy(buffer->desCampo1, "numero de inscricao do participante do ENEM");

	buffer->tagCampo2 = '2';
	strcpy(buffer->desCampo2, "nota do participante do ENEM na prova de matematica");

	buffer->tagCampo3 = '3';
	strcpy(buffer->desCampo3, "data");

	buffer->tagCampo4 = '4';
	strcpy(buffer->desCampo4, "cidade na qual o participante do ENEM mora");

	buffer->tagCampo5 = '5';
	strcpy(buffer->desCampo5, "nome da escola de ensino medio");
	return buffer;
}


