" When started as "evim", evim.vim will already have done these settings.
if v:progname =~? "evim"
  finish
endif

" Use Vim settings, rather than Vi settings (much better!).
" This must be first, because it changes other options as a side effect.
set nocompatible

"===========================Vundle=====================

filetype off
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()

"get from url
"Plugin 'git://git.wincent.com/command-t.git'

"get from local machine
" Plugin 'file:///home/gmarik/path/to/plugin'

" The sparkup vim script is in a subdirectory of this repo called vim.
" Pass the path to set the runtimepath properly.
" Plugin 'rstacruz/sparkup', {'rtp': 'vim/'}
" Avoid a name conflict with L9
" Plugin 'user/L9', {'name': 'newL9'}

"my plugins
Plugin 'tpope/vim-fugitive'  "get from github repo
Plugin 'L9'                  "get from vim-scripts.org
Plugin 'gmarik/Vundle.vim'
Plugin 'bling/vim-airline'
Plugin 'fatih/vim-go'
Plugin 'taglist.vim'
Plugin 'derekwyatt/vim-scala'
Plugin 'scrooloose/nerdtree'
Plugin 'Valloric/YouCompleteMe'
Plugin 'ervandew/supertab'
Plugin 'vim-scripts/winmanager'
Plugin 'vim-scripts/AutoComplPop'
Plugin 'honza/vim-snippets'
Plugin 'SirVer/ultisnips'

call vundle#end()
filetype plugin indent on

" Brief help
" :PluginList       - lists configured plugins
" :PluginInstall    - installs plugins; append `!` to update or just
" :PluginUpdate
" :PluginSearch foo - searches for foo; append `!` to refresh local cache
" :PluginClean      - confirms removal of unused plugins; append `!` to
" auto-approve removal
" see :h vundle for more details or wiki for FAQ
" Put your non-Plugin stuff after this line
"======================================================


" allow backspacing over everything in insert mode
set backspace=indent,eol,start

if has("vms")
  set nobackup		" do not keep a backup file, use versions instead
else
  set nobackup		" keep a backup file
endif

set history=50		" keep 50 lines of command line history
set ruler		" show the cursor position all the time
set showcmd		" display incomplete commands
set incsearch		" do incremental searching


" Don't use Ex mode, use Q for formatting
map Q gq

" CTRL-U in insert mode deletes a lot.  Use CTRL-G u to first break undo,
" so that you can undo CTRL-U after inserting a line break.
inoremap <C-U> <C-G>u<C-U>

" In many terminal emulators the mouse works just fine, thus enable it.
if has('mouse')
  set mouse=a
endif

" Switch syntax highlighting on, when the terminal has colors
" Also switch on highlighting the last used search pattern.
if &t_Co > 2 || has("gui_running")
  syntax on
  set hlsearch
endif

" Only do this part when compiled with support for autocommands.
if has("autocmd")
  " Enable file type detection.
  " Use the default filetype settings, so that mail gets 'tw' set to 72,
  " 'cindent' is on in C files, etc.
  " Also load indent files, to automatically do language-dependent indenting.
  filetype plugin indent on

  " Put these in an autocmd group, so that we can delete them easily.
  augroup vimrcEx
  au!

  " For all text files set 'textwidth' to 78 characters.
  autocmd FileType text setlocal textwidth=78

  " When editing a file, always jump to the last known cursor position.
  " Don't do it when the position is invalid or when inside an event handler
  " (happens when dropping a file on gvim).
  " Also don't do it when the mark is in the first line, that is the default
  " position when opening a file.
  autocmd BufReadPost *
    \ if line("'\"") > 1 && line("'\"") <= line("$") |
    \   exe "normal! g`\"" |
    \ endif

  augroup END

else

  set autoindent		" always set autoindenting on

endif " has("autocmd")


" Convenient command to see the difference between the current buffer and the
" file it was loaded from, thus the changes you made.
" Only define it when not defined already.
if !exists(":DiffOrig")
  command DiffOrig vert new | set bt=nofile | r ++edit # | 0d_ | diffthis
		  \ | wincmd p | diffthis
endif





"========= MY SERRINGS ======

"line number
set nu

"theme
color evening
color desert

"tab settings
set tabstop=4
set shiftwidth=4
set expandtab

"column color
set colorcolumn=80,100,120

"ctags settings
set tags=tags
set tags+=./tags;/

"winmanager
let g:winManagerWindowLayout="TagList|FileExplorer"
let g:winManagerWidth=30
nmap wm :WMToogle<cr> 

"status line
set laststatus=2
let g:airline_section_b = '%{fugitive#statusline()}'
let g:airline_section_c = '%{strftime("%c")}'
let g:airline#extensions#tabline#enabled = 1
let g:airline_theme="hybrid"

" for gvim encoding and gui settings
"encoding
"set encoding=utf-8
"set fileencoding=utf-8
"set termencoding=utf-8
"set fileencodings=ucs-bom,utf-8,chinese,cp936
"source $VIMRUNTIME/delmenu.vim
"source $VIMRUNTIME/menu.vim
"language messages zh_CN.utf-8
"
""guioptions
"set guioptions-=m
"set guioptions-=T
"set guioptions-=r
"set guioptions-=L
"set guifont=Consolas:h12

"youcompleteme
let g:ycm_global_ycm_extra_conf = "~/.vim/.ycm_extra_conf.py"
let g:ycm_key_list_select_completion=[]
let g:ycm_key_list_previous_completion=[]

"close preview window for completion
set completeopt=longest,menu

"supertab
let g:SuperTabDefaultCompletionType = 'context'

"Ultisnips
"let g:UltiSnipsExpandTrigger="<c-tab>"
"let g:UltiSnipsListSnippets="<c-s-tab>"
"let g:UltiSnipsSnippetsDir = '~/.vim/UltiSnips'
"let g:UltiSnipsSnippetDirectories = ['UltiSnips'] 

"eclim plugin 
"let g:EclimCompletionMethod = 'omnifunc'

inoremap qq <esc>

"set runtimepath-=~/.vim/bundle/YouCompleteMe
