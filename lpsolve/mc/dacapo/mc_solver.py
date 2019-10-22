import sys
from collections import defaultdict

input_file = sys.argv[1]
with open(input_file) as fin:
    lines = ''.join(fin.readlines()).split('\n')[:-1]
    n, m, target = lines[0].split(' ')
    edges = lines[2:int(m)+2]

    succs = defaultdict(list)

    for edge in edges:
        v, u, p = edge.split(' ')
        succs[v].append((u, float(p)))

    with open('out.txt', 'w') as fout:
        fout.write('min: ;\n')
        fout.write('x_%s = 1;\n' % target)

        for v, succ in succs.items():
            if v == target:
                continue

            res = 'x_%s =' % v
            for u, p in succ:
                res += ' + %s * x_%s' % (p, u)
            fout.write(res + ';\n')
