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
#include <termios.h>
#include <unistd.h>

#define __in
#define __out
#define __out_opt

#define BYTES_PER_KBYTE (1024.0)
#define SERIAL_BAUD ((speed_t) B9600) // speed
#define SERIAL_FLAGS (O_WRONLY | O_NOCTTY) // write-only terminal
#define SERIAL_PORT "/dev/ttyUSB0" // name
#define SLEEP_INTERVAL (100000) // us

void 
close_file(
	__in FILE *file
	)
{
	if(file) {
		fprintf(stdout, "[UNINIT] File closed on 0x%p\n", file);
		fclose(file);
	}
}

void 
close_serial(
	__in int port
	)
{
	if(port) {
		fprintf(stdout, "[UNINIT] Serial closed on port %d\n", port);
		close(port);
	}
}

int 
open_file(
	__in const char *path,
	__out FILE **file
	)
{
	int result = 0;

	if(!path || !file) {
		fprintf(stderr, "[ERROR] Invalid open file parameter\n");
		result = 1;
		goto exit;
	}

	*file = fopen(path, "r");
	if(!*file) {
		fprintf(stderr, "[ERROR] Failed to open input file: %s: %s\n", 
				path, strerror(errno));
		result = errno;
		goto exit;
	}

	fprintf(stdout, "[INIT] %s opened on 0x%p\n", path, *file);

exit:
	return result;
}

int 
open_serial(
	__in const char *name,
	__in speed_t baud,
	__in int flags,
	__out int *port
	)
{
	int result = 0;
	struct termios tty;

	if(!name || !port) {
		fprintf(stderr, "[ERROR] Invalid open serial parameter\n");
		result = 1;
		goto exit;
	}

	*port = open(name, flags);
	if(*port < 0) {
		fprintf(stderr, "[ERROR] Failed to open serial port: %s: %s\n",
				name, strerror(errno));
		result = errno;
		goto exit;
	}

	memset(&tty, 0, sizeof(struct termios));

	if(tcgetattr(*port, &tty)) {
		fprintf(stderr, "[ERROR] Failed to retrieve serial port attributes: %s: %s\n",
				name, strerror(errno));
		result = errno;
		goto exit;
	}

	cfsetospeed(&tty, baud);
	tty.c_oflag |= (OLCUC | ONLCR); // map all lower case to upper case, map NL to CR-NL
	tty.c_cflag &= ~(CSIZE | CSTOPB | CRTSCTS | PARENB); // single stop bit, no flow control, no parity bit
	tty.c_cflag |= CS8; // 8-bit character size
	tcflush(*port, TCOFLUSH);

	if(tcsetattr(*port, TCSANOW, &tty)) {
		fprintf(stderr, "[ERROR] Failed to set serial port attributes: %s: %s\n",
				name, strerror(errno));
		result = errno;
		goto exit;
	}

	fprintf(stdout, "[INIT] %s opened on port %d\n", name, *port);

exit:
	return result;
}

int 
write_serial(
	__in FILE *file,
	__in int port,
	__in useconds_t delay,
	__out_opt size_t *written
	)
{
	int result = 0;
	char str[2] = {0};
	size_t bytes_written = 0;

	if(!file || !port) {
		fprintf(stderr, "[ERROR] Invalid write parameter: File: 0x%p, Port: %d\n",
				file, port);
		result = errno;
		goto exit;
	}

	fprintf(stdout, "[INFO] Writting to serial... ");

	while((str[0] = fgetc(file)) != EOF) {
		write(port, str, 1);
		++bytes_written;
		usleep(delay);
	}

	fprintf(stdout, "Done.\n[INFO] %.02f KB (%lu bytes) written to serial\n", 
			(bytes_written / BYTES_PER_KBYTE), bytes_written);

	if(written) {
		*written = bytes_written;
	}

exit:
	return result;
}

int 
main(
	__in int argc,
	__in const char **argv
	)
{
	
	FILE *input = NULL;
	int result = 0, serial = 0;

	if(argc < 2) {
		fprintf(stderr, "Usage: %s [INPUT]\n", argv[0]);
		result = 1;
		goto exit;
	}

	result = open_serial(SERIAL_PORT, SERIAL_BAUD, SERIAL_FLAGS, &serial);
	if(result) {
		goto exit;
	}

	result = open_file(argv[1], &input);
	if(result) {
		goto exit;
	}

	result = write_serial(input, serial, SLEEP_INTERVAL, NULL);
	if(result) {
		goto exit;
	}

exit:

	close_file(input);
	input = NULL;
	close_serial(serial);
	serial = 0;

	return result;
}
