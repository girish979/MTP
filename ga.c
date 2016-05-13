#include <pgapack.h>
#include <math.h>

/*
#define CLASSES 3

#define CVRate 0.6 //Crossover rate
#define TSRate 0.75// Tournament selection rate
#define T_POPULATION 5*CLASSES  // 5 * k ; k= No. of classes
#define ITERATIONS 50*CLASSES //50 * k;
#define STOP2 CLASSES //Maximum number of repetitions of the best individual = k
//#define ELITISM_RATE 1/POPULATION

//#define REPLACE 1

*/

#define INPUTS 2
#define OUTPUTS 2
unsigned int cnt;

unsigned int pitr,citr;


double evaluate(double ,double, int,int, char []);
double eval_fitness(PGAContext *, int, int);//external
double eval_fitness2(PGAContext *, int, int);//external
void DEA900(int,int,int);//external
void do_900again(PGAContext *ctx);

double gen_eval(PGAContext *, int, int);
double gen_eval2(PGAContext *, int, int);

void grayfunc(PGAContext *, int, int,int);

/*******************************************************************
*                   user main program                              *
*******************************************************************/
int hyperplanes;
int POPULATION;
int CLASSES;
char ds_name[50];//dataset name
int main( int argc, char *argv[] ) 
{
	// a.out [datasetname] [No. of classes]

	
	
	if(argc<3)
	{
		printf("Usage:\ta.out [dataset name] [#classes in dataset]\n");
		exit(1);
	}

	CLASSES = atoi(argv[2]);
	strcpy(ds_name,argv[1]); 
	ds_name[strlen(ds_name)]='\0';
	hyperplanes = (CLASSES * (CLASSES-1)) /2;

	//printf("%s\t%d",ds_name,CLASSES);
	//exit(1);


	float CVRate = 0.6;//Crossover rate
	float TSRate = 0.75;// Tournament selection rate
	int T_POPULATION = 5*CLASSES; // 5 * k ; k= No. of classes
	int ITERATIONS = 50*CLASSES; //50 * k;
	int STOP2 = CLASSES; //Maximum number of repetitions of the best individual = k

	if(T_POPULATION%2) 
		POPULATION = T_POPULATION+1;
	else
		POPULATION = T_POPULATION;

	cnt=0;
    PGAContext *ctx;
    float f;

	int i;

	double Lower[2]      = { 1.0,0.0 };// min C, sigma
	double Upper[2]      = { 1000.0,1.0 };//max C,sigma
	double l[2], u[2]; /* for initializing lu ranges */

	for (i=0; i<2; i++) //2 No. of genes in string, C, sigma
	{
		l[i] = Lower[i];
		u[i] = Upper[i];
    }

    
    
    ctx = PGACreate(&argc, argv, PGA_DATATYPE_REAL, 2, PGA_MAXIMIZE);
	PGASetPopSize (ctx,POPULATION); 

	float elitism_rate = (100.0/POPULATION);
	int REPLACE = ( (elitism_rate - floor(elitism_rate)) >= 0.5 ) ? POPULATION-ceil(elitism_rate) : POPULATION-floor(elitism_rate);
	PGASetNumReplaceValue(ctx,REPLACE); // 1/pop are preserved
	//PGASetPopReplacementType(ctx,PGA_POPREPL_BEST); // Default: Best individuals are preserved

    PGASetRandomSeed(ctx, 1);
    PGASetRealInitRange(ctx, l, u);
    PGASetNoDuplicatesFlag(ctx,PGA_TRUE);//No duplicates in populations


	PGASetCrossoverType(ctx,PGA_CROSSOVER_UNIFORM);
    PGASetCrossoverProb(ctx,CVRate);//1pt cross over rate  march

    PGASetPTournamentProb(ctx, TSRate);//Tournament selection rate

	PGASetMutationProb(ctx,.001);

	/// Stopping criteria
    PGASetMaxGAIterValue(ctx, ITERATIONS);//No. of iterations 20*No.of classes
	PGASetMaxNoChangeValue(ctx, CLASSES);// Maximum number of repetitions of the best individual

	PGASetPrintFrequencyValue(ctx,1);
	PGASetFitnessType (ctx, PGA_FITNESS_RANKING );
    PGASetUp(ctx);



	pitr=-1;
	citr=-1;


	for(i=0;i<POPULATION;i++)
	{
		grayfunc(ctx,i,PGA_OLDPOP,0);
		//evaluate all efficiencies of populations
	}
	DEA900(POPULATION,0,hyperplanes);//Solve DEA with All populations, all hyperplanes
	
	PGAEvaluate(ctx, PGA_OLDPOP, eval_fitness, NULL);
	PGAFitness (ctx, PGA_OLDPOP);

	while(!PGADone(ctx, NULL)) 
	{
		//Call DEA Here
		int itr = PGAGetGAIterValue(ctx);
		citr = itr;
		printf("Iteration No:%d\n",itr);
		PGASelect                 (ctx, PGA_OLDPOP);
		PGARunMutationAndCrossover(ctx,PGA_OLDPOP,PGA_NEWPOP);

		//gen_eval2(PGAContext *ctx, int p, int pop);
		do_900again(ctx);

		
		PGAEvaluate               (ctx,PGA_NEWPOP,gen_eval2,NULL);
		PGAFitness                (ctx,PGA_NEWPOP);
		PGAUpdateGeneration       (ctx,NULL);
		//Check if they are updated or NOT
		// IF Update SVMOP is the updated population
		// Restore OLD Pop 

		//here
		
		//Print All
		int i=0;
		for(i=0;i<POPULATION;i++)
		{
			//printf("----------------------called %i time------------\n",i);
			f=eval_fitness(ctx, i, PGA_OLDPOP);
		}
		
		PGAPrintReport            (ctx,stdout,PGA_OLDPOP);
		//pitr=itr;
	}


   // PGARun(ctx, grayfunc);

/*
    int best = PGAGetBestIndex(ctx,PGA_OLDPOP);
	
    
	double x1,x2;
	x1 = PGAGetRealFromBinary(ctx,best,PGA_OLDPOP,0,19,1.0,1000.0);
	x2 = PGAGetRealFromBinary(ctx,best,PGA_OLDPOP,20,37,0.000001,1.0);
    
    printf("\nVals:%f\t%f",x1,x2);
    */

    PGADestroy(ctx);
    return(0);
}


