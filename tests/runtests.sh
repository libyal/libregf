#!/bin/sh
# Script to run tests
#
# Version: 20260714

if [ -f "${PWD}/libregf/.libs/libregf.1.dylib" ] && [ -f ./pyregf/.libs/pyregf.so ]
then
    install_name_tool -change /usr/local/lib/libregf.1.dylib "${PWD}/libregf/.libs/libregf.1.dylib" ./pyregf/.libs/pyregf.so
fi

make check-build > /dev/null

# shellcheck disable=SC2068
make check $@
RESULT=$?

if [ ${RESULT} -ne 0 ]
then
    find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

