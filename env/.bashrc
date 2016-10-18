# ~/.bashrc: executed by bash(1) for non-login shells.
# see /usr/share/doc/bash/examples/startup-files (in the package bash-doc)
# for examples

# If not running interactively, don't do anything
case $- in
    *i*) ;;
      *) return;;
esac

# don't put duplicate lines or lines starting with space in the history.
# See bash(1) for more options
HISTCONTROL=ignoreboth

# append to the history file, don't overwrite it
shopt -s histappend

# for setting history length see HISTSIZE and HISTFILESIZE in bash(1)
HISTSIZE=1000
HISTFILESIZE=2000

# check the window size after each command and, if necessary,
# update the values of LINES and COLUMNS.
shopt -s checkwinsize

# If set, the pattern "**" used in a pathname expansion context will
# match all files and zero or more directories and subdirectories.
#shopt -s globstar

# make less more friendly for non-text input files, see lesspipe(1)
[ -x /usr/bin/lesspipe ] && eval "$(SHELL=/bin/sh lesspipe)"

# set variable identifying the chroot you work in (used in the prompt below)
if [ -z "${debian_chroot:-}" ] && [ -r /etc/debian_chroot ]; then
    debian_chroot=$(cat /etc/debian_chroot)
fi

# set a fancy prompt (non-color, unless we know we "want" color)
case "$TERM" in
    xterm-color) color_prompt=yes;;
esac

# uncomment for a colored prompt, if the terminal has the capability; turned
# off by default to not distract the user: the focus in a terminal window
# should be on the output of commands, not on the prompt
#force_color_prompt=yes

if [ -n "$force_color_prompt" ]; then
    if [ -x /usr/bin/tput ] && tput setaf 1 >&/dev/null; then
	# We have color support; assume it's compliant with Ecma-48
	# (ISO/IEC-6429). (Lack of such support is extremely rare, and such
	# a case would tend to support setf rather than setaf.)
	color_prompt=yes
    else
	color_prompt=
    fi
fi

if [ "$color_prompt" = yes ]; then
    PS1='${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '
else
    PS1='${debian_chroot:+($debian_chroot)}\u@\h:\w\$ '
fi
unset color_prompt force_color_prompt

# If this is an xterm set the title to user@host:dir
case "$TERM" in
xterm*|rxvt*)
    PS1="\[\e]0;${debian_chroot:+($debian_chroot)}\u@\h: \w\a\]$PS1"
    ;;
*)
    ;;
esac

# enable color support of ls and also add handy aliases
if [ -x /usr/bin/dircolors ]; then
    test -r ~/.dircolors && eval "$(dircolors -b ~/.dircolors)" || eval "$(dircolors -b)"
    alias ls='ls --color=auto'
    #alias dir='dir --color=auto'
    #alias vdir='vdir --color=auto'

    alias grep='grep --color=auto'
    alias fgrep='fgrep --color=auto'
    alias egrep='egrep --color=auto'
fi

# some more ls aliases
alias ll='ls -alF'
alias la='ls -A'
alias l='ls -CF'

# Add an "alert" alias for long running commands.  Use like so:
#   sleep 10; alert
alias alert='notify-send --urgency=low -i "$([ $? = 0 ] && echo terminal || echo error)" "$(history|tail -n1|sed -e '\''s/^\s*[0-9]\+\s*//;s/[;&|]\s*alert$//'\'')"'

# Alias definitions.
# You may want to put all your additions into a separate file like
# ~/.bash_aliases, instead of adding them here directly.
# See /usr/share/doc/bash-doc/examples in the bash-doc package.

if [ -f ~/.bash_aliases ]; then
    . ~/.bash_aliases
fi

# enable programmable completion features (you don't need to enable
# this, if it's already enabled in /etc/bash.bashrc and /etc/profile
# sources /etc/bash.bashrc).
if ! shopt -oq posix; then
  if [ -f /usr/share/bash-completion/bash_completion ]; then
    . /usr/share/bash-completion/bash_completion
  elif [ -f /etc/bash_completion ]; then
    . /etc/bash_completion
  fi
fi

#########################  my configurations  ##########################

########  some common configurations  ########

export PS1='\[\033[01;32m\]\u@\h\[\033[01;34m\] \w\[\033[01;33m\]$(__git_ps1)\[\033[01;32m\] \$ :-)\[\033[01;00m\] '
export GIT_PS1_SHOWDIRTYSTATE=1
alias q='exit'
export CLICOLOR=1

# some alias 
alias tmux-attach='tmux attach-session -t'
alias tmux-ls='tmux ls'

# avoid rm -rf
mkdir -p ~/.trash
alias rm=trash
alias r=trash
alias rl='ls ~/.trash'
alias ur=undelfile

undelfile() {
  mv -i ~/.trash/$@ ./
}

trash() {
  mv $@ ~/.trash/
}

cleartrash() {
  read -p "clear sure?[n]" confirm
  [ $confirm == 'y' ] || [ $confirm == 'Y' ] && /bin/rm -rf ~/.trash/*
}

########  programming languages toolchains related  ########

ME=~/me
SOFTWARE_PATH=$ME/software
SOURCECODE_PATH=$ME/sourcecode
DEVTOOLS_PATH=$ME/devtools
MYCODE_PATH=$ME/mycode
WORKSPACE_PATH=$ME/workspace

# python cmdline  autocomplete
PYTHONSTARTUP='/home/tan/.pythonstartup'
export PYTHONSTARTUP

# java path
export JAVA_HOME=$SOFTWARE_PATH/jdk1.8.0_60
export JAVA_BIN=$JAVA_HOME/bin
export CLASSPATH=.:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar:$SOFTWARE_PATH/tla/tlatools
export PATH=$PATH:$JAVA_HOME/bin
# maven path 
export PATH=$PATH:$SOFTWARE_PATH/apache-maven-3.3.3/bin
# ant 
export PATH=$PATH:$SOFTWARE_PATH/apache-ant-1.9.7/bin
# scala, sbt and activator path 
export PATH=$PATH:$SOFTWARE_PATH/scala-2.10.4/bin
export PATH=$PATH:$SOFTWARE_PATH/sbt/bin

# gopath 
export GOROOT=$SOFTWARE_PATH/go
export GOPATH=$MYCODE_PATH/go
export GOBIN=$GOPATH/bin
export PATH=$GOROOT/bin:$GOBIN:$PATH

# scheme path 
#export PATH=$PATH:$SOFTWARE_PATH/scheme/bin
#export MITSCHEME_LIBRARY_PATH=$SOFTWARE_PATH/scheme/lib/mit-scheme-x86-64

# cmake 3.5
export PATH=$PATH:$SOFTWARE_PATH/cmake-3.5.2-Linux-x86_64/bin

# tracing and performance tools
export PATH=$PATH:$SOFTWARE_PATH/tracing_tools/systemtap/bin
export PATH=$PATH:$SOFTWARE_PATH/tracing_tools/perf-tools/bin
export PATH=$PATH:$SOFTWARE_PATH/tracing_tools/FlameGraph

######## some applications related ########

# spark path
export PATH=$PATH:$SOFTWARE_PATH/spark-1.3.1/bin
export SPARK_HOME=$SOFTWARE_PATH/spark-1.3.1

# hadoop path 
export HADOOP_HOME=$SOFTWARE_PATH/hadoop-2.6.0
export HADOOP_CLASSPATH=$JAVA_HOME/lib/tools.jar
export PATH=$PATH:$SOFTWARE_PATH/hadoop-2.6.0/bin

# tomcat 
export PATH=$PATH:$SOFTWARE_PATH/apache-tomcat-9.0.0/bin

