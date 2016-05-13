void get_opName(char *a)//,char *b)
{
        char str1[100],str2[100];
        char temp[10];
        strcpy(str1,op_dir); strcpy(str2,op_dir);
        strcat(str1,"op\0");   strcat(str2,"op\0");

  /*      sprintf(temp,"%d",curDMU);
        strcat(str1,temp);   strcat(str2,temp);
        strcat(str1,"1\0");   strcat(str2,"2\0"); */       
        
        strcpy(a,str1);  //strcpy(b,str2);
        
       
}

double solve(char f1[])//,char f2[])
{

        int cols,rows;
        int i,j;
        double obval;
        double tmp_double;   
        int thetaCol;
        int isEfficient=0;    
        char t_chr[10];       

        int cInd[MAXCOLS]; double cVal[MAXCOLS];
        double slacks[n_con_lp2];
        int ref_set[DMU_COUNT];         //Reference Set
        double ref_set_val[DMU_COUNT];  //Values of reference set
        double temp_io[MAXCOLS];
        double giv_ip[INPUTS];          //given inputs
        double giv_op[OUTPUTS];         //given outputs
        double opt_ip[INPUTS];          //optimal inputs
        double opt_op[OUTPUTS];         //optimal outputs     

        char ofname1[100];              //stores output filename for Phase I
        char ofname2[100];


        get_opName(ofname1);//,ofname2);    //Generate filename for output file
        glp_term_out(GLP_OFF);          //Enable(GLP_ON) or Disable terminal output of glp function

        glp_prob *P1,*P2;
        P1 = glp_create_prob();         //Create New Problem object
        P2 = glp_create_prob();
        
        /////////////////////////////////////////////////////////////////////////
        /*PHASE I*/
        
        glp_read_lp(P1, NULL, f1);                      //Read Input from file

        /*************START of get corresponding Inputs & outputs*************/       
       // glp_get_mat_col(P1,curDMU+1,NULL,temp_io); // curDMU+1
     //   
      //  for(j=0,i=1;i<=INPUTS;i++,j++)                  //extract inputs
     //           giv_ip[j] = -1*temp_io[i];           
    //    for(j=0,i=1+INPUTS;i<=INPUTS+OUTPUTS;i++,j++)   //Extract outputs
    //            giv_op[j]=temp_io[i]; 
        /*************END of get corresponding Inputs & outputs**************/ 
        
        glp_adv_basis(P1, 0);           // constructs an advanced initial LP basis 
        glp_simplex(P1, NULL);          //driver to the LP solver based on the simplex method
       // glp_print_sol(P1, ofname1);
        //glp_write_sol(P1, "../output/op1.txt");
        
        ///////////////////////////////////////////////////////////////////////// End of PHASE I

        obval = glp_get_obj_val(P1);// Get obj value of PHASE I



        /////////////////////////////////////////////////////////////////////////
/*        ///PHASE II//
        
        glp_read_lp(P2, NULL, f2);              //Read Input from file

        ///////////////////////START of multiply by theta//////////////
        cols = glp_get_num_cols(P2); 
        for(i=1;i<=cols;i++)                    //Find out the column No. of theta
        {
                if( !strcmp(glp_get_col_name(P2,i),"theta") )
                        break;
        }
        thetaCol=i;
       
        glp_get_mat_col(P2,thetaCol,cInd,cVal); //Extract column theta
        rows = glp_get_num_rows(P2);
        for(i=1;i<=INPUTS;i++)          //Multiply with optimal solution of PHASE I
        {
               // if(cVal[i])
                        cVal[i] *= obval;  
                        cInd[i]=i;
               // else break; 
        }       
        
        glp_set_mat_col(P2,thetaCol,INPUTS,cInd,cVal);          //INPUTS are multipled by theta inserted back into matrix
        ////////////////////END of multiply by theta////////////////////////////
        
        glp_adv_basis(P2, 0);           // constructs an advanced initial LP basis 
        glp_simplex(P2, NULL);          //driver to the LP solver based on the simplex method
        //glp_print_sol(P2, ofname2);
        //glp_write_sol(P2, "../output/op2.txt");

        /////////////////////////////////////////////////////////////////////////End of PHASE II



        //Get all slack variables
        for(i=1;i<=n_con_lp2;i++)
        {
                 //slacks[i-1] = glp_get_col_prim(P2,i); 
                 sprintf(t_chr,"%.6f ",glp_get_col_prim(P2,i));      // Taking only 6 values after decimal of slacks
                 slacks[i-1] = atof(t_chr);
        }

        //Check for Efficiency
        if(obval==1.0)
        {
                for(i=0;i<n_con_lp2;i++)
                        if(!slacks[i])
                                continue;
                        else break;
                if(i>=n_con_lp2)
                        isEfficient=1;
        }
/*
        if(isEfficient)
        {
          //      printf("\n--------------------------------------\n");
        //        printf("The DMU %d is Efficient\n",curDMU);
        //        printf("Objective value %f\n",obval);
       //         printf("\nGiven Input\t Optimized Input\n");
       //         for(i=0;i<INPUTS;i++)
       //                 printf("%f\t%f\n",giv_ip[i],giv_ip[i]);
        ////         printf("\nGiven Output\t Optimized Output\n");
        //        for(i=0;i<OUTPUTS;i++)
        //                printf("%f\t%f\n",giv_op[i],giv_op[i]); 
        //        printf("\n--------------------------------------\n\n");                                               
        }
        else    //Make it efficient
        {
                //cols = glp_get_num_cols(P1);
                //1ST Col will be theta
                for(i=2,j=0;i<=2+DMU_COUNT-1;i++,j++)
                {
                        sprintf(t_chr,"%.6f ",glp_get_col_prim(P2,i));
                        tmp_double = atof(t_chr);
                        if(tmp_double)
                        {
                                ref_set[j] = 1;
                                ref_set_val[j] = tmp_double;
                        }
                        else ref_set[j] = 0;       
                }

                for(i=0;i<INPUTS;i++)
                {
                    opt_ip[i] =  obval * giv_ip[i] - slacks[i];
                }
                for(i=0;i<OUTPUTS;i++)
                {
                    opt_op[i] =  giv_op[i] + slacks[i+INPUTS];
                }

            //    printf("\n--------------------------------------\n");
          //      printf("The DMU %d is MADE Efficient\n",curDMU);
          ///      printf("Objective value %f\n",obval);
          //      printf("\nGiven Input\t Optimized Input\n");
          //      for(i=0;i<INPUTS;i++)
          //              printf("%f\t%f\n",giv_ip[i],opt_ip[i]);
             //    printf("\nGiven Output\t Optimized Output\n");
             //   for(i=0;i<OUTPUTS;i++)
            //            printf("%f\t%f\n",giv_op[i],opt_op[i]); 
             //   printf("\n--------------------------------------\n\n");                 

                
        }

  */     

        glp_delete_prob(P1);
     //   glp_delete_prob(P2);
        curDMU++;
        // dont forget to incriment curDMU counter

		/*FILE *write;

		write = fopen("./Eff","a+");
		fprintf(write,"%f\n",obval);
		fclose(write);*/
		printf("%f\n",obval);
	
}
