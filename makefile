a: main.o
	gcc main.o -o a

main.o: main.cpp
	gcc -c main.cpp

clean:
	rm main *.o