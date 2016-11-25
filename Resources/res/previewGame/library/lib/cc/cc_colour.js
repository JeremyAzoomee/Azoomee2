/*cc colours
 *author: Jonathan 'JK' Kernick
 *description: the classes and functions required to manipulate colour
 */

CC.Colour = {
	hexToRGB: function(hexNum){
		var rgbObj = {};
		rgbObj.r = Math.floor(hexNum/(256*256))%256;
		rgbObj.g = Math.floor(hexNum/256)%256;
		rgbObj.b = hexNum%256;
		return rgbObj;
	},
	hexToString: function(hexNum){
		var returnString = hexNum.toString(16);
		returnString = "00000" + returnString;
		returnString = "#" + returnString.substr(-6,6);
		return returnString;
	},
	stringToHex: function(stringColour){
		return parseInt(stringColour.substr(-6,6),16);
	},
	hexToRGBvalue: function(hexNum){
		var rgbObj = this.hexToRGB(hexNum);
		rgbObj.r /= 255;
		rgbObj.g /= 255;
		rgbObj.b /= 255;
		return rgbObj;
	},
	RGBToHex: function(rgbObj){
       red = 0x10000*Math.floor(rgbObj.r);
       green = 0x100*Math.floor(rgbObj.g);
       blue = Math.floor(rgbObj.b);
		return ((red+green)+blue);
	},
	RGBvalueToHex: function(rgbObj){
       red = 0x10000*Math.floor(0xFF*rgbObj.r);
       green = 0x100*Math.floor(0xFF*rgbObj.g);
       blue = Math.floor(0xFF*rgbObj.b);
		return ((red+green)+blue);
	},
	HexLerp: function(hexA,hexB,value){
		var rgbObj = {};
		var rgbA = this.hexToRGBvalue(hexA);
		var rgbB = this.hexToRGBvalue(hexB);
		rgbObj.r = (((rgbB.r-rgbA.r)*value)+rgbA.r);
		rgbObj.g = (((rgbB.g-rgbA.g)*value)+rgbA.g);
		rgbObj.b = (((rgbB.b-rgbA.b)*value)+rgbA.b);
		return this.RGBvalueToHex(rgbObj);
	},
	HexSqrLerp: function(hexA,hexB,value){
		var rgbObj = {};
		var rgbA = this.hexToRGBvalue(hexA);
		var rgbB = this.hexToRGBvalue(hexB);
		rgbA.a *= rgbA.r;
		rgbA.g *= rgbA.g;
		rgbA.b *= rgbA.b;

		rgbB.r *= rgbB.r;
		rgbB.g *= rgbB.g;
		rgbB.b *= rgbB.b;

		rgbObj.r = Math.sqrt(((rgbB.r-rgbA.r)*value)+rgbA.r);
		rgbObj.g = Math.sqrt(((rgbB.g-rgbA.g)*value)+rgbA.g);
		rgbObj.b = Math.sqrt(((rgbB.b-rgbA.b)*value)+rgbA.b);
		return this.RGBvalueToHex(rgbObj);
	}

};

CC.Color = CC.Colour;