import sys
from collections import defaultdict

input_file = sys.argv[1]
with open(input_file) as fin:
    lines = ''.join(fin.readlines()).split('\n')[:-1]
    cn, pn, m, target = lines[0].split(' ')
    choose_vertices = lines[3].split(' ')
    prob_vertices = lines[5].split(' ')
    edges = lines[7:int(m) + 7]
    succs = defaultdict(list)

    for edge in edges:
        v, u, p, flag = edge.split(' ')
        if v in choose_vertices and flag == '0':
            continue

        succs[v].append((u, float(p)))

    with open('model.txt', 'w') as fout:
        fout.write('mdp\n')
        fout.write('module graph\n')
        fout.write('x : [0..%s];\n' % (int(cn) + int(pn) - 1))

        for v, succ in succs.items():
            if v in choose_vertices:
                for u, p in succ:
                    fout.write('[] x=%s -> 1.0: (x\'=%s);\n' % (v, u))
                continue

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
        fout.write('filter(print, Pmax=? [F x=%s]);\n' % target)
