# If previous process exists, kill it and exit programm
previous_process=$(ps -ef | grep "x2x" | grep -v "grep" | awk '{ print $2}')
if [ ! -z "$previous_process" ]
then
    kill $previous_process
    echo "Terminated previous process"
    exit 0
fi

# Check which computer to connect to
hosts=(
    "mike-ross"
    "harvey-specter"
)
host="$(for i in ${hosts[@]}; do echo $i; done | dmenu -p 'Select computer')"
if [ -z "$host" ]
then
    echo "No host selected"
    exit 1
fi

# Check if mike-ross is available in the network
available="drill -Q $host"
if [ -z "$available" ]
then
    echo "Host not available"
    exit 1
fi

# Get the position to where to put the display
positions=(
    "north"
    "south"
    "east"
    "west"
)
position="$(for i in ${positions[@]}; do echo $i; done | dmenu -p 'Select position')"
if [ -z "$position" ]
then
    echo "No position selected"
    exit 1
fi

ssh -YC $host x2x -$position -to :0.0
