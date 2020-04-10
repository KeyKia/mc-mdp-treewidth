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
    lines = ''.join(f.readlines()).split('\n')[:-1]
    for i in range(0, len(lines), 15):
        method_name = lines[i + 2].split(':')[1]
        output_file = output_dir + '/' + method_name + '.txt'
        with open(output_file, 'w') as out:
            n = int(lines[i].split(':')[1])

            successors = get_successors(lines[i + 1].split(':')[1])
            vertices = [j + 1 for j in range(n)]
            random.shuffle(vertices)

            edge_probs = np.zeros((n + 1, n + 1))
            for v in vertices:
                probs = [random.random() for u in successors[v]]
                norm = sum(probs)
                for j, u in enumerate(successors[v]):
                    edge_probs[v][u] = probs[j] / norm

            edges = []
            edge_indices = np.zeros((n + 1, n + 1), dtype=int)
            edge_indices[:, :] = -1
            bags = get_bags(lines[i + 11])
            for bag in bags:
                for v in bag:
                    for u in bag:
                        if edge_indices[v][u] == -1:
                            edge_indices[v][u] = len(edges)
                            if edge_probs[v][u] == 0:
                                edges.append((v, u, edge_probs[v][u], 0))
                            else:
                                edges.append((v, u, edge_probs[v][u], random.randint(-100, 100)))

            lamda = min(random.random(), 0.9)
            out.write("%s %s %s\n\n" % (n, len(edges), lamda))  # totV, totE, lambda

            for edge in edges:
                out.write("%s %s %s %s\n" % (edge[0] - 1, edge[1] - 1, edge[2], edge[3]))  # 0-based

            out.write("\n%s\n\n" % len(bags))  # number of bags

            for bag in bags:
                out.write("%s %s\n" % (len(bag), len(bag) * len(bag)))
                out.write(' '.join(map(str, list(v - 1 for v in bag))) + "\n")
                for u in bag:
                    for v in bag:
                        out.write("%s " % edge_indices[u][v])
                out.write("\n\n")

            tree = get_tree(lines[i + 9])
            for edge in tree:
                out.write("%s %s\n" % (edge[0]-1, edge[1]-1))
