##  At terminal: - use command "make" to compile --> clear screen --> execute app   ##
##				 - use command "make clean" to remove execute file  	     		##
##				 - use command "make debug" to compile and execute with GDB  		##

all:
	g++ -o IP_mng_app main.cpp
	clear
	./IP_mng_app

debug:
	g++ -o -g IP_mng_app main.cpp
	gdb ./IP_mng_app

clean:
	rm -rf IP_mng_app