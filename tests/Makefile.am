AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
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
	test_pyregf_set_ascii_codepage.sh \
	test_pyregf_open_close.sh
endif

TESTS = \
	test_set_ascii_codepage.sh \
	test_open_close.sh \
	test_regfinfo.sh \
	test_regfexport.sh \
	$(TESTS_PYREGF)

check_SCRIPTS = \
	pyregf_test_open_close.py \
	pyregf_test_set_ascii_codepage.py \
	test_open_close.sh \
	test_pyregf_open_close.sh \
	test_pyregf_set_ascii_codepage.sh \
	test_regfexport.sh \
	test_regfinfo.sh \
	test_runner.sh \
	test_set_ascii_codepage.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	regf_test_open_close \
	regf_test_set_ascii_codepage

regf_test_open_close_SOURCES = \
	regf_test_libcerror.h \
	regf_test_libregf.h \
	regf_test_libcstring.h \
	regf_test_libcsystem.h \
	regf_test_open_close.c

regf_test_open_close_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libregf/libregf.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

regf_test_set_ascii_codepage_SOURCES = \
	regf_test_libcerror.h \
	regf_test_libclocale.h \
	regf_test_libregf.h \
	regf_test_libcstring.h \
	regf_test_set_ascii_codepage.c

regf_test_set_ascii_codepage_LDADD = \
	../libregf/libregf.la \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

