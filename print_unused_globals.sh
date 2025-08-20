#!/bin/bash

unused_globals=$(cat src/* | grep "static constexpr" | awk '{print $4}' | grep -o '^[A-Za-z0-9_]\+')

for global in $unused_globals
do
    count=$(git grep $global | wc --lines)
    if [ $count -eq 1 ]
    then
        echo "unused global: "$global
    fi
done
