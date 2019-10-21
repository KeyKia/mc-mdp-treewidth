import sys
import json
import random

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
        method_name = lines[i+2].split(':')[1]
        output_file = output_dir + '/' + method_name + '.txt'
        with open(output_file, 'w') as out:
            n = int(lines[i].split(':')[1])

            successors = get_successors(lines[i+1].split(':')[1])
            vertices = [j+1 for j in range(n)]
            random.shuffle(vertices)
            choose_vertices, probability_vertices = vertices[:len(vertices) / 2], vertices[len(vertices) / 2:]

            out.write("Graph:\n")
            out.write("Choose Vertices:\n")
            out.write("Number of Vertices: %s\n" % len(choose_vertices))
            out.write(' '.join(map(str, choose_vertices)) + "\n")
            out.write("Probability Vertices:\n")
            out.write("Number of Vertices: %s\n" % len(probability_vertices))
            out.write(' '.join(map(str, probability_vertices)) + "\n")

            choose_edges = []
            probability_edges = []
            for v in vertices:
                if v in choose_vertices:
                    for u in successors[v]:
                        choose_edges.append((v, u))
                else:
                    probs = [random.random() for u in successors[v]]
                    norm = sum(probs)
                    for j, u in enumerate(successors[v]):
                        probability_edges.append((v, u, probs[j] / norm))

            out.write("Choose Edges:\n")
            out.write("Number of Edges: %s\n" % len(choose_edges))
            for edge in choose_edges:
                out.write("%s %s\n" % edge)

            out.write("Probability Edges:\n")
            out.write("Number of Edges: %s\n" % len(probability_edges))
            for edge in probability_edges:
                out.write("%s %s %s\n" % edge)
            out.write('\n')

            bags = get_bags(lines[i + 11])
            tree = get_tree(lines[i + 9])

            out.write("Tree:\n")
            out.write("Vertices:\n")
            out.write("Number of Vertices: %s\n" % len(bags))
            for bag in bags:
                out.write(' '.join(map(str, bag)) + "\n")
            out.write("Edges:\n")
            for edge in tree:
                out.write("%s %s\n" % edge)

