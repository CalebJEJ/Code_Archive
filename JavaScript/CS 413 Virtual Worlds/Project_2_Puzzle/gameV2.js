var gameport = document.getElementById("gameV2");

var renderer = PIXI.autoDetectRenderer({width: 640,
                                        height: 320,
                                        backgroundColor: 0x000000});
gameport.appendChild(renderer.view);

var stage = new PIXI.Container();

//Load up the start screen
var startScreen = PIXI.Texture.from("resources/GameV2StartScreen.png");

var startSprite = new PIXI.Sprite(startScreen);

startSprite.anchor.x = 0.5;
startSprite.anchor.y = 0.5;

startSprite.position.x = 320;
startSprite.position.y = 160;

//set the screen to start the game on click
startSprite.interactive = true;
startSprite.on('mousedown', startGame);

stage.addChild(startSprite);

//begin animation
animate();

//set up variables for later sprites
var backgroundSprite;
var backgroundSprite2;
var howToPlayTextSprite;
var menuTextSprite;
var levelZeroSprite;
var optionsMenuCloseSprite;
var optionsMenuBackSprite;
var optionsMenuMusicSprite;
var optionsMenuCloseSprite;
var creditsCloseSprite;
var creditsIconSprite;

//global music
var backgroundSong;


function startGame()
{
	//load music
	backgroundSong = new PIXI.sound.Sound.from('resources/song01.mp3');
	//play music
	backgroundSong.loop = true;
	backgroundSong.play();
	
	//remove all children from the stage
	stage.removeChildren();
	//load in the "how to play" screen sprite sheet
	PIXI.Loader.shared.add('resources/waterAssets.json').load(setup);
	
	function setup() {
	    // create an array of textures from an image path
		var waterFrames = [];

		for (var i = 1; i < 6; i++) {

			waterFrames.push(PIXI.Texture.from('Water_Background0' + i + '.png'));
		}

		// create our animated sprite with the array
		backgroundSprite = new PIXI.AnimatedSprite(waterFrames);
		
		backgroundSprite.anchor.set(0.5);

		backgroundSprite.position.x = 160;
		backgroundSprite.position.y = 160;
		
		backgroundSprite.animationSpeed = 0.01;
		
		backgroundSprite.gotoAndPlay(0);

		//set the screens to go to the menu on click
		backgroundSprite.interactive = true;
		backgroundSprite.on('mousedown', loadMenu);
		
		stage.addChild(backgroundSprite);
		
		//second half of the how to screen background
		
		backgroundSprite2 = new PIXI.AnimatedSprite(waterFrames);
		
		backgroundSprite2.position.x = 480;
		backgroundSprite2.position.y = 160;
		
		backgroundSprite2.anchor.set(0.5);
		
		backgroundSprite2.animationSpeed = 0.01;
		
		backgroundSprite2.gotoAndPlay(0);
		
		stage.addChild(backgroundSprite2);
		
		
		//load in the text of how to play on top
		
		var howToPlayText = PIXI.Texture.from("resources/How_To_Play_Text.png");

		howToPlayTextSprite = new PIXI.Sprite(howToPlayText);

		howToPlayTextSprite.anchor.x = 0.5;
		howToPlayTextSprite.anchor.y = 0.5;
		
		howToPlayTextSprite.position.x = -320;
		howToPlayTextSprite.position.y = 160;
		
		createjs.Tween.get(howToPlayTextSprite.position).to({x: 320, y: 160}, 2000, createjs.Ease.bounceOut);

		
		//set the text to go to the menu on click as well just in case
		howToPlayTextSprite.interactive = true;
		howToPlayTextSprite.on('mousedown', loadMenu);
		
		stage.addChild(howToPlayTextSprite);
		
	}

	
}


