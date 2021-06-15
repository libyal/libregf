AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFWNT_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBREGF_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYREGF = \
	test_python_module.sh
endif

TESTS = \
	test_library.sh \
	test_tools.sh \
	test_regfinfo.sh \
	test_regfexport.sh \
	$(TESTS_PYREGF)

check_SCRIPTS = \
	pyregf_test_file.py \
	pyregf_test_support.py \
	test_library.sh \
	test_python_module.sh \
	test_regfexport.sh \
	test_regfinfo.sh \
	test_runner.sh \
	test_tools.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	regf_test_checksum \
	regf_test_data_block_key \
	regf_test_data_block_stream \
	regf_test_data_type \
	regf_test_dirty_vector \
	regf_test_error \
	regf_test_file \
	regf_test_file_header \
	regf_test_hive_bin \
	regf_test_hive_bin_cell \
	regf_test_hive_bin_header \
	regf_test_hive_bins_list \
	regf_test_io_handle \
	regf_test_key \
	regf_test_key_descriptor \
	regf_test_key_item \
	regf_test_multi_string \
	regf_test_named_key \
	regf_test_notify \
	regf_test_security_key \
	regf_test_sub_key_list \
	regf_test_support \
	regf_test_tools_info_handle \
	regf_test_tools_output \
	regf_test_tools_signal \
	regf_test_value \
	regf_test_value_item \
	regf_test_value_key

regf_test_checksum_SOURCES = \
	regf_test_checksum.c \
	regf_test_libcerror.h \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_unused.h

regf_test_checksum_LDADD = \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@

regf_test_data_block_key_SOURCES = \
	regf_test_data_block_key.c \
	regf_test_libcerror.h \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_memory.c regf_test_memory.h \
	regf_test_unused.h

regf_test_data_block_key_LDADD = \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@

regf_test_data_block_stream_SOURCES = \
	regf_test_data_block_stream.c \
	regf_test_functions.c regf_test_functions.h \
	regf_test_libbfio.h \
	regf_test_libcerror.h \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_unused.h

regf_test_data_block_stream_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libregf/libregf.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

regf_test_data_type_SOURCES = \
	regf_test_data_type.c \
	regf_test_libcerror.h \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_memory.c regf_test_memory.h \
	regf_test_unused.h

regf_test_data_type_LDADD = \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@

regf_test_dirty_vector_SOURCES = \
	regf_test_dirty_vector.c \
	regf_test_libcerror.h \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_memory.c regf_test_memory.h \
	regf_test_unused.h

regf_test_dirty_vector_LDADD = \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@

regf_test_error_SOURCES = \
	regf_test_error.c \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_unused.h

regf_test_error_LDADD = \
	../libregf/libregf.la

regf_test_file_SOURCES = \
	regf_test_file.c \
	regf_test_functions.c regf_test_functions.h \
	regf_test_getopt.c regf_test_getopt.h \
	regf_test_libbfio.h \
	regf_test_libcerror.h \
	regf_test_libclocale.h \
	regf_test_libcnotify.h \
	regf_test_libregf.h \
	regf_test_libuna.h \
	regf_test_macros.h \
	regf_test_memory.c regf_test_memory.h \
	regf_test_unused.h

regf_test_file_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libregf/libregf.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

regf_test_file_header_SOURCES = \
	regf_test_file_header.c \
	regf_test_functions.c regf_test_functions.h \
	regf_test_libbfio.h \
	regf_test_libcerror.h \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_memory.c regf_test_memory.h \
	regf_test_unused.h

regf_test_file_header_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@

regf_test_hive_bin_SOURCES = \
	regf_test_functions.c regf_test_functions.h \
	regf_test_hive_bin.c \
	regf_test_libbfio.h \
	regf_test_libcerror.h \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_memory.c regf_test_memory.h \
	regf_test_unused.h

regf_test_hive_bin_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@

regf_test_hive_bin_cell_SOURCES = \
	regf_test_hive_bin_cell.c \
	regf_test_libcerror.h \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_memory.c regf_test_memory.h \
	regf_test_unused.h

regf_test_hive_bin_cell_LDADD = \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@

regf_test_hive_bin_header_SOURCES = \
	regf_test_functions.c regf_test_functions.h \
	regf_test_hive_bin_header.c \
	regf_test_libbfio.h \
	regf_test_libcerror.h \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_memory.c regf_test_memory.h \
	regf_test_unused.h

regf_test_hive_bin_header_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@

regf_test_hive_bins_list_SOURCES = \
	regf_test_hive_bins_list.c \
	regf_test_libcerror.h \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_memory.c regf_test_memory.h \
	regf_test_unused.h

