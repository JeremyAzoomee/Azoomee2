/*
Continually positions all registered elements to their specified anchor locations.
 */

CC.Phaser.Anchor =
{
    LOCATION_TL: "topLeft",
    LOCATION_TR: "topRight",
    LOCATION_BL: "bottomLeft",
    LOCATION_BR: "bottomRight",
    elements: [],
    margin: 0,
    resize: function()
    {
        for (var i = 0; i < this.elements.length; i++)
        {
            this.positionElement(this.elements[i].element, this.elements[i].location);
        }
    },
    positionElement: function(element, location)
    {
        var game = CC.Phaser.currentGame;

        switch(location)
        {
            case this.LOCATION_TL:
                element.position.x = (element.width - game.width) / 2 + this.margin;
                element.position.y = (element.height - game.height) / 2 + this.margin;
                break;
            case this.LOCATION_TR:
                element.position.x = (game.width - element.width) / 2 - this.margin;
                element.position.y = (element.height - game.height) / 2 + this.margin;
                break;

            case this.LOCATION_BL:
                element.position.x = (element.width - game.width) / 2 + this.margin;
                element.position.y = (game.height - element.height) / 2 - this.margin;
                break;
            case this.LOCATION_BR:
                element.position.x = (game.width - element.width) / 2 - this.margin;
                element.position.y = (game.height - element.height) / 2 - this.margin;
                break;

        }
    },
    addElement: function(element, location)
    {
        // add the element to the list for repositioning
        this.elements.push({element:element, location:location});

        // position the element to begin with, as resize will not have occurred yet
        this.positionElement(element, location);
    },
    removeElement: function(element)
    {
        for (var i = 0; i < this.elements.length; i++)
        {
            if (this.elements[i].element == element)
            {
                this.elements.splice(i, 1);
            }
        }
    },
    removeAll: function()
    {
        this.elements = [];
    }
};