#!/bin/bash
# You can exit all tmux windows at once by pressing CTRL+B and typing :kill-window

./scripts/build.sh

if [ $? -ne 0 ]
then
    echo "Build failed!"
    exit 1
fi

if [ "$1" != "" ]; then
    CLIENT_COUNT=$1
else
    CLIENT_COUNT=1
fi

tmux new -s ungroup -d
tmux rename-window -t ungroup game
tmux send-keys -t ungroup './build/src/server/ug-server' C-m
for (( i = 0; i < $CLIENT_COUNT; i++ ))
do
    tmux split-window -v -t ungroup
    tmux send-keys -t ungroup './build/src/client/ug-client' C-m
done
tmux select-window -t ungroup:1
tmux attach -t ungroup

