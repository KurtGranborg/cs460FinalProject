fileName = H4-1-10

P3.out : Project3.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o CodeGenerator.o
	g++ -g -o P3.out Project3.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o CodeGenerator.o

Project3.o : Project3.cpp SetLimits.h SyntacticalAnalyzer.h
	g++ -g -c Project3.cpp

SetLimits.o : SetLimits.c SetLimits.h
	g++ -g -c SetLimits.c

LexicalAnalyzer.o : LexicalAnalyzer.cpp LexicalAnalyzer.h
	g++ -g -c LexicalAnalyzer.cpp

SyntacticalAnalyzer.o : SyntacticalAnalyzer.cpp SyntacticalAnalyzer.h LexicalAnalyzer.h CodeGenerator.h
	g++ -g -c -std=c++11 SyntacticalAnalyzer.cpp

CodeGenerator.o : CodeGenerator.h CodeGenerator.cpp LexicalAnalyzer.h
	g++ -g -c CodeGenerator.cpp

clean :
	rm *.o P3.out *.gch

run :
	make; ./P3.out testCases/$(fileName).ss; more testCases/$(fileName).cpp

submit : Project3.cpp LexicalAnalyzer.h LexicalAnalyzer.cpp SyntacticalAnalyzer.h SyntacticalAnalyzer.cpp makefile README.txt
	rm -rf TeamJeckP3
	mkdir TeamJeckP3
	cp Project3.cpp TeamJeckP3
	cp LexicalAnalyzer.h TeamJeckP3
	cp LexicalAnalyzer.cpp TeamJeckP3
	cp SyntacticalAnalyzer.h TeamJeckP3
	cp SyntacticalAnalyzer.cpp TeamJeckP3
	cp makefile TeamJeckP3
	cp README.txt TeamJeckP3
	tar cfvz TeamVP2.tgz TeamJeckP3
	cp TeamVP2.tgz ~tiawatts/cs460drop
