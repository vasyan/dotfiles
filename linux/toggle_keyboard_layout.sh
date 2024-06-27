#!/bin/bash

CURRENT_LAYOUT=$(setxkbmap -query | grep layout | awk '{print $2}')

LAYOUTS=("us" "ru")

for i in "${!LAYOUTS[@]}"; do
    if [[ "${LAYOUTS[$i]}" == "$CURRENT_LAYOUT" ]]; then
        CURRENT_INDEX=$i
        break
    fi
done

NEXT_INDEX=$(( (CURRENT_INDEX + 1) % ${#LAYOUTS[@]} ))

setxkbmap -layout "${LAYOUTS[$NEXT_INDEX]}"

