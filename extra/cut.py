import sys
import numpy as np

fi = open(sys.argv[1])

datas = {}

for lin in fi:
  pname = lin.split('/')[0]
  instn = '/'.join(lin.split(' ')[0].split('/')[1:])
  if pname not in datas:
    datas[pname] = {}
  valu = float(lin.split(' ')[-1])
  datas[pname][instn] = valu

for key in datas:
  mini = np.min(datas[key].values())
  maxi = np.max(datas[key].values())
  mean = np.mean(datas[key].values())
  idxs = list(datas[key].keys())
  maxia = np.argmax([datas[key][x] for x in idxs])
  maaa = idxs[maxia]
  print("%20s : %15.6f %15.6f %15.6f (%s)"%(key,mini,mean,maxi,maaa))

