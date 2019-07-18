#include <stdint.h>

struct fbInfo{
	uint32_t size;
	uint32_t height;
	uint32_t width;
	uint8_t bpp;
	uint8_t rlen;
	uint8_t glen;
	uint8_t blen;
	uint8_t ros;
	uint8_t gos;
	uint8_t bos;
};
class framebuffer{
public:
	framebuffer();
	virtual ~framebuffer();
	int init(const char* path);
	fbInfo& info();
	int read(void* ,uint32_t );
	int destory();
private:
	fbInfo fbi;
	int fbfd;
    char* fbp;
};
