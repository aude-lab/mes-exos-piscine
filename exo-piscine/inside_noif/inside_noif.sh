#!/bin/sh

[ $# -eq 1 ] && {
    [ -f "$1" ] && {
        cat "$1"
        exit 0
    } || {
        echo "$1:"
        echo "    is not a valid file"
        exit 2
    }
} || {
    [ $# -eq 0 ] && {
        echo "Sorry, expected 1 argument but 0 were passed"
    } || {
        echo "Sorry, expected 1 argument but $# were passed"
    }
    exit 1
}
