const cvip = require("./cvip.passoa");

function Cvip(){
	this.jpeg=1;
	this.webp=64;
	this.png=16;
    this.imageMatch=function(img,tmp){
        return cvip.jsCvipMatch(img,tmp);
    };
    this.imageSave=function(tmp,sim,cvfmt){
        return cvip.jsCvipEncode(tmp,sim,cvfmt);
    };
    this.imageCut=function(tmp,sim,cvfmt,x,y,w,h){
        return cvip.jsCvipCut(tmp,sim,cvfmt,x,y,w,h);
    }
	this.stdev=function(){
		return cvip.stdev.apply(this,arguments);
	}
	this.absdiff=function(){
		return cvip.absdiff.apply(this,arguments);
	}
	this.horizontal=function(){
		return cvip.horizontal.apply(this,arguments);
	}
	this.resave=function(){
		return cvip.resave.apply(this,arguments);
	}
	this.cutcap=function(){
		return cvip.cutcap.apply(this,arguments);
	}
}

module.exports=new Cvip();


