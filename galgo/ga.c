/*
 *  This is a test program for PGAPack.  The objective is to maximize the
 *  number of 1-bits in a chromosome.
 */

#include <pgapack.h>

unsigned int cnt;

double evaluate(double ,double, int);

double grayfunc(PGAContext *, int, int);

/*******************************************************************
*                   user main program                              *
*******************************************************************/
int main( int argc, char **argv ) {

	cnt=0;
    PGAContext *ctx;
    
    ctx = PGACreate(&argc, argv, PGA_DATATYPE_BINARY, 38, PGA_MAXIMIZE);
	PGASetPopSize (ctx,20); //default 100
	PGASetNumReplaceValue(ctx,5); //default 10%
    //PGASetRandomSeed(ctx, 1);
    PGASetNoDuplicatesFlag(ctx,PGA_TRUE);//No duplicates in populations


    PGASetCrossoverProb(ctx,0.6);//2pt cross over rate

    PGASetMaxGAIterValue(ctx, 200);//20*No.of classes
//    PGARandumUniform(ctx

/*	int stringlen,i;
	
	stringlen = PGAGetStringLength(ctx);

	int *low = (int *)malloc(sizeof(int)*stringlen);
	int *high = (int *)malloc(sizeof(int)*stringlen);
	for(i=0;i<stringlen;i++) {
		low [i] = 0;
		high [i] = i;

//		printf()
	}
	PGASetIntegerInitRange(ctx, low, high);
*/


	PGAEncodeRealAsBinary(ctx,0,PGA_OLDPOP,20,37,0.000001,1.0,.0000305);
	PGAEncodeRealAsBinary(ctx,1,PGA_OLDPOP,20,37,0.000001,1.0,.0001222);
	PGAEncodeRealAsBinary(ctx,2,PGA_OLDPOP,20,37,0.000001,1.0,.00048);


    
    PGASetUp(ctx);
    PGARun(ctx, grayfunc);


    int best = PGAGetBestIndex(ctx,PGA_OLDPOP);
	
    
	double x1,x2;
	x1 = PGAGetRealFromBinary(ctx,best,PGA_OLDPOP,0,19,1.0,1000.0);
	x2 = PGAGetRealFromBinary(ctx,best,PGA_OLDPOP,20,37,0.000001,1.0);
    
    printf("\nVals:%f\t%f",x1,x2);

    PGADestroy(ctx);
    return(0);
}


/*******************************************************************
*               user defined evaluation function                   *
*   ctx - contex variable                                          *
*   p   - chromosome index in population                           *
*   pop - which population to refer to                             *
*******************************************************************/
double grayfunc(PGAContext *ctx, int p, int pop) 
{
	double x1,x2;
	x1 = PGAGetRealFromBinary(ctx,p,pop,0,19,1.0,1000.0);
	x2 = PGAGetRealFromBinary(ctx,p,pop,20,37,0.000001,1.0);

	return evaluate(x1,x2,cnt++);
}