/*******************************************************************
*               user defined evaluation function                   *
*   ctx - contex variable                                          *
*   p   - chromosome index in population                           *
*   pop - which population to refer to                             *
*******************************************************************/
void grayfunc(PGAContext *ctx, int p, int pop,int iter) 
{
	double x1,x2;
	//x1 = PGAGetRealFromBinary(ctx,p,pop,0,19,1.0,1000.0);
	//x2 = PGAGetRealFromBinary(ctx,p,pop,20,37,0.000001,1.0);

	x1 = PGAGetRealAllele (ctx, p, pop, 0);//0the gene value
	x2 = PGAGetRealAllele (ctx, p, pop, 1);//1st gene value

	evaluate(x1,x2,p,iter,ds_name);//cnt++);
}

double eval_fitness(PGAContext *ctx, int p, int pop)
{
	float f,f_tmp;
	int i_tmp,itr = PGAGetGAIterValue(ctx);

	char str[50];

	FILE *fp,*res;
	fp=fopen("./SVMOP2/Eff","r");
	res = fopen("./RESULTS","a+");

	double x1,x2;
	//x1 = PGAGetRealFromBinary(ctx,p,pop,0,19,1.0,1000.0);
	//x2 = PGAGetRealFromBinary(ctx,p,pop,20,37,0.000001,1.0);

	x1 = PGAGetRealAllele (ctx, p, pop, 0);//0the gene value
	x2 = PGAGetRealAllele (ctx, p, pop, 1);//1st gene value


	if(pitr!=citr)
	{
		//fprintf(res,"\n\n\n");
		pitr = itr;
	}

	fprintf(res,"%4f %f ",x1,x2);//Print C, gamma


	FILE *io;
	char pIdx[5];
	sprintf(pIdx,"%d",p);

	strcpy(str,"./SVMOP/");strcat(str,pIdx);  strcat(str,"_acc_op1");
	io = fopen(str,"r");
	while(fscanf(io,"%f",&f_tmp)!=EOF)
	{
		fprintf(res,"%f ",f_tmp);
	}
	fclose(io);

	strcpy(str,"./SVMOP/");strcat(str,pIdx);  strcat(str,"_margin_op2");
	io = fopen(str,"r");
	while(fscanf(io,"%f",&f_tmp)!=EOF)
	{
		fprintf(res,"%f ",f_tmp);
	}
	fclose(io);

	strcpy(str,"./SVMOP/");strcat(str,pIdx);  strcat(str,"_sv_ip1");
	io = fopen(str,"r");
	while(fscanf(io,"%d",&i_tmp)!=EOF)
	{
		fprintf(res,"%d ",i_tmp);
	}
	fclose(io);

	strcpy(str,"./SVMOP/");strcat(str,pIdx);  strcat(str,"_spans_ip2");
	io = fopen(str,"r");
	while(fscanf(io,"%f",&f_tmp)!=EOF)
	{
		fprintf(res,"%f ",f_tmp);
	}
	fclose(io);


	
	int i,hyp;
	hyp = (CLASSES * (CLASSES-1)) /2;
	f=0.0;

	printf("file:%s\tIteration:%d\tChromosome No. %d\n",str,itr,p);

	for(i=1;i<=hyp*p;i++)//Skip previous
	{
		fscanf(fp,"%f",&f_tmp);
	}
	f_tmp=0.0;
	for(i=1;i<=hyp;i++)//Skip previous
	{
		fscanf(fp,"%f",&f_tmp);
		//printf("----------%f------------%d\n",f_tmp,hyp);
		fprintf(res,"%f%c",f_tmp,(i==hyp)?'\n':' ');
		f+=f_tmp;
	}	

	
	fclose(fp);




	fclose(res);
	//Open the file find sum of efficiencies of DEA900, return sum
	return f;
}

