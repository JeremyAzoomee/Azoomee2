CC.Phaser.Audio = function(gameRef)
{
    this.gameRef = gameRef || CC.Phaser.currentGame;
    this.audiosprite;
    this.sounds;
    this.init();
};

// audio types //:ma:todo: not part of the prototype
CC.Phaser.Audio.prototype.TYPE_MISC = "misc";
CC.Phaser.Audio.prototype.TYPE_SFX = "sfx";
CC.Phaser.Audio.prototype.TYPE_MUSIC = "music";

CC.Phaser.Audio.FORCE_SILENCE = false;

CC.Phaser.Audio.prototype.init = function()
{
    this.sounds = [];

    // containers for requested audio to be applied to
    this.sounds[CC.Phaser.Audio.prototype.TYPE_MISC] = [];
    this.sounds[CC.Phaser.Audio.prototype.TYPE_SFX] = [];
    this.sounds[CC.Phaser.Audio.prototype.TYPE_MUSIC] = [];

    if (CC.isMasterAudio)
    {
        this.audiosprite = this.gameRef.add.audioSprite(CC.MASTER_AUDIO_KEY);
        this.audiosprite.allowMultiple = true;
    }
};

/**
 * Plays the specified sound by id and type.
 *
 * @param id (string)           sound id in the audio sprite
 * @param [type] (string)       optional; type of sound; default: CC.Phaser.Audio.prototype.TYPE_MISC
 * @param [volume] (number)     optional, default:1;
 * @param [loop] (boolean)      optional, default:false;
 * @param [fadeIn] (number)      optional, default:0;
 */
CC.Phaser.Audio.prototype.playSound = function(id, type, volume, loop, fadeIn)
{
    var sound;

    if (this.audiosprite)
    {
        sound = this.audiosprite.sounds[id];

        // set the defaults
        type = type || CC.Phaser.Audio.prototype.TYPE_MISC;
        loop = loop || false;

        if (CC.Phaser.Audio.FORCE_SILENCE) volume = 0;
        else volume = volume || 1;

        // apply the sound to the required type group
        this.sounds[type][id] = sound;

        // listen for the end of the sound (if it doesn't loop) and play it
        if (!loop) sound.onStop.addOnce(this.onSoundComplete, this, 0, type);
        if(fadeIn){
            sound.fadeIn(fadeIn, loop, id);
        }
        else
        {
            sound.play(id, undefined, volume, loop);
        }

        // if the sound group is currently muted, mute the sound
        if (type == CC.Phaser.Audio.prototype.TYPE_MISC && AZM.Data.requestMuteAudio) sound.mute = true;
        if (type == CC.Phaser.Audio.prototype.TYPE_MUSIC && AZM.Data.requestMuteMusic) sound.mute = true;
        if (type == CC.Phaser.Audio.prototype.TYPE_SFX && AZM.Data.requestMuteSfx) sound.mute = true;
    }
    else
    {
        console.log("ERROR : CC.Phaser.Audio : playSound > master audio sprite does not exist");
    }
};

/**
 * sound and id come through by default. Type is specified when adding the listener.
 *
 * @param sound
 * @param id
 * @param type
 */
CC.Phaser.Audio.prototype.onSoundComplete = function(sound, id, type)
{
    // remove the sound from the type group
    if (this.sounds[type] && this.sounds[type][id])
    {
        delete this.sounds[type][id];
    }
};

CC.Phaser.Audio.prototype.muteAll = function(mute)
{
    var sound;
    var group;

    if (!CC.Phaser.Audio.FORCE_SILENCE)
    {
        for (var type in this.sounds)
        {
            group = this.sounds[type];

            for (var id in group)
            {
                sound = group[id];
                sound.mute = mute;
            }
        }
    }
};

CC.Phaser.Audio.prototype.muteGroup = function(type, mute)
{
    var sound;
    var group;

    if (!CC.Phaser.Audio.FORCE_SILENCE)
    {
        for (var id in this.sounds[type])
        {
            group = this.sounds[type];
            sound = group[id];
            sound.mute = mute;
        }
    }
};

/**
 * Stops all sounds.
 */
CC.Phaser.Audio.prototype.stopAll = function(testType)
{
    var group;
    var sound;

    for (var type in this.sounds)
    {
        if(testType === undefined || testType === type)
        {
            group = this.sounds[type];

            for (var id in group)
            {
                sound = group[id];
                sound.stop();
                delete group[id];
            }
        }
    }
};

/**
 * Stops the specified sound and kills the sound.
 *
 * @param id (string)           sound id in the audio sprite
 * @param [type] (string)       optional; type of sound; default: CC.Phaser.Audio.prototype.TYPE_MISC
 */
CC.Phaser.Audio.prototype.stopSound = function(id, type)
{
    var group;
    var sound;

    group = this.sounds[type];

    for (var soundId in group)
    {
        if (soundId == id)
        {
            sound = group[soundId];
            sound.stop();
            delete group[soundId];
        }
    }
};

/**
 * Pauses all sounds.
 */
CC.Phaser.Audio.prototype.pauseAll = function()
{
    for (var type in this.sounds)
    {
        this.pauseGroup(type);
    }
};

/**
 * Resumes all sounds.
 */
CC.Phaser.Audio.prototype.resumeAll = function()
{
    for (var type in this.sounds)
    {
        this.resumeGroup(type);
    }
};

/**
 * Pauses all sounds within the specified group.
 *
 * @param type:String               sound group type
 * @param [killSounds:Array]        optional; list of sounds to kill, so they are not resumed later
 */
CC.Phaser.Audio.prototype.pauseGroup = function(type, killSounds)
{
    var group;
    var sound;

    group = this.sounds[type];

    // kill off those on the kill list
    if (killSounds)
    {
        for (var i = 0; i < killSounds.length; i++)
        {
            if (group[killSounds[i]])
            {
                sound = group[killSounds[i]];
                sound.stop();
                delete group[killSounds[i]];
            }
        }
    }

    // pause the remaining sounds in the group
    for (var id in group)
    {
        sound = group[id];
        sound.pause();
    }
};

/**
 * Resumes all sounds within the specified group.
 */
CC.Phaser.Audio.prototype.resumeGroup = function(type)
{
    var group;
    var sound;

    group = this.sounds[type];

    for (var id in group)
    {
        sound = group[id];
        sound.resume();
    }
};