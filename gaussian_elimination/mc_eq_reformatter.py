import sys
import json
import random
import numpy as np

random.seed(30)

"""
\<list of edges\>
"""

def get_successors(inp):
    inp = inp.strip()
    inp = inp[1:-1]
    inp = inp.split('],')

    succs = {}
    for literal in inp:
        if not literal.endswith(']'):
            literal += ']'
        fr, to_list = literal.split('=')
        succs[int(fr)] = json.loads(to_list)
    return succs


def get_bags(inp):
    inp = inp.split(':')[1].strip()[1:-1]
    inp = inp.split('], ')

    bs = []
    for literal in inp:
        if not literal.endswith(']'):
            literal += ']'

        _, to_list = literal.split('=')
        bs.append(json.loads(to_list))
    return bs



def get_tree(inp):
    inp = inp.split(':')[1].strip()[1:-1]
    inp = inp.split(', ')
    edges = []
    for literal in inp:
        fr, to = map(int, literal.split('='))
        if fr != to:
            edges.append((fr, to))
    return edges


input_file = sys.argv[1]
output_dir = sys.argv[2]
with open(input_file) as f:
    lines = ''.join(f.readlines()).split('\n')
    for i in range(0, len(lines), 15):
        method_name = lines[i+2].split(':')[1]
        output_file = output_dir + '/' + method_name + '.txt'
        with open(output_file, 'w') as out:
            n = int(lines[i].split(':')[1])

            successors = get_successors(lines[i+1].split(':')[1])
            vertices = [j+1 for j in range(n)]
            random.shuffle(vertices)

            edge_probs = np.zeros((n+1, n+1))
            for v in vertices:
                probs = [random.random() for u in successors[v]]
                norm = sum(probs)
                for j, u in enumerate(successors[v]):
                    edge_probs[v][u] = probs[j] / norm

            A = np.zeros((n, n))
            B = np.zeros(n)
            for v in vertices:
                if v == n: #target
                    A[n-1][n-1] = 1
                    B[n-1] = 1
                else:
                    A[v-1][v-1] = -1
                    for u in successors[v]:
                        A[v-1][u-1] = edge_probs[v][u]

            out.write("%s\n" % n)
            for j in range(n):
                for z in range(n):
                    out.write("%s " % A[j][z])
                out.write("\n")

            out.write("\n")
            for j in range(n):
                out.write("%s " % B[j])

