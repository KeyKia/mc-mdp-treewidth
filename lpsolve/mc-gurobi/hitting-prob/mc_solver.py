import sys
from collections import defaultdict

input_file = sys.argv[1]
with open(input_file) as fin:
    lines = ''.join(fin.readlines()).split('\n')[:-1]
    
    cn, pn, m, target = lines[0].split(' ')

    choose_vertices = lines[3].split(' ')
    prob_vertices = lines[5].split(' ')

    edges = lines[7:int(m)+7]

    succs = defaultdict(list)

    for edge in edges:
        v, u, p, flag = edge.split(' ')
        if v in choose_vertices and flag == '0':
            continue

        succs[v].append((u, float(p)))

    with open('out.lp', 'w') as fout:
        min_res = 'Minimize \n 0 '
        for v in choose_vertices:
            min_res += ' + x_%s' % v

        fout.write(min_res + '\nSubject To \n')
        fout.write('x_%s = 1\n' % target)

        for v, succ in succs.items():
            if v == target:
                continue

            if v in choose_vertices: #treat it like a probabilistic vertex anyway -- hence making it an MC
                res='0'
                for u, p in succ:
                    res += ' + %s x_%s' % (float(1)/len(succ),u)
                res += ' - x_%s = 0' % v
                fout.write(res + '\n')
                continue

            #res = 'x_%s = 0 ' % v
            res='0'
            for u, p in succ:
                res += ' + %s  x_%s' % (p, u)
            res += ' - x_%s = 0' % v
            fout.write(res + '\n')
	fout.write("End\n")
