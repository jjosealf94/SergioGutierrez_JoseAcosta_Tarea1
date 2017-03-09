import sys
import os
import matplotlib.pyplot as plt
import numpy as np

x = np.loadtxt("data.txt")
y = range(0,len(x))


fig= plt.figure()
plt.plot(y,x[:,0]/sum(x[:,0]),label="k=1")
plt.plot(y,x[:,1]/sum(x[:,1]),label="k=2")
plt.plot(y,x[:,2]/sum(x[:,2]),label="k=3")
plt.plot(0,0.02)
plt.legend()
plt.title("Energias en los difentes tiempo ")
plt.ylabel("Energia")
plt.xlabel("Paso temporal en el que fue impreso")
plt.savefig("Energias.png",format='png');


fig=plt.figure()
t1=np.loadtxt("tiempo1.dat")
t2=np.loadtxt("tiempo2.dat")
t3=np.loadtxt("tiempo4.dat")

tiempo=[t1,t2,t3]
num=[1,2,4]
plt.scatter(num,tiempo)
plt.title("Tiempos de ejecucion por numero de procesadores ")
plt.grid()
plt.ylabel("Tiempo(s)")
plt.xlabel("Numero de procesadores")
plt.savefig("Tiempo.png",format='png');

plt.show()


