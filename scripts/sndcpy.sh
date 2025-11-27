sound="sndcpy"
previous_process=$(ps -ef | grep "vlc -Idummy" | grep -v "grep" | awk '{ print $2}')

if [ -z "$previous_process" ]
then
    $sound
else
    kill $previous_process
fi
