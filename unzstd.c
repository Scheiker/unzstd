/*
* Copyright (C) 2017 - Charlton Head
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <zstd.h>

static void *
malloc_or_exit(size_t size)
{
	void *buf = malloc(size);

	if (!buf) {
		fputs("Malloc failed!\n", stderr);
		exit(2);
	}

	return buf;
}

int
main(void)
{
	size_t       result, read, toread;
	size_t       bufinsize  = ZSTD_DStreamInSize();
	size_t       bufoutsize = ZSTD_DStreamOutSize();
	void         *bufin     = malloc_or_exit(bufinsize);
	void         *bufout    = malloc_or_exit(bufoutsize);
	ZSTD_DStream *dstream   = ZSTD_createDStream();

	if (!dstream) {
		fputs("ZSTD_createDStream() failed!\n", stderr);
		return 1;
	}

	result = ZSTD_initDStream(dstream);

	if (ZSTD_isError(result)) {
		fprintf(stderr, "ZSTD_initDStream() failed: %s\n",
		        ZSTD_getErrorName(result));
		return 2;
	}

	toread = result;
	while ((read = fread(bufin, 1, toread, stdin)) > 0) {
		ZSTD_inBuffer input = {bufin, read, 0};
		while (input.pos < input.size) {
			ZSTD_outBuffer output = {bufout, bufoutsize, 0};
			toread = ZSTD_decompressStream(dstream, &output , &input);
			fwrite(bufout, 1, output.pos, stdout);
		}
	}

	ZSTD_freeDStream(dstream);
	free(bufin);
	free(bufout);

	return 0;
}