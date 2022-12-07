#!bin/usr/env bash

echo "Running tests..."
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

  echo "Test - ${NUM}"
  ./main "./test-cases/entrada-${NUM}.txt" "./output/saida-${NUM}.txt"
  diff -y "./test-cases/saida-${NUM}.txt" "./output/saida-${NUM}.txt" > "./diff/diff-${NUM}.txt"

done 
echo "Tests completed!"