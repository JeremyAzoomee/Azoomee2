/*
 Author: Jonathan "JK" Kernick
 Description: object for handling geometry for vertexed based objects,

 Programers Note:if there are spelling mistakes i don't care :)
 i am a dislexic graphics programer not a english teacher

 i use the word polygon alot though out this code, what i exsacly mean is
 an ordered list of vertexes that when mapped with a render produce a polygon
 */

//-----------creating Maths Propertys
// Pi - 180 degs
Math.hPI = 3.14159265359 * 0.5;
// Tau - 360 degs
Math.TAU = 6.28318530718;
// Golden Ratio
Math.PHI = 1.61803398875;

//interplates between two values using a value
Math.lerp = function (start, end, value) {
    return (((end - start) * value) + start);
};
Math.sineEasing = function (value) {
    return ((-Math.sin(value * Math.PI)) + 1) * 0.5;
};
Math.value = function (start, end, position) {
    return (position - start) / (end - start);
};
Math.equate = function (sampleStart, sampleEnd, samplePos,start,end) {
    return Math.lerp(start,end,Math.value(sampleStart,sampleEnd,samplePos));
};
Math.clamp = function (min, max, value) {
    return Math.max(min,Math.min(max,value));
};
Math.RadDegRatio = (Math.PI/180);

Math.degToRad = function(value){
    return Math.RadDegRatio*value;
};
Math.radToDeg = function(value){
    return Math.RadDegRatio/value;
};
//----------------Classes
Math.modo = function (value, size) {
    var returnValue = 0;
    if (value >= 0) {
        returnValue = value % size;
    }
    else {
        returnValue = (size + (value % size)) % size;
    }
    return returnValue;
};


