astyle "*.c,*.cpp,*.h"\
	--recursive\
	--style=bsd\
	--convert-tabs\
	--indent=spaces=4\
	--attach-closing-while\
	--indent-switches\
	--indent-namespaces\
	--indent-continuation=4\
	--indent-preproc-block\
	--indent-preproc-define\
	--indent-preproc-cond\
	--indent-col1-comments\
	--pad-oper\
	--unpad-paren\
	--delete-empty-lines\
	--align-pointer=name\
	--align-reference=name\
	--pad-comma\
	--break-one-line-headers\
	--remove-braces

# --recursive						遍历，如果是指定文件不选带上这个
# --style=bsd\						花括号换行，不换行的都是邪教！
# --convert-tabs\
# --indent=spaces=4\
# --attach-closing-while\
# --indent-switches\
# --indent-namespaces\
# --indent-continuation=4\
# --indent-preproc-block\
# --indent-preproc-define\
# --indent-preproc-cond\
# --indent-col1-comments\
# --pad-oper\
# --unpad-paren\
# --delete-empty-lines\
# --align-pointer=nam\
# --align-reference=name\
# --pad-comma\
# --break-one-line-headers\
# --remove-braces 					删除if后为单行时括号
