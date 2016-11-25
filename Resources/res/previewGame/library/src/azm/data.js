/**
 * Holds all Azoomee-specific data from the hard-coded config and applies any additional restored data from local
 * storage.
 */

AZM.Data.config; // populated by data loaded from config.js
AZM.Data.selectedLevel; // current level or version selected within the level state; zero-based
AZM.Data.levelsComplete = []; // number of levels unlocked or complete
AZM.Data.currentGameResult;
AZM.Data.totalZoomCount = 0; // total number of zooms collected
AZM.Data.distance = 0;
AZM.Data.newRecord = false; // whether the previous score beat the record
AZM.Data.requestMuteAudio = false;
AZM.Data.requestMuteMusic = false;
AZM.Data.requestMuteSfx = false;

AZM.Data.DEFAULT_SAVE_NAME = "default";
AZM.Data.APP_VARS_SAVE_NAME = "app_vars";

// data storage keys
AZM.Data.VIEWED_HELP = "viewedHelp";

// completion states
AZM.Data.LEVEL_LOCKED = -1;
AZM.Data.LEVEL_UNLOCKED = 0;
AZM.Data.LEVEL_COMPLETE_1 = 1;
AZM.Data.LEVEL_COMPLETE_2 = 2;
AZM.Data.LEVEL_COMPLETE_3 = 3;

// populates config data from specified json
AZM.Data.applyData = function(data)
{
    this.config = data;

    // ensure no more than 6 levels can be created
    AZM.Data.config.app_vars.levels = Math.min(AZM.Data.config.app_vars.levels, 6);

    // set the default level completions
    if (AZM.Data.config.app_vars.customStore)
    {
        AZM.Data.config.app_vars.customStore.levelsComplete = [];

        if (AZM.Data.config.app_vars.levels > 0)
        {
            for (var i = 0; i < AZM.Data.config.app_vars.levels; i++)
            {
                if (i == 0)
                {
                    AZM.Data.config.app_vars.customStore.levelsComplete.push(AZM.Data.LEVEL_UNLOCKED);
                }
                else
                {
                    AZM.Data.config.app_vars.customStore.levelsComplete.push(AZM.Data.LEVEL_LOCKED);
                }
            }
        }
    }
};

/**
 * Processes the specified game result data object, applying to top-level data and holding reference to the result.
 * @param win (boolean)                     whether the game was won
 * @param [result (AZM.Data.Result)]        optional; game result data object
 */
AZM.Data.processGameResult = function(win, result)
{
    result = result || new AZM.Data.Result();

    AZM.Data.currentGameResult = result;
    AZM.Data.totalZoomCount += result.zoomCount;
    AZM.Data.distance = result.distance;
    AZM.Data.newRecord = result.newRecord;

    if (win)
    {
        if (AZM.Data.config.app_vars.levels && AZM.Data.config.app_vars.levels > 0)
        {
            //AZM.Data.unlockedLevelCount++; //:ma:todo: make more intelligent - depends whether we're replaying levels
            // may not even require this any more
        }
    }

    console.log("results processed: " + AZM.Data.totalZoomCount + " : " + AZM.Data.distance + " : " + AZM.Data.newRecord);
};

/**
 * Returns the top-level, app stored data.
 */
AZM.Data.getAppStoredData = function()
{
    return AZM.Data.config.app_vars.customStore;
};

/**
 * Returns the top-level, app session data.
 */
AZM.Data.getAppSessionData = function()
{
    return AZM.Data.config.app_vars.customSession;
};

/**
 * Returns the specified state's stored data. If state is not specified, the current state's data will be returned.
 *
 * @param [stateId]         optional, deafault:current state; state of which the data is required
 * @returns {*}             state data or null, if not available
 */
AZM.Data.getStateStoredData = function(stateId)
{
    stateId = stateId || ccPhaserApp.state.current;

    if (AZM.Data.config.state_vars[stateId] && AZM.Data.config.state_vars[stateId].customStore)
    {
        return AZM.Data.config.state_vars[stateId].customStore;
    }

    return null;
};

