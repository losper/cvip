import { bindings } from './bindings';
import { Readable } from 'stream';

export class Camera extends Readable {
	private idx_: number = 0;
	// _write(chunk: any, encoding: string, callback: (error?: Error | null) => void) {
	// 	callback();
	// }
	constructor(idx: number) {
		super({ objectMode: true });
		bindings.cameraOpen(idx);
	}
	_read(size: number) {
		if (bindings.isCameraOpened(this.idx_)) {
			bindings.cameraRead(this.idx_, (data: Buffer) => {
				this.push(data);
			});
		} else {
			this.push(null);
		}
	}
	_final() {
		bindings.cameraClose(this.idx_);
	}
	get(prop_idx: number) {
		return bindings.get(prop_idx);
	}
	set(prop_idx: number, value: number) {
		return bindings.set(prop_idx, value);
	}
	static count() {
		return bindings.cameraCount();
	}
}
