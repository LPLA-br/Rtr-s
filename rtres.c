/* rtres
 * Programa de regra de três simples
 * com um simples interpretador.
 * arquivo do tipo:
 * 9N:9N:9N:tipo\n
 * 9N:9N:9N:tipo\n
 * \n
 * */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <inttypes.h>
#include <string.h>
#include <stdbool.h>

#define TAM_PADRAO 10
#define RESETAR_VALOR 0
#define STDVAL 1

char buffer[TAM_PADRAO];
uint8_t posicaoDeGravacao = 0; //usado por gravarMatriz()

uint8_t retornarNumeroDeCadeias(void);
void limparMatrizes(char *matriz1, char *matriz2, char *matriz3);
float regraDeTrezDireta(int n1, int n2, int n3);
float regraDeTrezInversa(int n1, int n2, int n3);
void trabalharDeAcordoComArquivo(const char *nomeDoArquivo);
void help(void);
uint8_t identificarDoisPontos(char alvo);
void gravarMatriz(char *qualMatriz, char caractere);
char identificarTipoDeRegraDeTrez(char alvo);
void depuracaoDeString(char *m);

int main(int argc, char *argv[])
{
	/* Tratamento da linha de comando.
	 * argv0        argv1          argv2 argv3 argv4
	 * NomePrograma ModoDeOperação n1    n2    n3*/

	if(argc > 4)
	{
		if( !(strcmp(argv[1], "-d")) )
		{
			/*regra de trez direta*/
			printf("%f\n", regraDeTrezDireta( atoi(argv[2]), atoi(argv[3]), atoi(argv[4])) );
		}
		else if( !(strcmp(argv[1], "-i")) )
		{
			/*regra de trez inversa*/
			printf("%f\n", regraDeTrezInversa( atoi(argv[2]), atoi(argv[3]), atoi(argv[4])) );
		}
		else	printf("\nModo de operação inválido.\n");
	}
	else if(argc == 3)
	{
		if( !(strcmp(argv[1], "-f")) )
		{
			trabalharDeAcordoComArquivo(argv[2]);
		}
		else if( !(strcmp(argv[1], "-h")) )
		{
			help();
		}
	}
	else
	{
		help();
		exit(11);
	}
	exit(0);
	return 0;
}

/* limpa o lixo contido em cada endereço
 * da matriz passada nos argumentos*/
void limparMatrizes(char *matriz1, char *matriz2, char *matriz3)
{
	uint8_t pos, tam;
	for(tam=TAM_PADRAO,pos=0; pos<tam; pos++)
	{
		if(pos > tam) break;
		else
		{
			*(matriz1+pos) = '\0';
			*(matriz2+pos) = '\0';
			*(matriz3+pos) = '\0';
		}
	}
}

/* faz regra de tres direta e retorna o resultado*/
float regraDeTrezDireta(int n1, int n2, int n3)
{
	int x=n2*n3;
	x=x/n1;
	return (float)x;
}

/* faz regra de tres inversa e retorna o resultado*/
float regraDeTrezInversa(int n1, int n2, int n3)
{
	int x=n1*n2;
	x=x/n3;
	return (float)x;
}

/*esta função interpreta um arquivo
 * de texto chamado "calculo"
 * imprimindo os resultados
 * das operações nele contidas no terminal.
 **/
void trabalharDeAcordoComArquivo(const char *nomeDoArquivo)
{
	FILE *arquivo = fopen(nomeDoArquivo, "r");
	if(!arquivo)
	{
		printf("\nERRO FATAL, não foi possível abrir arquivo especificado\n");
		exit(1);
	}

	/* --variáveis de controle-- */
	char caractere = 0; /*buffer de fgetc()*/
	char tipo = 'd';
	uint8_t qualMatrizGravar = 1;
	/*matrizes que são gravadas com caracteres número.*/
	char n1[TAM_PADRAO], n2[TAM_PADRAO], n3[TAM_PADRAO];
	uint8_t loop = true;

	limparMatrizes(n1, n2, n3);
	/*núcleo de interpretação de arquivo.*/
	/*ordem de leitura  numero:numero:numero:tipoNOVALINHA
	 *ATENÇÂO este interpretador não é tolerante a falhas.*/
	while( loop )
	{
		printf("%c", caractere);
		caractere = fgetc(arquivo);
		if(caractere == EOF) loop = false;
		switch(qualMatrizGravar)
		{
			case 1:
				gravarMatriz(n1, caractere);
				if( identificarDoisPontos(caractere) == true)
					qualMatrizGravar = 2;
				break;
			case 2:
				gravarMatriz(n2, caractere);
				if( identificarDoisPontos(caractere) == true)
					qualMatrizGravar = 3;
				break;
			case 3:
				gravarMatriz(n3, caractere);
				if( identificarDoisPontos(caractere) == true)
					qualMatrizGravar = 4;/*vai para default*/
				break;
			default:
				tipo = identificarTipoDeRegraDeTrez(caractere);
				caractere = fgetc(arquivo);
				switch(tipo)
				{
					case 'd':
						printf("\n %f",  regraDeTrezDireta( atoi(n1), atoi(n2), atoi(n3) ));
						break;
					case 'i':
						printf("\n %f",  regraDeTrezDireta( atoi(n1), atoi(n2), atoi(n3) ));
						break;
					default:
						printf("\n %f",  regraDeTrezDireta( atoi(n1), atoi(n2), atoi(n3) ));
						break;
				}

				/*fim da interpretação da linha atual
				 *para o comesso da próxima*/
				if( caractere == '\n' )
				{
					limparMatrizes(n1, n2, n3);
					qualMatrizGravar = 1;
				}
				else loop = false;
				break;
		}
	}
	fclose(arquivo);
}

void help(void)
{
	printf("\nrtres OPERAÇÃO n1 n2 n3\n");
	printf("  direta se n1 60km em n2 1hora entao n3 120km xhoras\n");
	printf("  inversa se n1 12pedreiros concluem em n2 5dias");
	printf("  entao xpedreiros concluem em n3 3dias\n\n");
	printf("\n  OPERAÇÃO:");
	printf("   -h, imprime esta ajuda.\n");
	printf("   -f interpreta arquivo passado na linha de comando.\n");
	printf("   -d DIRETA\n   -i INVERSA\n");
}

/* identificação de : significa que a gravação
 * de tal matriz deve parar para comessar a da proxima
 * e que a variável global posicaoDeGravacao foi zerada*/
uint8_t identificarDoisPontos(char alvo)
{
	switch(alvo)
	{
		case ':':
			posicaoDeGravacao = 0;
			return true;
		default:
			return false;
	}
}


/*apenas grava uma matriz respeitando limites do programa*/
void gravarMatriz(char *qualMatriz, char caractereLido)
{
	if( isdigit(caractereLido) )
	{
		if( !(posicaoDeGravacao > TAM_PADRAO ||  posicaoDeGravacao < 0) )
			*(qualMatriz+posicaoDeGravacao) = caractereLido;
	}
	else *(qualMatriz+posicaoDeGravacao) = '\0'; /*imprecendível para o funcionamento de atoi()*/
	posicaoDeGravacao++;
}

/*retorna 
 * 6 para direta
 * 7 para indireta*/
char identificarTipoDeRegraDeTrez(char alvo)
{
	switch(alvo)
	{
		case 'd':
			return 'd';
		case 'i':
			return 'i';
		default:
			return 'd';
	}
	return 'd';
}

inline void depuracaoDeString(char *m)
{
	uint8_t i;
	for(i=0; i<TAM_PADRAO; i++)
		printf("%c", *(m+i));
}

