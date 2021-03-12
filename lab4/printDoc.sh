#!/bin/sh
for file in `find . -name '*.c' -or -name '*.h'`;
do
   echo "============================================"
   echo  
   sed -e '/\/\/+/,/\/\/-/p' -e 'd' $file | sed -e 'sX//XX' -e 'sX+XX' -e 'sX-XX'
done

