/**
 * Created by jonathan.kernick on 08/01/2016.
 */

/**
 * Created by jonathan.kernick on 07/01/2016.
 */

var BasicBallPhysics = function()
{
    this.ball = {x:0,y:0,radius:30,radiusSqr:30*30,velocity:{x:0,y:0}};
    this.oldPos = {};
    this.speedLimit = 600;
    this.speedLimitSqr = this.speedLimit*this.speedLimit;
    this.gravity = 1200;
    this.rectangles = []
};

BasicBallPhysics.prototype.setBall = function(x,y,radius){
    this.ball.x = x||this.ball.x;
    this.ball.y = y||this.ball.y;
    this.ball.radius = radius||this.ball.radius;
    this.ball.radiusSqr = this.ball.radius * this.ball.radius;
};
BasicBallPhysics.prototype.addRect = function(x,y,width,height,hitFunc){
    var rect = {"x":x,"y":y,"width":width,"height":height,"hitFunc":hitFunc,enabled:true};
    this.rectangles.push(rect);
    return rect;
};


BasicBallPhysics.prototype.updatePhysics = function(delta){
    var i,rect,physicsData;

    delta = 0.03*0.7;

    this.ball.velocity.y += this.gravity*delta;
   /* var speed = CC.Geometry.vectorLengthSquared(this.ball.velocity);
    if(speed > this.speedLimitSqr && !this.ball.speedLimitPass)
    {
        speed = Math.sqrt(speed);
        this.ball.velocity.x = (this.ball.velocity.x/speed)*this.speedLimit;
        this.ball.velocity.y = (this.ball.velocity.y/speed)*this.speedLimit;
    }*/
    this.oldPos.x = this.ball.x;
    this.oldPos.y = this.ball.y;
    this.ball.x += this.ball.velocity.x*delta;
    this.ball.y += this.ball.velocity.y*delta;
    for(i = 0; i < this.rectangles.length; ++i){
        rect = this.rectangles[i];
        physicsData = BasicBallPhysics.calculateCircleRectCollsion(this.ball,rect);
        if(physicsData){
            this.ball.x = physicsData.x;
            this.ball.y = physicsData.y;
            rect.hitFunc();
            if(CC.Geometry.dotProduct2D(physicsData.norm,this.ball.velocity) < 0){

                this.ball.speedLimitPass = false;
                CC.Geometry.reflect2DRef(this.ball.velocity,physicsData.norm);
                if(Math.abs(this.ball.velocity.y) < 10)
                {
                    this.ball.velocity.x *= 0.5;
                }
                else
                {
                    this.ball.velocity.x *= 0.99;
                }
                this.ball.velocity.y *= 0.8;
              //  ball.velocity.x *=0.8;
               // ball.velocity.y *=0.8;
            }

        }
    }
    //CC.log(this.ball.x);
    if(this.ball.x === undefined || this.ball.y === undefined || isNaN(this.ball.x) || isNaN(this.ball.y) || CC.Geometry.vectorDistanceSquared(this.ball,this.oldPos) > 500*500)
    {

        this.ball.x = this.oldPos.x;
        this.ball.y = this.oldPos.y;
        this.ball.velocity.x = 0;
        this.ball.velocity.y = 0;
    }
};


BasicBallPhysics.calculateCircleRectCollsion = function(circle,rect){
    var collsionFeedBack = false;
    var fixedX,fixedY,fixedDist,fixedTest;
    if(rect.enabled)
    {
    var clampedCircle = {x:Math.clamp(rect.x,(rect.x+rect.width),circle.x),
    y:Math.clamp(rect.y,(rect.y+rect.height),circle.y)};

    var relativeVector = {};
    relativeVector.x = circle.x-clampedCircle.x;
    relativeVector.y = circle.y-clampedCircle.y;
        if(!relativeVector.x && !relativeVector.y)
        {
            fixedDist = Math.abs(rect.x-circle.x);
            fixedX = rect.x;
            fixedY = circle.y;
            fixedTest = Math.abs(rect.y-circle.y);
            if(fixedTest < fixedDist)
            {
                fixedDist = fixedTest;
                fixedX = circle.x;
                fixedY = rect.y;
            }
            fixedTest = Math.abs((rect.x+rect.width)-circle.x);
            if(fixedTest < fixedDist)
            {
                fixedDist = fixedTest;
                fixedX = (rect.x+rect.width);
                fixedY = circle.y;
            }
            fixedTest = Math.abs((rect.y+rect.height)-circle.y);
            if(fixedTest < fixedDist)
            {
                fixedDist = fixedTest;
                fixedX = circle.x;
                fixedY = (rect.y+rect.height);
            }
            relativeVector.x = fixedX-circle.x;
            relativeVector.y = fixedY-circle.y;
        }
    var distanceSqr = CC.Geometry.vectorLengthSquared(relativeVector);
    if(distanceSqr < circle.radiusSqr){
        collsionFeedBack = {};
        var distance = Math.sqrt(distanceSqr);
        var contactNormal = {};
        contactNormal.x = relativeVector.x/distance;
        contactNormal.y = relativeVector.y/distance;
        collsionFeedBack.x = ((contactNormal.x*circle.radius)+clampedCircle.x);
        collsionFeedBack.y = ((contactNormal.y*circle.radius)+clampedCircle.y);



        collsionFeedBack.norm = contactNormal;
    }

    }

    return collsionFeedBack;
};