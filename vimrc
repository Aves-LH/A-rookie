scriptencoding utf-8

set fileencodings=ucs-bom,utf-8,utf-16,gbk,big5,gb18030,latin1,IOS-8859

set fileencoding=utf-8

syntax on

" 语法高亮


set autoindent

" 自动对齐


set confirm

" 在处理未保存或只读文件的时候，弹出确认


set tabstop=4

" Tab键的宽度

set softtabstop=4

set shiftwidth=4

"  统一缩进为4


set noexpandtab

" 不要用空格代替制表符


set number

set backspace=indent,eol,start

" 显示行号
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""新文件标题
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"新建.c,.h,.sh,.java文件，自动插入文件头 
autocmd BufNewFile *.cpp,*.[ch],*.sh,*.rb,*.java,*.py exec ":call SetTitle()" 
""定义函数SetTitle，自动插入文件头 
func SetTitle() 
	"如果文件类型为.sh文件 
	if &filetype == 'sh' 
		call setline(1,"\#!/bin/bash") 
		call append(line("."), "") 
    elseif &filetype == 'python'
        call setline(1,"#!/usr/bin/env python")
        call append(line("."),"# coding=utf-8")
	    call append(line(".")+1, "") 

    elseif &filetype == 'ruby'
        call setline(1,"#!/usr/bin/env ruby")
        call append(line("."),"# encoding: utf-8")
	    call append(line(".")+1, "")

"    elseif &filetype == 'mkd'
"        call setline(1,"<head><meta charset=\"UTF-8\"></head>")
	else 
		call setline(1, "/*************************************************************************") 
		call append(line("."), "	> File Name: ".expand("%")) 
		call append(line(".")+1, "	> Author: lihuan") 
		call append(line(".")+2, "	> Mail: ") 
		call append(line(".")+3, "	> Created Time: ".strftime("%c")) 
		call append(line(".")+4, " ************************************************************************/") 
		call append(line(".")+5, "")
	endif
	if expand("%:e") == 'cpp'
		call append(line(".")+6, "#include<iostream>")
		call append(line(".")+7, "using namespace std;")
		call append(line(".")+8, "")
	endif
	if &filetype == 'c'
		call append(line(".")+6, "#include <stdio.h>")
		call append(line(".")+7, "#include <libavcodec/avcodec.h>")
		call append(line(".")+8, "#include <libavformat/avformat.h>")
		call append(line(".")+9, "#include <libavfilter/avfilter.h>")
		call append(line(".")+10, "#include <libavutil/pixfmt.h>")
		call append(line(".")+11, "#include <libswscale/swscale.h>")
		call append(line(".")+12, "")
		call append(line(".")+13, "")
		call append(line(".")+14, "int main(int argc, char **argv)")
		call append(line(".")+15, "{")
		call append(line(".")+16, "")
		call append(line(".")+17, "    return 0;")
		call append(line(".")+18, "}")
	endif
	if expand("%:e") == 'h'
		call append(line(".")+6, "#ifndef __".toupper(expand("%:r"))."_H__")
		call append(line(".")+7, "#define __".toupper(expand("%:r"))."_H__")
		call append(line(".")+8, "#endif")
	endif
	if &filetype == 'java'
		call append(line(".")+6,"public class ".expand("%:r"))
		call append(line(".")+7,"")
	endif
	"新建文件后，自动定位到文件末尾
endfunc 
autocmd BufNewFile * normal G

set smartindent

set tabstop=4

set shiftwidth=4

"set expandtab

inoremap ( ()<LEFT>
inoremap [ []<LEFT>
inoremap { {}<LEFT>
inoremap {<CR> {<CR>}<ESC>O
inoremap " ""<LEFT>
inoremap ' ''<LEFT>
"inoremap < <><LEFT>

function! RemovePairs()
    let s:line = getline(".")
    let s:previous_char = s:line[col(".")-1]

    if index(["(","[","{"],s:previous_char) != -1
        let l:original_pos = getpos(".")
        execute "normal %"
        let l:new_pos = getpos(".")
        " only right (
        if l:original_pos == l:new_pos
            execute "normal! a\<BS>"
            return
        end

        let l:line2 = getline(".")
        if len(l:line2) == col(".")
            execute "normal! v%xa"
        else
            execute "normal! v%xi"
        end
    else
        execute "normal! a\<BS>"
    end
endfunction

function! RemoveNextDoubleChar(char)
    let l:line = getline(".")
    let l:next_char = l:line[col(".")]

    if a:char == l:next_char
        execute "normal! l"
    else
        execute "normal! i" . a:char . ""
    end
endfunction

inoremap <BS> <ESC>:call RemovePairs()<CR>a
inoremap ) <ESC>:call RemoveNextDoubleChar(')')<CR>a
inoremap ] <ESC>:call RemoveNextDoubleChar(']')<CR>a
inoremap } <ESC>:call RemoveNextDoubleChar('}')<CR>a
"inoremap > <ESC>:call RemoveNextDoubleChar('>')<CR>a
