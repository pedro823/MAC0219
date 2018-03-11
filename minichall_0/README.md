### Mini EP & Mini Desafio 1 ###
Feito por: Pedro Pereira (github.com/pedro823, n.usp 9778794)

O mini-EP evidencia que existe cacheamento dentro do processador.  
O mini-desafio evidencia que existe execução especulativa 
dentro do processador.

#### Como? ####

**Mini-EP**: Cria um vetor grande cheio de 1's. Logo após percorre
o vetor linearmente e não linearmente, fazendo a mesma soma.
A soma em si não importa, o que importa é que ao percorrer o vetor
não linearmente, o cache não acerta tanto quanto na versão linear.
Logo, a versão linear se completa *em menor tempo*.

**Mini-Desafio**: Cria um vetor randômico de `true` e `false`.
Percorre primeiro no vetor *as-is*, linearmente, fazendo uma checagem:
```C
if (v[i]) {
    a++;
}
```
Logo depois, ordena o vetor
para ter `false` primeiro e `true` depois, e percorre 
uma segunda vez, fazendo a mesma comparação.  
A ideia aqui é de que, se há execução especulativa no CPU, ele irá
tentar rodar no *pipeline* a parte mais provável de acontecer no `if`.
No caso do primeiro percorrer, a execução especulativa erra muitas vezes,
causando um tempo de execução maior. Já no segundo caso, como o vetor está
ordenado, o CPU errará poucas vezes, fazendo uma execução mais rápida.

### Para rodar ###

    g++ mini_ep.cpp -o ep
    g++ mini_desafio.cpp -o desafio
    ./ep; ./desafio # Vai demorar um pouco. Fique tranquilo.

### Meus resultados ###

Specs: Intel Core i5-6200U @ 2.6GHz; RAM: 8GB DDR4; OS: Manjaro GNOME

./ep:
    Elapsed time (ns) in linear: 18999
    Elapsed time (ns) non linear: 44273

./desafio:
    4999058 Should equal 4999058
    Elapsed time UNORDERED: 0.311764
    Elapsed time ORDERED: 0.223905
