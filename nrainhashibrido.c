#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <mpi.h>
#include <omp.h>

int verificaSolucao(int n, int max_queens, int queen_rows[max_queens])//queen_rows representa a linha do tabuleiro onde está a rainha, n é o tamanho do tabuleiro
{
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = i+1; j < n; j++)//compara cada rainha com as outras que não foram comparadas ainda
        {
            if (queen_rows[i] == queen_rows[j]) return 0;//2 rainhas na mesma coluna, recusa

            if (queen_rows[i] - queen_rows[j] == i - j || queen_rows[i] - queen_rows[j] == j - i) return 0;//2 rainhas na mesma diagonal, recusa
        }
    }
    //por default, cada rainha já estará em colunas individuais
    return 1;
}

int main(int argc, char *argv[])
{
    int n, max_iter, max_queens, rank, size, i, iter;
    double start_time, end_time;
    int number_solutions = 0;
    int num_threads;


    // inicializa o ambiente MPI com MPI_Init e obtém o tamanho e a posição do processo atual no grupo com MPI_Comm_size e MPI_Comm_rank
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //se existe primeiro argumento da linha de comando, n = argumento, senão n recebe 8 por default
    n = (argc > 1) ? atoi(argv[1]) : 8;
    //se existe segundo argumento da linha de comando, num_threads = argumento, senão num_threads recebe 30 por default
    num_threads = (argc > 2) ? atoi(argv[2]) : 30;//determina quantas threads serão criadas


    // define o número máximo de rainhas e o número máximo de iterações que serão feitas
    max_queens = n;
    max_iter = 1;
    
    omp_set_num_threads(num_threads);


    // número de iterações é calculado usando um loop for que multiplica n pela potência do número de iterações
    for (i = 0; i < n; i++)
    {
        max_iter *= n;
    }

    //inicia timer
    start_time = MPI_Wtime();

#pragma omp parallel for private(iter)
    for (iter = rank; iter < max_iter; iter += size)
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
        }
    }

    int total_solutions = 0;
#pragma omp critical
    
    // programa soma as soluções encontradas por cada thread e armazena
    // o resultado na variável total_solutions com uma cláusula omp critical e MPI_Reduce.

    //acumula resultados de cada thread
    MPI_Reduce(&number_solutions, &total_solutions, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
   
    end_time = MPI_Wtime();
    //encerra timer

    if (rank == 0)
    {
        printf("Tempo de execucao: %g segundos\n", end_time - start_time);
        printf("Solucoes encontradas: %d\n", total_solutions);
    }

    MPI_Finalize();
    //fim da seção paralela

    return 0;
}
