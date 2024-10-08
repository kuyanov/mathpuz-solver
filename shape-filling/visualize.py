import sys
import os
from math import *
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt


def add(p1, p2):
    return [p1[0] + p2[0], p1[1] + p2[1]]


def mul(p1, x):
    return [p1[0] * x, p1[1] * x]


if len(sys.argv) != 3:
    print(f'usage: {sys.argv[0]} LATTICE_FILENAME DESTINATION_DIR')
    exit(1)

colors = ['blue', 'orange', 'green', 'red', 'purple', 'brown', 'pink', 'gray', 'olive', 'cyan',
          'lightblue', 'lightsalmon', 'lightgreen', 'lightcoral', 'plum', 'gold', 'indigo', 'lightgray', 'lime',
          'turquoise', 'sandybrown', 'magenta', 'darkviolet', 'mediumslateblue', 'whitesmoke', 'wheat', 'dodgerblue',
          'darkkhaki', 'chocolate', 'firebrick']

fin = open(sys.argv[1], 'r')
cnt_v = int(fin.readline())
vs = [list(map(float, fin.readline().split())) for _ in range(cnt_v)]
cnt_f = int(fin.readline())
fs = [list(map(int, fin.readline().split()))[1:] for _ in range(cnt_f)]
rot_ord = float(fin.readline())
v1 = [1, 0]
v2 = [cos(2 * pi / rot_ord), sin(2 * pi / rot_ord)]

dest = sys.argv[2]
os.makedirs(dest, exist_ok=True)

cnt = 0
while (status := input()) == 'FOUND':
    cnt += 1
    n = int(input())
    cols = [[[0] * cnt_f for _ in range(n)] for _ in range(n)]
    for i in range(n):
        arr = list(map(int, input().split()))
        for j in range(n):
            for f in range(cnt_f):
                cols[i][j][f] = arr[j * cnt_f + f]

    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.set_aspect('equal')
    plt.axis('off')
    plt.tight_layout()

    for i in range(n):
        for j in range(n):
            shift = add(mul(v1, i), mul(v2, j))
            for f in range(cnt_f):
                c = cols[i][j][f]
                if c != 0:
                    color = colors[(c - 1) % len(colors)] if c > 0 else 'white'
                    coords = [add(shift, vs[v]) for v in fs[f]]
                    coords.append(coords[0])
                    ax.fill(*zip(*coords), facecolor=color, edgecolor='black')

    filename = f'{dest}/{cnt}.svg'
    print(f'saving: {filename}')
    plt.savefig(filename)
    plt.close(fig)
