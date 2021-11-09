a: main.o
	g++ main.o -o a

o: main.cpp
	g++ -c main.cpp

clean:
	rm main *.o