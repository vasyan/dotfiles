#!/usr/bin/sh

. /etc/X11/xinit/xinitrc-common

#[[ -f ~/.Xresources ]] && xrdb -merge -I$HOME ~/.Xresources

export PATH="$HOME/.local/bin:$PATH" &
#export SHELL=/usr/bin/fish &
export XDG_SESSION_TYPE=x11


sxhkd -c .config/sxhkd/sxhkdrc &
clipmenud &
dwmstatus &
~/scripts/.fehbg &
# xinput set-prop <device-id> here is my mouse. check `xinput list | grep "mouse"`
xinput set-prop 11 "libinput Natural Scrolling Enabled" 1 &
~/scripts/set_monitor.sh &
exec /usr/local/bin/dwm


