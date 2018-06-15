.PHONY: clean

test: io.o test.o min.o error_handler.o
	nvcc $^ -o $@

clean:
	rm *.o *~ 

%.o: %.cpp
	g++ $^ -c

%.o: %.cu
	nvcc $^ -c
