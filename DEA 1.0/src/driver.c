#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>
#include <string.h>


#include "../include/header.h"
#include "../src/solver.c"


      

int main(int argc, char *argv[])
{     

        curDMU = 1;
        DMU_COUNT = 12;
        strcpy(ip_dir,argv[1]);
        dir_read(ip_dir);
    
        return 0;
}
