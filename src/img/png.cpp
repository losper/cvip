#include "png.h"
#include <stdint.h>

int pngWrite16(png_structp png_ptr, rawImage img) {
	png_bytep row = new png_byte[3 * img.width * sizeof(png_byte)];
	int bytes_per_pixel = img.bpp / 8;
	size_t offset = 0, pos = 0;
	uint32_t pixel = 0;

	int r_mask = (1 << img.rlen) - 1;
	int g_mask = (1 << img.glen) - 1;
	int b_mask = (1 << img.blen) - 1;

	for (size_t y = 0; y < img.height; y++)
	{
		for (size_t x = 0; x < img.width; x++)
		{
			pos = x * 3;
			offset = x*bytes_per_pixel + img.width*y*bytes_per_pixel;
			pixel = *((uint16_t *)(img.data + offset));
			row[pos] = (((pixel >> img.ros)&r_mask) * 0xFF) / r_mask;
			row[pos + 1] = (((pixel >> img.gos) & g_mask) * 0xFF) / g_mask;
			row[pos + 2] = (((pixel >> img.bos) & b_mask) * 0xFF) / b_mask;
		}
		png_write_row(png_ptr, row);
	}
	if (row != NULL) delete (row);
	return 0;
}
int pngWrite24(png_structp png_ptr, rawImage img) {
	png_bytep row = new png_byte[3 * img.width * sizeof(png_byte)];
	int bytes_per_pixel = img.bpp / 8;
	size_t offset = 0, pos = 0;
	uint32_t pixel = 0;

	int r_mask = (1 << img.rlen) - 1;
	int g_mask = (1 << img.glen) - 1;
	int b_mask = (1 << img.blen) - 1;

	for (size_t y = 0; y < img.height; y++)
	{
		for (size_t x = 0; x < img.width; x++)
		{
			pos = x * 3;
			offset = x*bytes_per_pixel + img.width*y*bytes_per_pixel;
			pixel = *(img.data + offset);
			pixel += *(img.data + offset + 1) << 8;
			pixel += *(img.data + offset + 2) << 16;
			row[pos] = (((pixel >> img.ros)&r_mask) * 0xFF) / r_mask;
			row[pos + 1] = (((pixel >> img.gos) & g_mask) * 0xFF) / g_mask;
			row[pos + 2] = (((pixel >> img.bos) & b_mask) * 0xFF) / b_mask;
		}
		png_write_row(png_ptr, row);
	}
	if (row != NULL) delete (row);
	return 0;
}
int pngWrite32(png_structp png_ptr, rawImage img) {
	png_bytep row = new png_byte[3 * img.width * sizeof(png_byte)];
	int bytes_per_pixel = img.bpp / 8;
	size_t offset = 0, pos = 0;
	uint32_t pixel = 0;

	int r_mask = (1 << img.rlen) - 1;
	int g_mask = (1 << img.glen) - 1;
	int b_mask = (1 << img.blen) - 1;

	for (size_t y = 0; y < img.height; y++)
	{
		for (size_t x = 0; x < img.width; x++)
		{
			pos = x * 3;
			offset = x*bytes_per_pixel + img.width*y*bytes_per_pixel;
			pixel = *((uint32_t *)(img.data + offset));
			row[pos] = (((pixel >> img.ros)&r_mask) * 0xFF) / r_mask;
			row[pos + 1] = (((pixel >> img.gos) & g_mask) * 0xFF) / g_mask;
			row[pos + 2] = (((pixel >> img.bos) & b_mask) * 0xFF) / b_mask;
		}
		png_write_row(png_ptr, row);
	}
	if (row != NULL) delete (row);
	return 0;
}

int raw2png(rawImage& img, png_rw_ptr cb)
{
	int code = 0;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;

	// Initialize write structure
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fprintf(stderr, "Could not allocate write struct\n");
		code = 1;
		goto finalise;
	}

	// Initialize info structure
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fprintf(stderr, "Could not allocate info struct\n");
		code = 1;
		goto finalise;
	}

	// Setup Exception handling
	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "Error during png creation\n");
		code = 1;
		goto finalise;
	}
	png_set_write_fn(png_ptr, &img, cb, NULL);
	//png_init_io(png_ptr, fp);

	// Write header (8 bit colour depth)
	png_set_IHDR(png_ptr, info_ptr, img.width, img.height,
		8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	// Set title
	if (img.title != NULL) {
		png_text title_text;
		title_text.compression = PNG_TEXT_COMPRESSION_NONE;
		title_text.key = "Title";
		title_text.text = img.title;
		png_set_text(png_ptr, info_ptr, &title_text, 1);
	}

	png_write_info(png_ptr, info_ptr);

	// Allocate memory for one row (3 bytes per pixel - RGB)
	//row = (png_bytep)malloc(3 * width * sizeof(png_byte));
	// Write image data
	switch (img.bpp)
	{
	case 16:
		pngWrite16(png_ptr, img);
		break;
	case 24:
		pngWrite24(png_ptr, img);
		break;
	case 32:
		pngWrite32(png_ptr, img);
		break;
	default:
		break;
	}
	// End write
	png_write_end(png_ptr, NULL);

finalise:
	if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);


	return code;
}
