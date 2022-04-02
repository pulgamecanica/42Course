#!/bin/bash

cat 19920104_091532.log | awk '{print $2}' | sort > original.txt
./dream_job | cat | awk '{print $2}' | sort > yours.txt

RES=$(diff original.txt yours.txt | wc -l)

if [ $RES -eq 0 ]; then
	echo "OK"
else
	echo "RIP.... you KO"
fi

rm -f original.txt yours.txt
