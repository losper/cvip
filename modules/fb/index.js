const fb=require("./fb.passoa");
function FrameBuffer(path){
	var inst=fb.open(path);
	this.read=function(buf){
		return fb.read(inst,buf);
	}
	this.info=function(){
		return fb.info(inst);
	}
}
module.exports={
	FrameBuffer:FrameBuffer
}
