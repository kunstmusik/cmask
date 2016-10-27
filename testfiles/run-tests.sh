#!/bin/bash

CMASK='../source/cmask'
TESTFILES='test03
	test03accu
	test03all
	test03item
	test03mask
	test03mix
	test03osc
	test03quant
	test03randwalk
	test03rnd
	test03seg
'

# Run CMask with each file in TESTFILES as the input
# NOTE: adds ".sco" to each input filename and overwrites any 
# existing files with those names!

for f in $TESTFILES ; do
	if test -f "$f"
	then
		$CMASK "$f" "$f.sco"
	else
		echo "ERROR(runtests.sh): $f does not exist or is not a regular file."
	fi
done
