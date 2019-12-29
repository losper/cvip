import * as cvip from '../src/index';
let cap = new cvip.Camera(0);
cap.set(cvip.CV_PROP.CAP_PROP_FPS, 30);
console.log('FPS:', cap.get(cvip.CV_PROP.CAP_PROP_FPS));
for (let index = 0; index < 10; index++) {
	const element = 10;
	cap.read('test' + index + '.jpg');
}
cap.close();
