import matplotlib.pyplot as plt
import json

with open('outs/2_threads_3000000_num.json') as f:
    j = json.load(f)

v_bakery = [0] * 2
v_gate = [0] * 2
for i in j:
    if i['algorithm'] == 'bakery':
        for idx, x in enumerate(i['access_array']):
            v_bakery[idx] += x / 100
    else:
        for idx, x in enumerate(i['access_array']):
            v_gate[idx] += x / 100

plt.figure(1)
plt.suptitle('Média de acesso à seção crítica, em 2 threads')
plt.subplot(211)
plt.bar([-5] + list(range(1, len(v_bakery) + 1)) + [5], [0] + v_bakery + [0], color='blue', align='center')
plt.text(4.5, 6000, 'Bakery')
plt.ylabel('Acessos')
plt.subplot(212)
plt.bar([-5] + list(range(1, len(v_gate) + 1)) + [5], [0] + v_gate + [0], color='red', align='center')
plt.text(4.6, 8000, 'Gate')
plt.xlabel('Thread')
plt.savefig('figs/access_array_2t_3000000n')
