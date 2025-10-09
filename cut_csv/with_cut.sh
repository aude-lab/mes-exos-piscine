#!/bin/sh

if [ $# -ne 2 ]; then
	exit 1
fi

filename="$1"
line_num="$2"

if [ ! -f "$filename" ]; then
    exit 1
fi

if [ ! -r "$filename" ]; then
    exit 1
fi

case "$line_num" in 
	''|*[!0-9]*)
		exit 1
		;;
esac


total_lines=$(wc -l < "$filename")

if [ ! -f "$filename" ] || [ "$line_num" -le 0 ] || [ "$line_num" -gt "$total_lines" ]; then
    exit 1
fi


line=$(head -n "$line_num" "$filename" | tail -n 1)

column2=$(echo "$line" | cut -d';' -f2)
column3=$(echo "$line" | cut -d';' -f3)
echo "$column2 is $column3"
