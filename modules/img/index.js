const inst=require("./img.passoa");
function raw2jpeg(src,opt,fn){
	return inst.raw2jpeg(src,opt.width,opt.height,opt.bpp,opt.rlen,opt.glen,opt.blen,opt.ros,opt.gos,opt.bos,fn);
}
module.exports={
    encode:raw2jpeg,
}
