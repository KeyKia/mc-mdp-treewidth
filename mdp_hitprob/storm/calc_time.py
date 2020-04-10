with open('ans.txt') as f:
    lines = ''.join(f.readlines()).split('\n')
    lines = [line for line in lines if line.startswith('Time for')]
    res = 0.
    for line in lines:
        if line.find('input parsing')!=-1:
            continue
        res += float(line.split(' ')[-1][:-2])
    print(res)
