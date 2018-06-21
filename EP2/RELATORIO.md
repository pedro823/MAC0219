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
    