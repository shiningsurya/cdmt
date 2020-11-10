
CUDA=/usr/local/cuda-10.1/targets/x86_64-linux

CXX=g++ -std=c++11
NXX=$(CUDA)/../../bin/nvcc -x cu -dc -g
XL=$(CUDA)/../../bin/nvcc  -g

#NXX_CFLAGS=-Xcompiler g++ 

PSRDADA_CFLAGS=-I$(FP)/include
PSRDADA_LFLAGS=-L$(FP)/lib
PSRDADA_LD=-lpsrdada

CUDA_CFLAGS=-I$(CUDA)/include -I$(CUDA)/../../samples/common/inc
CUDA_LFLAGS=-L$(CUDA)/lib
CUDA_LD=-lcuda -lcudart -lcufft


%.o : %.cpp
	$(NXX) -c $^ -o $@ $(CUDA_CFLAGS) $(PSRDADA_CFLAGS)

t1 : test.o DADAFileSource.o FilterbankSink.o Header.o
	$(CXX) $^ -o $@ -lpsrdada

tt : test1.cpp
	$(NXX) $^ $(CUDA_CFLAGS) -o $@

cfb: DADAFileSource.o FilterbankSink.o Header.o cfb.o FrequencyGeometry.o
	$(XL) $^  $(PSRDADA_LFLAGS) -o $@ $(CUDA_LD) $(PSRDADA_LD)

clean : 
	rm -f *.o

.PHONY:
	clean
