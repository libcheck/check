AC_DEFUN(CK_LYX_LINUXDOC,
[AC_MSG_CHECKING([for lyx with linuxdoc])
ck_lyxchkconfig="chkconfig.ltx"

ck_lyxenvdir="$LYX_DIR_11x"

ck_lyxpath=`which lyx | xargs dirname 2>/dev/null`

ck_lyxharddir='/usr/local/share/lyx'
ck_lyxldir="$ck_lyxpath/../lib"
ck_lyxsdir="$ck_lyxpath/../share/lyx"
for ck_path in "$ck_lyxenvdir" "$ck_lyxldir" "$ck_lyxsdir" "$ck_lyxharddir"; do
  	if test -n "$ck_path" -a -e "$ck_path/$ck_lyxchkconfig" ; then
		ck_lyxlibdir="$ck_path"
		break
    	fi
done

if grep "^\"linuxdoc" "$ck_lyxlibdir/textclass.lst" &> /dev/null; then
    	AC_MSG_RESULT([yes])
    	[$1]
else
    	AC_MSG_RESULT([no])
    	[$2]
fi])

