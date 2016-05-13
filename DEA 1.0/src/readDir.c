#include <sys/dir.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>


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


                solve(filename1);
                //printf("%f",o_value);
                filename1[0]=0;
                
            
        }  
         
}
 

