/*
 * Exports information from a Windows NT Registry File (REGF)
 *
 * Copyright (C) 2009-2021, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>
#include <system_string.h>
#include <types.h>

#include <stdio.h>

#if defined( HAVE_IO_H ) || defined( WINAPI )
#include <io.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#include "export_handle.h"
#include "regftools_getopt.h"
#include "regftools_libcerror.h"
#include "regftools_libclocale.h"
#include "regftools_libcnotify.h"
#include "regftools_libregf.h"
#include "regftools_output.h"
#include "regftools_signal.h"
#include "regftools_unused.h"

export_handle_t *regfexport_export_handle = NULL;
int regfexport_abort                      = 0;

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use regfexport to export information from a Windows NT\n"
	                 "Registry File (REGF).\n\n" );

	fprintf( stream, "Usage: regfexport [ -c codepage ] [ -K key_path ] [ -l logfile ]\n"
	                 "                  [ -hvV ] source\n\n" );

	fprintf( stream, "\tsource: the source file\n\n" );

	fprintf( stream, "\t-c:     codepage of ASCII strings, options: ascii, windows-874,\n"
	                 "\t        windows-932, windows-936, windows-949, windows-950,\n"
	                 "\t        windows-1250, windows-1251, windows-1252 (default),\n"
	                 "\t        windows-1253, windows-1254, windows-1255, windows-1256\n"
	                 "\t        windows-1257 or windows-1258\n" );
	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-K:     show information about a specific key path.\n" );
	fprintf( stream, "\t-l:     logs information about the exported items\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
}

/* Signal handler for regfexport
 */
void regfexport_signal_handler(
      regftools_signal_t signal REGFTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "regfexport_signal_handler";

	REGFTOOLS_UNREFERENCED_PARAMETER( signal )

	regfexport_abort = 1;

	if( regfexport_export_handle != NULL )
	{
		if( export_handle_signal_abort(
		     regfexport_export_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal export handle to abort.\n",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	if( _close(
	     0 ) != 0 )
#else
	if( close(
	     0 ) != 0 )
#endif
	{
		libcnotify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcerror_error_t *error                  = NULL;
	log_handle_t *log_handle                  = NULL;
	system_character_t *option_ascii_codepage = NULL;
	system_character_t *key_path              = NULL;
	system_character_t *log_filename          = NULL;
	system_character_t *source                = NULL;
	char *program                             = "regfexport";
	size_t source_length                      = 0;
	system_integer_t option                   = 0;
	int result                                = 0;
	int verbose                               = 0;

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
	if( regftools_output_initialize(
	     _IONBF,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize output settings.\n" );

		goto on_error;
	}
	regftools_output_version_fprint(
	 stdout,
	 program );

	while( ( option = regftools_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "c:hK:l:vV" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM "\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (system_integer_t) 'c':
				option_ascii_codepage = optarg;

				break;

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'K':
				key_path = optarg;

				break;

			case (system_integer_t) 'l':
				log_filename = optarg;

				break;

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				regftools_output_copyright_fprint(
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

	if( log_handle_initialize(
	     &log_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize log handle.\n" );

		goto on_error;
	}
	if( export_handle_initialize(
	     &regfexport_export_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize export handle.\n" );

		goto on_error;
	}
	if( log_handle_open(
	     log_handle,
	     log_filename,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open log file: %" PRIs_SYSTEM ".\n",
		 log_filename );

		goto on_error;
	}
	if( option_ascii_codepage != NULL )
	{
		result = export_handle_set_ascii_codepage(
		          regfexport_export_handle,
		          option_ascii_codepage,
		          &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set ASCII codepage in export handle.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported ASCII codepage defaulting to: windows-1252.\n" );
		}
	}
	if( export_handle_open_input(
	     regfexport_export_handle,
	     source,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open: %" PRIs_SYSTEM ".\n",
		 source );

		goto on_error;
	}
	if( key_path != NULL )
	{
		if( export_handle_export_key_path(
		     regfexport_export_handle,
		     key_path,
		     log_handle,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to export key path.\n" );

			goto on_error;
		}
	}
	else
	{
		if( export_handle_export_file(
		     regfexport_export_handle,
		     log_handle,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to export file.\n" );

			goto on_error;
		}
	}
	if( export_handle_close_input(
	     regfexport_export_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close export handle.\n" );

		goto on_error;
	}
	if( export_handle_free(
	     &regfexport_export_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free export handle.\n" );

		goto on_error;
	}
	if( log_handle_close(
	     log_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close log handle.\n" );

		goto on_error;
	}
	if( log_handle_free(
	     &log_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free log handle.\n" );

		goto on_error;
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( regfexport_export_handle != NULL )
	{
		export_handle_free(
		 &regfexport_export_handle,
		 NULL );
	}
	if( log_handle != NULL )
	{
		log_handle_free(
		 &log_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

