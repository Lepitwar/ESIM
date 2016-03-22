esim: esim.o 
	gcc -Wall -std=c99 esim.o  -o esim -lm 

esim.o: esim.c 
	gcc -Wall -std=c99 -c esim.c  -lm 

clean:
	-rm -f *.o esim core
