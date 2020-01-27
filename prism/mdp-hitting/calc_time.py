with open('ans.txt') as f:
    lines = ''.join(f.readlines()).split('\n')
    lines = [line for line in lines if line.startswith('Time')]
    res = 0.
    for line in lines:
        res += float(line.split(' ')[-2])
    print(res)
