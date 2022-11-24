
all:	 pish

pish:	 pish.o Scanner.o Parse.o
	g++ -std=c++11 -o pish pish.o Scanner.o Parse.o

pish.o:	 pish.cpp hash.h Scanner.h Parse.h
	g++ -std=c++11 -c pish.cpp

Scanner.o:	 Scanner.cpp Scanner.h
	g++ -std=c++11 -c Scanner.cpp

Parse.o:	 Parse.cpp Parse.h
	g++ -std=c++11 -c Parse.cpp

clean:	
	rm -f core *.o hashdemo pish setdemo testParse testScanner