/**
 * Returns the specified state's session data. If state is not specified, the current state's data will be returned.
 *
 * @param [stateId]         optional, deafault:current state; state of which the data is required
 * @returns {*}             state data or null, if not available
 */
AZM.Data.getStateSessionData = function(stateId)
{
    stateId = stateId || ccPhaserApp.state.current;

    if (AZM.Data.config.state_vars[stateId] && AZM.Data.config.state_vars[stateId].customSession)
    {
        return AZM.Data.config.state_vars[stateId].customSession;
    }

    return null;
};

/**
 * Sets the specified key/value pair.
 * If isGlobal is false, the data is set on the current state.
 * Data may also be saved to local storage; otherwise, it will be held during the current session only.
 *
 * @param key (string)                key to be used for the value
 * @param value (object)              value to be set
 * @param [isGlobal (boolean)]        optional, default:false; whether the data is to be set globally; otherwise, it will be set against the current state
 * @param [store (boolean)]           optional, default:false; whether the data is to be saved to local storage; otherwise, it will be held during the current session only
 * @param [saveName (string)]         optional; default:AZM.Data.DEFAULT_SAVE_NAME; allows for different profiles, if required
 */
AZM.Data.setVar = function(key, value, isGlobal, store, saveName)
{
    var stateId;

    if (isGlobal)
    {
        if (store) AZM.Data.config.app_vars.customStore[key] = value;
        else AZM.Data.config.app_vars.customSession[key] = value;
    }
    else
    {
        stateId = ccPhaserApp.state.current;

        if (store)
        {
            if (AZM.Data.config.state_vars[stateId] && AZM.Data.config.state_vars[stateId].customStore)
            {
                AZM.Data.config.state_vars[stateId].customStore[key] = value;
            }
        }
        else
        {
            if (AZM.Data.config.state_vars[stateId] && AZM.Data.config.state_vars[stateId].customSession)
            {
                AZM.Data.config.state_vars[stateId].customSession[key] = value;
            }

        }
    }

    if (store) AZM.Data.save(saveName);
};

/**
 * Gets the value for the specified key.
 * If isGlobal is false, the data is retrieved from the current state.
 *
 * @param stored (boolean)            whether the var is stored; otherwise it's a session var
 * @param key (string)                key used for the value
 * @param [isGlobal (boolean)]        optional, default:false; whether the data is retrieved globally; otherwise, it will be retrieved from the current state
 * @returns {*}                       returns the value or null, if it doesn't exist
 */
AZM.Data.getVar = function(stored, key, isGlobal)
{
    var stateId;

    if (isGlobal)
    {
        if (stored)
        {
            if (AZM.Data.config.app_vars.customStore[key])
            {
                return AZM.Data.config.app_vars.customStore[key];
            }
        }
        else
        {
            if (AZM.Data.config.app_vars.customSession[key])
            {
                return AZM.Data.config.app_vars.customSession[key];
            }
        }
    }
    else
    {
        stateId = ccPhaserApp.state.current;

        if (AZM.Data.config.state_vars[stateId])
        {
            if (stored)
            {
                if (AZM.Data.config.state_vars[stateId].customStore)
                {
                    if (AZM.Data.config.state_vars[stateId].customStore[key])
                    {
                        return AZM.Data.config.state_vars[stateId].customStore[key];
                    }
                }
            }
            else
            {
                if (AZM.Data.config.state_vars[stateId].customSession)
                {
                    if (typeof AZM.Data.config.state_vars[stateId].customSession[key] !== "undefined") //:ma:todo: do this on others: Malc (for 0 falsing in if);
                    {
                        return AZM.Data.config.state_vars[stateId].customSession[key];
                    }
                }
            }
        }
    }

    return null;
};

/**
 * Saves the current config data to local storage.
 *
 * @param saveName (string)         optional; default:AZM.Data.DEFAULT_SAVE_NAME; allows for different profiles, if required
 */
