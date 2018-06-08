#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
int main ()
{
	FILE *Arquivo;  // Arquivo Entrada
	FILE *Saida1; // Arquivo Saida1.txt
	FILE *Saida2;  // Arquivo Saida2.txt
	
	short int teste;
	short int op; // Var para guardar operador
	
	int i = 1;   // contador de structs
	int j = 0;   //valor anterior da posicao atual de i
	int v = 0;   //var de verificacao boolena

	struct tokens
	{
		short int lexema;
	};
	struct tokens *p;
	
	// Validando e abrindo um arquivo com a expressao de entrada
	if ((Arquivo = fopen("entrada.txt", "r")) == NULL)	
	{
		printf("\nNao foi possivel abrir o arquivo!\nVerifique o arquivo de entrada.txt");
		exit(1);
	}
	else
	{	
		Saida1 = fopen("saida1.txt", "w");	// Criando arquivo de saida1.txt fluxo de tokens
		Saida2 = fopen("saida2.txt", "w");	// Criando arquivo de saida2.txt tabela de simbolos
		
		p = (struct tokens *) malloc(1*sizeof(struct tokens)); //Alocando struct		
		fputs("\t..::Tabela de Simbolos::..\n", Saida2);
		
		while ((teste = fgetc(Arquivo)) != EOF)	//até o fim do arquivo for atingido
		{	
			if(teste == '=' || teste == '(' || teste == ')')
			{
				op = teste;
				fprintf(Saida1, "<%c> ", teste); // gravando na saida1.txt
				i--; //retomando a contagem anterior para o proximo lexema valido
			}
			else
			{			
				p[i].lexema = teste; //gravando lexema
				fputs("\n---------------------------------------------\n", Saida2);
				fprintf(Saida2, "Lexema: %c", p[i].lexema); //grava no arquivo de saida2.txt
				
				if(isdigit(p[i].lexema)) //verificando se eh um digito
				{	
					while(j >= 0 ) // gravando o fluxo de tokens na saida1.txt
					{	
						if(p[i].lexema == p[j].lexema)
						{	
							fprintf(Saida1, "<num,%d> ",j); // imprimindo na saida1.txt	
							v = 1;
							i--;
						}
						j--;		
					}
					if(v == 0) //é um digito q não ocorreu repeticao ainda
					{	
						fprintf(Saida1, "<num,%d> ",i); // imprimindo na saida1.txt
					}
					
					// gravando na saida2.txt	
					fputs("\nToken: Number", Saida2); 
					fputs("\nPadrao: Qualquer constante numerica", Saida2);
					
					//cod ocorrencia ("coluna" da linha da expressao)
					fprintf(Saida2, "\nOcorrencia: %d", &p[i].lexema);
				} 
				else if (p[i].lexema != op) 
				{	
					while(j >= 0)	// gravando o fluxo de tokens na saida1.txt
					{
						if(p[i].lexema == p[j].lexema)
						{
							fprintf(Saida1, "<operator,%d> ",j); // imprimindo na saida1.txt	
							v = 1;
							i--;
						}
						j--;
					}
					if(v == 0)
					{
						fprintf(Saida1, "<operator,%d> ",i);	// imprimindo na saida1.txt		
					}
					// Gravando uma string no arquivo de Saida2.txt
					fputs("\nToken: Operator", Saida2); 
					fputs("\nPadrao: Operadores aritmeticos +, -, *, /, %, ^ ", Saida2);
				
					//cod ocorrencia ("coluna" da linha da expressao)
					fprintf(Saida2, "\nOcorrencia: %d", &p[i].lexema);	
				}
				else
				{
					printf("\noperador invalido!\n");
				}
			}
			j = i;
			i++; //incrementado a posicao i
			v = 0;
		}
		fclose(Saida1);
		fclose(Saida2);
		printf("\nArquivo de Saida1.txt pronto!"); 
		printf("\nArquivo de Saida2.txt pronto!"); 
	}
	fclose(Arquivo);
	return 0;
}
