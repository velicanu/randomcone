script="randomcone"

if [ $# -ne 4 ]
then
  echo "Usage: ./${script}.sh <filelist> <tag> <centmin> <centmax>"
  exit 1
fi

now="${script}_$2_$3_$4_$(date +"%Y_%m_%d__%H_%M_%S")"
len=`wc -l $1 | awk '{print $1}'`
njobs=$((len))

mkdir $now
cp $1 $now
cp ${script}.sh $now
cat runrandcone.condor | sed "s@log_flag@$now@g" | sed "s@sh_flag@${script}.sh@g" | sed "s@dir_flag@$PWD/$now@g" | sed "s@user_flag@$USER@g" |  sed "s@arglist@$1 $2 $3 $4@g" | sed "s@transfer_filelist@$1,${script}.exe@g" | sed "s@njobs@$njobs@g" > $now/${script}.condor

NAME="${script}.C"
g++ $NAME $(root-config --cflags --libs) -Werror -Wall -O2 -o "${NAME/%.C/}.exe"
cp ${script}.exe $now
echo

echo 
echo condor_submit $now/${script}.condor
echo
echo $now/${script}.sh 0 $1 $2 $3 $4
echo 
# condor_submit $now/runjettrk.condor

