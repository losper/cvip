#include <string.h>
#include <errno.h>
#include <map>
#include <string>
#include <iostream>

#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdint.h>
#include "scut.h"


class scut {
public:
	scut() {
	}
	int cut(const char* src, png_rw_ptr cb) {
		readScreen(src);
		raw2png({ vinfo.xres, vinfo.yres,"??",fbp,
			vinfo.bits_per_pixel,
			vinfo.red.length,vinfo.green.length,vinfo.blue.length,
			vinfo.red.offset,vinfo.green.offset,vinfo.blue.offset }, cb);
		return 0;
	}
	int readScreen(const char* devname) {
		struct fb_fix_screeninfo finfo;
		int x = 0, y = 0;
		long int location = 0;

		// Open the file for reading and writing
		fbfd = open(devname, O_RDWR);
		if (fbfd == -1) {
			perror("Error: cannot open framebuffer device");
			return (1);
		}
		printf("The framebuffer device was opened successfully.\n");

		// Get fixed screen information
		if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
			perror("Error reading fixed information");
			return (2);
		}

		// Get variable screen information
		if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
			perror("Error reading variable information");
			return (3);
		}

		printf("memlen:%d,%dx%d, bpp:%d\n", finfo.line_length, vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
		printf("lenrgb:%d,%d,%d -- offsetrgb:%d,%d,%d\n", vinfo.red.length,vinfo.green.length,vinfo.blue.length,vinfo.red.offset,vinfo.green.offset,vinfo.blue.offset);

		// Figure out the size of the screen in bytes
		screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
		// Map the device to memory
		fbp = (char *)mmap(0, finfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
		if ((int)fbp == -1) {
			perror("Error: failed to map framebuffer device to memory\n");
			return (4);
		}
		printf("The framebuffer device was mapped to memory successfully.\n");
		/*munmap(fbp, screensize);
		close(fbfd);*/
		return 0;
	}
private:
	char *fbp;
	int fbfd;
	struct fb_var_screeninfo vinfo;
	long int screensize;
};
 
int cut_fb(const char* src, png_rw_ptr cb) {
	scut tmp;
	tmp.cut(src,cb);
}
