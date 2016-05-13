//#define DMU_COUNT 12 //No. of hyperplanes
#define INPUTS 2
#define OUTPUTS 2
#define MAXCOLS 10000 //MAX cols in matrix (check if necessary)
#define n_con_lp2 INPUTS+OUTPUTS //No. of variables in Phase II optimization. used to find the values

char ip_dir[30];//="../input";
char op_dir[30];//="../output/";

int DMU_COUNT;
int curDMU;
