/*
 * Reports about information stored in a Windows NT Registry File (REGF)
 *
 * Copyright (C) 2009-2015, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>
#include <types.h>

#include <stdio.h>

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "regfoutput.h"
#include "regftools_libcerror.h"
#include "regftools_libclocale.h"
#include "regftools_libcnotify.h"
#include "regftools_libcstring.h"
#include "regftools_libcsystem.h"
#include "regftools_libregf.h"
#include "report_handle.h"

report_handle_t *regfreport_report_handle = NULL;
int regfreport_abort                      = 0;

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use regfreport to report about information stored in a\n"
	                 "Windows NT Registry File (REGF).\n\n" );

	fprintf( stream, "Usage: regfreport [ -c codepage ] [ -hvV ] source\n\n" );

	fprintf( stream, "\tsource: the source file\n\n" );

	fprintf( stream, "\t-c:     codepage of ASCII strings, options: ascii, windows-874,\n"
	                 "\t        windows-932, windows-936, windows-949, windows-950,\n"
	                 "\t        windows-1250, windows-1251, windows-1252 (default),\n"
	                 "\t        windows-1253, windows-1254, windows-1255, windows-1256\n"
	                 "\t        windows-1257 or windows-1258\n" );
	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
}

/* Signal handler for regfreport
 */
void regfreport_signal_handler(
      libcsystem_signal_t signal LIBCSYSTEM_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function   = "regfreport_signal_handler";

	LIBCSYSTEM_UNREFERENCED_PARAMETER( signal )

	regfreport_abort = 1;

	if( regfreport_report_handle != NULL )
	{
		if( report_handle_signal_abort(
		     regfreport_report_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal report handle to abort.\n",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
	if( libcsystem_file_io_close(
	     0 ) != 0 )
	{
		libcnotify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcstring_system_character_t *option_ascii_codepage = NULL;
	libcstring_system_character_t *source                = NULL;
	libregf_error_t *error                               = NULL;
	char *program                                        = "regfreport";
	libcstring_system_integer_t option                   = 0;
	int result                                           = 0;
	int verbose                                          = 0;

	libcnotify_stream_set(
	 stderr,
	 NULL );
	libcnotify_verbose_set(
	 1 );

	if( libclocale_initialize(
	     "regftools",
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize locale values.\n" );

		goto on_error;
	}
        if( libcsystem_initialize(
             _IONBF,
             &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize system values.\n" );

		goto on_error;
	}
	regfoutput_version_fprint(
	 stdout,
	 program );

	while( ( option = libcsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBCSTRING_SYSTEM_STRING( "c:hvV" ) ) ) != (libcstring_system_integer_t) -1 )
	{
		switch( option )
		{
			case (libcstring_system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_LIBCSTRING_SYSTEM "\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (libcstring_system_integer_t) 'c':
				option_ascii_codepage = optarg;

				break;

			case (libcstring_system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libcstring_system_integer_t) 'v':
				verbose = 1;

				break;

			case (libcstring_system_integer_t) 'V':
				regfoutput_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source file.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	source = argv[ optind ];

	libcnotify_verbose_set(
	 verbose );
	libregf_notify_set_stream(
	 stderr,
	 NULL );
	libregf_notify_set_verbose(
	 verbose );

	if( report_handle_initialize(
	     &regfreport_report_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize report handle.\n" );

		goto on_error;
	}
	if( option_ascii_codepage != NULL )
	{
		result = report_handle_set_ascii_codepage(
		          regfreport_report_handle,
		          option_ascii_codepage,
		          &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set ASCII codepage in report handle.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported ASCII codepage defaulting to: windows-1252.\n" );
		}
	}
	if( report_handle_open_input(
	     regfreport_report_handle,
	     source,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Error opening file: %" PRIs_LIBCSTRING_SYSTEM ".\n",
		 argv[ optind ] );

		goto on_error;
	}
	if( report_handle_analyze_file(
	     regfreport_report_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to report about file.\n" );

		goto on_error;
	}
	if( report_handle_close_input(
	     regfreport_report_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close file.\n" );

		goto on_error;
	}
	if( report_handle_free(
	     &regfreport_report_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free report handle.\n" );

		goto on_error;
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libregf_error_free(
		 &error );
	}
	if( regfreport_report_handle != NULL )
	{
		report_handle_free(
		 &regfreport_report_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

