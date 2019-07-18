import sys 

input_file = sys.argv[1]
output_file = 'out.txt'

with open(input_file) as inp:
    with open(output_file, 'w') as out:
        states, _, actions = map(int, inp.readline().split(' '))

        for i in range(actions):
            line = inp.readline().split(' ')[:-1]
            if len(line) == 5:
                line = line[:-1]
            line = line[:-1]  # ignore probability 

            from_, action, to = tuple(map(int, line))
            action_state = states + action * states + from_

            out.write('%s %s\n' % (from_, action_state))
            out.write('%s %s\n' % (action_state, to))