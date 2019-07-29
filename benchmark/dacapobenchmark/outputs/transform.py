import sys

input_file = sys.argv[1]
output_dir = sys.argv[2]
with open(input_file) as f:
    lines = ''.join(f.readlines()).split('\n')
    for i in range(0, len(lines), 15):
        method_name = lines[i+2].split(':')[1]
        output_file = output_dir + '/' + method_name + '.txt'
        with open(output_file, 'w') as out:
            for j in range(15):
                out.write(lines[i+j] + '\n')
