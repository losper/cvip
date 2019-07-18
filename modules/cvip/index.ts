// @ts-ignore：./cvip.passoa是一个passoa项目特定的二进制文件，无法被typescript正确的处理
import * as cvip from './cvip.passoa';

class Cvip {
	jpeg = 1;
	webp = 64;
	png = 16;
	imageMatch(img: string, tmp: string) {
		return cvip.jsCvipMatch(img, tmp);
	}
	imageSave(tmp: string, sim: string, cvfmt: number) {
		return cvip.jsCvipEncode(tmp, sim, cvfmt);
	}
	imageCut(tmp: string, sim: string, cvfmt: number, x: number, y: number, w: number, h: number) {
		return cvip.jsCvipCut(tmp, sim, cvfmt, x, y, w, h);
	}
	stdev() {
		return cvip.stdev.apply(this, arguments);
	}
	absdiff() {
		return cvip.absdiff.apply(this, arguments);
	}
	horizontal() {
		return cvip.horizontal.apply(this, arguments);
	}
	resave() {
		return cvip.resave.apply(this, arguments);
	}
	cutcap() {
		return cvip.cutcap.apply(this, arguments);
	}
}

export default new Cvip();
