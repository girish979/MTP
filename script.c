#include <sys/dir.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

void remove_file(char []);

int dir_filter(const struct direct *entry) 
{
        if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
                return 0;
        return 1;
}
 
void dir_read( char dir[]) 
{
        struct direct **files;
        int nfiles, x;
        int dir_filter(const struct direct *entry);
        char path[200],filename1[200],filename2[200];

        double o_value=0.0;
 
        nfiles = scandir(dir, &files, dir_filter, alphasort);
  
  
        for(x=0;x<nfiles;x++)
        {
                if( files[x]->d_type == DT_DIR )
          	{
          	  	strcpy(path,"\0");
          	  	strcpy(path,dir);
		        strcat(path,"/");
                        strcat(path,files[x]->d_name);
                        dir_read(path);
          	  	
         	 }
        }
 
        if(nfiles <= 0);
            //return 0;
 
        for(x=0;x<nfiles;x++)
        {
                if(files[x]->d_type != DT_DIR)
                {
                        sprintf(filename1,"%s/%s", dir, files[x]->d_name);
                    	//printf("\n%s",filename1);
                }  

                //printf("%s\n",filename1);
                remove_file(filename1);
                filename1[0]=0;            
        }  
}

void remove_file(char f[])//Deletes file ./cur/abc/file
{
	remove(f);
}

void main(int argc, char *argv[])
{
	//	Execution ./a.out 1 .01 2 4 [ds name]
	//	./a.out C sigma pop iteration ds_name


	char ex_cmd[200];

	char dataset[100], dataset_model[100],validation[100];
	char path1[100],path2[100],path3[100],path4[100];

	
	strcpy(dataset,argv[5]);// "iris");//CHECK AGAIN source for getting dataset name here
	dataset[strlen(dataset)]='\0';
	strcpy(dataset_model,dataset);
	strcat(dataset_model,".model");
	dataset[strlen(dataset_model)]='\0';

	strcpy(validation,dataset);
	strcat(validation,".v");
	

	dir_read("./train_op"); //Remove all files from ./train_op directory
	remove(dataset_model);//Remove previous model file //CHECK AGAIN uncomment and take care of dataset filename
	dir_read("./LPs");

	//  ./svm-train -c $1 -g $2 iris > ./train_op/op
	
	strcpy(ex_cmd,"./svm-train -c ");
	strcat(ex_cmd,argv[1]);	strcat(ex_cmd," -g "); strcat(ex_cmd,argv[2]);
	strcat(ex_cmd," "); strcat(ex_cmd,dataset); strcat(ex_cmd," > ./train_op/op");
	printf("Training SVM with C:%s sigma:%s\n",argv[1],argv[2]);
	system(ex_cmd);// CHECK AGAIN  change printf to system call

	printf("Finding Inputs, Outputs\n");

	ex_cmd[0]='\0';
	strcpy(ex_cmd,"./gen-kernel "); strcat(ex_cmd,dataset_model); strcat(ex_cmd," "); strcat(ex_cmd,argv[1]);
	system(ex_cmd);//CHECK AGAIN make as system call


	//path2="./SVMOP/$3_spans_ip2"

	strcpy(path2,"./SVMOP/"); strcat(path2,argv[3]); strcat(path2,"_spans_ip2");
	ex_cmd[0]='\0';
	strcpy(ex_cmd,"./inverse ./kmats > "); // Find inverse //CHECK AGAIN remove comment
	strcat(ex_cmd,path2);
	system(ex_cmd);
	dir_read("./kmats");

	//path1="./SVMOP/$3_sv_ip1"
	strcpy(path1,"./SVMOP/"); strcat(path1,argv[3]); strcat(path1,"_sv_ip1");
	//grep -e '^nSV' ./train_op/op > ./train_op/SVs
	system("grep -e '^nSV' ./train_op/op > ./train_op/SVs");// CHECK AGAIN  change printf to system call
	//cut -d ' ' -f3 ./train_op/SVs > $path1
	ex_cmd[0]='\0';
	strcpy(ex_cmd,"cut -d ' ' -f3 ./train_op/SVs > "); strcat(ex_cmd,path1);
	system(ex_cmd);//CHECK AGAIN make as system call
	//sed -i 's/,//g' $path1
	ex_cmd[0]='\0';
	strcpy(ex_cmd,"sed -i 's/,//g' "); strcat(ex_cmd,path1);
	system(ex_cmd);//CHECK AGAIN make as system call

	//path4="./SVMOP/$3_margin_op2"
	strcpy(path4,"./SVMOP/");strcat(path4,argv[3]); strcat(path4,"_margin_op2");
	//./gen-margins iris.model > $path4
	ex_cmd[0]='\0';
	strcpy(ex_cmd,"./gen-margins "); strcat(ex_cmd,dataset_model); strcat(ex_cmd," > ");strcat(ex_cmd,path4);
	system(ex_cmd);//CHECK AGAIN make as system call

	//path3="./SVMOP/$3_acc_op1"
	strcpy(path3,"./SVMOP/");strcat(path3,argv[3]); strcat(path3,"_acc_op1");
	//`./svm-accuracy iris iris.model > $path3`
	ex_cmd[0]='\0';
	strcpy(ex_cmd,"./svm-accuracy "); strcat(ex_cmd,dataset); strcat(ex_cmd," "); strcat(ex_cmd,dataset_model); strcat(ex_cmd," > ");strcat(ex_cmd,path3);
	system(ex_cmd);//CHECK AGAIN make as system call
	
}
