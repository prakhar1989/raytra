EXECUTABLE=hw0_out

all: hw0

hw0: hw_zero.cpp
	g++ -g *.cpp -I. -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf -Wall -o $(EXECUTABLE)

production:
	g++ -O2 *.cpp -I. -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf -Wall -o hw0

clean:
	rm $(EXECUTABLE)
	rm -rf *.dSYM
