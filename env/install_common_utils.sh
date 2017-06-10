#!/bin/bash

sudo add-apt-repository ppa:neovim-ppa/unstable && sudo apt-get update

BASIC_TOOLS="vim neovim git subversion"
COMPILING_TOOLS="gcc g++ gdb llvm clang clang++ binutils"
SYSTEM_MONITOR_TRACING_PERF_TOOLS="sysstat dstat htop iotop iftop tcpdump systemtap ethtool ngrep"
LANGUAGE_TOOLS="python-pip python3-pip"
USER_TOOLS="tmux jq proxychains youtube-dl axel"

sudo apt-get install -y $BASIC_TOOLS
sudo apt-get install -y $COMPILING_TOOLS
sudo apt-get install -y $SYSTEM_MONITOR_TRACING_PERF_TOOLS
sudo apt-get install -y $LANGUAGE_TOOLS
sudo apt-get install -y $USER_TOOLS
