#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

#include <mpi.h>
#include <omp.h>

int verificaSolucao(int n, int max_queens, int queen_rows[max_queens])
{
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = i+1; j < n; j++)
        {
            if (queen_rows[i] == queen_rows[j]) return 0;

            if (queen_rows[i] - queen_rows[j] == i - j || queen_rows[i] - queen_rows[j] == j - i) return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[])
{
    int n, max_iter, max_queens, rank, size, i, iter;
    double start_time, end_time;
    int number_solutions = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    n = (argc > 1) ? atoi(argv[1]) : 8;
    max_queens = n;
    max_iter = 1;

    for (i = 0; i < n; i++)
    {
        max_iter *= n;
    }

    start_time = MPI_Wtime();

#pragma omp parallel for private(iter)
    for (iter = rank; iter < max_iter; iter += size)
    {
        int code = iter;
        int i;
        int queen_rows[max_queens];

        for (i = 0; i < n; i++)
        {
            queen_rows[i] = code % n;
            code /= n;
        }

        if (verificaSolucao(n, max_queens, queen_rows))
        {
#pragma omp atomic
            number_solutions++;
        }
    }

    int total_solutions = 0;
#pragma omp critical
    MPI_Reduce(&number_solutions, &total_solutions, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    end_time = MPI_Wtime();

    if (rank == 0)
    {
        printf("Tempo de execucao: %g segundos\n", end_time - start_time);
        printf("Solucoes encontradas: %d\n", total_solutions);
    }

    MPI_Finalize();

    return 0;
}
