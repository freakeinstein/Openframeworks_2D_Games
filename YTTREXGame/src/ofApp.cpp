#include "ofApp.h"


Game game;
ofVec2f gravityForce(0,1);

//--------------------------------------------------------------
void ofApp::setup(){
	ofVec2f startPoint(100,100);
	dino.body.setLocation(startPoint);
}

//--------------------------------------------------------------
void ofApp::update(){
	if(dino.body.getLocation().y < FLOOR_Y)
	{
		dino.body.applyForce(gravityForce);
	}
	else
	{
		float ycomp = dino.body.getVelocity().y;
		ofVec2f vec(0,-ycomp);
		dino.body.addVelocity(vec);
		vec.set(dino.body.getLocation().x, FLOOR_Y);
		dino.body.setLocation(vec);
	}
	dino.body.applyPhysics();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(255);
	if(game.gameInitFlag == 0)
		dino.display(DINO_IDLE);
	else
		dino.display(DINO_RUNNING);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(dino.body.getLocation().y == FLOOR_Y)
	{
		ofVec2f jumpForce(0,-30);
		dino.body.applyForce(jumpForce);
		game.gameInitFlag = 1;
	}
}


// DINO MEMBERS
void Dino::display(DINO_MODES_t mode)
{
	static int activeLeg = LEFT_LEG;
	float legTriggerSec = 0.3;
	static int leg_frameCounter = 0;
	float frameRate = ofGetFrameRate();

	switch(mode)
	{
		case DINO_RUNNING	: 	if(activeLeg == LEFT_LEG)
								{
									material.drawMaterial(body.getLocation().x,body.getLocation().y, 40,43,6,123);
									if(leg_frameCounter >= frameRate*legTriggerSec)
									{
										leg_frameCounter = 0;
										activeLeg = RIGHT_LEG;
									}
								}
								if(activeLeg == RIGHT_LEG)
								{
									material.drawMaterial(body.getLocation().x,body.getLocation().y, 40,43,55,123);
									if(leg_frameCounter >= frameRate*legTriggerSec)
									{
										leg_frameCounter = 0;
										activeLeg = LEFT_LEG;
									}
								}
								break;
		case DINO_DUCK		: 	if(activeLeg == LEFT_LEG)
								{
									material.drawMaterial(body.getLocation().x,body.getLocation().y, 56,25,7,201);
									if(leg_frameCounter >= frameRate*legTriggerSec)
									{
										leg_frameCounter = 0;
										activeLeg = RIGHT_LEG;
									}
								}
								if(activeLeg == RIGHT_LEG)
								{
									material.drawMaterial(body.getLocation().x,body.getLocation().y, 56,25,7,201);
									if(leg_frameCounter >= frameRate*legTriggerSec)
									{
										leg_frameCounter = 0;
										activeLeg = LEFT_LEG;
									}
								}
								break;
		case DINO_IDLE		: 	material.drawMaterial(body.getLocation().x,body.getLocation().y, 40,43,7,47);
								break;
		case DINO_DEAD		: 	material.drawMaterial(body.getLocation().x,body.getLocation().y, 40,43,69,48);
								break;
	}

	leg_frameCounter ++;
}


// MATERIAL CLASS MEMBERS DEFINED
void Material::loadSpriteSheet(const string &fileName)
{
	spriteSheet.load(fileName);
}
void Material::drawMaterial(float x, float y, float w, float h, float sx, float sy)
{
	spriteSheet.drawSubsection(x,y,w,h,sx,sy);
}


// BODY CLASS MEMBERS DEFINED

void Body::setLocation(ofVec2f inVal)
{
	location.set(inVal.x,inVal.y);
}
ofVec2f Body::getLocation()
{
	return location;
}
void Body::setVelocity(ofVec2f inVal)
{
	velocity.set(inVal.x,inVal.y);
}
ofVec2f Body::getVelocity()
{
	return velocity;
}
void Body::setAcceleration(ofVec2f inVal)
{
	acceleration.set(inVal.x,inVal.y);
}
ofVec2f Body::getAcceleration()
{
	return acceleration;
}
void Body::addVelocity(ofVec2f inVal)
{
	velocity += inVal;
}
void Body::addAcceleration(ofVec2f inVal)
{
	acceleration += inVal;
}
void Body::applyForce(ofVec2f inVal)
{
	addAcceleration(inVal/mass);
}
void Body::applyAcceleration()
{
	velocity += acceleration;
	acceleration.set(0,0);
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











