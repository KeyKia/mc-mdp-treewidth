from collections import defaultdict

input_file = 'inp.txt'
output_file = 'tree.txt'

with open(input_file) as inp:
    with open(output_file, 'w') as out:
        edges = ''.join(inp.readlines()).split('\n')[:-1]
        graph_dict = defaultdict(set)

        for edge in edges:
            v, u = edge.split(' ')
            graph_dict[v].add(u)
            graph_dict[u].add(v)

        for key in graph_dict.keys():
            graph_dict[key] = list(graph_dict[key])

        graph = Graph(graph_dict)
        x = graph.treewidth(k=20, certificate=True)

        vertices = list(x.vertices())
        out.write("Vertices:\n")
        out.write("Number of Vertices: %s\n" % len(vertices))
        for i, vertex in enumerate(vertices):
            out.write("%s\n" % ' '.join(vertex))

        out.write("Edges:\n")
        for fr, to, _ in x.edges():
            fr_idx = vertices.index(fr)
            to_idx = vertices.index(to)

            out.write("%s %s\n" % (fr_idx, to_idx))
        

# run script with sagemath software
