'''
Scrivere un programma python o uno script bash che faccia l'elenco dei pathname in un sottoalbero
che sono link fisici fra loro:
e.g.
searchln mydir
mydir/a mydir/d/b mydir/qq
mydir/uno mydir/due
significa che i path mydir/a mydir/d/b mydir/qq fanno riferimento allo stesso file, cosi' come mydir/uno
mydir/due indicano uno stesso file (diverso dal precedente).
Nel sottoalbero possono esserci molteplici altri file, se hanno un solo nome non vengono indicati
nell'output
'''

import os, sys

data = {}

for dirpath, _, filenames in os.walk(sys.argv[1]):
    for filename in filenames:
        path = os.path.join(dirpath, filename)
        ino = os.lstat(path).st_ino
        if ino in data:
            data[ino].append(path)
        else:
            data[ino] = [path]

for value in data.values():
    if len(value) > 1:
        print(*value)