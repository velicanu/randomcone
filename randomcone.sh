script="randomcone"

if [ $# -ne 5 ]
then
  echo "Usage: ./${script}.sh <filenum> <filelist> <tag> <centmin> <centmax>"
  exit 1
fi

echo | awk -v i=${1} -v flist=${2} -v tag=${3} -v centmin=${4} -v centmax=${5} -v script=${script} '{print "./"script".exe "i" "flist" "tag" "centmin" "centmax" "}' 
echo | awk -v i=${1} -v flist=${2} -v tag=${3} -v centmin=${4} -v centmax=${5} -v script=${script} '{print "./"script".exe "i" "flist" "tag" "centmin" "centmax" "}' | bash 


echo | awk -v tag=$3 -v user=$USER -v now=${24} '{print "mv *.root /mnt/hadoop/cms/store/user/"user"/unmerged/"}'
echo | awk -v tag=$3 -v user=$USER -v now=${24} '{print "mv *.root /mnt/hadoop/cms/store/user/"user"/unmerged/"}' | bash

# echo | awk -v tag=$4 '{print "rm "tag"*.root"}'
# echo | awk -v tag=$4 '{print "rm "tag"*.root"}' | bash

echo "job done successfully"
