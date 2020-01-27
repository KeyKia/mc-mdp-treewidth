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

    with open('model.txt', 'w') as fout:

        fout.write('dtmc\n')
        fout.write('module graph\n')
        fout.write('x : [0..%s];\n' % (int(n)-1))

        for v, succ in succs.items():
            prob_sum = sum([al[1] for al in succ])
            if prob_sum < 0.000000001:
                for i in range(len(succ)):
                    if succ[i][0] == v:
                        succ[i] = (v, 1.0)

            res = '[] x=%s -> ' % v
            for u, p in succ:
                res += '%s: (x\'=%s) + ' % (p, u)

            if res.endswith('+ '):
                res = res[:-2]

            fout.write(res + ';\n')

        fout.write('endmodule\n')
        fout.write('init\n')
        fout.write('true\n')
        fout.write('endinit\n')

    with open('property.txt', 'w') as fout:
        fout.write('%s' % target)

