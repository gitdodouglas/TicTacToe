/*=======================================
-> Software: Jogo da Velha
-> Autor: Douglas In�cio
-> Data: 26/06/2016
-> Vers�o: 1.02.20170201
-> Algoritmo minimax: Matthew Steel, 2009
https://gist.github.com/MatthewSteel/3158579
=======================================*/

/*============ Bibliotecas ============*/
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>
#include <time.h>
/*=====================================*/

/*============== Defines ==============*/
#define limparTela system("cls");
/*=====================================*/

/*========= Vari�veis globais =========*/
int tabuleiro[9]; //tabuleiro do jogo
/*=====================================*/

/*======= Prot�tipos de fun��es =======*/
void exibeTabuleiro();
int fimDeJogo(const int temp[9]);
int minimax(int tab[9], int jogador);
/*=====================================*/

/*========= Programa principal ========*/
int main(int argc, char const *argv[])
{
	// -> corre��o de acentua��o para windows
	setlocale(LC_ALL, "portuguese");

	// -> vari�veis locais
	static int estatisticas[3] = {0,0,0}; //estat�sticas da sess�o
	char opcao[20]; //usar a vari�vel como vetor evita erros de entrada. Obs: s� ser� utilizado o �ndice [0]!
	int i, cont, jogador, jogada, valor, tempValor, tempVetor[9];

	// -> limpar tela antes de come�ar
	limparTela;

	// -> loop principal
	do{
		printf("=======================================\n");
		printf("============ Jogo da Velha ============\n");
		printf("=======================================\n\n\n");
		printf(" 1. Jogar\n");
		printf(" 2. Instru��es\n");
		printf(" 3. Estat�sticas\n");
		printf(" 4. Sobre\n");
		printf(" 5. Sair\n\n\n");
		printf("   Digite a op��o: ");
		scanf("%s", &opcao);
		getchar();

		switch(opcao[0]){
			case '1' :
			{
				//sorteia quem come�a jogando
				srand((unsigned)time(NULL)); //o seed rand alimenta o random
				jogador = 1 + (rand() % 2); //1 + (sorteio entre 0 e 1)
				//jogador == 1 -> humano, jogador == 2 -> m�quina

				do{
					//inicializa o tabuleiro
					for(i = 0; i < 9; i++){ tabuleiro[i] = 0; }

					/*==================== Controle de jogadas ====================*/
					for(jogada = 0; jogada < 9 && fimDeJogo(tabuleiro) == 0; jogada++){ //o limite m�ximo de jogadas � 9, por�m se algum jogador vencer antes o loop � interrompido

						//vez da m�quina
						if((jogada + jogador) % 2 == 0){

							valor = -2;
							for(i = 0; i < 9; i++){
								if(tabuleiro[i] == 0){ //se a posi��o [i] estiver vazia
									tabuleiro[i] = 1; //faz a jogada
									tempValor = -minimax(tabuleiro, -1); //chama a fun��o minimax, que retornar o valor da efici�ncia da jogada
									tabuleiro[i] = 0; //esvazia a casa novamente

									if(tempValor > valor){ //se o valor retornado for maior que o valor armazenado anteriormente
										valor = tempValor; //armazena o novo valor retornado
										cont = 0; //zera o contador do tempVetor
										tempVetor[cont] = i; //armazena o �ndice, ou seja, a melhor jogada
									} else if(tempValor == valor){ //se o valor retornado for igual ao valor armazenado anteriormente
										cont++; //incrementa o contador do tempVetor
										tempVetor[cont] = i; //armazena o �ndice, pois tem o mesmo valor de efici�ncia da "melhor jogada", assim pode-se fazer um sorteio e obter jogadas din�micas
									}
								}
							}

							/* � feito um sorteio entre 0 e o valor de cont + 1, o resultado � usado como �ndice de busca das op��es
							de jogadas armazenadas no tempVetor anteriormente, logo, essa passa a ser a jogada marcada no tabuleiro */
							tabuleiro[ tempVetor[(rand() % (cont + 1))] ] = 1; //marca 'O' na posi��o informada
						}
						//vez do humano
						else{

							do{
								exibeTabuleiro();
								printf("\n\n\n\n\n   Digite a op��o: ");
								scanf("%s", &opcao);
								getchar();

								opcao[0] = (opcao[0] - 49); //reduz 1 para achar a posi��o no vetor do tabuleiro
								/* ASCII (49) -> int 1 ... ASCII (57) -> int 9
								Exemplo: se o usu�rio digitar o n�mero 1 -> ser� armazenado como char, que equivale
								a 49 em ASCII. Portanto, ser� feito o c�lculo (49 - 49) e o resultado ser�
								utilizado como busca no �ndice do vetor. */

								if(opcao[0] < 0 || opcao[0] > 8 || tabuleiro[opcao[0]] != 0){
									printf("\n\n\n   Op��o inv�lida! Por favor, digite novamente.");
									printf("\n\n   Continuar...");
									getchar();
								}

							} while(opcao[0] < 0 || opcao[0] > 8 || tabuleiro[opcao[0]] != 0); //enquanto a posi��o estiver fora dos limites do vetor ou ocupada

							tabuleiro[opcao[0]] = -1; //marca 'X' na posi��o informada
						}

					}
					/*=============================================================*/

					//resultado da partida
					exibeTabuleiro();
					switch(fimDeJogo(tabuleiro)){
						case 0 :
						{
							printf("\n\n\n\n\n   Empate!\n");
							estatisticas[0]++;
							break;
						}
						case 1 :
						{
							printf("\n\n\n\n\n   Eu venci! =P\n");
							estatisticas[1]++;
							break;
						}
						case -1 : //esse caso nunca dever� acontecer!
						{
							printf("\n\n\n\n\n   Isso � inaceit�vel!\n");
							estatisticas[2]++;
							break;
						}
					}

					//alterna quem come�a jogando na pr�xima partida
					jogador = jogador == 1 ? 2 : 1;

					printf("\n\n   Deseja jogar novamente? (s/n) ");
					scanf("%s", &opcao);
					getchar();

				} while(opcao[0] == 's' || opcao[0] == 'S');

				break;
			}
			case '2' :
			{
				limparTela;
				printf("\n\n     INSTRU��ES\n\n\n");
				printf("     - O jogo � apresentado em uma matriz de tr�s linhas por tr�s colunas.\n\n");
				printf("     - Os jogadores jogam alternadamente, uma marca��o por vez,\n");
				printf("     em uma posi��o que esteja vazia.\n\n");
				printf("     - A miss�o � conseguir tr�s X ou tr�s O na horizontal, vertical\n");
				printf("     ou diagonal, e ao mesmo tempo, quando poss�vel, impedir o advers�rio\n");
				printf("     de ganhar na pr�xima jogada.\n\n");
				printf("\n\n     Pressione ENTER para voltar ao menu principal...");
				getchar();
				break;
			}
			case '3' :
			{
				limparTela;
				printf("\n\n     ESTAT�STICAS\n\n\n");
				printf("     - Vit�rias do jogador........: %d\n\n", estatisticas[2]);
				printf("     - Vit�rias do computador.....: %d\n\n", estatisticas[1]);
				printf("     - Empates....................: %d\n\n", estatisticas[0]);
				printf("\n\n     Pressione ENTER para voltar ao menu principal...");
				getchar();
				break;
			}
			case '4' :
			{
				limparTela;
				printf("\n\n     SOBRE\n\n\n");
				printf("     Software: Jogo da Velha\n\n");
				printf("     Autor: Douglas In�cio\n\n");
				printf("     Data: 26/06/2016\n\n");
				printf("     Vers�o: 1.02.20170201\n\n");
				printf("\n\n     Pressione ENTER para voltar ao menu principal...");
				getchar();
				break;
			}
		} //fim do switch do menu

		limparTela;

	} while(opcao[0] != '5'); //fim do loop principal

	return 0;
}
/*=====================================*/

