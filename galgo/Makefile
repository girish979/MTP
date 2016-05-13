# Generated automatically from Makefile.in by configure.
CC          = cc
PRECFLAGS   = -O 
CPPFLAGS    = -I/home/jin/Desktop/impl_lib/pgapack/include -Dlinux -DWL=32 -DFORTRANUNDERSCORE -DOPTIMIZE -DFAKE_MPI 
FC          = f77
RM          = /bin/rm -f
LDFLAGS     = -s  -L/home/jin/Desktop/impl_lib/pgapack/lib/linux  -lpgaO  -lm
SHELL       = /bin/sh

#    "$@" expands to the target; "$?" expands to the dependency list
CFLAGS      = -o $@ $? $(PRECFLAGS)

LINK.c      = @echo "  Compiling $@" ; $(CC) -c $(CPPFLAGS) $(LDFLAGS)

default:
	@make ga
ga:	ga.c
	gcc -c ga.c -I/home/jin/Desktop/impl_lib/pgapack/include -Dlinux -DWL=32 -DFORTRANUNDERSCORE -DOPTIMIZE -DFAKE_MPI -s  -L/home/jin/Desktop/impl_lib/pgapack/lib/linux  -lpgaO  -lm	

#ga: ga.c
#	$(LINK.c)

#clean: 
#	@$(RM) driver  \
#	       udtstr *.o
