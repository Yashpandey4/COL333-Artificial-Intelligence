chmod 777 a.out
INPUT="$1.graph"
OUPUT="$1.subgraphs"
MINOUT="$1.satoutput"
./a.out $MINOUT < $INPUT > $OUPUT