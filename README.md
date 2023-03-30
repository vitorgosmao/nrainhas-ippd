# nrainhas ippd
 Repositório para o trabalho 1 da cadeira de IPPD

## Integrantes do Grupo

Pedro Thiago Roque

Ricardo Ferreira Padilha

Vitor Costa Gosmão

Yuri Borges Sena

# Informações sobre o código 
## Descrição

O código em C implementa o problema das N-Rainhas, que consiste em posicionar N rainhas em um tabuleiro de NxN de modo que elas não se ataquem.

## Como compilar o código

Para compilar o código em C, execute o seguinte comando em um terminal:

`gcc nrainhas.c -fopenmp`

## Como executar o código

Para executar o código, utilize o seguinte comando:

`/a.exe "tamanhoDoTabuleiro=numDeRainhas" "numeroDeThreads"`

Exemplo de uso

Para posicionar 8 rainhas utilizando 2 threads, execute o seguinte comando:

`./nqueens 8 2`