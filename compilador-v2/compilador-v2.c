#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> // Biblioteca que contém funções de manipulações de caracteres
#include <string.h>


// Declaração global de variáveis de arquivos
//------------------------------------------
FILE *Arquivo;   // Arquivo Entrada
FILE *Saida1;   // Arquivo Saida1
FILE *Saida2;  // Arquivo Saida2


// Declaracao global da estrutura
//---------------------------------
struct Token
{
	int       num; // num da struct
	int 	  tipo; // classifica o tipo do lexema
	short int lexema; // lexema valido
	char      token[15]; // token 
	char      padrao[100]; // padrao
	int       ocorrencia;  // ocorrencia, é o end. mem. desta struct
	struct    Token *prox; // ponteiro para a próx. struct da lista
};
typedef struct Token token;


// Funções que controlam a estrutura
//----------------------------------
void inicia(token *LISTA);
int  vazia(token *LISTA);
void insere_lexema(token *LISTA, int count);
int  verifica_repeticao_lexema(token *LISTA);

// Funções de analise lexica do programa
//--------------------------------------
void tabela_simbolos(token *novo, int count, int posicao_atual);
int  analisa_lexema (token *LISTA, short int lexema, int count);


// Função Principal, inicio do programa
//-------------------------------------
int main (void)
{	
	int  count = 1; // contador das posicoes dos caracteres no arquivo
	short int lexema;

	// Validando e abrindo o arquivo com a expressao de entrada
	if ((Arquivo = fopen("entrada.txt", "r")) == NULL) 
	{	
		printf("\nNao foi possivel abrir o arquivo!");
		printf("\nVerifique o arquivo de entrada.txt");
		exit(1);
	}
	else 
	{
		// Criando um arquivo vazio de saida1.txt
		Saida1 = fopen("saida1.txt", "w");
		// Criando arquivo vazio de saida2.txt
		Saida2 = fopen("saida2.txt", "w");

		//alocação dinamica da estrutura Lista
		token *LISTA = (token *)malloc(sizeof(token)); 
        inicia(LISTA);

		// Enquanto o fim do Arquivo nao foi atingido
		// Lê um caractere por vez do Arquivo
		while ((lexema = fgetc(Arquivo)) != EOF)
		{	
			// Envia esse caractere para a funcao analisar e classificar
			count = analisa_lexema(LISTA, lexema, count);
			count++;
		}
		free(LISTA);
		fclose(Saida1);
		fclose(Saida2);
		printf("\nArquivo de Saida1.txt pronto!"); 
		printf("\nArquivo de Saida2.txt pronto!\n"); 
	}
	fclose(Arquivo);
	return 0;
}


// Funções
//--------
void inicia(token *LISTA)
{
    LISTA -> prox = NULL;
    LISTA -> num = 0;
}

int vazia(token *LISTA)
{
	if(LISTA->prox == NULL)
		return 1;
	else
		return 0;
}

void insere_lexema(token *LISTA, int count) 
{
	int posicao_atual;
    token *novo=(token *) malloc(sizeof(token)); //alocando uma nova estrutura!
	
	if(!novo)
	{
		printf("Sem memoria disponivel!\n");
		exit(1);
	}

    novo->lexema = LISTA->lexema;
	novo->tipo = LISTA->tipo;
	

	posicao_atual = verifica_repeticao_lexema(LISTA);

	// printf("Retorno: %x", posicao_atual);
	// printf("\nLexema que vai para Tab Sinonimo: %c", LISTA->lexema);
	// novo->ocorrencia = posicao_atual;
    tabela_simbolos(novo, count, posicao_atual);
	
	
	//novo->ocorrencia = &novo->prox;
	// posicao_atual = 0;
	// tabela_simbolos(novo, count, posicao_atual);
	// printf("\nNao tem ocorrencia duplicada! %x\n", posicao_atual);

	// tabela_simbolos(novo, count, posicao_atual);
	novo->prox = NULL;
	if(vazia(LISTA))
		LISTA->prox = novo;
	else{
		token *tmp = LISTA->prox;
		
		while(tmp->prox != NULL)
			tmp = tmp->prox;
		
		tmp->prox = novo;
	}
}

