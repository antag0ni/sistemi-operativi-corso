#!/bin/bash
mkdir myfiles
mkdir myfiles/myfiles2
cat >myfiles/input.dat <<EOF
0 12
12 0
2 1
EOF

cat >myfiles/file1.txt <<EOF
0 12
44 33
2 1
EOF

cat >myfiles/myfiles2/file2.txt <<EOF
22 11
12 0
2 1
EOF

#DIR="myfiles"
DIR=$1

# YOUR CODE HERE

warn=$'\e[31m[!]\e[0m'
ok=$'\e[32m[+]\e[0m'
info=$'\e[36m[*]\e[0m'

if [[ $# -ne 1 || ! -d "$DIR" ]]; then
    echo "$warn Parameter invalid"
    echo "$info Usage: $0 <directory>"
    exit 1
fi

>output.txt
echo "$ok output.txt cleared"

for file in "$DIR"/*; do
    if [[ -f "$file" ]]; then
        echo "${file##*/}" >>output.txt
        head -n 1 "$file" >>output.txt
        echo "$ok output.txt updated"
    elif [[ -d "$file" ]]; then
        count=$(ls "$file" | wc -l)
        echo "$info ${file##*/} is a directory"
        echo "${file##*/}" >>output.txt
        echo "Found $count file inside ./$(realpath --relative-to="." "$file")" >>output.txt
        echo "$ok output.txt updated"
    fi
done

echo "$info output.txt preview:"
echo "-----------------------------------------------------"
cat output.txt
echo "-----------------------------------------------------"
