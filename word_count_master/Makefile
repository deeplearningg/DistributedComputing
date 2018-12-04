run:mpi.cpp dagnode.o sort.o
	mpicxx mpi.cpp dagnode.o sort.o -o run
dagnode.o:dagnode.cpp dagnode.h sort_word.h
	g++ -g -c dagnode.cpp -o dagnode.o
sort.o:sort_word.cpp sort_word.h
	g++ -g -c sort_word.cpp -o sort.o
