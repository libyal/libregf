AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBCSYSTEM_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBREGF_DLL_IMPORT@

if HAVE_PYTHON
TESTS_PYREGF = \
	test_python_functions.sh \
	test_pyregf_set_ascii_codepage.sh
endif

TESTS = \
	test_api_functions.sh \
	test_regfinfo.sh \
	test_regfexport.sh \
	$(TESTS_PYREGF)

check_SCRIPTS = \
	pyregf_test_get_version.py \
	pyregf_test_open_close.py \
	pyregf_test_set_ascii_codepage.py \
	test_api_functions.sh \
	test_pyregf_set_ascii_codepage.sh \
	test_python_functions.sh \
	test_regfexport.sh \
	test_regfinfo.sh \
	test_runner.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	regf_test_error \
	regf_test_notify \
	regf_test_open_close \
	regf_test_support

regf_test_error_SOURCES = \
	regf_test_error.c \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_unused.h

regf_test_error_LDADD = \
	../libregf/libregf.la

regf_test_notify_SOURCES = \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_notify.c \
	regf_test_unused.h

regf_test_notify_LDADD = \
	../libregf/libregf.la

regf_test_open_close_SOURCES = \
	regf_test_libcerror.h \
	regf_test_libregf.h \
	regf_test_libcsystem.h \
	regf_test_open_close.c

regf_test_open_close_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libregf/libregf.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCERROR_LIBADD@

regf_test_support_SOURCES = \
	regf_test_support.c \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_unused.h

regf_test_support_LDADD = \
	../libregf/libregf.la

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

