-!bin/usr/env bash

echo "Displaying diffs..."
NUM=""
for (( i = $1; i <= $2; i++ )) 
do
  if (($i < 10)); 
  then
    NUM="000${i}"
  elif (($i < 100));
  then 
    NUM="00${i}"
  else 
    NUM="0${i}"
  fi

  echo "---------- Diff - ${NUM} ----------"
  # cat "./output/diff-${NUM}.txt"
    diff "../test-cases/saida-${NUM}.txt" "./output/saida-${NUM}.txt"
  echo "----------------------------------"
  echo ""
done 
echo "Completed!"