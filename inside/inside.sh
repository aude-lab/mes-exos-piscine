#!/bin/sh

if [ $# -ne 1 ]; then
    echo "Sorry, expected 1 argument but $# were passed"
    exit 1
fi

if [ ! -f "$1" ]; then
    printf "\t$1:\n\t    is not a valid file\n"
    exit 2
fi

cat "$1"
