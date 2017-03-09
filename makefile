fout.png : data.txt plot.py
	python plot.py

data.txt : a.out
	./a.out 1  > data.txt
	./a.out 2  > data.txt
	./a.out 4  > data.txt
a.out : FPUTsol.c
	gcc FPUTsol.c -lm -fopenmp

