from sys import argv, exit
import random as r
random = r.SystemRandom()

def r():
    return random.randint(0, 2**16 - 1)

def print_usage():
    print("usage:", argv[0], "<N> <outfile>")
    print("Generates N matrices for EP2.")

def generate_matrices(N, outfile):
    ast = '***\n'
    with open(outfile, 'w') as f:
        f.write(str(N) + '\n' + ast)
        for _ in range(N):
            for i in range(3):
                for j in range(3):
                    f.write(str(r()) + ' ')
                f.write('\n')
            f.write(ast)

if __name__ == '__main__':
    if len(argv) < 2:
        print_usage()
        exit(-1)
    try:
        N = int(argv[1])
        generate_matrices(N, argv[2])
    except Exception as e:
        print('error', e)
        print_usage()
        exit(-1)
