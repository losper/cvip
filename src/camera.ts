import { bindings } from './bindings';
import { Readable } from 'stream';

export class Camera extends Readable {
	private inst: any;
	private _name: string;
	// _write(chunk: any, encoding: string, callback: (error?: Error | null) => void) {
	// 	callback();
	// }
	constructor(idx: number, name: string) {
		super({ objectMode: true });
		this._name = name;
		console.log('open:', this._name);
		this.inst = bindings.cameraOpen(idx, name, (data: any) => {
			if (this.isPaused()) this.emit('takephoto', data);
			else {
				this.push(data);
			}
		});
	}
	_read(size: number) {
		if (bindings.cameraIsOpened(this.inst)) {
			bindings.cameraRead(this.inst);
		} else {
			this.push(null);
		}
	}
	_final() {
		bindings.cameraClose(this.inst);
	}
	get(prop_idx: number) {
		return bindings.get(prop_idx);
	}
	set(prop_idx: number, value: number) {
		return bindings.set(prop_idx, value);
	}
	take() {
		return bindings.cameraTake(this.inst);
	}
	/*
	CV_FOURCC('P', 'I', 'M', '1') = MPEG - 1 codec
	CV_FOURCC('M', 'J', 'P', 'G') = motion - jpeg codec
	CV_FOURCC('M', 'P', '4', '2') = MPEG - 4.2 codec 
	CV_FOURCC('D', 'I', 'V', '3') = MPEG - 4.3 codec 
	CV_FOURCC('D', 'I', 'V', 'X') = MPEG - 4 codec 
	CV_FOURCC('U', '2', '6', '3') = H263 codec 
	CV_FOURCC('I', '2', '6', '3') = H263I codec 
	CV_FOURCC('F', 'L', 'V', '1') = FLV1 codec
	*/
	startRecord(path: string) {
		return bindings.cameraRecord(this.inst, path);
	}
	stopRecord() {
		return bindings.cameraRecord(this.inst);
	}
	static count() {
		return bindings.cameraCount();
	}
}
