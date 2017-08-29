#!/bin/sh

SRC_IP=10.10.114.181
DST_IP=10.10.114.182

USER=root

scp sender $USER@$SRC_IP:~
scp receiver $USER@$DST_IP:~

echo "Sender files:"
ssh $USER@$SRC_IP "ls"
echo "Receiver files:"
ssh $USER@$DST_IP "ls"

IN_FILE="/home/alex/covert-channel/in"
scp $IN_FILE $USER@$SRC_IP:~

TIME=$(ssh $USER@$SRC_IP "expr `date +%s` + 20")
echo "Starting execution at: $TIME"
ssh $USER@$SRC_IP "./sender in $TIME" &
ssh $USER@$DST_IP "./receiver out $TIME" &
