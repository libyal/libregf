/*
 * Checksum functions
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
#include <byte_stream.h>
#include <types.h>

#include "libregf_checksum.h"
#include "libregf_libcerror.h"

/* The largest primary (or scalar) available
 * supported by a single load and store instruction
 */
typedef unsigned long int libregf_aligned_t;

/* Calculates the little-endian XOR-32 of a buffer
 * It uses the initial value to calculate a new XOR-32
 * Returns 1 if successful or -1 on error
 */
int libregf_checksum_calculate_little_endian_xor32(
     uint32_t *checksum_value,
     const uint8_t *buffer,
     size_t size,
     uint32_t initial_value,
     libcerror_error_t **error )
{
	libregf_aligned_t *aligned_buffer_iterator = NULL;
	uint8_t *buffer_iterator                   = NULL;
	static char *function                      = "libregf_checksum_calculate_little_endian_xor32";
	libregf_aligned_t value_aligned            = 0;
	uint32_t big_endian_value_32bit            = 0;
	uint32_t safe_checksum_value               = 0;
	uint32_t value_32bit                       = 0;
	uint8_t alignment_count                    = 0;
	uint8_t alignment_size                     = 0;
	uint8_t byte_count                         = 0;
	uint8_t byte_order                         = 0;
	uint8_t byte_size                          = 0;

	if( checksum_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid checksum value.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	safe_checksum_value = initial_value;

	buffer_iterator = (uint8_t *) buffer;

	/* Only optimize when there is the alignment is a multitude of 32-bit
	 * and for buffers larger than the alignment
	 */
	if( ( ( sizeof( libregf_aligned_t ) % 4 ) == 0 )
	 && ( size > ( 2 * sizeof( libregf_aligned_t ) ) ) )
	{
		/* Align the buffer iterator
		 */
		alignment_size = (uint8_t) ( (intptr_t) buffer_iterator % sizeof( libregf_aligned_t ) );

		if( alignment_size > 0 )
		{
			byte_size = sizeof( libregf_aligned_t ) - alignment_size;

			/* Align the buffer iterator in 4-byte steps
			 */
			while( byte_size != 0 )
			{
				value_32bit = 0;
				byte_count  = 1;

				if( byte_size >= 4 )
				{
					value_32bit |= buffer_iterator[ 3 ];
					value_32bit <<= 8;

					byte_count++;
				}
				if( byte_size >= 3 )
				{
					value_32bit |= buffer_iterator[ 2 ];
					value_32bit <<= 8;

					byte_count++;
				}
				if( byte_size >= 2 )
				{
					value_32bit |= buffer_iterator[ 1 ];
					value_32bit <<= 8;

					byte_count++;
				}
				value_32bit |= buffer_iterator[ 0 ];

				buffer_iterator += byte_count;
				byte_size       -= byte_count;

				safe_checksum_value ^= value_32bit;
			}
			size -= byte_count;
		}
		aligned_buffer_iterator = (libregf_aligned_t *) buffer_iterator;

		if( *buffer_iterator != (uint8_t) ( *aligned_buffer_iterator & 0xff ) )
		{
			byte_order = _BYTE_STREAM_ENDIAN_BIG;
		}
		else
		{
			byte_order = _BYTE_STREAM_ENDIAN_LITTLE;
		}
		/* Calculate the XOR value using the aligned buffer iterator
		 */
		while( size > sizeof( libregf_aligned_t ) )
		{
			value_aligned ^= *aligned_buffer_iterator;

			aligned_buffer_iterator++;

			size -= sizeof( libregf_aligned_t );
		}
		/* Align the aligned XOR value with the 32-bit XOR value
		 */
		if( alignment_size > 0 )
		{
			byte_count      = ( alignment_size % 4 ) * 8;
			alignment_count = ( sizeof( libregf_aligned_t ) - alignment_size ) * 8;

			if( byte_order == _BYTE_STREAM_ENDIAN_BIG )
			{
				/* Shift twice to set unused bytes to 0
				 */
				big_endian_value_32bit = (uint32_t) ( ( value_aligned >> alignment_count ) << byte_count );

				/* Change big-endian into little-endian
				 */
				value_32bit = ( ( big_endian_value_32bit & 0x000000ffUL ) << 24 )
				            | ( ( big_endian_value_32bit & 0x0000ff00UL ) << 8 )
				            | ( ( big_endian_value_32bit >> 8 ) & 0x0000ff00UL )
				            | ( ( big_endian_value_32bit >> 24 ) & 0x000000ffUL );

				/* Strip-off the used part of the aligned value
				 */
				value_aligned <<= alignment_count;
			}
			else if( byte_order == _BYTE_STREAM_ENDIAN_LITTLE )
			{
				value_32bit = (uint32_t) ( value_aligned << byte_count );

				/* Strip-off the used part of the aligned value
				 */
				value_aligned >>= alignment_count;
			}
			safe_checksum_value ^= value_32bit;
		}
		/* Update the 32-bit XOR value with the aligned XOR value
		 */
		byte_size = (uint8_t) sizeof( libregf_aligned_t );

		while( byte_size != 0 )
		{
			byte_count = ( ( byte_size / 4 ) - 1 ) * 32;

			if( byte_order == _BYTE_STREAM_ENDIAN_BIG )
			{
				big_endian_value_32bit = (uint32_t) ( ( value_aligned >> byte_count ) & 0xffffffffUL );

				/* Change big-endian into little-endian
				 */
				value_32bit = ( ( big_endian_value_32bit & 0x000000ffUL ) << 24 )
				            | ( ( big_endian_value_32bit & 0x0000ff00UL ) << 8 )
				            | ( ( big_endian_value_32bit >> 8 ) & 0x0000ff00UL )
				            | ( ( big_endian_value_32bit >> 24 ) & 0x000000ffUL );
			}
			else if( byte_order == _BYTE_STREAM_ENDIAN_LITTLE )
			{
				value_32bit = (uint32_t) value_aligned;

				value_aligned >>= byte_count;
			}
			byte_size -= 4;

			safe_checksum_value ^= value_32bit;
		}
		/* Re-align the buffer iterator
		 */
		buffer_iterator = (uint8_t *) aligned_buffer_iterator;

		byte_size = 4 - ( alignment_size % 4 );

		if( byte_size != 4 )
		{
			value_32bit   = buffer_iterator[ 0 ];
			value_32bit <<= 8;

			if( byte_size >= 2 )
			{
				value_32bit |= buffer_iterator[ 1 ];
			}
			value_32bit <<= 8;

			if( byte_size >= 3 )
			{
				value_32bit |= buffer_iterator[ 2 ];
			}
			value_32bit <<= 8;

			buffer_iterator += byte_size;
			size            -= byte_size;

			safe_checksum_value ^= value_32bit;
		}
	}
	while( size > 0 )
	{
		value_32bit = 0;
		byte_count  = 1;

		if( size >= 4 )
		{
			value_32bit |= buffer_iterator[ 3 ];
			value_32bit <<= 8;

			byte_count++;
		}
		if( size >= 3 )
		{
			value_32bit |= buffer_iterator[ 2 ];
			value_32bit <<= 8;

			byte_count++;
		}
		if( size >= 2 )
		{
			value_32bit |= buffer_iterator[ 1 ];
			value_32bit <<= 8;

			byte_count++;
		}
		value_32bit |= buffer_iterator[ 0 ];

		buffer_iterator += byte_count;
		size            -= byte_count;

		safe_checksum_value ^= value_32bit;
	}
	*checksum_value = safe_checksum_value;

	return( 1 );
}