CC.Geometry = {
    //returns a pixi displayObject from an a vertex list array
    //requires pixi to be loaded if you have not loaded pixi then you will look like a right tool.
    renderToPixiGraphic: function (vertexArray, fillColor, lineColor) {
        var length = vertexArray.length;
        var graphics = new PIXI.Graphics();
        graphics.beginFill(fillColor, 0.1);
        graphics.lineStyle(1, lineColor);
        // draw a triangle using lines
        graphics.moveTo(vertexArray[length - 1].x, vertexArray[length - 1].y);

        var i;
        for (i = 0; i < length; ++i) {
            graphics.lineTo(vertexArray[i].x, vertexArray[i].y);
        }
        // end the fill
        /*
         var i;
         for(i = 0; i < length; ++i)
         {
         var j = (i+1)%length;
         var relVec = this.sumVectors(vertexArray[i],vertexArray[j])
         var relVecA = this.sumVectors(vertexArray[0],vertexArray[8])
         var startVec = this.scaleVector(relVec,0.5);
         var endVec = this.scaleVector(relVecA,0.5);
         var surfaceNormal = this.calculateSurfaceNormal(vertexArray[i],vertexArray[j]);
         var surfaceNormal = this.scaleVector(surfaceNormal,8);
         var surfaceNormal = this.sumVectors(surfaceNormal,startVec)
         graphics.lineTo(vertexArray[i].x,vertexArray[i].y);
         graphics.moveTo(startVec.x,startVec.y);
         graphics.lineTo(surfaceNormal.x,surfaceNormal.y);
         }*/
        graphics.endFill();

        return graphics;

    },
    //returns a dot product great way of squishing points onto lines for organisaion
    dotProduct2D: function (vertexA, vertexB) {
        return (vertexA.x * vertexB.x) + (vertexA.y * vertexB.y);
    },
    rotate2D: function (vector, theta) {

        return {
            x: ((Math.cos(-theta) * vector.x) + (Math.sin(-theta) * vector.y)),
            y: ((-Math.sin(-theta) * vector.x) + (Math.cos(-theta) * vector.y))
        };
    },
    rotate2DRef: function (vector, theta) {
        var X = vector.x;
        var Y = vector.y;
        vector.x = ((Math.cos(-theta) * X) + (Math.sin(-theta) * Y));
        vector.y = ((-Math.sin(-theta) * X) + (Math.cos(-theta) * Y));
    },
    rotate2DRef2: function (vector, theta,refVector) {
        refVector.x = ((Math.cos(-theta) * vector.x) + (Math.sin(-theta) * vector.y));
        refVector.y = ((-Math.sin(-theta) * vector.x) + (Math.cos(-theta) * vector.y));
    },
    //returns a reflected vector along a normal,
    //very good for reflecting objects and kinda pointless for anything else...at all
    reflect2D: function (vector, normal) {
        var dotProduct = this.dotProduct2D(normal, vector);

        return {
            x: vector.x - (2 * dotProduct * normal.x),
            y: vector.y - (2 * dotProduct * normal.y)
        };
    },
    reflect2DRef: function (vector, normal) {
        var dotProduct = this.dotProduct2D(normal, vector);

        vector.x -= (2 * dotProduct * normal.x);
        vector.y -= (2 * dotProduct * normal.y);
    },
    //returns a vector based on polar coordnates, basic bit of trig really the
    //y is inverted because computers rendered upside down for some reason, i blame crt monitors
    //theta is the usual mathmatical name for an angle fyi
    polarToVector2D: function (theta, radius) {
        return {
            x: Math.sin(theta) * radius,
            y: Math.cos(theta) * -radius
        };
    },
    //returns a cross product for a 2d vertex set(mercitly  much easyer then in 3d)
    crossProduct2D: function (vertexA, vertexB) {
        return (vertexA.x * vertexB.y) - (vertexA.y * vertexB.x);
    },
    //creates a relative vector asuiming that the first
    //vector is to be move to zero and the second is relative too that
    //basicly it just subs the first from the second but subbing is so
    //often used to find a relative vector that it really just has to be laid out this way
    relativeVector: function (vectorOrigin, VectorTarget) {
        return {
            x: VectorTarget.x - vectorOrigin.x,
            y: VectorTarget.y - vectorOrigin.y
        };
    },
    //returns a bool saying if the point is within a convex polygon,
    //if you put a concaved polygon though this then thats your own stupid fault
    pointTriangleTest: function (point, vertexArray) {
        //just to be optamistic we asume that there is overlap :)
        //that and if we didn't then it would screw up the intergrated
        //boolan function
        var resultBool = true;
        var i;
        for (i = 0; i < vertexArray.length; ++i) {
            var j = (i + 1) % vertexArray.length;
            //relative vector from the current vertex to the point
            var relativePoint = this.relativeVector(vertexArray[i], point);
            //relative vector from the current vertex to the next vertex
            var relativeNext = this.relativeVector(vertexArray[i], vertexArray[j]);
            //basicly says if the cross product is posative then asume its on the left hand side
            //because it has to be the same for all cases then we use an and
            resultBool = resultBool && this.crossProduct2D(relativePoint, relativeNext) > 0;

        }
        return resultBool;
    },
    //converts from vector pairs to a line equasion
    //mostly usless, unless we want to do some tricky shit with the vector
    convertToLineEquationX: function (vectorStart, VectorEnd) {
        //empty line object for your benafit reader ;)
        var line = {slope: 1, intercept: 0};
        //relativeVector from each vector
        var relVector = this.relativeVector(vectorStart, VectorEnd);
        //the slope is kinda easy to work out
        line.slope = relVector.y / relVector.x;
        //so is the offset to be honest... its just basic liner algbra
        line.intercept = vectorStart.y - (vectorStart.x * line.slope);
        return line;
    },
    //exsacly the same as the last one only with y as the func parameter
    convertToLineEquationY: function (vectorStart, VectorEnd) {
        //empty line object for your benafit reader ;)
        var line = {slope: 1, intercept: 0};
        //relativeVector from each vector
        var relVector = this.relativeVector(vectorStart, VectorEnd);
        //the slope is kinda easy to work out
        line.slope = relVector.x / relVector.y;
        //so is the offset to be honest... its just basic liner algbra
        line.intercept = vectorStart.x - (vectorStart.y * line.slope);
        return line;
    },
    //another usless liner algbra function unless you have to do
    //line equasions then its kinda important it is
    //exspecting both lines to have a slope and intercept member
    //this works for both f(x) and f(y) lines, just remeber what you plugged into it!
    solveTwoLines: function (lineA, lineB) {
        //basicly this is just rearanging a equation so we know what the unknown is, neat eh?
        //like your algbra from school, you remeber that right? yeah me nether.

        //if slopeDual is zero it means the lines are parallel,
        //it also means that in a few lines this is gonna bugout :)
        var slopeDual = lineA.slope - lineB.slope;
        var interceptDual = lineB.intercept - lineA.intercept;
        //because unlike you i am a giving person i have wrapped this in a
        //inline that will lie about the parrale nature making what i
        //said a few lines ago incorect, YOU'RE WELCOME.
        return (slopeDual !== 0) ? interceptDual / slopeDual : interceptDual / 0.00001;
        //small note about this, if you wanted to know they are parallel(why?) this will not really break that,
        //floats are too imprecise to ever really come up with a perfect zero on slope,
        //really you are looking at a less thant more than job, just sayin'
    },
    //ok this will solve just one line for x or y it really dosen't care,
    //all that matters is that you know wtich, if you are worred, write a
    //comment next to it where you invoke it
    solveLine: function (line, value) {
        //fundmently i shouldn't have to exsplane this but oh well,
        //it takes the line equasion like y=0.5x+5 or x=1.2x-3
        //and solves it with a given value
        return ((line.slope * value) + line.intercept);
    },
    //intersection test, no matter how you cut this one its always a bastard!
    vectorLineIntersection: function (vectorAStart, vectorAEnd, vectorBStart, vectorBEnd) {
        var interSectionPoint = {x: 0, y: 0};
        //converts to line equasions not the most optimal way of doing it but the most acadmeic
        var lineA = this.convertToLineEquationX(vectorAStart, vectorAEnd);
        var lineB = this.convertToLineEquationX(vectorBStart, vectorBEnd);
        //then solves them to find the common x;
        interSectionPoint.x = this.solveTwoLines(lineA, lineB);
        //solves using the new x value to find the corasponding y ether
        //line will do but first come first serve
        interSectionPoint.y = this.solveLine(lineA, interSectionPoint.x);
        return interSectionPoint;
    },
    //converts a non-complex polygon to a subset of triangles, via ear clipping this is quite
    //a simple thing just move though each point checking if its a triangle or not
    triangulateEar: function (vertexArray) {
        //with any triangulaion we know that the triangles will always be equal the vertex count minus 2
        var trianglesToExstract = vertexArray.length - 2;
        var triangleVertexArrays = [];
        var h, j;
        for (var z = 0; z < trianglesToExstract; ++z) {
            var vertexFound = false;
            var testTriAngle = [];
            var candiateIndex = 0;
            var i;
            for (i = 0; i < vertexArray.length && !vertexFound; ++i) {
                h = (i + (vertexArray.length - 1)) % vertexArray.length;
                j = (i + 1) % vertexArray.length;

                testTriAngle[0] = vertexArray[h];
                testTriAngle[1] = vertexArray[i];
                testTriAngle[2] = vertexArray[j];
                var validVertex = this.isPolygonClockWise(testTriAngle);
                for (var a = 0; a < vertexArray.length && validVertex; ++a) {
                    if (a !== i && a !== h && a !== j) {
                        validVertex = validVertex && !this.pointTriangleTest(vertexArray[a], testTriAngle);
                    }
                }
                vertexFound = validVertex;
                candiateIndex = i;
            }
            triangleVertexArrays.push(testTriAngle);
            vertexArray.splice(candiateIndex, 1);
        }
        return triangleVertexArrays;
    },
    //finds the perpendicular of a vector, if you try to use this on anything but a normal,
    //then i will laugh at you, LAUGH i said! I'm sure there could be a reason you need an
    //abitray point at a 90deg anti-clockwise turn about 0,0 of your current point, but then i
    //would urge you to use matrixes like a grownup
    calculatePerpendicularVector: function (vector) {
        return {
            x: vector.y,
            y: -vector.x
        };
    },
    //pythagoras therogm just remebeber that the "square of the hypotenuse(h) side is directly equaly
    //to the sum of the squares of the opposite(o) and adjacent (a) sides"
    //basicly h*h = a*a + o*o, i don't think i can do superscript in comments :(

    //this gives us the squared length or more aptly the unsquare rooted distance
    //SQUARE ROOT WARNING!!!
    vectorLengthSquared: function (vector) {
        return ((vector.x * vector.x) + (vector.y * vector.y));
    },
    //vector length basicly squared one thats
    //been square rooted to give the actual length
    vectorLength: function (vector) {
        return Math.sqrt(this.vectorLengthSquared(vector));
    },
    //same as vectorLengthSquared but with two vectors
    //for the ones too lazy to find the relative vector, you know who you are!
    vectorDistanceSquared: function (vectorA, vectorB) {
        return this.vectorLengthSquared(this.relativeVector(vectorA, vectorB));
    },
    //finds the unsquared length between two vectors SQUARE ROOT WARNING!!!
    vectorDistance: function (vectorA, vectorB) {
        return Math.sqrt(this.vectorDistanceSquared(vectorA, vectorB));
    },
    //changes the length of a vector to a length of one, this is kinda simple using clasic pythag
    //think hippotmus hides SQUARE ROOT WARNING!!!
    normaliseVector: function (vector) {
        return this.scaleVector(vector, 1 / this.vectorLength(vector));
    },
    normaliseVectorRef: function (vector,magnatude) {
        magnatude = ((magnatude===undefined)?1:magnatude);
        this.scaleVectorRef(vector, ((1/this.vectorLength(vector))*magnatude));
    },
    //returns a -1 to 1 randomish number based on keys
    keyRandomiser: function (keyA, keyB) {
        return ((Math.sin(keyA * keyB) + keyB * 100) % 2) - 1;
    },
    //warp polygon vertexes
    warpPolygon: function (vertexArray, amount) {
        var newVertexArray = [];
        var i;
        for (i = 0; i < vertexArray.length; ++i) {
            var flooredX = Math.round(vertexArray[i].x);
            var flooredY = Math.round(vertexArray[i].y);
            newVertexArray[i] = {
                x: vertexArray[i].x + (this.keyRandomiser(flooredX, flooredY) * amount),
                y: vertexArray[i].y + (this.keyRandomiser(flooredY, flooredX) * amount)
            };
        }
        return newVertexArray;
    },
    //checking if a polygon is clockwise
    isPolygonClockWise: function (vertexArray) {
        //a running total to keep track of while we iterate
        var runningTotal = 0;
        var i;
        for (i = 0; i < vertexArray.length; ++i) {
            var j = (i + 1) % vertexArray.length;
            /*
             this is kinda a difficult one to get your head around but its  with acumlative values
             with relative vectors just think that it does the job it needs to at a low cost to performance
             */
            runningTotal += (vertexArray[j].x - vertexArray[i].x) * (-vertexArray[j].y + -vertexArray[i].y);
        }
        //if the runningTotal is l is greater then zero then it is clockwise other wise its anti-clockwise
        return runningTotal > 0;
    },
    //generates a regular polygon with sine functions easy peasy
    generateRegularPolgyon: function (sides, radius, x, y) {
        var vertexArray = [];
        var stepSize = Math.TAU / sides;
        var i;
        for (i = 0; i < sides; ++i) {
            vertexArray.push({
                x: ((Math.sin(stepSize * i) * radius) + x),
                y: (( -Math.cos(stepSize * i) * radius) + y)
            });
        }
        return vertexArray;
    },
    //scales the vector with float, funamentaly just changes the length of the vector
    //0.5 will make it half the length and 2 will make it twice
    scaleVector: function (vector, scaler) {
        return {
            x: vector.x * scaler,
            y: vector.y * scaler
        };
    },
    //scales the vector with float, funamentaly just changes the length of the vector
    //0.5 will make it half the length and 2 will make it twice
    scaleVectorRef: function (vector, scaler) {
        vector.x *= scaler;
        vector.y *= scaler;
    },
    //clamps a vector to an Axis aligned Rect
    clampVectorToRect: function (vector, rect, rectOffset) {
        rectOffset = rectOffset || {x: 0, y: 0};
        return {
            x: Math.min(Math.max(vector.x, (rect.x + rectOffset.x)), ((rect.x + rectOffset.x) + rect.width)),
            y: Math.min(Math.max(vector.y, (rect.y + rectOffset.y)), ((rect.y + rectOffset.y) + rect.height))
        };
    },
    //adds two vectors together... you work it out.
    sumVectors: function (vectorA, vectorB) {
        return {
            x: (vectorA.x + vectorB.x),
            y: (vectorA.y + vectorB.y)
        };
    },
    //adds two vectors together... you work it out.
    translateVectorRef: function (vector, refVector) {
        refVector.x += vector.x;
        refVector.y += vector.y;
    },
    //liner interpilation is just using a scaler to choose a point between two vectors
    //a scalar of 0 will get you the start vector, a scalar of 1 will get you the end vector
    //a scalar of 0.5 will get you half way between
    lerpVector: function (vectorStart, VectorEnd, scaler) {
        var relVector = this.relativeVector(vectorStart, VectorEnd);
        var scaledVector = this.scaleVector(relVector, scaler);
        return this.sumVectors(scaledVector, vectorStart);
    },
    //calculates the surface normal from Vertex A to vertex B,
    //that is a vector with the length of 1(called a unit vector),
    //that is pointing perpendicular to the surface SQUARE ROOT WARNING!!!
    calculateSurfaceNormal: function (vertexA, vertexB) {
        var relVector = this.normaliseVector(this.relativeVector(vertexA, vertexB));
        return this.calculatePerpendicularVector(relVector);
    },
    //insetting is easy enough, just don't alway exspect a viable polgon at the end
    //while i could exsplane how this works internaly it really is kinda lengthy and boring
    //basicly it pushes the lines along there surface normals by the value provided :) a
    //negative value makes the polygon "smaller" and a positive makes it "bigger"...sorta
    insetPolygon: function (vertexArray, insetValue) {

        //sets up blank array for filling with vertexPairs
        var vertexPairsArray = [];
        //the array we want to create with insetted sides
        var insetVertexArray = [];
        //create a array of start and end pairs that have been adjusted
        var i, j, k, l;
        for (i = 0; i < vertexArray.length; ++i) {
            //we need the next vertex so with a wraper so we can stay inside the array
            j = (i + 1) % vertexArray.length;
            //we need the surface normal so we inset in the correct direction
            var surfaceNormal = this.calculateSurfaceNormal(vertexArray[i], vertexArray[j]);
            //we need to calculate the inset amount
            var insetAmount = this.scaleVector(surfaceNormal, insetValue);
            vertexPairsArray.push(this.sumVectors(insetAmount, vertexArray[i]));
            vertexPairsArray.push(this.sumVectors(insetAmount, vertexArray[j]));
        }
        //intergrates the pairs back into a instted vertex array
        for (i = 0; i < vertexPairsArray.length; i += 2) {
            j = (i + 1) % vertexPairsArray.length;
            k = (i + 2) % vertexPairsArray.length;
            l = (i + 3) % vertexPairsArray.length;
            insetVertexArray.push(this.vectorLineIntersection(
                vertexPairsArray[i],
                vertexPairsArray[j],
                vertexPairsArray[k],
                vertexPairsArray[l]
            ));

        }
        return insetVertexArray;
    },

    //for more complex polygon editing functions its helpful to convert a polygon to a vertex queue, basicly its a
    //list of vertexes that have a pointer to the next vertex in the array, this asumes of course that you have your
    //vertexs in consequtive order and not in some kind of crazy order...
    convertPolygonToVertexQueue: function (vertexArray) {
        var vertexQueue = [];
        var i;
        for (i = 0; i < vertexArray.length; ++i) {
            //gives the next index so the vertex knows what to point at
            var j = (i + 1) % vertexArray.length;
            vertexQueue.push({
                vertex: vertexArray[i],
                nextVertex: j,
                //the unused part is for later when the vertexQueue as to turn back into a vertexArray
                unused: true
            });
        }
        return vertexQueue;
    },
    //converts a vertexQueue to polygons(and array of vertexArrays)
    //remember for christ's sakes that this is an array of an arrays
    //if you do something silly like this.convertVertexQueueToPolygons(foo)[i].x
    //then you deserve the millions of errors in the console
    convertVertexQueueToPolygons: function (vertexQueue) {
        var vertexArrays = [];
        var i;
        for (i = 0; i < vertexQueue.length; ++i) {
            //if its allready been prossesed then don't bother
            if (vertexQueue[i].unused) {
                //creates a new vertexArray and provides an index for it
                var polygonIndex = vertexArrays.push([]) - 1;
                var currentVertexIndex = i;
                //this searches though the array for the next vertex adds
                //it unless its allready been added
                while (vertexQueue[currentVertexIndex].unused) {
                    //shoves the vertex into the new polygon
                    vertexArrays[polygonIndex].push(vertexQueue[currentVertexIndex].vertex);
                    //we are done with this vertex so lets false it
                    vertexQueue[currentVertexIndex].unused = false;
                    //changes the current vertex to the next
                    currentVertexIndex = vertexQueue[currentVertexIndex].nextVertex;
                }
            }
        }
        return vertexArrays;
    },
    //check if point is within a line,tollrence can be added if you want to make sure
    checkInsideLine: function (point, vertexA, vertexB, tollrence) {
        //i am going to asume that they are more or less parrallel because
        //this is kind of a pointless function if they are not
        var normal = this.normaliseVector(this.relativeVector(vertexA, vertexB));
        var dotPoint = this.dotProduct2D(normal, point);
        var dotStart = this.dotProduct2D(vertexA, normal);
        var dotEnd = this.dotProduct2D(vertexB, normal);
        if (dotStart > dotEnd) {
            var buffer = dotEnd;
            dotEnd = dotStart;
            dotStart = buffer;
        }
        dotEnd -= tollrence;
        dotStart += tollrence;
        return dotPoint > dotStart && dotPoint < dotEnd;
    },
    //deconstructs complex(self intersecting) vertexQueues into non-complex vertexQueues that are actualy useful
    decomposeVertexQueue: function (vertexQueue) {
        //this is an O(n*n) bordering on O(n*n*n) so don't exspect it to happen in a hurry
        //what we do here is check each side agenst all the other sides to see if they intersect
        //if they do we do whats known as a vertex tie and add two new vertex and conect them to seperate the vertexes
        //if there is a tie found then the for loop is reset because new passes need to
        //be done to make sure that the whole queue is covered
        //bulky yes but thougher

        //var orignalLength = vertexQueue.length;
        var i;
        for (i = 0; i < vertexQueue.length; ++i) {
            var j = vertexQueue[i].nextVertex;
            var k;
            for (k = 0; k < vertexQueue.length; ++k) {
                var l = vertexQueue[k].nextVertex;
                //this should go without saying but we do not want to
                //intersect test the same index or ajoining indexes
                if (i !== k && i !== l && k !== j) {
                    var intersection = this.vectorLineIntersection(
                        vertexQueue[i].vertex,
                        vertexQueue[j].vertex,
                        vertexQueue[k].vertex,
                        vertexQueue[l].vertex
                    );
                    //checks that the intersection is with in tollrences to prevent nearby vectors from interfearing
                    if (this.checkInsideLine(intersection, vertexQueue[i].vertex, vertexQueue[j].vertex, 0) &&
                        this.checkInsideLine(intersection, vertexQueue[k].vertex, vertexQueue[l].vertex, 0)) {
                        var a = vertexQueue.push({vertex: intersection, nextVertex: l, unused: true}) - 1;
                        var b = vertexQueue.push({vertex: intersection, nextVertex: j, unused: true}) - 1;
                        vertexQueue[i].nextVertex = a;
                        vertexQueue[k].nextVertex = b;
                        console.log("twist");
                        //because we have restructured the vertexQueue we have to revaulate it
                        //and that means starting from index 0 sorry about that
                        i = 0;
                        k = vertexQueue.length;
                    }
                }
            }
        }
        return vertexQueue;
    },
    //just invokes the decomposeVertexQueue function but does the queue conversion for you
    decomposePolygon: function (vertexArray) {
        var vertexQueue = this.convertPolygonToVertexQueue(vertexArray);
        var decomposedVertexQueue = this.decomposeVertexQueue(vertexQueue);
        return this.convertVertexQueueToPolygons(decomposedVertexQueue);
    },
    //cuts a vertex que using a line segment this is really useful, but takes vertex queues if they scare you,
    //you can use the polygon wrapper bellow it, but that will make you a wimp.
    cutVertexQueue: function (vertexQueue, cutVectorStart, cutVectorEnd) {
        //basicly this is a twp step prossess, first we go down
        //and find where the line intersects the vertex queue
        //while we are doing that we are ordering them based on there distance from the cut start
        //on ce we have complied this order pairs list, we then connect them to there relavent pairs
        //taking advantage of the fact that a polygons are intersected in pairs and entry point and an exit point

        //create an array for the sorted vectors
        var cutPointsArray = [];
        var cutUnit = this.normaliseVector(this.relativeVector(cutVectorStart, cutVectorEnd));
        //this stops us from trying to cut the added vertex pointer, because that would be stupid.
        var orignalLenth = vertexQueue.length;
        var i, j;
        for (i = 0; i < orignalLenth; ++i) {
            j = (i + 1) % orignalLenth;
            var intersection = this.vectorLineIntersection(
                vertexQueue[i].vertex,
                vertexQueue[j].vertex,
                cutVectorStart,
                cutVectorEnd
            );

            if (this.checkInsideLine(intersection, vertexQueue[i].vertex, vertexQueue[j].vertex, 0)) {
                //creates the new vertexs pointers,
                //note that a points to -1 this is so we know its not been asigned yet
                var a = vertexQueue.push({vertex: intersection, nextVertex: -1, unused: true}) - 1;
                var b = vertexQueue.push({vertex: intersection, nextVertex: -1, unused: true}) - 1;
                //dot product
                var dotProduct = this.dotProduct2D(cutUnit, intersection);
                //creates the new cut point 
                var newCutPoint = {dot: dotProduct, pointerA: a, pointerB: b, pointerC: i, pointerD: j};
                //sets the indexSlot to zero because this would be the
                //place to put it if the cut point array has nothing in it
                var indexSlot = 0;
                for (var c = cutPointsArray.length - 1; c >= 0; --c) {
                    //if the current dot product is less then the
                    if (cutPointsArray[c].dot > dotProduct) {
                        indexSlot = c;
                    }
                }
                //add the new cut point at the new location
                cutPointsArray.splice(indexSlot, 0, newCutPoint);
            }

        }
        //phew, now thats over we should have an array with a load of new cut points
        //and another array with an order reffrence to each one, now we itterate thought his reffrence to
        //join the pairs up (often with most polygons this will only be two,but more can happen)
        //var i,j;
        for (i = 0; i < cutPointsArray.length; i += 2) {
            j = (i + 1);
            //lets exstract all the pointers because i don't fancy mile long complex statements :)
            var iA = cutPointsArray[i].pointerA;
            var iB = cutPointsArray[i].pointerB;
            var iC = cutPointsArray[i].pointerC;
            var iD = cutPointsArray[i].pointerD;
            var jA = cutPointsArray[j].pointerA;
            var jB = cutPointsArray[j].pointerB;
            var jC = cutPointsArray[j].pointerC;
            var jD = cutPointsArray[j].pointerD;
            //then we just apply the re-jigging
            vertexQueue[iA].nextVertex = jB;
            vertexQueue[jA].nextVertex = iB;
            vertexQueue[iC].nextVertex = iA;
            vertexQueue[jC].nextVertex = jA;
            vertexQueue[iB].nextVertex = iD;
            vertexQueue[jB].nextVertex = jD;
        }

        //and the cut is done, was that so painful?
        //if you where the polygon it would be quite agonising
        return vertexQueue;

    },
    //cuts a polygon using the decomposeVertexArray but wraps it up with a nice polygon method
    cutPolygon: function (vertexArray, cutVectorStart, cutVectorEnd) {
        var vertexQueue = this.convertPolygonToVertexQueue(vertexArray);
        var cutDownVertexQueue = this.cutVertexQueue(vertexQueue, cutVectorStart, cutVectorEnd);
        return this.convertVertexQueueToPolygons(cutDownVertexQueue);
    },
    //because polygon manuipuluation can often leave you with a lot of undesirable polygons
    //inverted ones, giant ones, less than 3 vertex ones it is often a good idea to run the
    //a polygon list though a cleaner like this to get rid of the chaff, remeber binning all
    //the bits of paper after you finshed cutting something out, like that.
    //this dosen't actualy cull the polygon it just says if it should be kept or not
    //true means its a keeper, false means cut it loose.
    cullDirtyPolygon: function (vertexArray) {
        //lets start off optamistic that this is a perfectly valid polygon free of deformatys
        var cullBool;

        //each culling check will be enclosed inside a check to see if another has already sujested culling
        //no point spending time checking crap

        //checks if the polygon has 3 or more sides
        cullBool = vertexArray.length < 3;

        //check if the polygon is clockwise
        if (!cullBool) {
            cullBool = !this.isPolygonClockWise(vertexArray);
        }

        return cullBool;
    },
    getMinMaxVectorPair: function (vertexArray, normal) {
        var maxIndex = 0;
        var minIndex = 0;
        var maxDot = null;
        var minDot = null;
        var i;
        for (i = 0; i < vertexArray.length; ++i) {
            var dotProduct = this.dotProduct2D(normal, vertexArray[i]);
            if (maxDot === null || dotProduct > maxDot) {
                minIndex = i;
                maxDot = dotProduct;
            }
            if (minDot === null || dotProduct < minDot) {
                maxIndex = i;
                minDot = dotProduct;
            }
        }
        return {vector: {min: vertexArray[minIndex], max: vertexArray[maxIndex]}, dot: {min: minDot, max: maxDot}};
    },
    //things left to write
    //produce two vectors that show the topleft most and bottom left most bounding box of a polygon
    //actualy ill throw in the others for giggles lets get a quad going
    defineBoundingBox: function (vertexArray, yAxisNormal) {
        //first we calulate the y axis because Y not? heh get it?
        var xAxisNormal = this.calculatePerpendicularVector(yAxisNormal);
        var xAxisResultsDot = this.getMinMaxVectorPair(vertexArray, xAxisNormal).dot;
        var yAxisResultsDot = this.getMinMaxVectorPair(vertexArray, yAxisNormal).dot;
        var left = this.scaleVector(xAxisNormal, xAxisResultsDot.min);
        var right = this.scaleVector(xAxisNormal, xAxisResultsDot.max);
        var top = this.scaleVector(yAxisNormal, yAxisResultsDot.min);
        var bottom = this.scaleVector(yAxisNormal, yAxisResultsDot.max);
        var topLeft = this.sumVectors(top, left);
        var topRight = this.sumVectors(top, right);
        var bottomRight = this.sumVectors(bottom, right);
        var bottomLeft = this.sumVectors(bottom, left);

        return [topLeft, topRight, bottomRight, bottomLeft];
    },
    griddlePolygon: function (vertexArray, normal, xSize, ySize) {
        var runningPoly = [vertexArray];
        var gridQuad = this.defineBoundingBox(vertexArray, normal);
        var xLength = this.vectorDistance(gridQuad[0], gridQuad[3]);
        var yLength = this.vectorDistance(gridQuad[0], gridQuad[1]);
        var xBlocks = Math.floor(xLength / xSize);
        var yBlocks = Math.floor(yLength / ySize);
        var i, j, polygons,error,firstCutPoint,secondCutPoint;
        for (i = 1; i < xBlocks; ++i) {
            polygons = [];
            error = Math.sin(i + gridQuad[1].x) * 0.1;
            firstCutPoint = this.lerpVector(gridQuad[1], gridQuad[2], ((i / xBlocks) + error));
            secondCutPoint = this.lerpVector(gridQuad[0], gridQuad[3], ((i / xBlocks) + error));

            for (j = 0; j < runningPoly.length; ++j) {
                polygons = polygons.concat(this.cutPolygon(runningPoly[j], firstCutPoint, secondCutPoint));
            }
            runningPoly = polygons;

        }
        for (i = 1; i < yBlocks; ++i) {
            polygons = [];

            for (j = 0; j < runningPoly.length; ++j) {
                error = Math.sin((i + gridQuad[1].x) * j) * 0.05;
                firstCutPoint = this.lerpVector(gridQuad[0], gridQuad[1], ((i / yBlocks) + error));
                secondCutPoint = this.lerpVector(gridQuad[3], gridQuad[2], ((i / yBlocks) + error));
                polygons = polygons.concat(this.cutPolygon(runningPoly[j], firstCutPoint, secondCutPoint));
            }
            runningPoly = polygons;

        }
        return runningPoly;
    },
    //find longest side is really useful if you want to make carful toplogical choices
    findLongestSide: function (vertexArray) {
        var currentLongestSide = 0;
        var currentLongestLength = 0;
        var i, j;
        for (i = 0; i < vertexArray.length; ++i) {
            j = (i + 1) % vertexArray.length;
            var testDistance = this.vectorDistanceSquared(vertexArray[i], vertexArray[j]);
            if (testDistance > currentLongestLength) {
                currentLongestLength = testDistance;
                currentLongestSide = i;
            }
        }
        return currentLongestSide;
    }

};

CC.Geo = CC.Geometry;
CC.Geom = CC.Geometry;