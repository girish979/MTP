#usage
#./script.sh cvalue gammaval

# Train the given input, will generate model file

`rm -f ./train_op/*`
`rm -f ./iris.model`

`rm -f ./LPs/*`
#`rm -f ./SVMOP/*`
echo "Traing SVM with c value $1, gamma value $2";
`./svm-train -c $1 -g $2 iris > ./train_op/op` #$1 C value, $2 gamma values

echo "Finished training SVM";

echo "Finding Inputs, Outputs";
#########################################

#echo "Finidin Spans";
#`mkdir kmats`
#echo "Generating Kernel Matrices";
`./gen-kernel iris.model`

path2="./SVMOP/$3_spans_ip2"


#`./inverse ./kmats/ > $path2`
`rm -f tspan`
echo "finding inverse"
`./inverse ./kmats`
echo "DONE inverse"
`./spanupdate > $path2`
echo 



#echo "Spans written to ./SVMOP/spans_ip2";

#echo "removing kernel matrices";
#<<COMMENT1
`rm -f ./kmats/*`


############################################
path1="./SVMOP/$3_sv_ip1"

`grep -e '^nSV' ./train_op/op > ./train_op/SVs`
`cut -d ' ' -f3 ./train_op/SVs > $path1`
`sed -i 's/,//g' $path1`

############################################
path4="./SVMOP/$3_margin_op2"

`./gen-margins iris.model > $path4`

############################################
path3="./SVMOP/$3_acc_op1"

`./svm-accuracy iris iris.model > $path3`  #iris_t test data set

################################################

#`./lpg_phase1 $path1 $path2 $path3 $path4`
#`./lpg_phase2 $path1 $path2 $path3 $path4`

#COMMENT1
