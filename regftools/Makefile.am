AM_CPPFLAGS = \
	-I../include -I$(top_srcdir)/include \
	-I../common -I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFWNT_CPPFLAGS@ \
	@LIBFUSE_CPPFLAGS@ \
	@LIBREGF_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	regfexport \
	regfinfo \
	regfmount

regfexport_SOURCES = \
	export_handle.c export_handle.h \
	log_handle.c log_handle.h \
	regfexport.c \
	regftools_getopt.c regftools_getopt.h \
	regftools_i18n.h \
	regftools_libbfio.h \
	regftools_libcerror.h \
	regftools_libclocale.h \
	regftools_libcnotify.h \
	regftools_libfdata.h \
	regftools_libfdatetime.h \
	regftools_libfwnt.h \
	regftools_libregf.h \
	regftools_libuna.h \
	regftools_output.c regftools_output.h \
	regftools_signal.c regftools_signal.h \
	regftools_unused.h

regfexport_LDADD = \
	@LIBFDATETIME_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

regfinfo_SOURCES = \
	info_handle.c info_handle.h \
	regfinfo.c \
	regftools_getopt.c regftools_getopt.h \
	regftools_i18n.h \
	regftools_libbfio.h \
	regftools_libcerror.h \
	regftools_libclocale.h \
	regftools_libcnotify.h \
	regftools_libfdata.h \
	regftools_libfdatetime.h \
	regftools_libfwnt.h \
	regftools_libregf.h \
	regftools_libuna.h \
	regftools_output.c regftools_output.h \
	regftools_signal.c regftools_signal.h \
	regftools_unused.h

regfinfo_LDADD = \
	@LIBFDATETIME_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

regfmount_SOURCES = \
	mount_dokan.c mount_dokan.h \
	mount_file_entry.c mount_file_entry.h \
	mount_file_system.c mount_file_system.h \
	mount_fuse.c mount_fuse.h \
	mount_handle.c mount_handle.h \
	mount_path_string.c mount_path_string.h \
	regfmount.c \
	regftools_getopt.c regftools_getopt.h \
	regftools_i18n.h \
	regftools_libbfio.h \
	regftools_libcerror.h \
	regftools_libclocale.h \
	regftools_libcnotify.h \
	regftools_libcpath.h \
	regftools_libfdata.h \
	regftools_libfdatetime.h \
	regftools_libfwnt.h \
	regftools_libregf.h \
	regftools_libuna.h \
	regftools_output.c regftools_output.h \
	regftools_signal.c regftools_signal.h \
	regftools_unused.h

regfmount_LDADD = \
	@LIBFUSE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

DISTCLEANFILES = \
	Makefile \
	Makefile.in

splint-local:
	@echo "Running splint on regfexport ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(regfexport_SOURCES)
	@echo "Running splint on regfinfo ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(regfinfo_SOURCES)
	@echo "Running splint on regfmount ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(regfmount_SOURCES)

