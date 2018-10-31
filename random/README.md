Por: Pedro Pereira (9778794) e Breno Moura (9790972)
Mini Desafio 2  

### Introdução ###
Nesse "Mini" Desafio 2 conseguimos fazer um programa que apresenta um desempenho
melhor quando não usamos hyperThreading. O programa é um contador que soma em posições da memória diferentes (no caso com false sharing, quando a posições da memória estão próximas, e sem false sharing, quando elas estão distantes).

### Como rodar o programa ###

Compilar:

	$ make

Rodar:

	$ ./mini_ep

### Ativando e desativando hyperThreading ###
Primeiramente, é necessário saber se o seu CPU tem hyperThreading.  
Para isso, é possível seguir o nosso tutorial no PACA: https://paca.ime.usp.br/mod/forum/discuss.php?d=37578

Ou também copiado aqui:  
1. Existe um arquivo no linux chamado /proc/cpuinfo. Ele contém todas as informações do(s) CPU(s) da máquina.  
bash: $ cat /proc/cpuinfo | grep model  
Esse comando lhe dará o modelo do CPU da máquina.  

2. a) Se você está rodando um processador AMD antes da arquitetura Ryzen (AMD FX, A6, A8, A10 ou os da rede linux AMD Phenom II), você não tem SMT. O que você tem são 6 (ou 8) cores separados em 3 (ou 4) módulos. São cores genuinas mas com recursos compartilhados.  
   b) Se você está com um processador Intel, você pode checar nesse site: https://ark.intel.com . O primeiro número após o traço indica a geração do CPU (por exemplo, um intel i7-4790K foi um CPU da 4a geração Intel Core)  
   c) Se você está com um processador AMD após a arquitetura Ryzen (os mais novos), cheque este site: https://www.amd.com/pt  

Agora, **se e somente se o seu CPU realmente suporta hyperThreading ou SMT**,
você pode seguir os seguintes passos para **desativar** as hyperthreads:

#### Desativando ####

Digamos que seu CPU tem 4 *cores* com hyperThreading, fazendo 8 *cores* virtuais. Para desabilitar as 4 *cores* de hyperThreading, é preciso:

+ Identificar as *cores* que precisam ser desligadas:
  	$ cat /proc/info | grep -B12 core\ id

+ Desligar todas menos uma *core* de cada *core ID*. **Por exemplo** as *cores* 4, 5, 6 e 7.  

  	# echo 0 > /sys/devices/system/cpu/cpu4/online
  	# echo 0 > /sys/devices/system/cpu/cpu5/online
  	# echo 0 > /sys/devices/system/cpu/cpu6/online
  	# echo 0 > /sys/devices/system/cpu/cpu7/online

#### Reativando ####

Simplesmente troque todos os 0 do comando acima por 1:

	# echo 1 > /sys/devices/system/cpu/cpu4/online
	# echo 1 > /sys/devices/system/cpu/cpu5/online
	# echo 1 > /sys/devices/system/cpu/cpu6/online
	# echo 1 > /sys/devices/system/cpu/cpu7/online

### O programa ###
Criamos 4 threads, cada uma somando 1 em posições de memória.  
No caso do falso compartilhamento, as posições de memória estão próximas, o que
faz elas ficarem **no mesmo bloco de cache**. Isso implica que se uma thread
alterar o valor de uma das variáveis, as outras threads terão o
cache invalidado, *por mais que as posições de memória não tenham nada em comum.*
Isso causa um grande *overhead* em tempo computacional devido ao cache não ser
utilizado corretamente.  
Já no caso sem falso compartilhamento, as posições de memória estão longe o
suficiente para que não fiquem no mesmo bloco de cache. Isso implica que alterar
o valor em tais posições **não invalida o cache das outras**. Logo, o programa
roda muito mais rápido por melhor aproveitamento de cache.

O interessante é que hyperThreading não tem muito a ver com isso! Mas o fato de que
duas threads podem ir no mesmo *core* físico altera drasticamente o resultado, como
se pode ver abaixo.

### Resultados ###

Com hyperThreading:

	Elapsed time in sequential: 0.279355s  
	Elapsed time with false sharing: 5.576346s  
	Elapsed time without false sharing: 1.677878s  

Sem hyperThreading:  

	Elapsed time in sequential: 0.275732s
	Elapsed time with false sharing: 1.612686s
	Elapsed time without false sharing: 1.014331s

Sendo que os casos com false sharing e sem false sharing são executados em paralelo.

Nota: em ambos os testes foram utilizados 4 threads em CPUs dual-core com
hyperThreading habilitável.

### Conclusão ###
Nossa conclusão, por mais confusa que seja, é que o programa
de simulação de falso compartilhamento altamente depende do
hyperThreading estar ligado ou não. Infelizmente não conseguimos olhar mais a fundo
no problema para poder explicar o porquê disso, por falta de tempo. De qualquer
forma, é interessante saber que hyperThreading pode causar grande ineficiência em
programas paralelos.
