all:
	gcc -c ga.c -I/home/jin/Desktop/impl_lib/pgapack/include -Dlinux -DWL=32 -DFORTRANUNDERSCORE -DOPTIMIZE -DFAKE_MPI -s  -L/home/jin/Desktop/impl_lib/pgapack/lib/linux  -lpgaO  -lm	
	gcc -c solver.c -o solver.o
	gcc script.c -o script
	gcc  solver.o ga.o -I/home/jin/Desktop/impl_lib/pgapack/include -Dlinux -DWL=32 -DFORTRANUNDERSCORE -DOPTIMIZE -DFAKE_MPI -s  -L/home/jin/Desktop/impl_lib/pgapack/lib/linux  -lpgaO  -lm
