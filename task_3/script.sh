#!/bin/bash

for i in {1..20}; do
    mkdir -p "$i" && touch "$i/$i.txt"
    ln -s "$(pwd)/$i/$i.txt" "$i.txt"
    echo "$i.txt" > "$i/$i.txt"

    if ! grep -q "4" "$i/$i.txt"; then
        rm "$i/$i.txt"
        rm "$i.txt"
    fi
done

echo "Просто какой-то текст" > "14/14.txt"
