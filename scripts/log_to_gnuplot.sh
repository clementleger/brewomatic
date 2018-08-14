#!/bin/bash
set -eux

if [ "$#" -ne 1 ]; then
	echo "Missing log file !"
	exit 
fi

log=$1
gpout=${log}.gp

sampling_time=100
current_time=0

echo "" > ${gpout}

while read line
do
	echo "${current_time} ${line/:/ }" >> ${gpout}
	current_time=$((current_time + ${sampling_time}))
done < ${log}

gnuplot <<END
set title "Temperature curves"
set terminal png size 1280,860
set output '${log}.png'
set xlabel "Time"
show xlabel
set ylabel "Temperature (°C)"
show ylabel
plot "${gpout}" using 1:2 smooth csplines title "Measured temp",\
"${gpout}" using 1:3 with lines title "Target temp"
END
