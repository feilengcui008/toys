#!/bin/bash

CUR=$(cd `dirname $0`; pwd)

# install common libs
$CUR/install_common_utils.sh

# make dir structures
# ~/me is root dir
# ~/me/software contains all common softwares like go, jdk etc...
# ~/me/sourcecode contains interesting open source code repos
# ~/me/workspace is the main space for projects
DIRS="~/me ~/me/software ~/me/sourcecode ~/me/workspace ~/me/workspace/go ~/me/workspace/sourcecode"
for ele in $DIRS ; do
  mkdir -p $ele
done

# get dotfiles
cp $CUR/vimrc ~/.vimrc
ln -s ~/.vimrc ~/.config/nvim/init.vim
ln -s ~/.vim ~/.config/nvim
cp $CUR/tmux.conf ~/.tmux.conf
cp $CUR/gitignore_global ~/.gitignore_global
cp $CUR/gitconfig ~/.gitconfig
cp $CUR/proxychains.conf ~/.proxychains.conf
cp $CUR/bashrc ~/.bashrc
source ~/.bashrc

# install pip pkgs
PIP_PKGS="neovim ipython"
for ele in $PIP_PKGS ; do
  sudo pip install $ele
done

# install font
$CUR/install_gofont.sh
