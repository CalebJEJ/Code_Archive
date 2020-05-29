var gameport = document.getElementById("gameV1");

var renderer = PIXI.autoDetectRenderer({width: 320,
                                        height: 320,
                                        backgroundColor: 0x000000});
gameport.appendChild(renderer.view);

var stage = new PIXI.Container();

var bkgTexture = PIXI.Texture.from("Field_Sprite_01.png");

var bkgSprite = new PIXI.Sprite(bkgTexture);

bkgSprite.anchor.x = 0.5;
bkgSprite.anchor.y = 0.5;

bkgSprite.position.x = 160;
bkgSprite.position.y = 160;

stage.addChild(bkgSprite);

var rabbitTextureDefault = PIXI.Texture.from("Dagger_Rabbit_Sprite01.png");
var rabbitTextureUp = PIXI.Texture.from("Dagger_Rabbit_Sprite01_Up.png");
var rabbitTextureRight = PIXI.Texture.from("Dagger_Rabbit_Sprite01_Right.png");
var rabbitTextureLeft = PIXI.Texture.from("Dagger_Rabbit_Sprite01.png");
var rabbitTextureDown = PIXI.Texture.from("Dagger_Rabbit_Sprite01_Down.png");

var rabbitSprite = new PIXI.Sprite(rabbitTextureDefault);

rabbitSprite.anchor.x = 0.5;
rabbitSprite.anchor.y = 0.5;

rabbitSprite.position.x = 160;
rabbitSprite.position.y = 160;

stage.addChild(rabbitSprite);

renderer.render(stage);

document.addEventListener('keydown', (input) =>
{
	const keyName = input.key;
	
	if(isMovementKey(keyName))
	{
		moveSprite(keyName, rabbitSprite);
	}
}, false);

function isMovementKey(keyName)
{
	if(keyName == 'd'
	    || keyName == 'a'
        || keyName == 'w'
		|| keyName == 's')
		{
			return true;
		}
	return false;
}

function moveSprite(keyName, sprite)
{
	
	if(keyName == 'd')
	{
		rabbitSprite.texture = rabbitTextureRight;
		if(sprite.position.x <= 288)
		{
			sprite.position.x += 16;
		}
	}
	
		if(keyName == 'a')
	{
		rabbitSprite.texture = rabbitTextureLeft;
		if(sprite.position.x >= 32)
		{
			sprite.position.x -= 16;
		}
	}
	
		if(keyName == 'w')
	{
		rabbitSprite.texture = rabbitTextureUp;
		if(sprite.position.y >= 32)
		{
			sprite.position.y -= 16;
		}
	}
	
		if(keyName == 's')
	{
		rabbitSprite.texture = rabbitTextureDown;
		if(sprite.position.y <= 288)
		{
			sprite.position.y += 16;
		}
	}
	
	renderer.render(stage);
}
