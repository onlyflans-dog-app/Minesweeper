#!/bin/bash

clear

if [ "$(uname -o)" == "Cygwin" ]; then
    file_name="./bin/win32/template.exe"
else
    file_name="./bin/lnx/template.elf"
fi

#make clean;
make -j 8;

bytes=$(stat -c "%s" $file_name)
kb=$(awk "BEGIN {print $bytes/1024}")
mb=$(awk "BEGIN {print $kb/1024}")
echo "File: $file_name"
echo "Size: $bytes bytes / $kb KB / $mb MB"
size $file_name


if [ "$(uname -o)" == "Cygwin" ]; then
    cygcheck $file_name > cygcheck_output.txt
    sed 's/\\/\//g' cygcheck_output.txt | while read line; do
        dll_file="$(basename "$line")"
            echo "$dll_file"
            cp "/cygdrive/c/cygwin64/bin/$dll_file" ./bin/win32/ &>/dev/null
    done
    rm cygcheck_output.txt
    cp -r ./res ./bin/win32/
    cp -r ./dll ./bin/win32/
    cp -r ./userdata ./bin/win32/userdata/
else
    cp -r ./res ./bin/lnx/
    cp -r ./userdata ./bin/lnx/userdata/
fi

$file_name