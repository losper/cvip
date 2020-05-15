import { Camera } from '../src/index';
import { join } from 'path';
import { accessSync, F_OK } from 'fs-extra';
import { resolve } from 'dns';

function fsExistsSync(path) {
	try {
		accessSync(path, F_OK);
	} catch (e) {
		return false;
	}
	return true;
}
describe('cvip camera', () => {
	test(
		'camera',
		async () => {
			expect(Camera.count()).toBe(2);
			await new Promise((resolve, reject) => {
				let cap = new Camera(0);
				let cap1 = new Camera(1);
				let cap2 = new Camera(2);
				//console.log(cap.take());
				expect(cap.isOpened()).toBe(true);
				expect(cap1.isOpened()).toBe(true);
				expect(cap2.isOpened()).toBe(false);
				let idx = 0;
				cap.on('data', (data) => {
					//console.log('!!!:', data);
					if (idx == 0) {
						cap.startRecord(join(__dirname, 'test.avi'));
					}
					if (idx == 100) {
						cap.stopRecord();
					}
					if (idx == 102) {
						cap.destroy();
						cap1.destroy();
						expect(idx).toBeGreaterThan(100);
						expect(fsExistsSync(join(__dirname, 'test.avi'))).toBe(true);
						resolve(0);
					}
					idx++;
				});
				cap1.on('data', (data) => {
					//console.log(data);
				});
			});
		},
		20000
	);
});
