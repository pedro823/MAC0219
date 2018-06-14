.PHONY: clean

test: io.o test.o
	g++ $^ -o $@


%.o: %.cpp
	g++ $^ -c

%.o: %.cu
	nvcc $^ -c