/*============== Fun��es ==============*/

// -> exibe o tabuleiro na tela
void exibeTabuleiro(){
	limparTela;
	printf("\n\n\n\n");
	//loop de prints que exibe o tabuleiro na tela
	for(int i = 0; i < 9; i++){
		if(i == 0 || i == 3 || i == 6){
			printf("               "); //espa�os � esquerda
		}
		//exibi��o dos s�mbolos
		if(tabuleiro[i] == -1){
			printf(" X ");
		} else if(tabuleiro[i] == 0){ //se estiver vazio
			printf(" %d ", (i + 1)); //imprime o �ndice + 1
		} else if(tabuleiro[i] == 1){
			printf(" O ");
		}
		//divis�es do tabuleiro
		if(i == 0 || i == 1 || i == 3 || i == 4 || i == 6 || i == 7){
			printf("|"); //barra divis�ria
		} else if(i == 2 || i == 5){
			printf("\n               "); //espa�os � esquerda
			printf("-----------\n"); //linha divis�ria
		}
	}
}

// -> verifica se o jogo acabou, se h� vencedor ou deu empate
int fimDeJogo(const int temp[9]){
	//todas as possibilidades de vit�ria: linhas, colunas e diagonais
	static const int vitoria[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
	//o �ndice para buscar no temp � informado por vitoria[i][0], vitoria[i][1] ou vitoria[i][2]
	for(int i = 0; i < 8; i++) {
		if(temp[vitoria[i][0]] != 0 && //se a primeira casa n�o est� vazia
			temp[vitoria[i][0]] == temp[vitoria[i][1]] && //e ela � igual a segunda
			temp[vitoria[i][0]] == temp[vitoria[i][2]]){ //e igual a terceira
			return temp[vitoria[i][2]]; //retorna o valor que est� marcado na terceira casa, assim sabemos quem venceu
		}
	}
	return 0; //caso contr�rio, ainda n�o h� vencedor ou deu empate no estado final do jogo (depois da �ltima jogada)
}

// -> realiza as pr�ximas jogadas em profundidade m�xima e avalia sua efici�ncia
int minimax(int tab[9], int jogador){
	int vencedor = fimDeJogo(tab);
	if(vencedor != 0) return (vencedor * jogador);

	int posicao = -1, valor = -2, tempValor;
	for(int i = 0; i < 9; i++){
		if(tabuleiro[i] == 0){ //se a posi��o [i] estiver vazia
			tabuleiro[i] = jogador; //faz a jogada
			tempValor = -minimax(tabuleiro, (jogador * -1)); //chama a fun��o recursivamente, retornando o valor da efici�ncia da jogada
			tabuleiro[i] = 0; //esvazia a casa novamente

			if(tempValor >= valor){ //se o valor retornado for maior ou igual ao valor armazenado anteriormente
				valor = tempValor; //armazena o novo valor retornado
				posicao = i; //guarda o �ndice, ou seja, a melhor jogada
			}
		}
	}

	if(posicao == -1) return 0;
	return valor;
}

/*=====================================*/