double eval_fitness2(PGAContext *ctx, int p, int pop)
{
	float f,f_tmp;
	int i_tmp,itr = PGAGetGAIterValue(ctx);

	char str[50];
	FILE *fp;
	fp=fopen("./SVMOP2/Eff","r");
	int i,hyp;
	hyp = CLASSES * (CLASSES-1) /2;
	f=0.0;

	printf("file:%s\tIteration:%d\tChromosome No. %d\n",str,itr,p);

	for(i=1;i<=hyp*p;i++)//Skip previous
	{
		fscanf(fp,"%f",&f_tmp);
	}
	f_tmp=0.0;
	for(i=1;i<=hyp;i++)//Skip previous
	{
		fscanf(fp,"%f",&f_tmp);
		//printf("----------%f------------%d\n",f_tmp,hyp);
		//fprintf(res,"%f%c",f_tmp,(i==hyp)?'\n':' ');
		f+=f_tmp;
	}	

	fclose(fp);

	//Open the file find sum of efficiencies of DEA900, return sum
	return f;
}






double gen_eval(PGAContext *ctx, int p, int pop)
{

	double x1,x2;
	//x1 = PGAGetRealFromBinary(ctx,p,pop,0,19,1.0,1000.0);
	//x2 = PGAGetRealFromBinary(ctx,p,pop,20,37,0.000001,1.0);

	x1 = PGAGetRealAllele (ctx, p, pop, 0);//0the gene value
	x2 = PGAGetRealAllele (ctx, p, pop, 1);//1st gene value


	int iter = PGAGetGAIterValue(ctx);


	//Create backup of SVM_OPs
	char str[50]="mkdir ./ActPop/";
	char t[20];
	sprintf(t,"%d",iter);
	strcat(str,t);
	system(str);

/*	strcpy(str,"mv ./SVMOP2/Eff ./ActPop/");
	strcat(str,t);
	system(str);	
	strcpy(str,"cp ./SVMOP/* ./ActPop/");
	strcat(str,t);
	system(str);	

	strcpy(str,"rm -f ./SVMOP/");	
	sprintf(t,"%d",p);
	strcat(str,t);
	strcat(str,"_*");
	system(str);
*/

	system("rm -f ./SVMOP2/*");
	system("rm -f ./SVMOP/*");
	
	
	evaluate(x1,x2,p,iter,ds_name);//All IP's , OP's are generated

	//Call DEA900(); //Solved
	DEA900(POPULATION,iter,hyperplanes);


	return eval_fitness(ctx,p,pop);
}

void do_900again(PGAContext *ctx)
{

	int iter = PGAGetGAIterValue(ctx);


/*	//Create backup of SVM_OPs
	char str[50]="mkdir ./ActPop/";
	char t[20];
	sprintf(t,"%d",iter);
	strcat(str,t);
	system(str);

	strcpy(str,"mv ./SVMOP2/Eff ./ActPop/");
	strcat(str,t);
	system(str);	
	strcpy(str,"mv ./SVMOP/* ./ActPop/");
	strcat(str,t);
	system(str);
*/

	system("rm -f ./SVMOP2/*");
	system("rm -f ./SVMOP/*");

		
	int i;
	for(i=0;i<POPULATION;i++)
	{
		grayfunc(ctx,i,PGA_NEWPOP,0);
		//evaluate all efficiencies of populations
	}

	
	//evaluate(x1,x2,p,iter);//All IP's , OP's are generated

	//Call DEA900(); //Solved
	DEA900(POPULATION,iter,hyperplanes);
}


double gen_eval2(PGAContext *ctx, int p, int pop)
{

	double x1,x2;
	//x1 = PGAGetRealFromBinary(ctx,p,pop,0,19,1.0,1000.0);
	//x2 = PGAGetRealFromBinary(ctx,p,pop,20,37,0.000001,1.0);


	x1 = PGAGetRealAllele (ctx, p, pop, 0);//0the gene value
	x2 = PGAGetRealAllele (ctx, p, pop, 1);//1st gene value


/*	int iter = PGAGetGAIterValue(ctx);


	//Create backup of SVM_OPs
	char str[50]="mkdir ./ActPop/";
	char t[20];
	sprintf(t,"%d",iter);
	strcat(str,t);
	system(str);

	strcpy(str,"mv ./SVMOP2/Eff ./ActPop/");
	strcat(str,t);
	system(str);	
	strcpy(str,"mv ./SVMOP/* ./ActPop/");
	strcat(str,t);
	system(str);	
	int i;
	for(i=0;i<POPULATION;i++)
	{
		grayfunc(ctx,i,PGA_OLDPOP,0);
		//evaluate all efficiencies of populations
	}

	
	//evaluate(x1,x2,p,iter);//All IP's , OP's are generated

	//Call DEA900(); //Solved
	DEA900(POPULATION,iter,hyperplanes);
*/

	//	return eval_fitness(ctx,p,pop);
	return eval_fitness2(ctx,p,pop);
}

