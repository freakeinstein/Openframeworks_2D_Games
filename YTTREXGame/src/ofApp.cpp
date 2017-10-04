#include "ofApp.h"

#define FLOOR_Y 300
#define LEFT_CLIP 100
#define RIGHT_CLIP 644

ofVec2f gravity(0, -9.8);

Game game;
World world;
World ground1;
World ground2;
Enemy en;

void ofApp::haveBeenHit()
{
	game.gameInitFlag = 0;
}

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowShape(744, 400);

	ofAddListener(en.hitEvent, this, &ofApp::haveBeenHit);

	ofVec2f startPoint(644,FLOOR_Y);
	en.setLocation(startPoint);
	en.setEnemyType((ENEMY_TYPE_t)ofRandom(0,3));

	startPoint.set(300,FLOOR_Y);
	dino.body.setLocation(startPoint);
	world.loadSpriteSheet();

	startPoint.set(100,FLOOR_Y);
	ground1.loadSpriteSheet();
	ground1.setSprite(544,15,16,273,100,644,1);
	ground1.setLocation(startPoint);

	startPoint.set(644,FLOOR_Y);
	ground2.loadSpriteSheet();
	ground2.setSprite(544,15,16,273,100,644,1);
	ground2.setLocation(startPoint);

}

//--------------------------------------------------------------
void ofApp::update(){

	if(game.gameInitFlag == 1)
	{
		game.incrementTime();
		game.incrementLevel();

		ground1.update();
		ground2.update();

		en.CheckHitEventHappened(dino.body.getLocation()+dino.getCurrentDimension(), dino.getCurrentDimension().x);
		en.update();
	
		if(dino.body.getLocation().y < FLOOR_Y)
		{
			ofVec2f gravityForce(0,1);
			dino.body.applyForce(gravityForce);
		}
		else
		{
			float ycomp = dino.body.getVelocity().y;
			ofVec2f vec(0,-ycomp);
			dino.body.addVelocity(vec);
			ofVec2f vec1(dino.body.getLocation().x,FLOOR_Y);
			dino.body.setLocation(vec1);
		}
		dino.body.applyPhysics();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(255);

	ground1.display();
	ground2.display();

	en.display();

	if(game.gameInitFlag == 0)
		dino.display(DINO_IDLE);
	else
		dino.display(DINO_RUNNING);


	ofSetColor(255);
	ofFill();
	ofDrawRectangle(0,0,100,400);
	ofDrawRectangle(644,0,100,400);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(dino.body.getLocation().y == FLOOR_Y)
	{
		ofVec2f jumpForce(0,-15);
		dino.body.applyForce(jumpForce);
		game.gameInitFlag = 1;
	}
}

// -------- DEFINE: enemy members ----------------
void Enemy::CheckHitEventHappened(ofVec2f inVal, int inValWidth)
{
	if(location.x < inVal.x && (location.x+currentDimension.x) > (inVal.x - inValWidth) && location.y < inVal.y)
	{
		ofNotifyEvent(hitEvent);
	}
}
void Enemy::setLocation(ofVec2f inVal)
{
	location = inVal;
}
ofVec2f Enemy::getLocation()
{
	return location;
}
void Enemy::setEnemyType(ENEMY_TYPE_t type)
{
	enType = type;
}
void Enemy::update()
{
	if(location.x+currentDimension.x <= LEFT_CLIP)
	{
		setEnemyType((ENEMY_TYPE_t)ofRandom(0,3));
		location.x = RIGHT_CLIP + ofRandom(200);
	}
	else
	{
		location.x -= game.level;
	}
}
void Enemy::display()
{
	switch(enType)
	{
		case ONE_SMALL: world.material.drawMaterial(getLocation().x, getLocation().y-33, 15, 33, 227, 117);
						currentDimension.x = 15;
						currentDimension.y = 33;
						break;
		case ONE_LARGE: world.material.drawMaterial(getLocation().x, getLocation().y-46, 23, 46, 227, 62);
						currentDimension.x = 23;
						currentDimension.y = 46;
						break;
		case TWO_SMALL: world.material.drawMaterial(getLocation().x, getLocation().y-33, 32, 33, 161, 116);
						currentDimension.x = 32;
						currentDimension.y = 33;
						break;
		case THREE_SMALL: world.material.drawMaterial(getLocation().x, getLocation().y-79, 33, 79, 157, 69);
						currentDimension.x = 33;
						currentDimension.y = 79;
						break;
	}
}

// -------- DEFINE: WORLD MEMBERS ----------------------
void World::loadSpriteSheet()
{
	material.loadSpriteSheet("dinosprite.png");
}
void World::setLocation(ofVec2f inVal)
{
	resetLocation = inVal;
	location = inVal;
}
void World::setSprite(int w, int h, int sx, int sy, int cb, int ce, int loop)
{
	sw = w;
	sh = h;
	ssx = sx;
	ssy = sy;
	scb = cb;
	sce = ce;
	sloop = loop;
}
void World::update()
{
	if(location.x+sw <= scb)
	{
		location = resetLocation;
		location.x = scb+sw-game.level;
	}
	if(sloop != 0)
	{
		location.x -= game.level;
	}
}
void World::display()
{
	int startw_t = 0;
	int endw_t = 0;

	// if(location.x < scb)
	// {
	// 	startw_t = scb - location.x;
	// 	material.drawMaterial(scb,location.y,sw-startw_t,sh,ssx+startw_t,ssy);
	// }
	// else if(location.x+sw > sce)
	// {
	// 	endw_t = location.x+sw - sce;
	// 	material.drawMaterial(location.x,location.y,sw-endw_t,sh,ssx,ssy);
	// }
	// else
	// {
		material.drawMaterial(location.x,location.y,sw,sh,ssx,ssy);
	// }

	
}

// ------- DEFINE: Game members --------------
void Game::incrementTime()
{
	time ++;
}
void Game::incrementLevel()
{
	if(time % 5000 == 0)
	{
		level ++;
	}
}

//-------- DEFINE: dino members ------------------
ofVec2f Dino::getCurrentDimension()
{
	return currentDimension;
}
void Dino::display(DINO_MODES_t mode){
	float legTriggerSec = 0.3;
	static int leg_frameCounter = 0;
	float frameRate = ofGetFrameRate();
	static int activeLeg = LEFT_LEG;

	switch(mode){
		case DINO_RUNNING: 	if(activeLeg == LEFT_LEG){
								world.material.drawMaterial(body.getLocation().x, body.getLocation().y-43, 40, 43, 6, 123);
								currentDimension.x = 40;
								currentDimension.y = 43;
								if(leg_frameCounter >= frameRate*legTriggerSec)
								{
									leg_frameCounter = 0;
									activeLeg = RIGHT_LEG;
								}
							}
							else{
								world.material.drawMaterial(body.getLocation().x, body.getLocation().y-43, 40, 43, 55, 123);
								currentDimension.x = 40;
								currentDimension.y = 43;
								if(leg_frameCounter >= frameRate*legTriggerSec)
								{
									leg_frameCounter = 0;
									activeLeg = LEFT_LEG;
								}
							}
							break;
		case DINO_DUCK: 	if(activeLeg == LEFT_LEG){
								world.material.drawMaterial(body.getLocation().x, body.getLocation().y-25, 56, 25, 7, 201);
								currentDimension.x = 56;
								currentDimension.y = 25;
								if(leg_frameCounter >= frameRate*legTriggerSec)
								{
									leg_frameCounter = 0;
									activeLeg = RIGHT_LEG;
								}
							}
							else{
								world.material.drawMaterial(body.getLocation().x, body.getLocation().y-25, 56, 25, 73, 201);
								currentDimension.x = 56;
								currentDimension.y = 25;
								if(leg_frameCounter >= frameRate*legTriggerSec)
								{
									leg_frameCounter = 0;
									activeLeg = LEFT_LEG;
								}
							}
							break;
		case DINO_IDLE: 	world.material.drawMaterial(body.getLocation().x, body.getLocation().y-43, 40, 43, 7, 47);
							break;
		case DINO_DEAD: 	world.material.drawMaterial(body.getLocation().x, body.getLocation().y-43, 40, 43, 69, 48);
							break;
	}


	leg_frameCounter ++;
}



//-------- DEFINE: material members ------------------
void Material::loadSpriteSheet(const string &fileName){
	spriteSheet.load(fileName);
}

void Material::drawMaterial(float x, float y, float w, float h, float sx, float sy){
	spriteSheet.drawSubsection(x, y, w, h, sx, sy);
}

//-------- DEFINE: object members ------------------
void Body::setLocation(ofVec2f inVal){
	location.set(inVal.x, inVal.y);
}

ofVec2f Body::getLocation(){
	return location;
}

void Body::setVelocity(ofVec2f inVal){
	velocity.set(inVal.x, inVal.y);
}

ofVec2f Body::getVelocity(){
	return velocity;
}
void Body::setAcceleration(ofVec2f inVal){
	acceleration.set(inVal.x, inVal.y);
}
ofVec2f Body::getAcceleration(){
	return acceleration;
}
void Body::addVelocity(ofVec2f inVal){
	velocity += inVal;
}
void Body::addAcceleration(ofVec2f inVal){
	acceleration += inVal;
}
void Body::applyForce(ofVec2f inVal)
{
	addAcceleration(inVal/mass);
}
void Body::applyAcceleration()
{
	velocity += acceleration;
	acceleration.set(0, 0);
}
void Body::updateLocation()
{
	location += velocity;
}
void Body::applyPhysics()
{
	applyAcceleration();
	velocity.limit(velocityLimit);
	updateLocation();
}





