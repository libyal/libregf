#!/bin/sh
# Script to run tests
#
# Version: 20260609

if test -f ${PWD}/libregf/.libs/libregf.1.dylib && test -f ./pyregf/.libs/pyregf.so
then
	install_name_tool -change /usr/local/lib/libregf.1.dylib ${PWD}/libregf/.libs/libregf.1.dylib ./pyregf/.libs/pyregf.so
fi

make check-build > /dev/null

make check $@
RESULT=$?

if test ${RESULT} -ne 0
then
	find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

