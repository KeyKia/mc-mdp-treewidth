import sys 

input_file = sys.argv[1]
output_file = 'inp.txt'
graph_file = 'graph.txt'

choose_vertices = set()
prob_vertices = set()
choose_edges = []
prob_edges = []

with open(input_file) as inp:
    with open(output_file, 'w') as out:
        states, _, actions = map(int, inp.readline().split(' '))

        for i in range(actions):
            line = inp.readline().split(' ')[:-1]
            if len(line) == 5:
                line = line[:-1]
        
            prob = float(line[-1])
            line = line[:-1] 

            from_, action, to = tuple(map(int, line))
            action_state = states + action * states + from_

            out.write('%s %s\n' % (from_, action_state))
            out.write('%s %s\n' % (action_state, to))

            choose_vertices.add(from_)
            prob_vertices.add(action_state)

            choose_edges.append((from_, action_state))
            prob_edges.append((action_state, to, prob))


with open(graph_file, 'w') as out:
    out.write("Choose Vertices:\n")
    out.write("Number of Vertices: %s\n" % len(choose_vertices))
    out.write(' '.join(map(str, choose_vertices)) + "\n")

    out.write("Probability Vertices:\n")
    out.write("Number of Vertices: %s\n" % len(prob_vertices))
    out.write(' '.join(map(str, prob_vertices)) + "\n")

    out.write("Choose Edges:\n")
    out.write("Number of Edges: %s\n" % len(choose_edges))
    for edge in choose_edges:
        out.write("%s %s\n" % edge)

    out.write("Probability Edges:\n")
    out.write("Number of Edges: %s\n" % len(prob_edges))
    for edge in prob_edges:
        out.write("%s %s %s\n" % edge)

