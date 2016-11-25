/**
 * Defines the top-level objects for the CC library.
 */

var CC = {
    MASTER_AUDIO_KEY:"masterAudio",
    debug:true,
    isMobile:navigator.userAgent.match(/BlackBerry|Windows Phone|Android|iPad|iPhone|Silk|iPod|Kindle|Nexus/i) !== null,
    log:function(debugLog) { if (typeof console !== "undefined" && console.log && CC.debug) { console.log(debugLog); }},
    disableConsole:function() { console = {log:function(){}}; CC.debug = false; },
    error:function(errorLog) { if (typeof console !== "undefined" && console.log &&  CC.debug) { CC.log("ERROR: " + errorLog); }},
    resizeEnabled:false,
    firstState:"",
    assetURL:"",
    isAudioManagerCreated:false, // whether the audio manager has been created
    isMasterAudio:false, // whether there is a master audio sprite available; there should be only one but this is the main one
    audioDecoded:false // whether the downloaded audio has been decoded and ready to play
};

//log out to let the person know that they could clear there cache
CC.log("Can't see an error log? Try clearing your cache!");

CC.isMobile = CC.isMobile || (
    navigator.userAgent.match(/Chrome/i)!== null &&
    navigator.userAgent.match(/Apple/i)!== null &&
    navigator.userAgent.match(/Linux/i)!== null);

