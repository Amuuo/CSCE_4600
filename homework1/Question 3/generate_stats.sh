#!/bin/bash
#
#======================================================================
# Name        : Homework #1 : Question #3
# Author      : Adam Williams
# Version     : 1.0
# Copyright   : 2018
# Description : Script creates fileA and fileB run file_copy c program
#               with program arguments from 1-128MB and 1-1024 bytes
#======================================================================
#


printf "\n\n \
       [37;42m +------------------------------------------------+ [0m\n \
       [37;42m |        Computer Sceince and Engineering        | [0m\n \
       [37;42m |          CSCE 4600 - Operating Systems         | [0m\n \
       [37;42m | Adam Williams arw0174 adamwilliams2@my.unt.edu | [0m\n \
       [37;42m +------------------------------------------------+ [0m\n\n\n";

# compile file_copy.c program
gcc file_copy.c

# open csv file with appropiate header line
echo "MB,bytes per write,seconds" > data.csv

# loop through file sizes from 1 to 128 MB, increments of i*2
for ((i=1;i<=128;)) do

    # Open file of $i MB
    truncate -s "$i"M /tmp/fileA
    file_size=$(($i*1048576))
    printf "\nFile Size: %s MB\n" "$i"

    # run file_copy.c with file copy sizes from 1 to 1024, increments of i*2
    for ((j=1;j<=1024;)) do

        touch /tmp/fileB
        printf "\tcopy size: %s bytes\n" "$j"

        # record times for each option into csv file
        # columns: MB, bytes per copy, seconds
        { time ./a.out $file_size $j; } 2>&1 | cat > time.txt
        printf "%s,%s," "$i" "$j" >> data.csv
        gawk '{ if ($0 ~ /real.*/) { print $2 }}' time.txt | \
            sed 's/\(0m\|s\)//g' >> data.csv;

        # delete fileB
        rm /tmp/fileB;
        j=$((j*2))

    done

    # delete fileA
    rm /tmp/fileA;
    i=$((i*2))


done

rm time.txt

#python3 graph_data.py
