import random
n = 10
m = 20
file = 'dna.seq'
f = open(file, 'w')
seq=[str(n)+'\n',
     str(m)+'\n',
     ''.join(random.choices(['A','T','C','G','-'],k=n))+'\n',
     ''.join(random.choices(['A','T','C','G','-'],k=m))]
f.writelines(seq)
f.close()
print(''.join(seq))

