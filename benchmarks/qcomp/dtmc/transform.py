import sys
import random

input_file = sys.argv[1]
output_file = 'inp.txt'
graph_file = 'graph.txt'

choose_vertices = set()
prob_vertices = set()
choose_edges = []
prob_edges = []

with open(input_file) as inp:
    with open(output_file, 'w') as out:
        states, actions = map(int, inp.readline().split(' '))
        out.write('p tw %s %s\n' % (states, actions))

        for i in range(actions):
            line = inp.readline().split(' ')

            prob = float(line[-1])
            line = line[:-1]

            from_, to = tuple(map(int, line))
            out.write('%s %s\n' % (from_+1, to+1))

            if from_ not in choose_vertices and from_ not in prob_vertices:
                if random.random() < 0.4:
                    prob_vertices.add(from_)
                else:
                    choose_vertices.add(from_)

            if from_ in choose_vertices:
                choose_edges.append((from_, to))
            else:
                prob_edges.append((from_, to, prob))

for state in range(states):
    if state not in choose_vertices and state not in prob_vertices:
        if random.random() < 0.4:
            prob_vertices.add(state)
        else:
            choose_vertices.add(state)

with open(graph_file, 'w') as out:
    out.write("Choose Vertices:\n")
    choose_vertices = [v+1 for v in choose_vertices]
    out.write("Number of Vertices: %s\n" % len(choose_vertices))
    out.write(' '.join(map(str, choose_vertices)) + "\n")

    out.write("Probability Vertices:\n")
    prob_vertices = [v+1 for v in prob_vertices]
    out.write("Number of Vertices: %s\n" % len(prob_vertices))
    out.write(' '.join(map(str, prob_vertices)) + "\n")

    out.write("Choose Edges:\n")
    out.write("Number of Edges: %s\n" % len(choose_edges))
    for (v, u) in choose_edges:
        out.write("%s %s\n" % (v+1, u+1))

    out.write("Probability Edges:\n")
    out.write("Number of Edges: %s\n" % len(prob_edges))
    for (v, u, prob) in prob_edges:
        out.write("%s %s %s\n" % (v+1, u+1, prob))
