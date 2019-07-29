from collections import defaultdict

input_file = 'out.txt'

with open(input_file) as inp:
    edges = ''.join(inp.readlines()).split('\n')[:-1]
    graph_dict = defaultdict(set)

    for edge in edges:
        v, u = edge.split(' ')
        graph_dict[v].add(u)
        graph_dict[u].add(v)

    for key in graph_dict.keys():
        graph_dict[key] = list(graph_dict[key])

    print(len(graph_dict.keys()))
    graph = Graph(graph_dict)
    print graph.treewidth(k=20)

# run script with sagemath software
