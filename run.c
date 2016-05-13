#include <stdio.h>

void main()
{


	system("./ga vowel 11");
	system("mv RESULTS ./finalres/RESULTS_vowel");
	system("rm -f vowel vowel.model");
	system("rm -f SVMOP/*");
	system("rm -f SVMOP2/*");
	system("rm -f LPs2/*");


	
	system("./ga vehicle 4");
	system("mv RESULTS ./finalres/RESULTS_vehicle");
	system("rm -f vehicle vehicle.model");
	system("rm -f SVMOP/*");
	system("rm -f SVMOP2/*");
	system("rm -f LPs2/*");
	system("rm -f train_op/*");


	system("./ga segment 7");
	system("mv RESULTS ./finalres/RESULTS_segment");
	system("rm -f segment segment.model");
	system("rm -f SVMOP/*");
	system("rm -f SVMOP2/*");
	system("rm -f LPs2/*");
	system("rm -f train_op/*");	


	system("./ga satimage 6");
	system("mv RESULTS ./finalres/RESULTS_satimage");
	system("rm -f satimage satimage.model");
	system("rm -f SVMOP/*");
	system("rm -f SVMOP2/*");
	system("rm -f LPs2/*");
	system("rm -f train_op/*");		
	
	
	
}
