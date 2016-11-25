/*cc utility framework
 *author: Jonathan 'JK' Kernick
 *description: a set of common utilities that are useful for building web apps
 */

CC.Utility =
{
    // Pi - 180 degs
    PI: 3.14159265359,
    // half Pi - 90 degs
    HALF_PI: 3.14159265359*0.5,
    // quater Pi - 45 degs
    QUATER_PI: 3.14159265359*0.25,
    // third Pi - 120 degs
    THIRD_PI: 6.28318530718/3,
    // Tau - 360 degs
    TAU: 6.28318530718,
    // Golden Ratio
    PHI: 1.61803398875,
    splitText: function(_text, _lineLength)
    {
        var wordArray = _text.split(" ");
        //console.log(wordArray);

        var newString = "";
        var newStringLength = 0;
        var lineLength = _lineLength;

        for (var i = 0; i < wordArray.length; i++) {
            if(newStringLength + wordArray[i].length + 1 < lineLength)
            {
                newString = newString +  " " + wordArray[i];
                newStringLength = newStringLength + wordArray[i].length + 1;
            }
            else
            {
                newString = newString + " " + "\n" + wordArray[i];
                newStringLength = wordArray[i].length;
            }
        };

        return newString;
    },
    //creates a data URL from data and mimetype
    createDataUrl: function(dataText,mimeType){
        var base64Data = btoa(dataText);
        return "data:" + mimeType + ";base64," + base64Data;
    },
    //Fischer Yates shuffling method
    shuffleArray:function(array)
    {
        var m = array.length, t, i;

        // While there remain elements to shuffle…
        while (m) {

            // Pick a remaining element…
            i = Math.floor(Math.random() * m--);

            // And swap it with the current element.
            t = array[m];
            array[m] = array[i];
            array[i] = t;
        }

        return array;
    },
    //an modulus function that lets you go into negative values and still work
    modo: function(value,size)
    {
        var returnValue = 0;
        if(value >= 0)
        {
            returnValue = value % size;
        }
        else
        {
            returnValue = (size + (value % size))%size;
        }
        return returnValue;
    },
    //a function that finds the sign of the value(aka normal of a singled dimention value)
    sign: function(value)
    {
        var returnVar = 1;
        if(value < 0)
        {
            returnVar = -1;
        }
        return returnVar;
    },
    //interpolating function that moves between frames
    lerp: function(start,end,value)
    {
        return (((end - start)*value)+start);
    },
    //interpolating function that moves between frames
    sineEasing: function(value)
    {
        return ((-Math.sin(value*this.PI))+1)*0.5;
    },
    //finds the value position btween two numbers
    value: function(start,end,position)
    {
        return (position - start)/(end - start);
    },
    //a int to string that includes leading zeros
    intToString: function(num,zeros)
    {
        zeros = zeros || 0;
        var numStr = ""+num;
        var space  = zeros-numStr.length;
        var zeroStr = '00000000000000000';
        return zeroStr.substring(0,space)+numStr;
    },
    //RGB to colurhex
    RGBToColor: function(r,g,b)
    {
        var returnValue = 0;
        r = Math.floor(r%256);
        g = Math.floor(g%256);
        b = Math.floor(b%256);
        returnValue += r*256*256;
        returnValue += g*256;
        returnValue += b;
        return returnValue;
    },
    //RGB value to colurhex
    RGBValueToColor: function(r,g,b)
    {
        var returnValue = 0;
        r = Math.floor(r*256)%256;
        g = Math.floor(g*256)%256;
        b = Math.floor(b*256)%256;
        returnValue += r*256*256;
        returnValue += g*256;
        returnValue += b;
        return returnValue;
    },
    //converts a string into the postion requested
    _stringToAnchor: function(anchorString)
    {
        var anchor = {x:0.5,y:0.5};
        anchorString = anchorString.toLowerCase();
        switch(anchorString)
        {
            case 'tl':
            case 'topleft':
            case 'top left':
            case 'top_left':
            case 'nw':
            case 'northwest':
            case 'north west':
            case 'north_west':
            anchor.x = 0;
            anchor.y = 0;
                break;
            case 'tr':
            case 'topright':
            case 'top right':
            case 'top_right':
            case 'ne':
            case 'northeast':
            case 'north east':
            case 'north_east':
            anchor.x = 1;
            anchor.y = 0;
                break;
            case 'bl':
            case 'bottomleft':
            case 'bottom left':
            case 'bottom_left':
            case 'sw':
            case 'southwest':
            case 'south west':
            case 'south_west':
            anchor.x = 0;
            anchor.y = 1;
                break;
            case 'br':
            case 'bottomright':
            case 'bottom right':
            case 'bottom_right':
            case 'se':
            case 'southeast':
            case 'south east':
            case 'south_east':
            anchor.x = 1;
            anchor.y = 1;
                break;
            case 't':
            case 'top':
            case 'n':
            case 'north':
            anchor.y = 0;
                break;
            case 'r':
            case 'right':
            case 'e':
            case 'east':
            anchor.x = 1;
                break;
            case 'b':
            case 'bottom':
            case 's':
            case 'south':
            anchor.y = 1;
                break;
            case 'l':
            case 'left':
            case 'w':
            case 'west':
            anchor.x = 0;
                break;
        }
        return anchor;
    },
    //function to set standard propperties on a pixi display object
    _applyStyle: function(pixiDisplayObject,style)
    {
        //checks to see if there was a x
        if(style.x !== undefined)
        {
            pixiDisplayObject.position.x = style.x;
        }
        //checks to see if there was a y
        if(style.y !== undefined)
        {
            pixiDisplayObject.position.y = style.y;
        }
        //checks to see if there was a position
        if(style.position !== undefined)
        {
            pixiDisplayObject.position.x = style.position.x;
            pixiDisplayObject.position.y = style.position.y;
        }
        //checks to see if there was a alpha
        if(style.alpha !== undefined)
        {
            pixiDisplayObject.alpha = style.alpha;
        }
        //checks to see if there was a tint
        if(style.tint !== undefined)
        {
            pixiDisplayObject.tint = style.tint;
        }
        //checks to see if there was a visibility
        if(style.visible !== undefined)
        {
            pixiDisplayObject.visible = style.visible;
        }
        //checks to see if there was a rotation
        if(style.rotation !== undefined && pixiDisplayObject.rotation !== undefined)
        {
            switch(typeof style.rotation)
            {
                case 'number':
                //if its a number rotation
                    pixiDisplayObject.rotation = style.rotation;
                    break;
                case 'string': 
                    //removes the d to indecate that its in degrees
                    var stringRotation = style.rotation.substring(stringRotation.length-1);
                    pixiDisplayObject.rotation = Number(stringRotation)*0.0174532925 ;
                    break;
            }
        }
        //checks to see if there was a scale
        if(style.scale !== undefined && pixiDisplayObject.scale !== undefined)
        {
            switch(typeof style.scale)
            {
                case 'number':
                    pixiDisplayObject.scale.x = style.scale;
                    pixiDisplayObject.scale.y = style.scale;
                    break;
                case 'object':
                    pixiDisplayObject.scale.x = style.scale.x;
                    pixiDisplayObject.scale.y = style.scale.y;
                    break;
            }
        }
        //checks to see if there was a flipping rule
        if(style.flip !== undefined)
        {
            if(style.flip === 'vertical')
            {
                pixiDisplayObject.scale.x *= -1;
            }
            else
            {
                pixiDisplayObject.scale.y *= -1;
            }
        }
        //checks to see if there is an anchor then to see if an anchor property was given
        if(pixiDisplayObject.anchor !== undefined)
        {
            pixiDisplayObject.anchor.x = 0.5;
            pixiDisplayObject.anchor.y = 0.5;
            if(style.anchor !== undefined)
            {
                switch(typeof style.anchor)
                {
                    case 'number':
                        pixiDisplayObject.anchor.x = style.anchor;
                        pixiDisplayObject.anchor.y = style.anchor;
                        break;
                    case 'string': 
                        var stringAnchor = this._stringToAnchor(style.anchor);
                        pixiDisplayObject.anchor.x = stringAnchor.x;
                        pixiDisplayObject.anchor.y = stringAnchor.y;
                        break;
                    case 'object':
                        pixiDisplayObject.anchor.x = style.anchor.x;
                        pixiDisplayObject.anchor.y = style.anchor.x;
                        break;
                }
            }
        }

        return pixiDisplayObject;
    },
	//function to set standard propperties on a pixi display object
	pixi: function(pixiDisplayObject,style)
	{
        //checks to see if there is a style
        if(style !== undefined)
        {
        //checks to see if style is an array
            if(style instanceof Array)
            {
                for(var i = 0; i < style.length; ++i)
                {
                    this._applyStyle(pixiDisplayObject,style[i]);
                }
            }
            else
            {
                this._applyStyle(pixiDisplayObject,style);
            }
        }
        else
        {
            //checks to see if there is an anchor then to see if an anchor property was given
            if(pixiDisplayObject.anchor !== undefined)
            {
                pixiDisplayObject.anchor.x = 0.5;
                pixiDisplayObject.anchor.y = 0.5;
            }
        }

		return pixiDisplayObject;
	},
    pixiSprite: function(path,style)
    {
        return CC.Utility.pixi(new PIXI.Sprite.fromImage(path,true),style);
    }, 
    pixiAtlasSprite: function(path,style)
    {
        return CC.Utility.pixi(new PIXI.Sprite.fromFrame(path),style);
    }, 
    pixiMovieClip: function(paths,style)
    {
        var i = 0;
        var textureArray = [];
        for(i = 0; i < paths.length; ++i)
        {
            textureArray.push(PIXI.Texture.fromImage(paths[i]));
        }
        return CC.Utility.pixi(new PIXI.MovieClip(textureArray),style);
    },
    pixiAtlasMovieClip: function(paths,style)
    {
        var i = 0;
        var textureArray = [];
        for(i = 0; i < paths.length; ++i)
        {
            textureArray.push(PIXI.Texture.fromFrame(paths[i]));
        }
        return CC.Utility.pixi(new PIXI.MovieClip(textureArray),style);
    },
    pixiObject: function(style)
    {
        return CC.Utility.pixi(new PIXI.DisplayObjectContainer(),style);
    },  
    checkRadius: function(coords,radius)
    {
        var squardedAdj = coords.x*coords.x;
        var squardedOps = coords.y*coords.y;
        var squardedHyp = (squardedAdj+squardedOps);
        return (squardedHyp < radius*radius);
    }
};
