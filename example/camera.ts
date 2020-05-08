import { Camera } from '../src/index';
import { join } from 'path';
async function main() {
	let cap = new Camera(1, 'test0');
	let cap1 = new Camera(0, 'test1');
	//console.log(cap.take());
	let idx = 0;
	cap.on('data', (data) => {
		//console.log('!!!:', data);
		if (idx == 0) {
			cap.startRecord(join(__dirname, 'test.avi'));
		}
		if (idx == 100) {
			cap.stopRecord();
		}
		idx++;
	});
	cap1.on('data', (data) => {
		//console.log(data);
	});
}
main();
