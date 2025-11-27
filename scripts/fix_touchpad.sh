sudo udevadm trigger --subsystem-match=input --action=change
sleep 2
setxkbmap de -option caps:swapescape
xset r rate 200 50
