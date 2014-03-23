OBJS = main.o DecTree.o DataSet.o DataSetView.o DepAttrInfo.o AttrInfo.o Thread.o
FLAGS = -std=c++11

all: main

main: $(OBJS)
	g++ $(OBJS) $(FLAGS) -o dectree
	
main.o: main.cpp
	g++ $(FLAGS) -c main.cpp 

DecTree.o: DecTree.h DecTree.cpp
	g++ $(FLAGS) -c DecTree.cpp

DataSet.o: DataSet.h DataSet.cpp
	g++ $(FLAGS) -c DataSet.cpp

DataSetView.o: DataSetView.h DataSetView.cpp
	g++ $(FLAGS) -c DataSetView.cpp

DepAttrInfo.o: DepAttrInfo.h DepAttrInfo.cpp
	g++ $(FLAGS) -c DepAttrInfo.cpp

AttrInfo.o: AttrInfo.h AttrInfo.cpp
	g++ $(FLAGS) -c AttrInfo.cpp

Thread.o: Thread.h Thread.cpp
	g++ $(FLAGS) -c Thread.cpp

clean:
	rm -f *.o dectree

test: all
	./main