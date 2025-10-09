#!/bin/sh

if [ $# -ne 2 ]; then
	exit 1
fi

filename="$1"
line_num="$2"

if [ ! -f "$filename" ] || [ "$line_num" -le 0 ]; then
    exit 1
fi

total_lines=$(wc -l < "$filename")
if [ "$line_num" -gt "$total_lines" ]; then
    exit 1
fi

head -n "$line_num" "$filename" | tail -n 1 | cut -d';' -f2,3 | sed 's/;/ is /'
