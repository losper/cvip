module.exports = (opts) => {
	let env = process.env;
	let module_name = opts.module_name;
	let config = {
		module_name: 'cvip',
		remote_path: 'repertory/cxb/',
		package_name: `${module_name}.tar.gz?version=${opts.version}-${opts.platform}-${opts.arch}`,
		host: 'https://passoa-generic.pkg.coding.net',
		external: {},
		build_cmd: {
			windows_x86: [ '-DNODE=0' ],
			vc14_x64: [ '-G', 'Visual Studio 14 2015 Win64', '-DNODE=1' ],
			node_x64: [ '-G', 'Visual Studio 15 2017 Win64', '-DNODE=1' ]
		}
	};
	return config;
};
