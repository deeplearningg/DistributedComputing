run:mpi.cpp dagnode.o nn.o
	mpicxx mpi.cpp dagnode.o nn.o -o run -I /usr/local/include/mpich -std=c++11
dagnode.o:dagnode.cpp dagnode.h nn.h
	g++ -g -c dagnode.cpp -o dagnode.o -std=c++11
nn.o:nn.cpp nn.h
	g++ -g -c nn.cpp -o nn.o -std=c++11
