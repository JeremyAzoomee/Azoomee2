/*
	lerp function
	node class

*/
//if there is no CC 'namespace' then create one
if (typeof CC === "undefined") {
    var CC = {
        dummy: "dummy"
    };
    CC.isMobile = navigator.userAgent.match(/BlackBerry|IEMobile|Windows Phone|Android|iPad|iPhone|Silk|iPod|Kindle|Nexus/i) !== null;

}

/*
Coder: JK
Name: Bezier Curve
Decription: class for handling burger parts

*/

CC.BezierCurve = function() {
    //PIXI.DisplayObjectContainer.call(this);
    this.nodeArray = [];
    this.controlPArray = [];
    this.controlNArray = [];
};
//CC.BezierCurve.prototype = Object.create(PIXI.DisplayObjectContainer.prototype);
//CC.BezierCurve.prototype.constructor = CC.BezierCurve;

CC.BezierCurve.prototype.addNode = function(x,y,cx,cy)
{
	this.nodeArray.push({"x":x,"y":y});
	this.controlPArray.push({"x":cx,"y":cy});
	this.controlNArray.push({"x":-cx,"y":-cy});
	this.vectorArray = [];
	this.totalDistance = 0;

};
CC.BezierCurve.prototype.generateVectors = function(steps,lengthThreshold)
{
	this.vectorArray = [{x:this.nodeArray[0].x,y:this.nodeArray[0].y,d:0}];
	this.totalDistance = 0;
	var lastPoint = {x:this.nodeArray[0].x,y:this.nodeArray[0].y};
	for(var i = 0; i < this.nodeArray.length-1; ++i)
	{
		for(var j = 0; j < steps; ++j)
		{
			var distanceCurve = (i+(j/steps))
			var testPoint = this.findPoint(distanceCurve);
			var newDistance = CC.BezierCurve.distance(lastPoint,testPoint);
			if(newDistance > lengthThreshold)
			{
				this.totalDistance += newDistance;
				currentDistance = this.totalDistance;
				lastPoint.x = testPoint.x;
				lastPoint.y = testPoint.y;
				this.vectorArray.push({x:testPoint.x,y:testPoint.y,d:currentDistance})
			}
		}
	}
};
CC.BezierCurve.prototype.findPoint = function(distance)
{
	var location = distance%(this.nodeArray.length-1);
	var scalar = location%1;
	var index = Math.floor(location);
	return this.calculateCurvePoint(index,scalar);

};
CC.BezierCurve.prototype.findPointVector = function(distance)
{
	var returnPoint = {x:0,y:-distance};
	for(var i = 0; i < this.vectorArray.length-2; ++i)
	{
		var vecA = this.vectorArray[i];
		var vecB = this.vectorArray[(i+1)];
		if(vecA.d <= distance && vecB.d > distance)
		{
			var interDist = vecB.d-vecA.d;
			var desiredDist = distance-vecA.d;
			var cx = (vecB.x-vecA.x)/interDist;
			var cy = (vecB.y-vecA.y)/interDist;
			returnPoint.x = ((cx*desiredDist)+vecA.x);
			returnPoint.y = ((cy*desiredDist)+vecA.y);
			i = this.vectorArray.length;
		}
	}
	return returnPoint;

};
CC.BezierCurve.prototype.calculateCurvePoint = function(index,scalar)
{
	var nodeA = this.nodeArray[index];
	var controlA = this.controlPArray[index];
	var nodeB = this.nodeArray[(index+1)];
	var controlB = this.controlNArray[(index+1)];
	var fixedControlA = {
		x:(controlA.x+nodeA.x),
		y:(controlA.y+nodeA.y),
	};
	var fixedControlB = {
		x:(controlB.x+nodeB.x),
		y:(controlB.y+nodeB.y),
	};
	/*
	nodeA--->controlA--->controlB---->nodeB
	*/
	return this.grindCurve(nodeA,fixedControlA,fixedControlB,nodeB,scalar);

};
CC.BezierCurve.prototype.grindCurve = function(vecA,vecB,vecC,vecD,scalar)
{
	var point1A = CC.BezierCurve.lerp(vecA,vecB,scalar);
	var point1B = CC.BezierCurve.lerp(vecB,vecC,scalar);
	var point1C = CC.BezierCurve.lerp(vecC,vecD,scalar);

	var point2A = CC.BezierCurve.lerp(point1A,point1B,scalar);
	var point2B = CC.BezierCurve.lerp(point1B,point1C,scalar);

	return CC.BezierCurve.lerp(point2A,point2B,scalar);
};

CC.BezierCurve.lerp = function(vectorA,vectorB,scaler)
{
	var cx = vectorB.x-vectorA.x;
	var cy = vectorB.y-vectorA.y;
	var dx = ((cx*scaler)+vectorA.x);
	var dy = ((cy*scaler)+vectorA.y);
	return {x:dx,y:dy};
};
CC.BezierCurve.distance = function(vectorA,vectorB)
{
	var cx = vectorB.x-vectorA.x;
	var cy = vectorB.y-vectorA.y;
	return Math.sqrt((cx*cx)+(cy*cy));
};