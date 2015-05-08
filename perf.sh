#!/usr/bin/env bash
if [ $# -ne 7 ]; then
  echo "Usage: ./perf.sh A B C D E F G"
  echo ""
  echo "Runs G iterations of each test with D to E threads incremented by F on datasets from A to B as multipls of C"
  echo "Example:"
  echo "./perf.sh 10 100000 10 1 4 1 10"
  exit 1
fi

echo "Running tests between $1 and $2 with multiples of $3 with threads between $4 and $5 with increments of $6 with $7 iterations each"
COUNT=$1
while [ $COUNT -le $2 ]; do
  THREAD=$4
  while [ $THREAD -le $5 ]; do
    ITERATION=1
    while [ $ITERATION -le $7 ]; do
      if [ $ITERATION -eq 1 ]; then
        #Throw out the first iteration to compensate for optimizations by kernel
        ./a.out $COUNT $THREAD
      else
        START=$(date +%s.%N)
        MEMORY=$(/usr/bin/time -f "%M" ./a.out $COUNT $THREAD 2>&1)
        FINISH=$(date +%s.%N)
        echo -e "$COUNT\t$THREAD\t$ITERATION\t$MEMORY\t$(python -c "print(${FINISH}-${START})")"
      fi
      ((ITERATION=ITERATION+1))
    done
    ((THREAD=THREAD+$6))
  done
  ((COUNT=COUNT*$3))
done
