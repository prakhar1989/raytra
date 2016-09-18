EXECUTABLE=hw0_out

all: production

# tested locally
hw0: hw_zero.cpp
	g++ -g *.cpp -I. -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf -Wall -o $(EXECUTABLE)

# should work on clic machines
production:
	g++ -O2 *.cpp -I. -I/usr/include/OpenEXR -lIlmImf -lImath -lHalf -Wall -o $(EXECUTABLE)

clean:
	rm $(EXECUTABLE)
	rm -rf *.dSYM