function loadMenu()
{
	//remove all uneeded children from the stage
		//use a tween to have the text leave
	createjs.Tween.get(howToPlayTextSprite.position).to({x: 320, y: -320}, 2000, createjs.Ease.bounceOut);
	//turn off interactivity with background
	backgroundSprite.interactive = false;
	
	//load in the text for the menu
	var menuText = PIXI.Texture.from("resources/Menu_text.png");

	menuTextSprite = new PIXI.Sprite(menuText);

	menuTextSprite.anchor.x = 0.5;
	menuTextSprite.anchor.y = 0.5;

	menuTextSprite.position.x = 320;
	menuTextSprite.position.y = 480;
	
	createjs.Tween.get(menuTextSprite.position).to({x: 320, y: 160}, 3000, createjs.Ease.circOut);
	
	//once the menu is in postion get rid of the offscreen how to text
	if(menuTextSprite.position.y == 160)
	{
		stage.removeChild(howToPlayTextSprite);
	}

	stage.addChild(menuTextSprite);
	
	//load in the options menu sprite
	var options = PIXI.Texture.from("resources/Options_Sprite.png");

	optionsSprite = new PIXI.Sprite(options);

	optionsSprite.anchor.x = 1;
	optionsSprite.anchor.y = 0;

	optionsSprite.position.x = 640;
	optionsSprite.position.y = 480;
	
	createjs.Tween.get(optionsSprite.position).to({x: 640, y: 0}, 2500, createjs.Ease.circOut);

	//set the level icon to go to the level on click
	optionsSprite.interactive = true;
	optionsSprite.on('mousedown', loadOptions);

	stage.addChild(optionsSprite);
	
	
	//load in the sprites for the levels

    //Level Zero sprite
	var levelZero = PIXI.Texture.from("resources/Level0Menu.png");

	levelZeroSprite = new PIXI.Sprite(levelZero);

	levelZeroSprite.anchor.x = 0.5;
	levelZeroSprite.anchor.y = 0.5;

	levelZeroSprite.position.x = 160;
	levelZeroSprite.position.y = 480;
	
	createjs.Tween.get(levelZeroSprite.position).to({x: 160, y: 160}, 2000, createjs.Ease.circOut);

	//set the level icon to go to the level on click
	howToPlayTextSprite.interactive = true;
	howToPlayTextSprite.on('mousedown', loadLevel0);

	stage.addChild(levelZeroSprite);
	
	
	//Level One sprite
	var levelOne = PIXI.Texture.from("resources/Level01Menu.png");

	levelOneSprite = new PIXI.Sprite(levelOne);

	levelOneSprite.anchor.x = 0.5;
	levelOneSprite.anchor.y = 0.5;

	levelOneSprite.position.x = 320;
	levelOneSprite.position.y = 480;
	
	createjs.Tween.get(levelOneSprite.position).to({x: 160, y: 160}, 2500, createjs.Ease.circOut);

	//set the level icon to go to the level on click
	howToPlayTextSprite.interactive = true;
	howToPlayTextSprite.on('mousedown', loadLevel0);

	stage.addChild(levelOneSprite);
	
	//credits sprite
		
	var creditsIcon = PIXI.Texture.from("resources/Credit_Icon_Sprite.png");

	creditsIconSprite = new PIXI.Sprite(creditsIcon);

	creditsIconSprite.position.x = 580;
	creditsIconSprite.position.y = 1000;
	
	createjs.Tween.get(creditsIconSprite.position).to({x: 580, y: 280}, 2500, createjs.Ease.cricOut);
	
	//set the credit icon to show credits on click
	creditsIconSprite.interactive = true;
	creditsIconSprite.on('mousedown', showCredits);

	stage.addChild(creditsIconSprite);
	
}

