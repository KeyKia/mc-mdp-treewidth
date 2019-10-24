import sys
from collections import defaultdict
import time
import re
import os


diff = 0

input_file = sys.argv[1]
with open(input_file) as fin:
    lines = ''.join(fin.readlines()).split('\n')[:-1]
    cn, pn, m, target = lines[0].split(' ')

    choose_vertices = lines[3].split(' ')
    stra = {v: 0 for v in choose_vertices}
    prob_vertices = lines[5].split(' ')

    edges = lines[7:int(m)+7]

    succs = defaultdict(list)

    for edge in edges:
        v, u, p, flag = edge.split(' ')
        if v in choose_vertices and flag == '0':
            continue

        succs[v].append((u, float(p)))

    ok = False
    while not ok:
        with open('out.txt', 'w') as fout:
            min_res = 'min:'
            for v in choose_vertices:
                min_res += ' + x_%s' % v

            fout.write(min_res + ';\n')
            fout.write('x_%s = 1;\n' % target)

            for v, succ in succs.items():
                if v == target:
                    continue

                if v in choose_vertices:
                    if len(succ) == 0:
                        continue
                    u, p = succ[stra[v]]
                    fout.write('x_%s = x_%s;\n' % (v, u))
                    continue

                res = 'x_%s =' % v
                for u, p in succ:
                    res += ' + %s * x_%s' % (p, u)
                fout.write(res + ';\n')

        start = time.time()
        os.system("lp_solve < out.txt > res.txt")
        diff += time.time() - start

        with open("res.txt") as rin:
            lines = ''.join(rin.readlines()).split('\n')[:-1]
            probs = {}
            for l in lines[4:]:
                l = re.sub(' +', ' ', l)
                v, p = l.split(' ')
                probs[v[2:]] = float(p)

        ok = True
        new_stra = {}
        for v in choose_vertices:
            best = 0
            for i, (u, p) in enumerate(succs[v]):
                if probs[succs[v][best][0]] < probs[u]:
                    best = i
            new_stra[v] = best
            if new_stra[v] != stra[v]:
                if abs(probs[succs[v][best][0]] - probs[succs[v][stra[v]][0]]) < 1e-9:
                    continue
                ok = False
                break

        stra.update(new_stra)

    print diff