AZM.Data.save = function(saveName)
{
    var filename = AZM.Data.getDataFileName(saveName);
    var stateId;
    var configData;

    if (filename)
    {
        configData = {states:{}};

        // set the custom app vars
        if (AZM.Data.config.app_vars.customStore)
        {
            configData[AZM.Data.APP_VARS_SAVE_NAME] = AZM.Data.config.app_vars.customStore;
        }

        // set the custom state vars
        for (var id in ccPhaserApp.state.states)
        {
            stateId = ccPhaserApp.state.states[id].stateId;

            if (AZM.Data.config.state_vars[stateId] && AZM.Data.config.state_vars[stateId].customStore)
            {
                configData.states[stateId] = AZM.Data.config.state_vars[stateId].customStore;
            }
        }

        // save the entire config data
        if (this.canSave())
        {
            localStorage[filename] = JSON.stringify(configData);
        }
        else
        {
            //:ma:todo: save to cookie
        }
    }
};

/**
 * Loads the currently local storage config data and applies to the already-loaded config file's data.
 *
 * @param saveName (string)         optional; default:AZM.Data.DEFAULT_SAVE_NAME; allows for different profiles, if required
 */
AZM.Data.load = function(saveName)
{
    var filename = AZM.Data.getDataFileName(saveName);
    var configData; // data in json then object format

    if (this.canSave())
    {
        configData = localStorage[filename];
    }
    else
    {
        //:ma:todo: save to cookie
    }

    if (filename && configData)
    {
        // convert the data to an object
        configData = JSON.parse(configData);

        // set the custom app vars
        AZM.Data.config.app_vars.customStore = configData[AZM.Data.APP_VARS_SAVE_NAME];

        // ensure that the completed level number is not greater than the number of levels defined
        if (AZM.Data.config.app_vars.customStore.levelsComplete)
        {
            if (AZM.Data.config.app_vars.levels < AZM.Data.config.app_vars.customStore.levelsComplete.length)
            {
                AZM.Data.config.app_vars.customStore.levelsComplete.length = AZM.Data.config.app_vars.levels;
            }
        }

        // set the custom state vars
        for (var stateId in configData.states)
        {
            if (AZM.Data.config.state_vars[stateId])
            {
                AZM.Data.config.state_vars[stateId].customStore = configData.states[stateId];
            }
        }
    }
};

AZM.Data.getDataFileName = function(saveName)
{
    var gameId = AZM.Data.config.app_vars.filename;

    if (gameId)
    {
        saveName = saveName || AZM.Data.DEFAULT_SAVE_NAME;
        return "azoomee_" + gameId + "_" + saveName;
    }

    return null;
};

AZM.Data.canSave = function()
{
    var testKey = 'test', storage = window.sessionStorage;

    try {
        storage.setItem(testKey, '1');
        storage.removeItem(testKey);
        return true;
    }
    catch (error)
    {
        return false;
    }
}

/**
 * Sets the completion state of the specified level.
 *
 * @param level
 * @param state         e.g. AZM.Data.LEVEL_LOCKED
 */
AZM.Data.setLevelCompletion = function(level, state)
{
    // ensure the level is not beyond the levels available
    if (level < AZM.Data.config.app_vars.levels)
    {
        AZM.Data.config.app_vars.customStore.levelsComplete[level] = state;
        this.save();
    }
    else
    {
        console.log("ERROR : AZM.Data.setLevelCompletion > level being set is greater than the levels available: " + level);
    }
};

AZM.Data.getLevelCompletion = function()
{
    if (AZM.Data.config.app_vars.customStore.levelsComplete)
    {
        return AZM.Data.config.app_vars.customStore.levelsComplete;
    }

    return [];
};

AZM.Data.isLevelComplete = function(level)
{
    var completion = AZM.Data.getLevelCompletion();

    if (completion.length > 0)
    {
        if (completion[level] && completion[level] > AZM.Data.LEVEL_UNLOCKED)
        {
            return true;
        }
    }

    return false;
};