graph_file = 'graph.txt'
tree_file = 'tree.txt'
output_file = 'out.txt'

with open(graph_file) as gg:
    with open(tree_file) as tt:
        with open(output_file, 'w') as out:
            out.write("Graph: \n")
            out.write(gg.read())
            out.write('\n')
            out.write("Tree: \n")
            out.write(tt.read())

