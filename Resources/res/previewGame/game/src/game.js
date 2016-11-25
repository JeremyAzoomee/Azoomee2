var ccPhaserApp;

function correctSize()
{
    var objWrapper = document.getElementById('wrapper');

    if (objWrapper)
    {
        objWrapper.style.height = window.innerHeight + "px";
        objWrapper.style.width = window.innerWidth + "px";
    }

    ccPhaserApp = new Phaser.Game(960, 640, Phaser.AUTO, 'wrapper');

    ccPhaserApp.state.add('boot', new BootState());
    ccPhaserApp.state.add('load', new LoadState());
    ccPhaserApp.state.add('start', new StartState());
    ccPhaserApp.state.add('level', new SelectionState());
    ccPhaserApp.state.add('game', new GameState());
    ccPhaserApp.state.add('gameWin', new GameWinState());
    ccPhaserApp.state.add('end', new EndState());

    CC.resizeEnabled = true;

    this.urlSplit = window.location.href.split("?");
    if(this.urlSplit.length > 1)
    {
        CC.firstState = "game";
    }
    else
    {
        CC.firstState = "start";
    }
    //CC.assetURL = "";

    ccPhaserApp.state.start('boot');
}

window.setTimeout(function(){ correctSize(); }, 1000);
