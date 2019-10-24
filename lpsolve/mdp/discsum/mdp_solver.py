import sys
from collections import defaultdict

def additive(x):
	x=float(x)
	if x>=0:
		return " + "+str(x)
	else:
		return " - "+str(-x)

input_file = sys.argv[1]
with open(input_file) as fin:
    lines = ''.join(fin.readlines()).split('\n')[:-1]
    
    cn, pn, m, lamda = lines[0].split(' ')

    choose_vertices = lines[3].split(' ')
    prob_vertices = lines[5].split(' ')

    edges = lines[7:int(m)+7]

    succs = defaultdict(list)
    reward = {}

    for edge in edges:
        v, u, p, r, flag = edge.split(' ')
        if v in choose_vertices and flag == '0':
            continue

        succs[v].append((u, float(p)))
        reward[(v, u)]=float(r)

    with open('out.txt', 'w') as fout:
        min_res = 'min: 0 '
        for v in choose_vertices:
            min_res += ' + x_%s' % v

        fout.write(min_res + ';\n')
        

        for v, succ in succs.items():
            if len(succs[v])==0:
				fout.write('x_%s = 0;\n' % v)
				continue

            if v in choose_vertices:
                for u, p in succ:
                    fout.write('x_%s >= %s + %s * x_%s;\n' % (v, reward[(v, u)], lamda, u))
                continue

            res = 'x_%s = 0 ' % v
            for u, p in succ:
                res += '  %s %s * x_%s ' % (additive(p*reward[(v, u)]), additive(p*float(lamda)), u)
            fout.write(res + ';\n')