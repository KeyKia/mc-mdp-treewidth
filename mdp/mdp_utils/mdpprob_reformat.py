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
            choose_vertices, probability_vertices = vertices[:int(len(vertices) / 2)], vertices[int(len(vertices) / 2):]

            edge_probs = np.zeros((n+1, n+1))
            for v in vertices:
            	if v in probability_vertices:
	                probs = [random.random() for u in successors[v]]
	                norm = sum(probs)
	                for j, u in enumerate(successors[v]):
	                    edge_probs[v][u] = probs[j] / norm

            edges = []
            edge_indices = np.zeros((n+1, n+1), dtype=int)
            edge_indices[:, :] = -1
            bags = get_bags(lines[i + 11])
            bag_edges = []
            for bag in bags:
                cur_bag_edges = []
                for v in bag:
                    for u in bag:
                        if edge_indices[v][u] == -1:
                            edge_indices[v][u] = len(edges)
                            edges.append((v, u, edge_probs[v][u]))
                        cur_bag_edges.append(edge_indices[v][u])

            target = random.randint(1, n)
            out.write("%s %s %s %s\n\n" % (len(choose_vertices), len(probability_vertices), len(edges), target-1)) # totV, totE, target
            out.write("\n" + ' '.join(map(str, choose_vertices)) + "\n")
            out.write("\n" + ' '.join(map(str, probability_vertices)) + "\n\n")

            for edge in edges:
            	if edge[0] in choose_vertices and edge[1] in successors[edge[0]]:
            		out.write("%s %s %s 1\n" % (edge[0]-1, edge[1]-1, edge[2])) # 0-based
            	else:
                	out.write("%s %s %s 0\n" % (edge[0]-1, edge[1]-1, edge[2])) # 0-based

            out.write("\n%s\n\n" % (len(bags)+1)) # number of bags

            first_tree_edge = ()
            for j, bag in enumerate(bags):
                if target in bag:
                    out.write("1 1\n%s\n%s\n\n" % (target-1, edge_indices[target][target]))
                    first_tree_edge = (0, j+1)
                    break

            for bag in bags:
                out.write("%s %s\n" % (len(bag), len(bag)*len(bag)))
                out.write(' '.join(map(str, list(v-1 for v in bag))) + "\n")
                for u in bag:
                    for v in bag:
                        out.write("%s " % edge_indices[u][v])
                out.write("\n\n")

            tree = get_tree(lines[i + 9])
            out.write("%s %s\n" % first_tree_edge)
            for edge in tree:
                out.write("%s %s\n" % edge)
