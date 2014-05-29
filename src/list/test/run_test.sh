#!/bin/bash

FILENAME=list

source ../../share.sh

cd .. && make
cd test

if [ -z $1 ]
then
    run_cmd "$CC $CFLAG $HEADER $LIB test_$FILENAME.c -lcunit -lpthread -lm $OUT"
    run_ts "./test.out" del
else
    if [ $1 == 'c' ]
    then
        run_cmd "$CC $CFLAG $HEADER $LIB test_$FILENAME.c -lcunit -lpthread -lm $OUT"
    fi
fi
