from math import ceil
a = 1000003
while True:
    if a % 2 == 0:
        print(a, 'nope')
        a += 1
        continue
    sq = a ** 0.5
    for i in range(3, ceil(sq)):
        print('i = ', i)
        if a % i == 0:
            print('a', a, 'i', i, 'nope')
            a += 2
            break
    else:
        print(a)
        break
