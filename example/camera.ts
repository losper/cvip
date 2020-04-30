import { Camera } from '../src/index';

let cap = new Camera(0);
// cap.on('data', (data) => {
// 	console.log(data);
// });
cap.on('data', (data) => {
	console.log(data.length);
});
console.log(cap.read());
