#!/bin/bash


dataFiles=`ls Data | grep .graph`

for file in $dataFiles
do
	filename=`echo $file | cut -d'.' -f1`
	echo $file $filename
	build/exec -inst $filename -alg Approx -time 10 -seed $1


done
