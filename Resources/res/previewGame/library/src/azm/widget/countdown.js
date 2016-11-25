/*
* Constructor
* @time:Integer            time, in seconds for the timer to run for
* @[scope:Object]          optional; scope of the callback function
* @[callback:Function]     optional; function to callback once timer has ended
* @[gameRef:Phaser.Game]   Phaser game instance
 */
AZM.Widget.Countdown = function(scope, callback, gameRef)
{
    this.gameRef = gameRef || CC.Phaser.currentGame;
    Phaser.Group.call(this, this.gameRef, null, null, true);

    this.scope = scope;
    this.callback = callback;

    this.endTime = 60;
    this.progressMaxWidth;
    this.prevMs;
    this.totalMs = 0;
    this.prevSecs = 0;
    this.active = false; // whether the countdown has been started and is active, even if currently paused
    this.paused = false; // whether the countdown has been paused
    this.base;
    this.progress;
    this.border;

    this.init();
};

AZM.Widget.Countdown.prototype = Object.create(Phaser.Group.prototype);
AZM.Widget.Countdown.prototype.constructor = AZM.Widget.Countdown;

AZM.Widget.Countdown.prototype.destroy = function()
{
    if (this.base) this.base.destroy();
    if (this.progress) this.progress.destroy();
    if (this.border) this.border.destroy();

    this.gameRef = null;
    this.scope = null;
    this.callback = null;
    this.base = null;
    this.progress = null;
    this.border = null;

    Phaser.Group.prototype.destroy.call(this);
};

AZM.Widget.Countdown.prototype.getSecsElapsed = function()
{
    return Math.round(this.totalMs / 10);
};

AZM.Widget.Countdown.prototype.changeColour = function(num)
{
    var name = "timeout_bar/timeout_bar_progress_0" + num + ".png";
    var key = CC.Phaser.reverseImageLookUpTable[name];
    this.progress.loadTexture(key, name);
};

AZM.Widget.Countdown.prototype.init = function()
{
    this.base = CC.Phaser.Utility.Display.createSprite("timeout_bar/timeout_bar_base.png");
    this.progress = CC.Phaser.Utility.Display.createSprite("timeout_bar/timeout_bar_progress_00.png", true);
    this.border = CC.Phaser.Utility.Display.createSprite("timeout_bar/timeout_bar_border.png");

    this.progress.x -= this.progress.width / 2;
    this.progress.y -= this.progress.height / 2;

    this.progressMaxWidth = this.progress.width;

    this.add(this.base);
    this.add(this.progress);
    this.add(this.border);
};

AZM.Widget.Countdown.prototype.update = function(delta)
{
    var currMs; // current milliseconds on the computer's clock
    var deltaMs; // milliseconds elapsed since last update
    var secsElapsed; // total number of seconds elapsed
    var timeRatio; // time ratio for scaling the sprite

    if (this.active && !this.paused)
    {
        // stop execution of code if total time has elapsed
        if(this.prevSecs < this.endTime)
        {
            currMs = new Date().getTime();
            deltaMs = currMs - this.prevMs;

            // store the new ms as the previous for next time
            this.prevMs = currMs;

            // add to the total ms
            this.totalMs += deltaMs;

            // the secs are divided by 100 due to requirement of smoother progress bar (used to 1000)
            secsElapsed = this.getSecsElapsed();

            // change the display every second, not every update
            if (secsElapsed > this.prevSecs)
            {
                if (secsElapsed > (this.endTime * 0.7)) this.changeColour(2); //:ma:todo: don't keep setting
                else if (secsElapsed > (this.endTime * 0.3)) this.changeColour(1);

                // store the new secs as the previous for next time
                this.prevSecs = secsElapsed;

                // scale the sprite according to the time elapsed, not allowing it to be less than the 0
                timeRatio = (this.endTime - secsElapsed) / this.endTime;
                this.progress.width = Math.max(0, timeRatio) * this.progressMaxWidth;
            }
        }
        else
        {
            this.active = false;
            if (this.scope && this.callback) this.callback.call(this.scope);
        }
    }
};

AZM.Widget.Countdown.prototype.reset = function()
{
    this.totalMs = 0;
    this.prevSecs = 0;
    this.active = false;
    this.progress.width = this.progressMaxWidth;
};

AZM.Widget.Countdown.prototype.start = function(time)
{
    // the secs are multiplied by 100 due to requirement of smoother progress bar
    this.endTime = time * 100;

    this.reset();
    this.active = true;
    this.prevMs = new Date().getTime();
};

AZM.Widget.Countdown.prototype.pause = function(pause)
{
    this.paused = pause;
    if (!pause) this.prevMs = new Date().getTime();
};

AZM.Widget.Countdown.prototype.stop = function()
{
    this.pause(true);
    this.active = false;
};

/**
 * Removes the specified amount of time from the current time.
 *
 * @param time
 */
AZM.Widget.Countdown.prototype.loseTime = function(time)
{
    this.prevMs -= time * 1000;
};
