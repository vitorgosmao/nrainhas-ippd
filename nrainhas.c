/* 
N-Rainhas em openmp
compilar com "gcc nrainhas.c -fopenmp"
executar com ./a.exe "tamanhoDoTabuleiro=numDeRainhas" "numeroDeThreads"
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

#include <omp.h>


int verificaSolucao(int n, int max_queens, int queen_rows[max_queens])//queen_rows representa a linha do tabuleiro onde está a rainha, n é o tamanho do tabuleiro
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = i+1; j < n; j++)//compara cada rainha com as outras que não foram comparadas ainda
		{
			if (queen_rows[i] == queen_rows[j]) return 0;//2 rainhas na mesma linha, recusa
			
			if (queen_rows[i] - queen_rows[j] == i - j || queen_rows[i] - queen_rows[j] == j - i) return 0;//2 rainhas na mesma diagonal, recusa
		}
	}
	//por default, cada rainha já estará em colunas individuais
	return 1;
}

int main(int argc, char* argv[])
{
    int n;
    int max_iter = 1;
	int max_queens;
    
    double start_time, end_time;
    int number_solutions = 0;
        
	{
	    int num_threads;
        int i;

		//se existe primeiro argumento da linha de comando, n = argumento, senão n recebe 8 por default
        n = (argc > 1) ? atoi(argv[1]) : 8;//atoi converte string para inteiro, determina tamanho do tabuleiro
		
		//se existe segundo argumento da linha de comando, num_threads = argumento, senão num_threads recebe 30 por default
        num_threads = (argc > 2) ? atoi(argv[2]) : 30;//determina quantas threads serão criadas

        max_queens = n;//programa só irá posicionar quantas rainhas quanto existem linhas no tabuleiro
        
        omp_set_num_threads(num_threads);
	        
        for (i = 0; i < n; i++)
        {
            max_iter *= n;//determina quantas iterações serão feitas, dado o tamanho do tabuleiro
        }
    }
  
    start_time = omp_get_wtime();//inicia timer
    
	int iter;//número da iteração

	#pragma omp parallel for
	for (iter = 0; iter < max_iter; iter++)
	{
		/*
		o código em paralelo essencialmente testa todas as 
		posições de rainhas na força bruta usando da função 
		'verificaSolucao' para recusar soluções incorretas.
		Como todas as variações de posicionamento são testadas, 
		eventualmente encontra-se somente as soluções corretas
		*/
		
		int code = iter;
		int i;
	    int queen_rows[max_queens];
		
		for (i = 0; i < n; i++)//'i' representa o número da rainha, que também define qual coluna está posicionada no tabuleiro
		{
			queen_rows[i] = code % n;//o valor de queen_rows[i] determina em qual linha a rainha será posicionada
			
			code /= n;
		}
		
		if (verificaSolucao(n, max_queens, queen_rows))//verifica se, no vetor de rainhas, existem rainhas em posições inválidas
		{
			#pragma omp atomic
		    number_solutions++;//quando a solução é encontrada, garante sincronização da atualização do número de soluções encontradas
		    /*
			#pragma omp critical//usado somente para imprimir tabuleiro da solução no terminal
            {
			    printf("\n");
			    for (i = 0; i < n; i++)
			    {
			        int j;
				    for (j = 0; j < n; j++)
				    {
					    if (queen_rows[i] == j)	printf("|X");
					    else printf("| ");
				    }
				    printf("|\n");
			    }
			    printf("\n");
			}
			*/
		}
	}//fim da seção paralela

    end_time = omp_get_wtime();//recebe tempo decorrido
    printf("Tempo de execução: %g segundos\n", end_time - start_time);
    printf("Soluções encontradas: %d\n", number_solutions);
    
	return 0;
}
