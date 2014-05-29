#!/bin/bash
# 
# Script name: QEBATS.sh
# 
# Author: Janice Yip
# Date: 05/28/2014 
#
# Description: Script to test AMStartup.c 
#
# Command line options: none
#
# Input: Calls script multiple times with different parameters 
# 	to check the operation of the program.  
#
# Output: The results from the tests outputed to a file 
# 	"indexerTestlog.‘date +"%a_%b_%d_%T_%Y"‘". 
#

# Testing 
DATE=$(date +"%a_%b_%d_%T_%Y")
OUTPUT=amazingTestlog.$DATE
echo $OUTPUT > $OUTPUT 
echo "This is a test shell script for the amazing project." >> $OUTPUT
echo "Hostname:" `echo $HOSTNAME` >> $OUTPUT
echo "Operating System: Linux" >> $OUTPUT 
echo "" >> $OUTPUT
echo "Author: Oseleta" >> $OUTPUT
echo "Date written: 05/28/2014" >> $OUTPUT

echo 'make clean: ' `make clean` | cat >> $OUTPUT 
make >> $OUTPUT
echo "Makefile and AMStartup and amazing_client created. Begin testing." >> $OUTPUT
echo "" >> $OUTPUT 
echo "---" >> $OUTPUT
echo "" >> $OUTPUT


# Invalid number of avatars
echo "Invalid number of avatars; error expected" >> $OUTPUT 
echo "./AMStartup A 2 pierce.cs.dartmouth.edu" >> $OUTPUT
echo "" >> $OUTPUT
./AMStartup A 2 pierce.cs.dartmouth.edu >> $OUTPUT 2>&1
echo "" >> $OUTPUT
echo "---" >> $OUTPUT 
echo "" >> $OUTPUT


# Invalid difficulty 
echo "Invalid difficulty; error expected" >> $OUTPUT 
echo "./AMStartup 2 A pierce.cs.dartmouth.edu" >> $OUTPUT
echo "" >> $OUTPUT
./AMStartup 2 A pierce.cs.dartmouth.edu >> $OUTPUT 2>&1
echo "" >> $OUTPUT
echo "---" >> $OUTPUT 
echo "" >> $OUTPUT


# Invalid server 
echo "Invalid server; error expected" >> $OUTPUT 
echo "./AMStartup 2 3 tahoe.cs.dartmouth.edu" >> $OUTPUT
echo "" >> $OUTPUT
./AMStartup 2 3 tahoe.cs.dartmouth.edu >> $OUTPUT 2>&1
echo "" >> $OUTPUT
echo "---" >> $OUTPUT 
echo "" >> $OUTPUT


## Test hashtable 
#echo "Test hashtable" >> $OUTPUT 
#echo "make hashtable_test" >> $OUTPUT 
#echo `make hashtable_test` >> $OUTPUT 
#echo "" >> $OUTPUT
#echo "hashtable_test" >> $OUTPUT
#hashtable_test >> $OUTPUT 2>&1
#echo "" >> $OUTPUT
#
#echo "science 2 2 3 1 1 " > realindex.dat_hash
#echo "math 1 8 1 " >> realindex.dat_hash
#echo "computer 3 1 2 2 3 3 2 " >> realindex.dat_hash
#
#echo "index.dat_hash file : " >> $OUTPUT
#cat index.dat_hash >> $OUTPUT
#echo "" >> $OUTPUT
#echo "Compare the output: diff index.dat_hash realindex.dat_hash" >> $OUTPUT
#
#if diff index.dat_hash realindex.dat_hash; then
#    echo "Hashtable correct." >> $OUTPUT
#    echo SUCCESS >> $OUTPUT
#else
#    echo FAIL >> $OUTPUT
#fi
#
#echo "Hashtable test finished:" `date +"%a_%b_%d_%T_%Y" ` >> $OUTPUT
#echo "" >> $OUTPUT
#echo "---" >> $OUTPUT
#echo "" >> $OUTPUT
#
#
## Test query 
#echo "Test query" >> $OUTPUT 
#echo "make query_test" >> $OUTPUT 
#echo `make query_test` >> $OUTPUT 
#echo "" >> $OUTPUT
#echo "query_test" >> $OUTPUT
#query_test ~cs50/tse/indexer/cs_lvl3.dat ~cs50/tse/crawler/lvl3 >> $OUTPUT 2>&1
#echo "" >> $OUTPUT
#
#echo "" >> $OUTPUT
#echo "Query test finished:" `date +"%a_%b_%d_%T_%Y" ` >> $OUTPUT
#echo "" >> $OUTPUT
#echo "---" >> $OUTPUT
#echo "" >> $OUTPUT
#

# Begin testing 
echo "Begin testing different inputs." >> $OUTPUT
echo "   Expect log file in format AMAZING_$USER_nAvatars_difficulty.log" >> $OUTPUT
echo "   that contains avatar ID and its move and whether it solved the maze or not" >> $OUTPUT
echo "" >> $OUTPUT
echo "---" >> $OUTPUT
echo "" >> $OUTPUT


# Testing 2 avatars on difficulty 2 
echo "Testing 2 avatars on difficulty 2" >> $OUTPUT
echo "./AMStartup 2 2 pierce.cs.dartmouth.edu" >> $OUTPUT
echo "" >> $OUTPUT
./AMStartup 2 2 pierce.cs.dartmouth.edu 
echo "" >> $OUTPUT
echo "---" >> $OUTPUT
echo "" >> $OUTPUT


# Testing 6 avatars on difficulty 5 
echo "Testing 6 avatars on difficulty 5" >> $OUTPUT
echo "./AMStartup 6 5 pierce.cs.dartmouth.edu" >> $OUTPUT
echo "" >> $OUTPUT
./AMStartup 6 5 pierce.cs.dartmouth.edu 
echo "" >> $OUTPUT
echo "---" >> $OUTPUT
echo "" >> $OUTPUT


echo "Testing finished:" `date +"%a_%b_%d_%T_%Y" ` >> $OUTPUT
echo "" >> $OUTPUT


echo 'make clean: ' `make clean` | cat >> $OUTPUT 
echo "" >> $OUTPUT 
echo "DONE" >> $OUTPUT 
