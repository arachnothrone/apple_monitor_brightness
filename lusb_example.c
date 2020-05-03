#include <errno.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//#include "libusb.h"
//#include <libusb-1.0>
#include "libusb-1.0/libusb.h"

static struct libusb_device_handle *devh = NULL;

static int find_dpfp_device(void)
{
	devh = libusb_open_device_with_vid_pid(NULL, 0x05ac, 0x9226);
	return devh ? 0 : -EIO;
}

int main(void)
{
    int Var1 = 0x55;
    printf("Start main: \n");

    struct sigaction sigact;
	int r;

	r = libusb_init(NULL);
	if (r < 0) {
		fprintf(stderr, "failed to initialise libusb\n");
		exit(1);
	}

    r = find_dpfp_device();
	if (r < 0) {
		fprintf(stderr, "Could not find/open device\n");
		// goto out;
	}

    return 0;
}

/*
"/Library/Developer/CommandLineTools/usr/bin/ld" -demangle -lto_library /Library/Developer/CommandLineTools/usr/lib/libLTO.dylib -dynamic -arch x86_64 -image_base 100000000 -macosx_version_min 10.13.0 -pagezero_size 10000 -o wrk -Lobj/lib -L/usr/local/opt/openssl/lib obj/wrk.o obj/net.o obj/ssl.o obj/aprintf.o obj/stats.o obj/script.o obj/units.o obj/ae.o obj/zmalloc.o obj/http_parser.o obj/bytecode.o obj/version.o -lluajit-5.1 -lpthread -lm -lcrypto -lssl -lSystem /Library/Developer/CommandLineTools/usr/lib/clang/9.1.0/lib/darwin/libclang_rt.osx.a
"/Library/Developer/CommandLineTools/usr/bin/ld" -demangle -lto_library /Library/Developer/CommandLineTools/usr/lib/libLTO.dylib -dynamic -arch x86_64 -image_base 100000000 -macosx_version_min 10.13.0 -pagezero_size 10000 -o wrk -L/usr/local/opt/openssl/lib -Lobj/lib obj/wrk.o obj/net.o obj/ssl.o obj/aprintf.o obj/stats.o obj/script.o obj/units.o obj/ae.o obj/zmalloc.o obj/http_parser.o obj/bytecode.o obj/version.o -lluajit-5.1 -lpthread -lm -lcrypto -lssl -lSystem /Library/Developer/CommandLineTools/usr/lib/clang/9.1.0/lib/darwin/libclang_rt.osx.a

*/