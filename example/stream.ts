const Readable = require('stream').Readable;

// 底层数据
const dataSource = [ 'a', 'b', 'c', 'd', 'e' ];

const readable = Readable({ objectMode: true });
function readData(that: any) {
	return new Promise((resolve, reject) => {
		setTimeout(() => {
			if (dataSource.length) {
				console.log(dataSource);
				that.push(dataSource.shift());
				resolve(0);
			} else {
				that.push(null);
				resolve(-1);
			}
		}, 1000);
	});
}
function waitData() {
	return new Promise((resolve, reject) => {
		setTimeout(() => {
			resolve(0);
		}, 5000);
	});
}
readable._read = async function(size: number) {
	//process.nextTick(() => {
	console.log(size);
	await readData(this);

	//});
};

readable.pause();
readable.on('data', async (data: string) => {
	console.log('wait start', data);
	await waitData();
	console.log('wait end', data);
});

readable.on('readable', function() {
	while (null !== readable.read());
});
