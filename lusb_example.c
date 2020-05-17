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
	printf("devh: %0x\n", (unsigned int) devh);
	return devh ? 0 : -EIO;
}

static void print_devs(libusb_device **devs)
{
	libusb_device *dev;
	int i = 0, j = 0;
	uint8_t path[8]; 
	int status;

	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		unsigned char mfg[100];
		unsigned char prd[100];
		unsigned char serNum[20]={0};

		status = libusb_open(dev, &devh);
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0) {
			fprintf(stderr, "failed to get device descriptor");
			return;
		}

		libusb_get_string_descriptor_ascii(devh, desc.iManufacturer, mfg, 100);
		libusb_get_string_descriptor_ascii(devh, desc.iProduct, prd, 100);
		if (desc.iSerialNumber !=0)
		{
			libusb_get_string_descriptor_ascii(devh, desc.iSerialNumber, serNum, 20);
		}
		else
		{
			serNum[0] = 'u'; serNum[1] = 'n'; serNum[2] = 'k'; serNum[3] = 'n'; 
			serNum[4] = 'o'; serNum[5] = 'w'; serNum[6] = 'n'; serNum[7] = '\t'; 
			/*serNum[8] = '\t'; */serNum[8] = '\0';
		}

		printf("%04x:%04x (bus %3d, device %2d); bcdDevice:%04x, S/N(%d):%s\t MFG:%s, PRODUCT:%s, status:%d ",
			desc.idVendor, desc.idProduct, 
			libusb_get_bus_number(dev), libusb_get_device_address(dev), 
			desc.bcdDevice, desc.iSerialNumber, serNum, mfg, prd, status);

		r = libusb_get_port_numbers(dev, path, sizeof(path));
		if (r > 0) {
			printf(" path: %d", path[0]);
			for (j = 1; j < r; j++)
				printf(".%d", path[j]);
		}
		printf("\n");

		libusb_close(devh);
	}
}

int main(void)
{
	printf("Start main: \n");

    struct sigaction sigact;
	int r;
	libusb_device **devs;
	ssize_t cnt;

	r = libusb_init(NULL);
	printf("1. INIT: %d\n", r);
	if (r < 0) {
		fprintf(stderr, "failed to initialise libusb\n");
		exit(1);
	}

	cnt = libusb_get_device_list(NULL, &devs);
	printf("1a. List: %ld\n", cnt);

	print_devs(devs);
	libusb_free_device_list(devs, 1);

    // r = find_dpfp_device();
	// printf("2. FIND: %d\n", r);
	// if (r < 0) {
	// 	fprintf(stderr, "Could not find/open device\n");
	// 	// goto out;
	// }

	// r = libusb_claim_interface(devh, 0);
	// printf("3. CLAIM: %d\n", r);
	// if (r < 0) {
	// 	fprintf(stderr, "usb_claim_interface error %d\n", r);
	// }
	// printf("claimed interface\n");


    return 0;
}

/*
"/Library/Developer/CommandLineTools/usr/bin/ld" -demangle -lto_library /Library/Developer/CommandLineTools/usr/lib/libLTO.dylib -dynamic -arch x86_64 -image_base 100000000 -macosx_version_min 10.13.0 -pagezero_size 10000 -o wrk -Lobj/lib -L/usr/local/opt/openssl/lib obj/wrk.o obj/net.o obj/ssl.o obj/aprintf.o obj/stats.o obj/script.o obj/units.o obj/ae.o obj/zmalloc.o obj/http_parser.o obj/bytecode.o obj/version.o -lluajit-5.1 -lpthread -lm -lcrypto -lssl -lSystem /Library/Developer/CommandLineTools/usr/lib/clang/9.1.0/lib/darwin/libclang_rt.osx.a
"/Library/Developer/CommandLineTools/usr/bin/ld" -demangle -lto_library /Library/Developer/CommandLineTools/usr/lib/libLTO.dylib -dynamic -arch x86_64 -image_base 100000000 -macosx_version_min 10.13.0 -pagezero_size 10000 -o wrk -L/usr/local/opt/openssl/lib -Lobj/lib obj/wrk.o obj/net.o obj/ssl.o obj/aprintf.o obj/stats.o obj/script.o obj/units.o obj/ae.o obj/zmalloc.o obj/http_parser.o obj/bytecode.o obj/version.o -lluajit-5.1 -lpthread -lm -lcrypto -lssl -lSystem /Library/Developer/CommandLineTools/usr/lib/clang/9.1.0/lib/darwin/libclang_rt.osx.a

*/