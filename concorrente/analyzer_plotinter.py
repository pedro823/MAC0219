import matplotlib.pyplot as plt
import json
with open('outs/5_threads_3000000_num.json') as f:
    j1 = json.load(f)
with open('outs/rr_sched.json') as f:
    j2 = json.load(f)
with open('outs/5_threads_500000_num.json') as f:
    j3 = json.load(f)
with open('outs/5_threads_3000000_num.json') as f:
    j4 = json.load(f)

plt.figure(1)
plt.subplot(211)
plt.suptitle('gate, 5 Threads, 3000000 interações')
plt.title('SCHED_OTHER')
x = [i['execution_number'] for i in j1 if i['algorithm'] == 'gate']
y = [i['elapsed_time_sec'] for i in j1 if i['algorithm'] == 'gate']
mean = sum(y) / len(y)
print(mean)
m = [mean] * len(y)
plt.plot(x, y, 'ro')
plt.plot(x, m, 'k--')
plt.ylabel('Tempo (seg)')
plt.subplot(212)
plt.title('SCHED_RR')
x = [i['execution_number'] for i in j2 if i['algorithm'] == 'gate']
y = [i['elapsed_time_sec'] for i in j2 if i['algorithm'] == 'gate']
mean = sum(y) / len(y)
print(mean)
m = [mean] * len(y)
plt.plot(x, y, 'ro')
plt.plot(x, m, 'k--')
plt.xlabel('Número da iteração')
# plt.subplot(221)
# plt.title('500000 interações')
# x = [i['execution_number'] for i in j3 if i['algorithm'] == 'bakery']
# y = [i['elapsed_time_sec'] for i in j3 if i['algorithm'] == 'bakery']
# mean = sum(y) / len(y)
# m = [mean] * len(y)
# plt.plot(x, y, 'bo')
# plt.plot(x, m, 'k--')
# plt.xlabel('Número da iteração')
# plt.subplot(222)
# plt.title('3000000 interações')
# x = [i['execution_number'] for i in j4 if i['algorithm'] == 'bakery']
# y = [i['elapsed_time_sec'] for i in j4 if i['algorithm'] == 'bakery']
# mean = sum(y) / len(y)
# m = [mean] * len(y)
# plt.plot(x, y, 'bo')
# plt.plot(x, m, 'k--')
# plt.xlabel('Número da iteração')
plt.savefig('figs/5_threads_inter1.png')
plt.show()
