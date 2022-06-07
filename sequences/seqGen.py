import random

max_sequence_length = 300
filename = 0

for n in range(20, max_sequence_length + 21, 20):

    file = f"./exaus/dna{filename}.seq"
    f = open(file, 'w')
    seq=[str(n)+'\n',
        str(50)+'\n',
        ''.join(random.choices(['A','T','C','G','-'],k=n))+'\n',
        ''.join(random.choices(['A','T','C','G','-'],k=50))]
    f.writelines(seq)
    f.close()
    filename += 1
