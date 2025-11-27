#!/bin/bash

FOUND=$(lsusb | grep 'Lenovo ThinkPad Thunderbolt 3 Dock MCU' | head -n 1)

if [ ! -z "$FOUND" ]; then
    xrandr --output eDP1 --off --output DP1 --off --output DP1-1 --mode 1920x1080 --pos 0x0 --rotate normal --output DP1-2 --mode 3840x2160 --pos 1920x0 --rotate normal --output DP1-3 --off --output DP2 --off --output HDMI1 --off --output VIRTUAL1 --off
    #dockd --set docked
    setxkbmap de -option
    notify-send -c "dock" "🔗 Dock connected"
else
    #dockd --set undocked
    setxkbmap de -option caps:swapescape
    notify-send -c "dock" "❌ Dock disconnected"
fi

feh --bg-scale "$(cat $HOME/.cache/wal/wal)"
