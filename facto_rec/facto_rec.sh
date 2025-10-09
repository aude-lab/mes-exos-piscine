#!/bin/sh

factorial() {
	if [ "$1" -eq 0 ]; then
		echo 1
		return
	fi
	echo $(($1 * $(factorial $(($1 - 1)))))
}
	if [ $# -ne 1 ];then
		exit 1
	fi
factorial "$1"
