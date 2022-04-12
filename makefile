# Programmers: Rogelio Schevenin, Sawyer Thompson 
# RedID: 824107681, 823687079

# CXX Make variable for compiler
CXX=g++ -g

# Make variable for compiler options
#	-std=c++11  C/C++ variant to use, e.g. C++ 2011
#	-g          include information for symbolic debugger e.g. gdb 
CXXFLAGS=-std=c++11 -g

rideshare : rideshare.o
	$(CXX) $(CXXFLAGS) -o rideshare $^
rideshare.o : rideshare.cpp rideshare.h

clean :
	rm -rf *.o