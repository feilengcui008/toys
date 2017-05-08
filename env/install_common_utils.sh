#!/bin/bash

BASIC_TOOLS="vim git"
COMPILING_TOOLS="gcc g++ gdb llvm clang clang++ binutils"
SYSTEM_MONITOR_TRACING_PERF_TOOLS="sysstat dstat htop iotop iftop tcpdump systemtap ethtool ngrep"
USER_TOOLS="tmux jq proxychains youtube-dl axel"

sudo apt-get install -y $BASIC_TOOLS
sudo apt-get install -y $COMPILING_TOOLS
sudo apt-get install -y $SYSTEM_MONITOR_TRACING_PERF_TOOLS
sudo apt-get install -y $USER_TOOLS
