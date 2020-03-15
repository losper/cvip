#ifndef __DPZ__CVIP__HPP__ 
#define __DPZ__CVIP__HPP__

struct stPos {
	int status;
	int x;
	int y;
	double val;
};
stPos cvipMatch(const char* src, const char* dst);
int cvipEncode(const char* src, const char* dst, int cvfmt);
int cvipCut(const char* src, const char* dst, int cvfmt, int x, int y, int w, int h);
#endif
