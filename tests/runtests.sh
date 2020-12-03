#!/bin/sh
# Script to run tests
#
# Version: 20201121

if test -f ${PWD}/libregf/.libs/libregf.1.dylib && test -f ./pyregf/.libs/pyregf.so;
then
	install_name_tool -change /usr/local/lib/libregf.1.dylib ${PWD}/libregf/.libs/libregf.1.dylib ./pyregf/.libs/pyregf.so;
fi

make check CHECK_WITH_STDERR=1;
RESULT=$?;

if test ${RESULT} -ne 0 && test -f tests/test-suite.log;
then
	cat tests/test-suite.log;
fi
exit ${RESULT};

