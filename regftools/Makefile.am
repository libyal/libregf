AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
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
	@LIBFGUID_CPPFLAGS@ \
	@LIBFOLE_CPPFLAGS@ \
	@LIBFWNT_CPPFLAGS@ \
	@LIBFWPS_CPPFLAGS@ \
	@LIBFWSI_CPPFLAGS@ \
	@LIBCSYSTEM_CPPFLAGS@ \
	@LIBFUSE_CPPFLAGS@ \
	@LIBREGF_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	regfexport \
	regfinfo \
	regfmount \
	regfreport

regfexport_SOURCES = \
	export_handle.c export_handle.h \
	log_handle.c log_handle.h \
	regfexport.c \
	regfoutput.c regfoutput.h \
	regftools_libbfio.h \
	regftools_libcerror.h \
	regftools_libclocale.h \
	regftools_libcnotify.h \
	regftools_libcsystem.h \
	regftools_libfdata.h \
	regftools_libfdatetime.h \
	regftools_libfguid.h \
	regftools_libfwnt.h \
	regftools_libfwsi.h \
	regftools_libregf.h \
	regftools_libuna.h

regfexport_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libregf/libregf.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@

regfinfo_SOURCES = \
	info_handle.c info_handle.h \
	regfinfo.c \
	regfoutput.c regfoutput.h \
	regftools_libbfio.h \
	regftools_libcerror.h \
	regftools_libclocale.h \
	regftools_libcnotify.h \
	regftools_libcstring.h \
	regftools_libcsystem.h \
	regftools_libfdata.h \
	regftools_libfdatetime.h \
	regftools_libfguid.h \
	regftools_libfwnt.h \
	regftools_libfwsi.h \
	regftools_libregf.h \
	regftools_libuna.h

regfinfo_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libregf/libregf.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@

regfmount_SOURCES = \
	mount_handle.c mount_handle.h \
	regfmount.c \
	regfoutput.c regfoutput.h \
	regftools_libbfio.h \
	regftools_libcerror.h \
	regftools_libclocale.h \
	regftools_libcnotify.h \
	regftools_libcstring.h \
	regftools_libcsystem.h \
	regftools_libfdata.h \
	regftools_libfdatetime.h \
	regftools_libfguid.h \
	regftools_libfwnt.h \
	regftools_libfwsi.h \
	regftools_libregf.h \
	regftools_libuna.h

regfmount_LDADD = \
	@LIBFUSE_LIBADD@ \
	@LIBCSYSTEM_LIBADD@ \
	../libregf/libregf.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@

regfreport_SOURCES = \
	regfreport.c \
	regfoutput.c regfoutput.h \
	regftools_libbfio.h \
	regftools_libcerror.h \
	regftools_libclocale.h \
	regftools_libcnotify.h \
	regftools_libcstring.h \
	regftools_libcsystem.h \
	regftools_libfdata.h \
	regftools_libfdatetime.h \
	regftools_libfguid.h \
	regftools_libfwnt.h \
	regftools_libfwsi.h \
	regftools_libregf.h \
	regftools_libuna.h \
	registry_file.c registry_file.h \
	report_handle.c report_handle.h \
	shell_items.c shell_items.h

regfreport_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	@LIBFWSI_LIBADD@ \
	@LIBFWPS_LIBADD@ \
	@LIBFOLE_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libregf/libregf.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on regfexport ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(regfexport_SOURCES)
	@echo "Running splint on regfinfo ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(regfinfo_SOURCES)
	@echo "Running splint on regfmount ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(regfmount_SOURCES)
	@echo "Running splint on regfreport ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(regfreport_SOURCES)

