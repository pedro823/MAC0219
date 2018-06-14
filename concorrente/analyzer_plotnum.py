import matplotlib.pyplot as plt
import json
with open('outs/5_threads_500_num.json') as f:
    j = json.load(f)

plt.figure(1)
plt.subplot(211)
plt.suptitle('5 Threads, numero variável de interações')
plt.text(45, 12.45, 'Bakery')
x = [i['execution_number'] for i in j if i['algorithm'] == 'bakery']
y = [i['elapsed_time_sec'] for i in j if i['algorithm'] == 'bakery']
mean = sum(y) / len(y)
std_dev = sum([(i - mean) ** 2 for i in y]) ** 0.5
print('Bakery: Média', str(round(mean, 4)) + 's,', 'desvio padrão', str(round(std_dev, 4)) + 's')
plt.plot(x, y, 'bo')
plt.ylabel('Tempo (seg)')
plt.subplot(212)
x = [i['execution_number'] for i in j if i['algorithm'] == 'gate']
y = [i['elapsed_time_sec'] for i in j if i['algorithm'] == 'gate']
mean = sum(y) / len(y)
std_dev = sum([(i - mean) ** 2 for i in y]) ** 0.5
print('Gate: Média', str(round(mean, 4)) + 's,', 'desvio padrão', str(round(std_dev, 4)) + 's')
plt.plot(x, y, 'ro')
plt.text(45, 9.01, 'Gate')
plt.xlabel('Número da iteração')
plt.savefig('figs/2_threads_3000000_num_time.png')
plt.show()
