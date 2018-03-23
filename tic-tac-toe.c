/*=======================================
-> Software: Jogo da Velha
-> Autor: Douglas Inácio
-> Data: 26/06/2016
-> Versão: 1.02.20170201
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

/*========= Variáveis globais =========*/
int tabuleiro[9]; //tabuleiro do jogo
/*=====================================*/

/*======= Protótipos de funções =======*/
void exibeTabuleiro();
int fimDeJogo(const int temp[9]);
int minimax(int tab[9], int jogador);
/*=====================================*/

/*========= Programa principal ========*/
int main(int argc, char const *argv[])
{
	// -> correção de acentuação para windows
	setlocale(LC_ALL, "portuguese");

	// -> variáveis locais
	static int estatisticas[3] = {0,0,0}; //estatísticas da sessão
	char opcao[20]; //usar a variável como vetor evita erros de entrada. Obs: só será utilizado o índice [0]!
	int i, cont, jogador, jogada, valor, tempValor, tempVetor[9];

	// -> limpar tela antes de começar
	limparTela;

	// -> loop principal
	do{
		printf("=======================================\n");
		printf("============ Jogo da Velha ============\n");
		printf("=======================================\n\n\n");
		printf(" 1. Jogar\n");
		printf(" 2. Instruções\n");
		printf(" 3. Estatísticas\n");
		printf(" 4. Sobre\n");
		printf(" 5. Sair\n\n\n");
		printf("   Digite a opção: ");
		scanf("%s", &opcao);
		getchar();

		switch(opcao[0]){
			case '1' :
			{
				//sorteia quem começa jogando
				srand((unsigned)time(NULL)); //o seed rand alimenta o random
				jogador = 1 + (rand() % 2); //1 + (sorteio entre 0 e 1)
				//jogador == 1 -> humano, jogador == 2 -> máquina

				do{
					//inicializa o tabuleiro
					for(i = 0; i < 9; i++){ tabuleiro[i] = 0; }

					/*==================== Controle de jogadas ====================*/
					for(jogada = 0; jogada < 9 && fimDeJogo(tabuleiro) == 0; jogada++){ //o limite máximo de jogadas é 9, porém se algum jogador vencer antes o loop é interrompido

						//vez da máquina
						if((jogada + jogador) % 2 == 0){

							valor = -2;
							for(i = 0; i < 9; i++){
								if(tabuleiro[i] == 0){ //se a posição [i] estiver vazia
									tabuleiro[i] = 1; //faz a jogada
									tempValor = -minimax(tabuleiro, -1); //chama a função minimax, que retornar o valor da eficiência da jogada
									tabuleiro[i] = 0; //esvazia a casa novamente

									if(tempValor > valor){ //se o valor retornado for maior que o valor armazenado anteriormente
										valor = tempValor; //armazena o novo valor retornado
										cont = 0; //zera o contador do tempVetor
										tempVetor[cont] = i; //armazena o índice, ou seja, a melhor jogada
									} else if(tempValor == valor){ //se o valor retornado for igual ao valor armazenado anteriormente
										cont++; //incrementa o contador do tempVetor
										tempVetor[cont] = i; //armazena o índice, pois tem o mesmo valor de eficiência da "melhor jogada", assim pode-se fazer um sorteio e obter jogadas dinâmicas
									}
								}
							}

							/* é feito um sorteio entre 0 e o valor de cont + 1, o resultado é usado como índice de busca das opções
							de jogadas armazenadas no tempVetor anteriormente, logo, essa passa a ser a jogada marcada no tabuleiro */
							tabuleiro[ tempVetor[(rand() % (cont + 1))] ] = 1; //marca 'O' na posição informada
						}
						//vez do humano
						else{

							do{
								exibeTabuleiro();
								printf("\n\n\n\n\n   Digite a opção: ");
								scanf("%s", &opcao);
								getchar();

								opcao[0] = (opcao[0] - 49); //reduz 1 para achar a posição no vetor do tabuleiro
								/* ASCII (49) -> int 1 ... ASCII (57) -> int 9
								Exemplo: se o usuário digitar o número 1 -> será armazenado como char, que equivale
								a 49 em ASCII. Portanto, será feito o cálculo (49 - 49) e o resultado será
								utilizado como busca no índice do vetor. */

								if(opcao[0] < 0 || opcao[0] > 8 || tabuleiro[opcao[0]] != 0){
									printf("\n\n\n   Opção inválida! Por favor, digite novamente.");
									printf("\n\n   Continuar...");
									getchar();
								}

							} while(opcao[0] < 0 || opcao[0] > 8 || tabuleiro[opcao[0]] != 0); //enquanto a posição estiver fora dos limites do vetor ou ocupada

							tabuleiro[opcao[0]] = -1; //marca 'X' na posição informada
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
						case -1 : //esse caso nunca deverá acontecer!
						{
							printf("\n\n\n\n\n   Isso é inaceitável!\n");
							estatisticas[2]++;
							break;
						}
					}

					//alterna quem começa jogando na próxima partida
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
				printf("\n\n     INSTRUÇÕES\n\n\n");
				printf("     - O jogo é apresentado em uma matriz de três linhas por três colunas.\n\n");
				printf("     - Os jogadores jogam alternadamente, uma marcação por vez,\n");
				printf("     em uma posição que esteja vazia.\n\n");
				printf("     - A missão é conseguir três X ou três O na horizontal, vertical\n");
				printf("     ou diagonal, e ao mesmo tempo, quando possível, impedir o adversário\n");
				printf("     de ganhar na próxima jogada.\n\n");
				printf("\n\n     Pressione ENTER para voltar ao menu principal...");
				getchar();
				break;
			}
			case '3' :
			{
				limparTela;
				printf("\n\n     ESTATÍSTICAS\n\n\n");
				printf("     - Vitórias do jogador........: %d\n\n", estatisticas[2]);
				printf("     - Vitórias do computador.....: %d\n\n", estatisticas[1]);
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
				printf("     Autor: Douglas Inácio\n\n");
				printf("     Data: 26/06/2016\n\n");
				printf("     Versão: 1.02.20170201\n\n");
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

/*============== Funções ==============*/

// -> exibe o tabuleiro na tela
void exibeTabuleiro(){
	limparTela;
	printf("\n\n\n\n");
	//loop de prints que exibe o tabuleiro na tela
	for(int i = 0; i < 9; i++){
		if(i == 0 || i == 3 || i == 6){
			printf("               "); //espaços à esquerda
		}
		//exibição dos símbolos
		if(tabuleiro[i] == -1){
			printf(" X ");
		} else if(tabuleiro[i] == 0){ //se estiver vazio
			printf(" %d ", (i + 1)); //imprime o índice + 1
		} else if(tabuleiro[i] == 1){
			printf(" O ");
		}
		//divisões do tabuleiro
		if(i == 0 || i == 1 || i == 3 || i == 4 || i == 6 || i == 7){
			printf("|"); //barra divisória
		} else if(i == 2 || i == 5){
			printf("\n               "); //espaços à esquerda
			printf("-----------\n"); //linha divisória
		}
	}
}

// -> verifica se o jogo acabou, se há vencedor ou deu empate
int fimDeJogo(const int temp[9]){
	//todas as possibilidades de vitória: linhas, colunas e diagonais
	static const int vitoria[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
	//o índice para buscar no temp é informado por vitoria[i][0], vitoria[i][1] ou vitoria[i][2]
	for(int i = 0; i < 8; i++) {
		if(temp[vitoria[i][0]] != 0 && //se a primeira casa não está vazia
			temp[vitoria[i][0]] == temp[vitoria[i][1]] && //e ela é igual a segunda
			temp[vitoria[i][0]] == temp[vitoria[i][2]]){ //e igual a terceira
			return temp[vitoria[i][2]]; //retorna o valor que está marcado na terceira casa, assim sabemos quem venceu
		}
	}
	return 0; //caso contrário, ainda não há vencedor ou deu empate no estado final do jogo (depois da última jogada)
}

// -> realiza as próximas jogadas em profundidade máxima e avalia sua eficiência
int minimax(int tab[9], int jogador){
	int vencedor = fimDeJogo(tab);
	if(vencedor != 0) return (vencedor * jogador);

	int posicao = -1, valor = -2, tempValor;
	for(int i = 0; i < 9; i++){
		if(tabuleiro[i] == 0){ //se a posição [i] estiver vazia
			tabuleiro[i] = jogador; //faz a jogada
			tempValor = -minimax(tabuleiro, (jogador * -1)); //chama a função recursivamente, retornando o valor da eficiência da jogada
			tabuleiro[i] = 0; //esvazia a casa novamente

			if(tempValor >= valor){ //se o valor retornado for maior ou igual ao valor armazenado anteriormente
				valor = tempValor; //armazena o novo valor retornado
				posicao = i; //guarda o índice, ou seja, a melhor jogada
			}
		}
	}

	if(posicao == -1) return 0;
	return valor;
}

/*=====================================*/