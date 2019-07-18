#include <png.h>
#include <stdint.h>
#include <vector>

struct rawImage {
	size_t width;
	size_t height;
	char* title;
	png_bytep data;
	size_t bpp;
	size_t rlen;
	size_t glen;
	size_t blen;
	size_t ros;
	size_t gos;
	size_t bos;
	std::vector<uint8_t> buf;
};
int raw2png(rawImage& img, png_rw_ptr cb);