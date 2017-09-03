#pragma once

#include "ofMain.h"

#define VELOCITY_LIMIT 20
#define LEFT_LEG 0
#define RIGHT_LEG 1
#define FLOOR_Y 300

typedef enum
{
	DINO_RUNNING,
	DINO_DUCK,
	DINO_IDLE,
	DINO_DEAD
}DINO_MODES_t;


class Game
{
public:
	int score;
	short gameInitFlag;
	Game()
	{
		score = 0;
		gameInitFlag = 0;
	}
};


class Body
{
	ofVec2f location;
	ofVec2f velocity;
	ofVec2f acceleration;
	float mass;
public:
	int velocityLimit;
	Body()
	{
		velocityLimit = VELOCITY_LIMIT;
		mass = 1;
		location.set(0,0);
		velocity.set(0,0);
		acceleration.set(0,0);
	}
	void setLocation(ofVec2f inVal);
	ofVec2f getLocation();
	void setVelocity(ofVec2f inVal);
	ofVec2f getVelocity();
	void setAcceleration(ofVec2f inVal);
	ofVec2f getAcceleration();
	void addVelocity(ofVec2f inVal);
	void addAcceleration(ofVec2f inVal);
	void applyForce(ofVec2f inVal);
	void applyAcceleration();
	void updateLocation();
	void applyPhysics();
};

class Material
{
	ofImage spriteSheet;
public:
	void loadSpriteSheet(const string &fileName);
	void drawMaterial(float x, float y, float w, float h, float sx, float sy);
};

class Dino
{
	Material material;
public:
	Body body;
	Dino()
	{
		material.loadSpriteSheet("dinosprite.png");
	}
	void display(DINO_MODES_t mode);
};

class ofApp : public ofBaseApp{
	Dino dino;
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
};





















