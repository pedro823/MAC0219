.PHONY: clean

test: io.o test.o min.o
	nvcc $^ -o $@

clean:
	rm *.o *~ 

%.o: %.cpp
	g++ $^ -c

%.o: %.cu
	nvcc $^ -c
