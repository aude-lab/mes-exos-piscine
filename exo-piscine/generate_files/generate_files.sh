#!/bin/sh

FILENAME="default"
NUMBER=1
EXTENSION="txt"

while [ $# -gt 0 ]; do
	case "$1" in
		-f|--filename)
			if [ -z "$2" ]; then
                		exit 1
            		fi
            		FILENAME="$2"
            		shift 2
            		;;
        	-n|--number)
            		if [ -z "$2" ]; then
                		exit 1
            		fi
            		NUMBER="$2"
            		shift 2
            		;;
        	-e|--extension)
            		if [ -z "$2" ]; then
                		exit 1
            		fi
            		EXTENSION="$2"
            		shift 2
            		;;
        	*)
            		exit 1
            		;;
    esac
done
if [ "$NUMBER" -lt 1 ]; then
	exit 1
fi
for i in $(seq 1 "$NUMBER"); do
	filename="${FILENAME}-${i}.${EXTENSION}"
	touch -- "$filename"
done 
