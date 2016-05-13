all	: 
	gcc -c ./readDir.c -o ../obj/readDir.o
	gcc -c ./driver.c -o ../obj/driver.o
	gcc -g -o ../bin/run ../obj/readDir.o ../obj/driver.o -lglpk -lm

run     :
	@../bin/run
clean   :       
	rm -rf ../bin/run ../obj/driver.o ../obj/readDir.o
