## NOME: Breno Helfstein Moura
## NUSP: 9790972

## NOME: Pedro Leyton Pereira
## NUSP: 9778794

--> Sobre a máquina utilizada
    Estamos usando uma Tesla K20C, que foi disponibilizada na rede linux. Essa placa tem 5GB de RAM, 1 chip GK110 com a capacidade de 706Mhz em base clock e 1300Hz em memory clock. Sabemos que o problema é memory-bound. Na teoria, com uma memória de 320-bit cujo o clock efetivo é de 1300 * 4 = 5200Mhz, conseguimos até 208GB/s.

--> Sobre a estratégia adotada para a redução
    Para a redução pedida optamos por seguir a estratégia utilizada nos slides "Parallel Reduction in Shared Memory", que foram disponibilizados no PACA. Os slides da NVIDIA utilizam uma estratégia de de decomposição do kernel para a redução em paralelo. Utilizamos a mesma estratégia, fazendo uma redução para cada posição da matriz.

--> Tamanho dos Blocos e Número de threads
    Para o tamanho dos blocos e número de threads, acreditamos que 32 Threads e dividir o vetor em blocos de tamanho 64 (2 * NUM_THREADS) é suficientemente rápido. Assim temos teto de (N / 64) Blocos para uma instância com N matrizes.

--> Tempos obtidos
    Todos os tempos abaixo foram feitos com matrizes geradas aleatoriamente pelo generate.py. Os resultados são de arquivos com os seguintes tamanhos de matrizes:

    10.000.000 Matrizes:
    # CUDA: 35261 Microsegundos
    # Sequencial: 403150 Microsegundos

    1.000.000 Matrizes:
    # CUDA: 9336 Microsegundos
    # Sequencial: 39043 Microsegundos

    100.000 Matrizes:
    # CUDA: 3455 Microsegundos
    # Sequencial: 9354 Microsegundos

    10.000 Matrizes:
    # CUDA: 3220 Microsegundos
    # Sequencial: 988 Microssegundos

    1.000 Matrizes:
    # CUDA: 2178 Microsegundos
    # Sequencial: 81 Microssegundos

--> Conclusão
    Vemos que a redução em CUDA apresentou um resultado superior com aproximadamente 10.000 matrizes, isso é 10^5 matrizes.
    Isso faz sentido, dado que temos um overhead para compartilhar a memória do computador com a memória da GPU e para copiar as matrizes.
    Para de tamanho 10^5 tivemos uma melhora de 3x (Isto é, a execução em paralelo é 3x mais rápida). Para 10^6, tivemos uma melhora de 4x, e para 10^7 uma melhora de 11x.
    Vemos que a execução em paralelo trás benefícios para casos em que temos muitas matrizes.

    Tentamos evitar a divergência de branch no minimo, contudo usualmente isso implicava em casts ou em overflow (O que causaria resultado possivelmente equivocado). Optamos por usar o min do CUDA, que se mostrou mais confiável e rápido que nossas implementações.