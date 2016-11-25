/*cc sound framework
 *author: Jonathan 'JK' Kernick
 *description: as class for handling matrixes
 */

/*
a matrix table is ALWAYS!! exspressed like this no exseptions!!
and its in m[y][x]
	m:[
	  [0,0,0],
	  [0,0,0],
	  [0,0,0]
	  ],
*/
Math.cot = function(theta){
	return (Math.tan(theta))?(1/Math.tan(theta)):0;
};
//do not use this constructor this is not unless you want to create a blank matrix that is basicly usless
CC.Matrix = function(m)
{
	this._m = m || [
		[1,0,0,0],
		[0,1,0,0],
		[0,0,1,0],
		[0,0,0,1]
	  ];
};

CC.Matrix.prototype = {
	//the matrixs numbers don't bother editing them directly you are just gonna look silly
	_m:[
	  [1,0,0,0],
	  [0,1,0,0],
	  [0,0,1,0],
	  [0,0,0,1]
	  ],
	//matrix muplication lets not try and think too much about it but this is why they are so damned powerfull
	muliply: function(matrix)
	{
		//pulls out the bit of the matrix we actualy need, the matrix part...
		var m = matrix._m;
		//fashions up a new grid for pluging the numbers into
		var newGrid =
		[
		  [1,0,0,0],
		  [0,1,0,0],
		  [0,0,1,0],
		  [0,0,0,1]
		];
		var i;
		for(i = 0; i < 16; ++i)
		{
			//exstrapolate xy from i
			var x = i%4;
			var y = Math.floor(i/4);
			var dot = 0;
			for(var j = 0; j < 4; ++j)
			{
				dot += this._m[y][j]*m[j][x];
			}
			newGrid[y][x] = dot;
		}
		for(i = 0; i < 4; ++i)
		{
			this._m[i][0] = newGrid[i][0]; 
			this._m[i][1] = newGrid[i][1]; 
			this._m[i][2] = newGrid[i][2]; 
			this._m[i][3] = newGrid[i][3]; 
		}
	},
	//and here is where the matrix proves it's quality by manipulating vectors
	intergrateVector: function(vector)
	{
		var returnVector = {x:0,y:0,z:0};

		var m = this._m;
		returnVector.x = ((vector.x*m[0][0])+(vector.y*m[0][1])+(vector.z*m[0][2])+m[0][3]);
		returnVector.y = ((vector.x*m[1][0])+(vector.y*m[1][1])+(vector.z*m[1][2])+m[1][3]);
		returnVector.z = ((vector.x*m[2][0])+(vector.y*m[2][1])+(vector.z*m[2][2])+m[2][3]);
		return returnVector;
	},
	//and here is where the matrix proves it's quality by manipulating vectors
	reset: function()
	{
		this._m[0][0] = 1;
		this._m[0][1] = 0;
		this._m[0][2] = 0;
		this._m[0][3] = 0;
		this._m[1][0] = 0;
		this._m[1][1] = 1;
		this._m[1][2] = 0;
		this._m[1][3] = 0;
		this._m[2][0] = 0;
		this._m[2][1] = 0;
		this._m[2][2] = 1;
		this._m[2][3] = 0;
		this._m[3][0] = 0;
		this._m[3][1] = 0;
		this._m[3][2] = 0;
		this._m[3][3] = 1;
	}
};
//rotation matrix will rotate around the x axis
CC.Matrix.createRotationMatrixX = function(angle)
{
	return new CC.Matrix([
	  [1,0,0,0],
	  [0,Math.cos(angle),-Math.sin(angle),0],
	  [0,Math.sin(angle),Math.cos(angle),0],
	  [0,0,0,1]
	  ]);
};
//rotation matrix will rotate around the y axis
CC.Matrix.createRotationMatrixY = function(angle)
{
	return new CC.Matrix([
	  [Math.cos(angle),0,Math.sin(angle),0],
	  [0,1,0,0],
	  [-Math.sin(angle),0,Math.cos(angle),0],
	  [0,0,0,1]
	  ]);
};
//rotation matrix will rotate around the y axis
CC.Matrix.createRotationMatrixZ = function(angle)
{
	return new CC.Matrix([
	  [Math.cos(angle),-Math.sin(angle),0,0],
	  [Math.sin(angle),Math.cos(angle),0,0],
	  [0,0,1,0],
	  [0,0,0,1]
	  ]);
};
//creates a translation matrix
CC.Matrix.createTranslationMatrix = function(vector)
{
	return new CC.Matrix([
	  [1,0,0,vector.x],
	  [0,1,0,vector.y],
	  [0,0,1,vector.z],
	  [0,0,0,1]
	  ]);
};
//creates a scale matrix it looks slightly more complicated
//because it has an option for a uniform or seperate scale
CC.Matrix.createScaleMatrix = function(scale)
{
	if(scale.x)
	{
		return new CC.Matrix([
		  [scale.x,0,0],
		  [0,scale.y,0],
		  [0,0,scale.z,0],
		  [0,0,0,1]
		  ]);
	}
	else
	{
		return new CC.Matrix([
		  [scale,0,0],
		  [0,scale,0],
		  [0,0,scale,0],
		  [0,0,0,1]
		  ]);
	}
};
//creates a projection matrix
CC.Matrix.createProjectionMatrix = function(fov,near,far,aspect_ratio)
{
	return new CC.Matrix([
	  [Math.cot(fov/2)/aspect_ratio,0,0,0],
	  [0,Math.cot(fov/2),0,0],
	  
	  [0,0,far/(far-near),1],
	  [0,0,-((far*near)/(far-near)),0]
	  ]);
};
CC.Matrix.createWorldMatrix = function(right,up,look,position)
{
	return new CC.Matrix([
	  [right.x,right.y,right.z,0],
	  [up.x,up.y,up.z,0],
	  [look.x,look.y,look.z,0],
	  [position.x,position.y,position.z,1]
	  ]);
};