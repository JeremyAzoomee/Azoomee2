/*cc random sequence framework
 *author: Jonathan 'JK' Kernick
 *description: an robust randomiser 
*/

/*
  #####                                                                                
 #     #   ####   #    #   ####   #####  #####   #    #   ####   #####   ####   #####  
 #        #    #  ##   #  #         #    #    #  #    #  #    #    #    #    #  #    # 
 #        #    #  # #  #   ####     #    #    #  #    #  #         #    #    #  #    # 
 #        #    #  #  # #       #    #    #####   #    #  #         #    #    #  #####  
 #     #  #    #  #   ##  #    #    #    #   #   #    #  #    #    #    #    #  #   #  
  #####    ####   #    #   ####     #    #    #   ####    ####     #     ####   #    # 
                                                                                       
*/

//cc Random constructor
CC.Random = function(totalCountOrSequence,delay)
{
	this._delay = delay || 0;
	//chceks if its a total count or a array
	if(totalCountOrSequence.length)
	{
		//if the seqence is an array then copy the reffrence straite over
		this._masterSequence = totalCountOrSequence;
	}
	else
	{
		//if it is not an array then asume it is an integer and use it to generate a linear array
		this._masterSequence = this._generateTheLinearArray(totalCountOrSequence);
	}
	//press a copy of the master array and scramble it.
	this._pressCopy();
	this.scramble();
}
/*
 #     #  #######  #######  #     #  #######  ######    #####  
 ##   ##  #           #     #     #  #     #  #     #  #     # 
 # # # #  #           #     #     #  #     #  #     #  #       
 #  #  #  #####       #     #######  #     #  #     #   #####  
 #     #  #           #     #     #  #     #  #     #        # 
 #     #  #           #     #     #  #     #  #     #  #     # 
 #     #  #######     #     #     #  #######  ######    #####  
                                                               
*/
//set up the prototype for the randomiser
CC.Random.prototype =
{
	//orignal sequence, kept for refrence :Array
	_masterSequence:'no master sequence',
	//current sequence.:Array
	_workingSequence:'no working sequence',
	//current index. :int
	_currentIndex:0,
	//delaycount. :int
	_delay:'no delay',
	//list of items on delay. :Array
	_delayedItems:'no delayed Items',
	//generate the linear array
	_generateTheLinearArray: function(totalCount)//:array
	{
		//return array
		var returnArray = new Array();
		var i = 0;
		//creates fills array with number sequence
		for(i;i < totalCount; ++i)
		{
			returnArray.push(i);
		}
		return returnArray();

	},
	//creates a copy of the master array to work with
	_pressCopy: function()//:void
	{
		//clear the working array
		this._workingSequence = new Array();
		//iterates through every item in the masterarray and copys them to the working array
		var masterSequenceLength = this._masterSequence.length;
		var i = 0;
		for(i = 0;i < masterSequenceLength; ++i)
		{
			this._workingSequence[i] = this._masterSequence[i];
		}
	},
	//checks if the delay has been acountedfor
	_handleDelay: function(index)//:void
	{
		//if delay is valid
		if(this._delay)
		{
			this._delayedItems.push(this._workingSequence.splice(index,1)[0]);

			if(this._delay <= this._delayedItems.length)
			{
				//need to return an item because the delay array has gotten too big
				this._workingSequence.push(this._delayedItems.splice(0,1)[0]);
				this.scramble();
			}

		}
	},
	//creates a copy of the master array to work with
	scramble: function()//:void
	{
		var bufferArray = new Array();
		var workingSequenceLength = this._workingSequence.length;
		var i = 0;
		for(i = 0; i < workingSequenceLength; ++i)
		{
			//picks a random index
			var itemIndex = Math.floor(Math.random()*this._workingSequence.length);
			//splice out an item from the working array
			var item = this._workingSequence.splice(itemIndex,1)[0];
			//add to the buffer
			bufferArray.push(item)
		}
		this._workingSequence = bufferArray;
	},
	//creates a copy of the master array to work with
	reset: function()//:void
	{
		this._pressCopy();
		this._currentIndex = 0;
	},
	//requests the next number
	next: function()//:object
	{
		//current item is chosen because we are about to change it
		var chosenItem = this._workingSequence[this._currentIndex];
		//handles the delay if any
		this._handleDelay(this._currentIndex);
		//advances the current index
		this._currentIndex = (this._currentIndex + 1)%this._workingSequence.length;
		//returns the currentIndex
		return chosenItem;
	},
	//requests a random number from the array
	rand: function()//:object
	{
		//creates a random index
		var randomIndex = Math.floor(this._workingSequence.length*Math.random());
		//current item is chosen because we are about to change it
		var randomItem = this._workingSequence[randomIndex];
		//handles the delay if any
		this._handleDelay(randomIndex);
		//returns the randomItem
		return randomItem;
	}

};