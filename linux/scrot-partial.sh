#!/bin/bash
#scrot -shole -f -e 'optipng $f' | xclip -sel clip
scrot -shole -f - | xclip -selection clipboard -target image/png

