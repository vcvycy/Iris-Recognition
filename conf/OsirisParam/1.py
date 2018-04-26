import os
import math
f=open("points.txt")
pnts_cnt=int(f.readline())
print(pnts_cnt)
for i in range(0,pnts_cnt):
  pnt=f.readline().strip().split()
#print(pnt)
  x=int(pnt[0])
  y=int(pnt[1])
  print("%d %d" %(64-(64-x)*0.8,y) )
