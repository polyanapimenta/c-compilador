%option noyywrap

%top {
  #include <stdio.h>
  #include <string.h>

  // prototipos funcoes
  void inicializar();
  void arquivosaida1(char *token, int posicao);
  void arquivosaida2(char *token, int tipo, int posicao);

  // variaveis dos arquivos de saida
  FILE *saida1;
  FILE *saida2;

  // outras variaveis
  char id[50];
  char id_atual[50];

  char operator[2];
  char operator_atual[2];

  char number[10];
  char number_atual[10];

  char temp[50];
  char tipo[10];

  int  count = 1;

  int  ID_tmpcount = 1;
  int  ID_tmp_tmpcount = 1;

  int  NUM_tmpcount = 1;
  int  NUM_tmp_tmpcount = 1;

  int  OP_tmpcount = 1;
  int  OP_tmp_tmpcount = 1;
}

ID [A-Za-z][A-Za-z0-9\_]*
NUMBER [0-9]*
OPERATOR [\+\-\*\/\%\^]
ASSIGNMENT [\=]
OPEN_PARENTESIS [\(]
CLOSE_PARENTESIS [\)]
BLANK_SPACE [\ \t]
END_OF_STATEMENT [\;]

%%

{ID}  {
  strcpy (tipo, "id");
  strcpy (id_atual, strdup(yytext));

  if (*id == *id_atual) {
    ID_tmpcount = ID_tmp_tmpcount;
    arquivosaida1(tipo, ID_tmpcount);
    arquivosaida2(id_atual, 1, ID_tmpcount);
  }
  else if (*temp == *id_atual) {
    arquivosaida1(tipo, ID_tmpcount);
    arquivosaida2(id_atual, 1, ID_tmpcount);
    ID_tmpcount = ID_tmp_tmpcount;
  }
  else {
    arquivosaida1(tipo, count);
    arquivosaida2(id_atual, 1, count);
    ID_tmpcount = ID_tmp_tmpcount;
    ID_tmp_tmpcount = count;
    count++;
  }
  strcpy (temp, id);
  strcpy (id, id_atual);
}

{NUMBER}  {
  strcpy (tipo, "num");
  strcpy (number_atual, strdup(yytext));

  if (*number == *number_atual) {
    NUM_tmpcount = NUM_tmp_tmpcount;
    arquivosaida1(tipo, NUM_tmpcount);
    arquivosaida2(number_atual, 2, NUM_tmpcount);
  }
  else if (*temp == *number_atual) {
    arquivosaida1(tipo, NUM_tmpcount);
    arquivosaida2(number_atual, 2, NUM_tmpcount);
    NUM_tmpcount = NUM_tmp_tmpcount;
  }
  else {
    arquivosaida1(tipo, count);
    arquivosaida2(number_atual, 2, count);
    NUM_tmpcount = NUM_tmp_tmpcount;
    NUM_tmp_tmpcount = count;
    count++;
  }
  strcpy (temp, number);
  strcpy (number, number_atual);
}

{OPERATOR}  {
  strcpy (tipo, "operator");
  strcpy (operator_atual, strdup(yytext));

  if (*operator == *operator_atual) {
    OP_tmpcount = OP_tmp_tmpcount;
    arquivosaida1(tipo, OP_tmpcount);
    arquivosaida2(operator_atual, 3, OP_tmpcount);
  }
  else if (*temp == *operator_atual) {
    arquivosaida1(tipo, OP_tmpcount);
    arquivosaida2(operator_atual, 3, OP_tmpcount);
    OP_tmpcount = OP_tmp_tmpcount;
  }
  else {
    arquivosaida1(tipo, count);
    arquivosaida2(operator_atual, 3, count);
    OP_tmpcount = OP_tmp_tmpcount;
    OP_tmp_tmpcount = count;
    count++;
  }
  strcpy (temp, operator);
  strcpy (operator, operator_atual);
}

{ASSIGNMENT} {
  arquivosaida1(strdup(yytext), 0);
}

{OPEN_PARENTESIS} {
  arquivosaida1(strdup(yytext), 0);
}

{CLOSE_PARENTESIS}  {
  arquivosaida1(strdup(yytext), 0);
}

{END_OF_STATEMENT}  {
  arquivosaida1(strdup(yytext), 0);
}

{BLANK_SPACE} { }
.             { printf("Caractere nao reconhecido\n"); }
%%

int main()
{
  // Abrindo o arquivo com a expressao de entrada
  FILE *arquivo = fopen("entrada.txt", "r");

  if (arquivo == NULL) {
    printf("\nNao foi possivel abrir entrada.txt!");
    printf("\nVerifique se o arquivo existe");
    exit(1);
  }
  else {
    // atribuindo como entrada padrão para analise, os caracteres de entrada.txt
    yyin = arquivo;

    // iniciar arquivos de saida
    inicializar();

    // funcao que chama o analisador
    yylex();

    // fecha o arquivo de entrada
    fclose(yyin);
  }
}

void inicializar() {
  saida1 = fopen("saida1.txt", "w");
  yyout = saida1;

  saida2 = fopen("saida2.txt", "w");
  yyout = saida2;
}

void arquivosaida1(char *token, int posicao) {

  if (posicao != 0) {
    fprintf(saida1, "<%s,%d> ",token,posicao);
    posicao++;
  }
  else {
    fprintf(saida1, "<%s> ",token);
  }
}

void arquivosaida2(char *token, int tipo, int posicao) {

  fprintf(saida2, "\nLexema: %s", token);

  switch(tipo) {
    case 1:
      fputs("\nToken: ID", saida2);
      fputs("\nPadrao: Letra seguida por letras ou/e digitos", saida2);
    break;

    case 2:
      fputs("\nToken: NUMBER", saida2);
      fputs("\nPadrao: Qualquer constante numerica", saida2);
    break;

    case 3:
      fputs("\nToken: OPERATOR", saida2);
      fputs("\nPadrao: Operadores aritmeticos +, -, *, /, %, ^", saida2);
    break;
  }

  fprintf(saida2, "\nOcorrencia: Linha 1 Coluna %d", posicao);
  fputs("\n---------------------------------------------\n", saida2);
}
