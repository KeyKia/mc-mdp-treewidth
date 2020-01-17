import sys

input_file = sys.argv[1]
output_file = 'tree.txt'

with open(input_file) as inp:
    with open(output_file, 'w') as out:
        lines = ''.join(inp.readlines()).split('\n')[:-1]
        lines = [line for line in lines if not line.startswith('c')]

        _, _, number_of_bags, treewidth, number_of_vertices = lines[0].split(' ')
        lines = lines[1:]

        bags = []
        edges = []
        for line in lines:
            if line.startswith('b'):
                bags.append(line)
            else:
                edges.append(line)

        out.write("Vertices:\n")
        out.write("Number of Vertices: %s\n" % number_of_bags)
        for vertex in bags:
            vertex = ' '.join(vertex.split(' ')[2:])
            out.write("%s\n" % vertex)

        out.write("Edges:\n")
        for edge in edges:
            out.write('%s\n' % edge)