int verifica_repeticao_lexema(token *LISTA)
{
	int posicao_atual;
	int posicao_struct;
	short int lexema_atual;
	short int lexema_struct;

	if(vazia(LISTA)){
		printf("\nLista vazia!");
		return ;
	}
		
	token *tmp;
	tmp = LISTA->prox;
	lexema_atual = tmp->lexema;
	posicao_atual = &tmp->prox;

	printf("\n\nAtual---");
    printf("\nLexema: %c", lexema_atual);
	printf("\nPosicao: %x", posicao_atual);

	while(tmp != NULL)
	{
		printf("\n\nSTRUCT: %d", tmp->num);
		printf("\nLexema: %c", tmp->lexema);
		printf("\nOcorrencia: %x", &tmp->prox);

		tmp->num += 1;
		lexema_struct = tmp->lexema;
		posicao_struct = &tmp->prox;

		if (lexema_atual == lexema_struct && posicao_atual != posicao_struct)
		{
			printf("\n\n---Repetido---");
			printf("\nLexema: %c", lexema_struct);
			printf("\nPosicao: %x\n\n", posicao_struct);
			printf("\nPOSICAO ATUAL: %x\n", posicao_atual);
			return posicao_atual;
		}
		tmp = tmp->prox;
	}
	return 1;	
}

void tabela_simbolos(token *novo, int count, int posicao_atual)
{	
	novo->ocorrencia = &novo->prox;

	// if(posicao_atual != 1)
	// 	novo->ocorrencia = posicao_atual;

	switch (novo->tipo) 
	{
		case 1:
			strcpy(novo->padrao, "Qualquer constante numerica");
			strcpy(novo->token, "Number");

			fprintf(Saida2, "\nLexema: %c", novo->lexema);
			fprintf(Saida2, "\nToken: %s", novo->token);
			fprintf(Saida2, "\nPadrao: %s", novo->padrao);
			fprintf(Saida2, "\nOcorrencia: %x", novo->ocorrencia);
			fputs("\n---------------------------------------------", Saida2);
			
			fprintf(Saida1, "<num,%d> ", count);
		break;

		case 2:
			strcpy(novo->padrao, "Letra seguida por letras ou/e digitos");
			strcpy(novo->token, "Id");

			fprintf(Saida2, "\nLexema: %c", novo->lexema);
			fprintf(Saida2, "\nToken: %s", novo->token);
			fprintf(Saida2, "\nPadrao: %s", novo->padrao);
			fprintf(Saida2, "\nOcorrencia: %x", novo->ocorrencia);
			fputs("\n---------------------------------------------", Saida2);
			
			fprintf(Saida1, "<id,%d> ", count);
		break;

		case 3:
			strcpy(novo->padrao, "Operadores aritmeticos +, -, *, /, %");
			strcpy(novo->token, "Operator");
			
			fprintf(Saida2, "\nLexema: %c", novo->lexema);
			fprintf(Saida2, "\nToken: %s", novo->token);
			fprintf(Saida2, "\nPadrao: %s", novo->padrao);
			fprintf(Saida2, "\nOcorrencia: %x", novo->ocorrencia);
			fputs("\n---------------------------------------------", Saida2);
			
			fprintf(Saida1, "<%c> ", novo->lexema);
		break;
	}
} 

int analisa_lexema (token *LISTA, short int lexema, int count) 
{
	// int tipo;
	
	if (isdigit(lexema)) 
	{ 
		LISTA->tipo = 1;
		LISTA->lexema = lexema;
		insere_lexema(LISTA, count); // vai iserir dado na struct

		// printf("count: %d\n", count);
	} 
	else if (isalpha(lexema)) 
	{
		LISTA->tipo = 2;
		LISTA->lexema = lexema;
		insere_lexema(LISTA, count);

		// printf("count: %d\n", count);
	} 
	else if (lexema == '+' || lexema == '-' || lexema == '*' || lexema == '/' || lexema == '%') 
	{
		LISTA->tipo = 3;
		LISTA->lexema = lexema;
		insere_lexema(LISTA, count);

		// printf("count: %d\n", count);
		count--;
	} 
	else if (isblank(lexema))
		count--;
		
	else 
	{ 
		count--;
		fprintf(Saida1, "<%c> ", lexema);
	}
	return count;
}
