#pragma once

#include "ofMain.h"

#define LEFT_LEG 0
#define RIGHT_LEG 1

typedef enum
{
	DINO_RUNNING,
	DINO_DUCK,
	DINO_IDLE,
	DINO_DEAD
}DINO_MODES_t;

typedef enum
{
	ONE_SMALL,
	ONE_LARGE,
	TWO_SMALL,
	THREE_SMALL
}ENEMY_TYPE_t;

class Game
{
	public:
		int score;
		short gameInitFlag;
		int level;
		int time;
		Game()
		{
			score = 0;
			gameInitFlag = 0;
			level = 4;
		}
		void incrementTime();
		void incrementLevel();
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
			velocityLimit = 20;
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

class World
{
	ofVec2f resetLocation;
	ofVec2f location;
	int clipCount;
	int sw;
	int sh;
	int ssx;
	int ssy;
	int scb;
	int sce;
	int sloop;
public:
	Material material;
	World()
	{
		resetLocation.set(0,0);
		location.set(0,0);
		clipCount = 0;
		sw = 0;
		sh = 0;
		ssx = 0;
		ssy = 0;
		scb = 0;
		sce = 0;
		sloop = 0;
	}
	void loadSpriteSheet();
	void setLocation(ofVec2f inVal);
	void setSprite(int w, int h, int sx, int sy, int cb, int ce, int loop);
	void update();
	void display();
};

class Enemy
{
	ofVec2f location;
	ofVec2f currentDimension;
	ENEMY_TYPE_t enType;
public:
	ofEvent<void> hitEvent;
	Enemy()
	{
		location.set(0,0);
		currentDimension.set(0,0);
	}
	void setLocation(ofVec2f inVal);
	ofVec2f getLocation();
	void update();
	void CheckHitEventHappened(ofVec2f inVal, int inValWidth);
	void setEnemyType(ENEMY_TYPE_t type);
	void display();
};

class Dino
{
		Material material;
		ofVec2f currentDimension;
	public:
		Body body;
		Dino()
		{
			currentDimension.set(0,0);
		}
		ofVec2f getCurrentDimension();
		void display(DINO_MODES_t mode);
};

class ofApp : public ofBaseApp{

		Dino dino;
	public:
		void setup();
		void update();
		void draw();
		void haveBeenHit();

		void keyPressed(int key);	
};