//loads the options menu on top of whatever is on screen
function loadOptions()
{
	//load in a box to hold the options
	
	var optionsMenuBack = PIXI.Texture.from("resources/Option_Menu_Back.png");

	optionsMenuBackSprite = new PIXI.Sprite(optionsMenuBack);

	optionsMenuBackSprite.anchor.x = 0.5;
	optionsMenuBackSprite.anchor.y = 0.5;

	optionsMenuBackSprite.position.x = 320;
	optionsMenuBackSprite.position.y = 0;
	
	optionsMenuBackSprite.scale.x = 1.8;
	optionsMenuBackSprite.scale.y = 1.8;
	
	createjs.Tween.get(optionsMenuBackSprite.position).to({x: 320, y: 160}, 2000, createjs.Ease.circOut);

	stage.addChild(optionsMenuBackSprite);
	
	//option 1 = restart the game
	//TODO
	
	//option 2 = turn music on or off
	
	var optionsMenuMusic = PIXI.Texture.from("resources/Sound_Sprite.png");

	optionsMenuMusicSprite = new PIXI.Sprite(optionsMenuMusic);

	optionsMenuMusicSprite.anchor.x = 0.5;
	optionsMenuMusicSprite.anchor.y = 0.5;

	optionsMenuMusicSprite.position.x = 200;
	optionsMenuMusicSprite.position.y = 0;
	
	createjs.Tween.get(optionsMenuMusicSprite.position).to({x: 200, y: 160}, 2500, createjs.Ease.circOut);
	
	optionsMenuMusicSprite.interactive = true;
	optionsMenuMusicSprite.on('mousedown', switchOnOffMusic);

	stage.addChild(optionsMenuMusicSprite);
	
	//create a sprite for showing if music is on or off
	var optionsMenuMusicOff = PIXI.Texture.from("resources/X_Cross_Sprite.png");

	optionsMenuMusicOffSprite = new PIXI.Sprite(optionsMenuMusicOff);

	optionsMenuMusicOffSprite.anchor.x = 0.5;
	optionsMenuMusicOffSprite.anchor.y = 0.5;

	optionsMenuMusicOffSprite.position.x = 200;
	optionsMenuMusicOffSprite.position.y = 0;
	
	createjs.Tween.get(optionsMenuMusicOffSprite.position).to({x: 200, y: 160}, 2500, createjs.Ease.circOut);
	
	//if music is playing set visiblity to false
	if(backgroundSong.volume == 1)
	{
		optionsMenuMusicOffSprite.visible = false;
	}

	stage.addChild(optionsMenuMusicOffSprite);
	
	//close option menu sprite
	var optionsMenuClose = PIXI.Texture.from("resources/X_Cross_Sprite.png");

	optionsMenuCloseSprite = new PIXI.Sprite(optionsMenuClose);

	optionsMenuCloseSprite.anchor.x = 0.5;
	optionsMenuCloseSprite.anchor.y = 0.5;

	optionsMenuCloseSprite.position.x = 600;
	optionsMenuCloseSprite.position.y = 0;
	
	createjs.Tween.get(optionsMenuCloseSprite.position).to({x: 600, y: 30}, 2500, createjs.Ease.circOut);
	
	optionsMenuCloseSprite.interactive = true;
	optionsMenuCloseSprite.on('mousedown', closeMenu);

	stage.addChild(optionsMenuCloseSprite);
}

//function closes out the options menu
function closeMenu()
{
	createjs.Tween.get(optionsMenuCloseSprite.position).to({x: 600, y: -1000}, 2500, createjs.Ease.circOut);
	createjs.Tween.get(optionsMenuBackSprite.position).to({x: 320, y: -1000}, 2500, createjs.Ease.circOut);
	createjs.Tween.get(optionsMenuMusicOffSprite.position).to({x: 320, y: -1000}, 2500, createjs.Ease.circOut);
	createjs.Tween.get(optionsMenuMusicSprite.position).to({x: 320, y: -1000}, 2500, createjs.Ease.circOut);
	
	//once off screen remove them
	if(optionsMenuCloseSprite.position.y < -900)
	{
		stage.removeChild(optionsMenuCloseSprite);
	}
	
		
	if(optionsMenuBackSprite.position.y < -900)
	{
		stage.removeChild(optionsMenuBackSprite);
	}
	
	if(optionsMenuMusicOffSprite.position.y < -900)
	{
		stage.removeChild(optionsMenuMusicOffSprite);
	}
	
		
	if(optionsMenuMusicSprite.position.y < -900)
	{
		stage.removeChild(optionsMenuMusicSprite);
	}
}

//function that stops or starts music if already stopped
function switchOnOffMusic()
{
	if(backgroundSong.volume == 1)
	{
		backgroundSong.volume = 0;
		optionsMenuMusicOffSprite.visible = true;
	}
	else
	{
		backgroundSong.volume = 1;
		optionsMenuMusicOffSprite.visible = false;
	}
}

