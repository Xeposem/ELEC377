#!/bin/sh
if (($# != 1)); then
   echo "Usage: progStats.sh $0"
else
   if test -d $1; then
      echo "Main Files:"
      main=1
      for file in `find $1 -type f -name '*.c'`; do
         if `grep -q 'int main' $file`; then
	    main=0
	    pfcount=`grep -c printf $file`
	    fpfcount=`grep -c fprintf $file`
	    ((pfcount=pfcount-fpfcount))
	    echo $file $pfcount $fpfcount | awk '{printf "%s: %d,%d\n", $1, $2, $3}'
	 fi
      done
      if (($main != 0)); then
         echo "   No main files..."
      fi
      echo
      echo "Module Files:"
      module=1
      for file in `find $1 -type f -name '*.c'`; do
         if `grep -q 'init_module' $file`; then
	    module=0
	    tst=`grep -n 'printk' $file | sed "s/[^0-9]//g" | sed -r "s/(.*)0/\1 /" | tr '\n' ',' | sed "s/ //g" | sed -r 's/(.*),/\1 /'` 
	    echo "$file: $tst"
	 fi
      done
      if(($module != 0)); then
         echo "   No module files..."
      fi
   else
      echo "   $1 is not a directory"
   fi
fi   

