import * as cvip from '../src/index';
import { join } from 'path';


describe('cvip camera', () => {
	test('camera', () => {
		expect(cvip.Camera.count()).toBe(2);
		
		// let cam = new cvip.Camera();
		// cam.on('error', (err) => {
		// 	console.log(err);
		// });
		// cam.on('data', (data) => {
		// 	console.log(data);
		// });
		// cam.push('hello');
		// cam.push('world');
		// cam.push('hello');
		// cam.push('kity');
		// cam.open(0);
		// cam.start();
		// cam.stop();
	});
});
