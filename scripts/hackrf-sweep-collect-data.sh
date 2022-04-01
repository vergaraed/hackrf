#!/bin/bash
SAMPLE_DURATION=60

if [ -z "$1" ] 
then
    read -p "Enter filename: " FILENAME
else
    FILENAME="$1"
fi

FILENAME="`date +"%Y%m%d"`_$FILENAME.csv"
echo $FILENAME
if [ ! -f $FILENAME ]; then
	echo "Starting to log data"
	gtimeout $SAMPLE_DURATION hackrf_sweep -f 2400:2500 -n 16384 -w 600000 > $FILENAME
    #hackrf_sweep -f 350:5600  2>/dev/null | awk -F"[ '\'''\'''\''\,]+" '\''/ -3/ || / -2/ || / -1. /  {print $2, $3, $4, $7, $8, $9, $10}'\'''
else
    echo "Starting to log data, appending to existing file."
    gtimeout $SAMPLE_DURATION hackrf_sweep -f 2400:2500 -n 16384 -w 600000 >> $FILENAME
fi