regf_test_hive_bins_list_LDADD = \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@

regf_test_io_handle_SOURCES = \
	regf_test_io_handle.c \
	regf_test_libcerror.h \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_memory.c regf_test_memory.h \
	regf_test_unused.h

regf_test_io_handle_LDADD = \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@

regf_test_key_SOURCES = \
	regf_test_key.c \
	regf_test_libcerror.h \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_memory.c regf_test_memory.h \
	regf_test_unused.h

regf_test_key_LDADD = \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@

regf_test_key_descriptor_SOURCES = \
	regf_test_key_descriptor.c \
	regf_test_libcerror.h \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_memory.c regf_test_memory.h \
	regf_test_unused.h

regf_test_key_descriptor_LDADD = \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@

regf_test_key_item_SOURCES = \
	regf_test_functions.c regf_test_functions.h \
	regf_test_key_item.c \
	regf_test_libcerror.h \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_memory.c regf_test_memory.h \
	regf_test_unused.h

regf_test_key_item_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libregf/libregf.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

regf_test_multi_string_SOURCES = \
	regf_test_libcerror.h \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_memory.c regf_test_memory.h \
	regf_test_multi_string.c \
	regf_test_unused.h

regf_test_multi_string_LDADD = \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@

regf_test_named_key_SOURCES = \
	regf_test_libcerror.h \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_memory.c regf_test_memory.h \
	regf_test_named_key.c \
	regf_test_unused.h

regf_test_named_key_LDADD = \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@

regf_test_notify_SOURCES = \
	regf_test_libcerror.h \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_notify.c \
	regf_test_unused.h

regf_test_notify_LDADD = \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@

regf_test_security_key_SOURCES = \
	regf_test_libcerror.h \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_memory.c regf_test_memory.h \
	regf_test_security_key.c \
	regf_test_unused.h

regf_test_security_key_LDADD = \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@

regf_test_sub_key_list_SOURCES = \
	regf_test_libcerror.h \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_memory.c regf_test_memory.h \
	regf_test_sub_key_list.c \
	regf_test_unused.h

regf_test_sub_key_list_LDADD = \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@

regf_test_support_SOURCES = \
	regf_test_functions.c regf_test_functions.h \
	regf_test_getopt.c regf_test_getopt.h \
	regf_test_libbfio.h \
	regf_test_libcerror.h \
	regf_test_libclocale.h \
	regf_test_libcnotify.h \
	regf_test_libregf.h \
	regf_test_libuna.h \
	regf_test_macros.h \
	regf_test_memory.c regf_test_memory.h \
	regf_test_support.c \
	regf_test_unused.h

regf_test_support_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@

regf_test_tools_info_handle_SOURCES = \
	../regftools/info_handle.c ../regftools/info_handle.h \
	regf_test_libcerror.h \
	regf_test_macros.h \
	regf_test_memory.c regf_test_memory.h \
	regf_test_tools_info_handle.c \
	regf_test_unused.h

regf_test_tools_info_handle_LDADD = \
	@LIBFDATETIME_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@

regf_test_tools_output_SOURCES = \
	../regftools/regftools_output.c ../regftools/regftools_output.h \
	regf_test_libcerror.h \
	regf_test_macros.h \
	regf_test_tools_output.c \
	regf_test_unused.h

regf_test_tools_output_LDADD = \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@

regf_test_tools_signal_SOURCES = \
	../regftools/regftools_signal.c ../regftools/regftools_signal.h \
	regf_test_libcerror.h \
	regf_test_macros.h \
	regf_test_tools_signal.c \
	regf_test_unused.h

regf_test_tools_signal_LDADD = \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@

regf_test_value_SOURCES = \
	regf_test_libcerror.h \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_memory.c regf_test_memory.h \
	regf_test_unused.h \
	regf_test_value.c

regf_test_value_LDADD = \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@

regf_test_value_item_SOURCES = \
	regf_test_functions.c regf_test_functions.h \
	regf_test_libbfio.h \
	regf_test_libcerror.h \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_memory.c regf_test_memory.h \
	regf_test_unused.h \
	regf_test_value_item.c

regf_test_value_item_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@

regf_test_value_key_SOURCES = \
	regf_test_libcerror.h \
	regf_test_libregf.h \
	regf_test_macros.h \
	regf_test_memory.c regf_test_memory.h \
	regf_test_unused.h \
	regf_test_value_key.c

regf_test_value_key_LDADD = \
	../libregf/libregf.la \
	@LIBCERROR_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

