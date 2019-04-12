#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define QTD_MAX_PALAVRAS 999

struct Palavra{
	char dica[80];
	char palavra[50];
	int marcaLetra[50];
	int qtdCaracter;
};

//variaveis para representar o arquivo com as palavras
struct Palavra bancoPalavras[QTD_MAX_PALAVRAS];
int qtdPalavras;FILE *arquivo;

//variaveis para o jogo
struct Palavra palavraForca;
int qtdDica,jogando,tentativa,qtdAcerto;
char chuteLetra;

//variaveis gerais
int ativo,opcao,i=0;

void carregarPalavras(){
	arquivo = fopen("palavras.txt","r");
	
	//Carrega o array palavras.
	qtdPalavras = 0;
	while(fscanf(arquivo,"%s %s\n",bancoPalavras[i].palavra,bancoPalavras[i].dica) != EOF){
		qtdPalavras++;
		i++;
	}
	
	fclose(arquivo);
}

void cadastrarPalavra(){
	//Verifica o limite de palavra
	if(qtdPalavras != QTD_MAX_PALAVRAS){
		struct Palavra p;
		//Pede a palavra e a dica.
		system("cls");
		printf("Cadastrar Palavra\n");
		printf("Obs: Colocar '_' em vez de espaço!\n");
		printf("Obs: Não escreva as palavras com acentos!\n");
		printf("Palavra: ");
		scanf("%s",p.palavra);
		printf("Dica: ");
		scanf("%s",p.dica);
		
		//Adiciona a palavra
		arquivo = fopen("palavras.txt","a");
		fprintf(arquivo,"\n%s %s",p.palavra,p.dica);
		fclose(arquivo);
		
	}else
		printf("Máximo de Palavras Já Cadastrada!\n");
}


void prepararJogo(){
	tentativa = 7;
	qtdDica = 1;
	qtdAcerto = 0;
	
	srand(time(NULL));
	palavraForca = bancoPalavras[rand() % qtdPalavras];
	
	palavraForca.qtdCaracter = 0;
	for(i=0;i<strlen(palavraForca.palavra);i++){
		palavraForca.marcaLetra[i] = 0;
		if(palavraForca.palavra[i] != '_')
			palavraForca.qtdCaracter++;
	}
	
	opcao = 0;	
	jogando = 1;
}

void exibirForca(){
	system("cls");
	printf("Tentativas Restantes: %d\n",tentativa);
	printf("\n\n\t%d LETRAS\n\n",palavraForca.qtdCaracter);
			
	printf("\t  __________\n");
	printf("\t  |        |\n");
	printf("\t  |       _|_\n");
	if(tentativa < 7){
		printf("\t  |        O\n");//Cabeça
		if(tentativa < 6){
			//Tronco, braço esquerdo e braço direito
			(tentativa == 5) ? printf("\t  |        |\n") : 
				(tentativa == 4) ? 	printf("\t  |       /| \n") : printf("\t  |       /|\\ \n");
			
			//tronco
			printf("\t  |        |\n");
			
			//Pé direito e pé esquerdo
			(tentativa < 3) ? (tentativa == 2) ? printf("\t  |       / \n") : 
				printf("\t  |       / \\ \n") : printf("\t  |\n");
				 			
		}else{
			for(i=0;i<3;i++)
				printf("\t  |\n");
		}		
	}else {
		for(i=0;i<4;i++)
			printf("\t  |\n");
	}
	printf("\t__|__\n\n\n\t");
	
	for(i=0;i<strlen(palavraForca.palavra);i++){
		if(palavraForca.palavra[i] == '_')
			printf(" ");
		else if(palavraForca.marcaLetra[i] == 0)
			printf("__ ");
		else
			printf("%c",toupper(palavraForca.palavra[i]));
	}
}

void jogar(){
	//Solicita o carctere e disponibiliza a dica
	if(qtdDica == 1){
		do{
			printf("\n\n1 - Jogar");
			printf("\n2 - Pedir Dica\n");
			printf("opcao: ");
			scanf("%d",&opcao);
			fflush(stdin);
		}while(opcao != 1 && opcao != 2);
		
		if(opcao == 2){
			printf("\nDica: ");
			for(i=0;i<strlen(palavraForca.dica);i++){
				(palavraForca.dica[i] != '_') ? printf("%c",palavraForca.dica[i]) : printf(" ");
			}
			qtdDica--;
		}
	}
	
	printf("\n\n\nDigite a letra: ");
	chuteLetra = getch();	
}

void marcarVerificar(){
	//Marca o Jogo
	int acerto = 0;
	for(i=0;i<strlen(palavraForca.palavra);i++){
		if(toupper(palavraForca.palavra[i]) == toupper(chuteLetra)){
			if(palavraForca.marcaLetra[i] == 0){
				palavraForca.marcaLetra[i] = 1;
				qtdAcerto++;
			}
			acerto++;
		}else if( (i==strlen(palavraForca.palavra)-1) && (acerto == 0) )
			tentativa--;
	}
	
	//Verifica o jogo
	if(qtdAcerto == palavraForca.qtdCaracter){//Significa que saiu da forca 
		exibirForca();
		printf("\n\nPARABÉNS! VOCÊ SAIU DA FORCA!\n\n");
		jogando = 0;
	}else if(tentativa == 0){ //Significa que foi enforcado
		printf("\n\nVOCÊ FOI ENFORCADO!\n");
		printf("Palavra: %s\n\n",palavraForca.palavra);
		jogando = 0;
	}
}


int main(int argc, char *argv[]) {
	setlocale(LC_ALL,"portuguese"); //Configuração do idioma
	
	carregarPalavras();
	
	ativo = 1;
	while(ativo){
		printf("----------------------------\n");
		printf("|      JOGO DA FORCA       |\n");
		printf("----------------------------\n");
		printf("1 - Cadastrar Palavra\n");
		printf("2 - Jogar\n");
		printf("3 - Sair\n");
		printf("opcao: ");
		scanf("%d",&opcao);
		fflush(stdin);
			
		if(opcao == 1) //Cadatrar Nova Palavra
			cadastrarPalavra();
			
		else if(opcao == 2){ //Jogar
		
			prepararJogo();	
			while(jogando){
				exibirForca();
				jogar();
				marcarVerificar();
			}
			
		}else if(opcao == 3) //Sair
			ativo = 0;
	}
	
	return 0;
}