//function to show credits from the main menu
function showCredits()
{
	//hide the menu offscreen
	createjs.Tween.get(levelZeroSprite.position).to({x: -1000, y: 160}, 2000, createjs.Ease.circOut);
	createjs.Tween.get(optionsSprite.position).to({x: -1000, y: 0}, 2000, createjs.Ease.circOut);
	createjs.Tween.get(menuTextSprite.position).to({x: -1000, y: 160}, 2000, createjs.Ease.circOut);
	createjs.Tween.get(creditsIconSprite.position).to({x: -1000, y: 320}, 2500, createjs.Ease.bounceOut);
	
	//load the credits text
	
	var creditsText = PIXI.Texture.from("resources/Credit_Text_Sprite.png");

	creditsTextSprite = new PIXI.Sprite(creditsText);

	creditsTextSprite.anchor.x = 0.5;
	creditsTextSprite.anchor.y = 0.5;

	creditsTextSprite.position.x = 1000;
	creditsTextSprite.position.y = 160;
	
	createjs.Tween.get(creditsTextSprite.position).to({x: 320, y: 160}, 2500, createjs.Ease.bounceOut);

	stage.addChild(creditsTextSprite);
	
	//close credits sprite
	var creditsClose = PIXI.Texture.from("resources/X_Cross_Sprite.png");

	creditsCloseSprite = new PIXI.Sprite(creditsClose);

	creditsCloseSprite.anchor.x = 0.5;
	creditsCloseSprite.anchor.y = 0.5;

	creditsCloseSprite.position.x = -1000;
	creditsCloseSprite.position.y = 30;
	
	createjs.Tween.get(creditsCloseSprite.position).to({x: 40, y: 30}, 2500, createjs.Ease.bounceOut);
	
	creditsCloseSprite.interactive = true;
	creditsCloseSprite.on('mousedown', hideCredits);

	stage.addChild(creditsCloseSprite);
}

//function to hide credits from the main menu
function hideCredits()
{
	//hide and close credits
	createjs.Tween.get(creditsCloseSprite.position).to({x: 1000, y: 30}, 2000, createjs.Ease.bounceOut);
	createjs.Tween.get(creditsTextSprite.position).to({x: 1000, y: 30}, 2000, createjs.Ease.bounceOut);
	
	if(creditsCloseSprite.position.x > 900)
	{
		stage.removeChild(creditsCloseSprite);
	}
	
	if(creditsTextSprite.position.x > 900)
	{
		stage.removeChild(creditsTextSprite);
	}
	
	//reshow the menu offscreen
	createjs.Tween.get(levelZeroSprite.position).to({x: 160, y: 160}, 2000, createjs.Ease.circOut);
	createjs.Tween.get(optionsSprite.position).to({x: 640, y: 0}, 2500, createjs.Ease.circOut);
	createjs.Tween.get(menuTextSprite.position).to({x: 320, y: 160}, 3000, createjs.Ease.circOut);
	createjs.Tween.get(creditsIconSprite.position).to({x: 580, y: 280}, 2500, createjs.Ease.bounceOut);
}

//this function loads up level 0
function loadLevel0()
{
	//remove all non background children from the stage
		//create a keboard listener for WASD keys for character movement
		//the listen will also listen for the O key which will open an options menu
			//Option menu conatian the following:
				//A button for turning music off
				//A reset the level button
				//A go back to menu button
			//levels have the following:
				//A background sprite
				//Several sprites for the puzzle blocks
					//One puzzle black is hidden the rest are jumbled
				//A completed version of the puzzle
			//Levels end when each of the jumbled puzzle's pieces postioning matches the complete ones
}

//this function loads up level 1
function loadLevel1()
{
	//remove all non background children from the stage
		//create a keboard listener for WASD keys for character movement
		//the listen will also listen for the O key which will open an options menu
			//Option menu conatian the following:
				//A button for turning music off
				//A reset the level button
				//A go back to menu button
			//levels have the following:
				//A background sprite
				//Several sprites for the puzzle blocks
					//One puzzle black is hidden the rest are jumbled
				//A completed version of the puzzle
			//Levels end when each of the jumbled puzzle's pieces postioning matches the complete ones
}

//a function for keeping the animations going
function animate()
{
	requestAnimationFrame(animate);
	renderer.render(stage);
}
