# Changelog

All notable changes to this project will be documented in this file. See [standard-version](https://github.com/conventional-changelog/standard-version) for commit guidelines.

### [1.0.14](https://github.com/pass0a/cvip/compare/v1.0.13...v1.0.14) (2020-05-15)


### Bug Fixes

* 修复类型文件不匹配的问题，去掉摄像头命名，增加isOpened接口,并修改azurepipelines的编译流程 ([cccaf15](https://github.com/pass0a/cvip/commit/cccaf15a0b82a7b293f90dfcbcd5e344dcadc985))

### [1.0.13](https://github.com/pass0a/cvip/compare/v1.0.12...v1.0.13) (2020-05-14)


### Bug Fixes

* 修改保存摄像头数据为图片文件，修改摄像头原始内容转换为jpg内容 ([fa42533](https://github.com/pass0a/cvip/commit/fa4253313987f053a277a362f3bd4a7ae907505b))

### [1.0.12](https://github.com/pass0a/cvip/compare/v1.0.11...v1.0.12) (2020-05-08)


### Bug Fixes

* 增加npm cache功能，更新cxb版本 ([01125d8](https://github.com/pass0a/cvip/commit/01125d8a32035ac17c652048b55ee58f80805de7))
* 增加关闭摄像头的接口，并修复调用接口不一致的问题 ([4d4f288](https://github.com/pass0a/cvip/commit/4d4f28879936db29735fba148e98ed0fde9fa52d))

### [1.0.12](https://github.com/pass0a/cvip/compare/v1.0.11...v1.0.12) (2020-05-08)


### Bug Fixes

* 增加npm cache功能，更新cxb版本 ([0a6c464](https://github.com/pass0a/cvip/commit/0a6c464448abfc56a178febd52ac8e81e629f8f4))
* 增加关闭摄像头的接口，并修复调用接口不一致的问题 ([4d4f288](https://github.com/pass0a/cvip/commit/4d4f28879936db29735fba148e98ed0fde9fa52d))

### [1.0.11](https://github.com/pass0a/cvip/compare/v1.0.10...v1.0.11) (2020-05-08)


### Features

* 增加多路摄像头的功能 ([8aa3660](https://github.com/pass0a/cvip/commit/8aa3660cc373e21eb13822906e21cbdcb3ac9bbf))


### Bug Fixes

* 修复压缩包文件错误的问题，修复opencv依赖dll没有拷贝的问题 ([0ada84c](https://github.com/pass0a/cvip/commit/0ada84c50db87a50e0ff18ad45de481fc322dd7e))
* 修复在azurepipelines上无法编译通过的步骤，并增加单元测试报告 ([6a4a05e](https://github.com/pass0a/cvip/commit/6a4a05e7aa332d6151839f4e7225ded53bbf432e))
* 增加camera功能，增加jest测试框架，更新cxb设置 ([6be8365](https://github.com/pass0a/cvip/commit/6be8365ce9366e97f76c7f103668c68b65578c3d))

### [1.0.10](https://github.com/pass0a/cvip/compare/v1.0.9...v1.0.10) (2020-03-20)


### Bug Fixes

* 修改远程下载的包版本包没有包含引导字符v的问题 ([fbe2ab5](https://github.com/pass0a/cvip/commit/fbe2ab519011d429bcb75ec3859dffc7a985c2f8))

### [1.0.9](https://github.com/pass0a/cvip/compare/v1.0.7...v1.0.9) (2020-03-19)


### Bug Fixes

* 不指定平台编译，方便根据当前平台选择不同的二进制包 ([3f370b4](https://github.com/pass0a/cvip/commit/3f370b4bfa73afcd93ba7ca603a2681506264b8a))

### [1.0.10](https://github.com/pass0a/cvip/compare/v1.0.7...v1.0.10) (2020-03-19)


### Bug Fixes

*  不指定平台编译，方便根据当前平台选择不同的二进制包 ([59a2903](https://github.com/pass0a/cvip/commit/59a2903c8d45c318f42cc0eb4217592b9a4660e9))

### [1.0.9](https://github.com/pass0a/cvip/compare/v1.0.7...v1.0.9) (2020-03-19)


### Bug Fixes

* 不指定平台编译，方便根据当前平台选择不同的二进制包 ([a9b3bb9](https://github.com/pass0a/cvip/commit/a9b3bb96aa5896864a34b969dd41e1cf1e3d74f8))

### [1.0.7](https://github.com/pass0a/cvip/compare/v1.0.6...v1.0.7) (2020-03-18)


### Features

* upgrade opencv 3 to 4 ([85777bb](https://github.com/pass0a/cvip/commit/85777bb947c223e89e405dc4b636fb32e9a71948))


### Bug Fixes

* 解决拷贝opencv的dll版本不一致的问题 ([1bbf829](https://github.com/pass0a/cvip/commit/1bbf8296fbdecf4aec83cd924c7807eb41988cd2))

### [1.0.6](https://github.com/pass0a/cvip/compare/v1.0.4...v1.0.6) (2020-03-16)


### Features

* 合并回主分支，增加node版本 ([59fa190](https://github.com/pass0a/cvip/commit/59fa190eca673459432e6ccc6979fa9ec5b3233c))
* 实现匹配子图，重新编码和截图功能 ([e664c35](https://github.com/pass0a/cvip/commit/e664c353b8d4b8d550e0dfa79fb15aa0281b8e3c))
