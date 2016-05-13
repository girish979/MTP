#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NO_OF_IP 2
#define NO_OF_OP 2
//#define DMUs 3

void evaluate(double x1,double x2,int p,int iter,char dsname[])
{
	FILE *fp;
	
	char s1[150];

	sprintf(s1,"%f %f %d %d %s",x1,x2,p,iter,dsname);

	
	char t_str[150]="./script ";// march char t_str[150]="./script.sh ";
	strcat(t_str,s1);

	system(t_str);


	float eff=0;
	//fp= fopen("./SVMOP/toteff","r");
	//fscanf(fp,"%f",&eff);

	//fclose(fp);

	//printf("Efficiency of iteration: %f\n\n",eff);

	//fp = fopen("./RESULTS","a+");
	//fprintf(fp,"%d. c = %f\tgamma = %f\t Efficiency = %f\n",iter,x1,x2,eff);
	//fclose(fp);

	//return (double)eff;
}

void generate_LPs(int pop_size, int iter,int DMUs)
{

	char phase[2];
	int ip = NO_OF_IP;
	int op = NO_OF_OP;
	long n = DMUs*pop_size;

	char path[4][50];
	char temp[50];

	float ip_val[ip][n];
	float op_val[op][n];
	float tmp_float;

	long i,k,l;
	FILE *f_tmp;
	for(i=0;i<pop_size;i++)//For all 2 inputs, 2 outputs
	{

		sprintf(temp,"%d",i);
		strcat(temp,"_sv_ip1");
		strcpy(path[0],"./SVMOP/");
		strcat(path[0],temp);
		strcpy(temp,"");		

		sprintf(temp,"%d",i);
		strcat(temp,"_spans_ip2");	
		strcpy(path[1],"./SVMOP/");
		strcat(path[1],temp);
		strcpy(temp,"");

	
		sprintf(temp,"%d",i);
		strcat(temp,"_acc_op1");
		strcpy(path[2],"./SVMOP/");
		strcat(path[2],temp);
		strcpy(temp,"");

		sprintf(temp,"%d",i);
		strcat(temp,"_margin_op2");
		strcpy(path[3],"./SVMOP/");
		strcat(path[3],temp);
		strcpy(temp,"");

		
		for(k=0;k<NO_OF_IP;k++)
		{
			f_tmp = fopen(path[k],"r");

			long j=0;
			while(fscanf(f_tmp,"%f",&tmp_float)!=EOF)
			{
				ip_val[k][(i*DMUs)+j++]= tmp_float;//j hsould depend on i
			}
			fclose(f_tmp);		
		
		}


		for(l=0,k=ip;k<ip+op;k++,l++)//+1 is executable file
		{
			//fip[j++] = fopen(argv[k],"r");
			f_tmp = fopen(path[k],"r");

			long j=0;
			while(fscanf(f_tmp,"%f",&tmp_float)!=EOF)
			{
				op_val[l][(i*DMUs)+j++]= tmp_float;//j 
			}
			fclose(f_tmp);		
		
		}	
			
					
	}

	FILE *fp;

	strcpy(phase,"a");
	i=0;
	char str[50];
	for(i=0;i<n;i++)
	{
		char op_path[100] = "./LPs2/f";
/*		if(i<=9)
		{
			str[0]='0';
			str[1]='0';
			sprintf(&str[2],"%d",i);
		}
		else if(i>=10&&i<=99)
		{
			str[0]='0';
			sprintf(&str[1],"%d",i);
		}
		else*/
			sprintf(str,"%09ld",i);	
		
		strcat(str,phase);strcat(str,".lp");
		strcat(op_path,str);
		str[0]=0;
		//printf("%s\n",op_path);
		
		fp = fopen(op_path,"w");		


		fprintf(fp,"Minimize\n");
		fprintf(fp,"   value: theta\n\n");
		fprintf(fp,"Subject To\n\n");

		//print input constraints

		long t_ip =0;
		while(t_ip<NO_OF_IP)//Print input constraints
		{
			fprintf(fp,"   c%d:    ",t_ip+1);

			fprintf(fp,"%f theta - ",ip_val[t_ip][i]);

			long t_cnt=0;
			while(t_cnt<n)
			{
				fprintf(fp,"%f lam%d - ",ip_val[t_ip][t_cnt],t_cnt+1);
				t_cnt++;
			}
			fprintf(fp,"s%dMinus = 0",t_ip+1);

			fprintf(fp,"\n\n");
			t_ip++;
		}

		long t_op=0;
		while(t_op<NO_OF_OP)//Print input constraints
		{
			fprintf(fp,"   c%d:    ",t_ip+1);


			long t_cnt=0;
			while(t_cnt<n)
			{
				fprintf(fp,"%f lam%d ",op_val[t_op][t_cnt],t_cnt+1);
				
				t_cnt++;
				if(t_cnt<n)
					fprintf(fp,"+ ");
			}
			fprintf(fp,"- s%dPlus = %f",t_op+1,op_val[t_op][i]);

			fprintf(fp,"\n\n");
			t_ip++;
			t_op++;
		}		


		//print end constraints

		
		fprintf(fp,"Bounds\n");
		//Input Bounds

		long q;
		for(q=0;q<n;q++)
		{
			fprintf(fp,"          lam%d >=  0\n",q+1);
		}
		
		

		//Output Bounds
		for(q=0;q<ip;q++)
		{
			fprintf(fp,"          s%dMinus >=  0\n",q+1);
		}

		for(q=0;q<op;q++)
		{
			fprintf(fp,"          s%dPlus >=  0\n",q+1);
		}		
		fprintf(fp,"End");

		
		fclose(fp);
	}

/*	strcpy(phase,"b");
	for(i=0;i<n;i++)
	{
		char path[100] = "./LPs2/f";

			sprintf(str,"%09ld",i);	
		
		strcat(str,phase);strcat(str,".lp");
		strcat(path,str);
		str[0]=0;
		//printf("%s\n",path);
		
		fp = fopen(path,"w");		


		fprintf(fp,"Minimize\n");
		//fprintf(fp,"   value: - s1Minus -  s2Minus - s1Plus - s2Plus\n\n");
		fprintf(fp,"   value:");
		long q;
		for(q=0;q<ip;q++)
		{
			fprintf(fp," - s%dMinus",q+1);
		}
		
		for(q=0;q<op;q++)
		{
			fprintf(fp," - s%dPlus",q+1);
		}		
		
		fprintf(fp,"\n\nSubject To\n\n");

		//print input constraints

		long t_ip =0;
		while(t_ip<NO_OF_IP)//Print input constraints
		{
			fprintf(fp,"   c%d:    ",t_ip+1);

			fprintf(fp,"%f theta - ",ip_val[t_ip][i]);

			long t_cnt=0;
			while(t_cnt<n)
			{
				fprintf(fp,"%f lam%d - ",ip_val[t_ip][t_cnt],t_cnt+1);
				t_cnt++;
			}
			fprintf(fp,"s%dMinus = 0",t_ip+1);

			fprintf(fp,"\n\n");
			t_ip++;
		}

		long t_op=0;
		while(t_op<NO_OF_OP)//Print input constraints
		{
			fprintf(fp,"   c%d:    ",t_ip+1);


			long t_cnt=0;
			while(t_cnt<n)
			{
				fprintf(fp,"%f lam%d ",op_val[t_op][t_cnt],t_cnt+1);
				
				t_cnt++;
				if(t_cnt<n)
					fprintf(fp,"+ ");
			}
			fprintf(fp,"- s%dPlus = %f",t_op+1,op_val[t_op][i]);

			fprintf(fp,"\n\n");
			t_ip++;
			t_op++;
		}		


		//print end constraints

		
		fprintf(fp,"Bounds\n");
		//Input Bounds

		
		for(q=0;q<n;q++)
		{
			fprintf(fp,"          lam%d >=  0\n",q+1);
		}
		
		

		//Output Bounds
		for(q=0;q<ip;q++)
		{
			fprintf(fp,"          s%dMinus >=  0\n",q+1);
		}
		fprintf(fp,"          theta = 1\n");
		for(q=0;q<op;q++)
		{
			fprintf(fp,"          s%dPlus >=  0\n",q+1);
		}		
		fprintf(fp,"End");

		
		fclose(fp);
	}
*/	

}

void DEA900(int pop_size,int iter,int dmus)
{
	system("rm -f ./LPs2/*");
	//system();



	generate_LPs(pop_size,iter,dmus);
	

	char p[100]="./dea900 ./LPs2 > ./SVMOP2/Eff";
	//char temp[5];
	//sprintf(temp,"%d",iter);
	//strcat(p,temp);


	system(p);//"./dea900 ./LPs2 > ./SVMOP2/0000");



	
}
