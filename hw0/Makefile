EXECUTABLE=hw0_out

all: production

# tested locally
hw0: hw_zero.cc
	g++ -g *.cc -I. -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf -Wall -o $(EXECUTABLE)

# should work on clic machines
production:
	g++ -O2 *.cc -I. -I/usr/include/OpenEXR -lIlmImf -lImath -lHalf -Wall -o $(EXECUTABLE)

clean:
	rm $(EXECUTABLE)
	rm -rf *.dSYM
