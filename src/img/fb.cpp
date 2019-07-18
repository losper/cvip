#include "fb.h"
#include <iostream>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

framebuffer::framebuffer(){
}
framebuffer::~framebuffer(){
	munmap(fbp,fbi.size);
	close(fbfd);
}
int framebuffer::init(const char* path){
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;

	// Open the file for reading and writing
	fbfd = open(path, O_RDWR);
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

	// Figure out the size of the screen in bytes
	fbi.size = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
	// Map the device to memory
	fbp = (char *)mmap(0, finfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if ((int)fbp == -1) {
		perror("Error: failed to map framebuffer device to memory\n");
		return (4);
	}
	printf("The framebuffer device was mapped to memory successfully.\n");
	fbi.height=vinfo.yres;
	fbi.width=vinfo.xres;
	fbi.bpp=vinfo.bits_per_pixel;
	fbi.rlen=vinfo.red.length;
	fbi.glen=vinfo.green.length;
	fbi.blen=vinfo.blue.length;
	fbi.ros=vinfo.red.offset;
	fbi.gos=vinfo.green.offset;
	fbi.bos=vinfo.blue.offset;
	return 0;
}
fbInfo& framebuffer::info(){
	return fbi;
}
int framebuffer::read(void* buf, uint32_t len){
	struct fb_var_screeninfo vinfo;

	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
		perror("Error reading variable information");
		return (3);
	}
	if (vinfo.yoffset) {
		memcpy(buf, fbp + vinfo.yoffset*fbi.width*fbi.bpp/8, len);
	}
	else {
		memcpy(buf, fbp, len);
	}
	return 0;
}
int framebuffer::destory(){
	return 0;
}
