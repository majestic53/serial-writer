/**
 * Superboard III Serial Writer
 * Copyright (C) 2015 David Jolly
 * ----------------------
 *
 * This tool is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This tool is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * This tool slows serial traffic between the host machine
 * and Superboard III client machine, allowing for clean data
 * transfers.
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define __in

#define SERIAL_PORT "/dev/ttyUSB0" // port name
#define SLEEP_INTERVAL 20000 // us

int 
main(
	__in int argc,
	__in const char **argv
	)
{
	char str[2] = {0};
	FILE *input = NULL;
	int result = 0, serial = 0;

	if(argc < 2) {
		fprintf(stderr, "Usage: %s [INPUT]\n", argv[0]);
		result = 1;
		goto exit;
	}

	serial = open(SERIAL_PORT, O_WRONLY | O_NOCTTY | O_SYNC);
	if(serial < 0) {
		fprintf(stderr, "Failed to open serial port: %s: %s\n",
				SERIAL_PORT, strerror(errno));
		result = errno;
		goto exit;
	}

	input = fopen(argv[1], "r");
	if(!input) {
		fprintf(stderr, "Failed to open input file: %s: %s\n", 
				argv[1], strerror(errno));
		result = errno;
		goto exit;
	}

	while((str[0] = fgetc(input)) != EOF) {
		write(serial, str, 1);
		usleep(SLEEP_INTERVAL);
	}

exit:

	if(input) {
		fclose(input);
		input = NULL;
	}

	if(serial) {
		close(serial);
		serial = 0;
	}

	return result;
}
