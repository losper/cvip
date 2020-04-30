import * as cvip from '../src';
import { join } from 'path';
describe('cvip image', () => {
	test('image', () => {
		let src = join(__dirname + '/test.jpg');
		let dst = join(__dirname + '/tmp.jpg');
		let spath = join(__dirname + '/tmp.png');
		expect(cvip.imageCut(src, dst, cvip.jpeg, 1000, 200, 100, 120)).toBe(0);
		let result = cvip.imageMatch(dst, src);
		expect(result.valid).toBe(1);
		expect(result.val).toBeGreaterThan(0.9);
		expect(cvip.imageSave(dst, spath, cvip.png)).toBe(0);
	});
});
