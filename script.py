import subprocess

# definir as entradas
inputs = [
    (5, 0),
    (7, 0),
    (9, 0),
    (5, 2),
    (7, 2),
    (9, 2),
    (5, 4),
    (7, 4),
    (9, 4),
    (5, 8),
    (7, 8),
    (9, 8)
]

# número de vezes que o código será executado para cada entrada
num_runs = 1

# nome do arquivo de saída
output_file = "results.txt"

# executar o código C várias vezes com diferentes entradas
with open(output_file, "w") as f:
    for n, num_threads in inputs:
        for i in range(num_runs):
            command = ["./a", str(n), str(num_threads)]
            result = subprocess.run(command, capture_output=True, text=True)
            f.write(f"Entrada: {n} rainhas, {num_threads} threads\n")
            f.write(f"Execução {i+1}/{num_runs}\n")
            f.write(result.stdout)
            f.write("\n\n")