#pragma once

#include <iostream>
#include <raylib.h>
#include <string>
#include "PictureManager.h"

using namespace std;

class Character
{

public:

	PictureManager * pmanager;


	Texture2D * characterParts[8];//Body,head,feet,hands,mouth,eyes, hair, hat
	Color characterColor[8];
	float partRotation[8];
	Vector2 partSizes = { 200,300 };

	//Game mode varibles
	bool civilian = true;

	//Location of character
	float x = 0;
	float y = 0;
	//movement stuff
	Vector2 v = { 0,0 };
	float MaxV = .2;
	Vector2 a = { 0,0 };
	float speedOrignial = .5;
	float speed = speedOrignial;
	float acceloration = 0.05;
	float breakingForce = 1;
	//WanderGoal
	Vector2 goal = { 0,0 };
	//Values
	bool player = false;
	int ranDeath = 1;
	float life = 1;

	//Animation stuff

	int dirAF = -1;
	int dirAH = 1;
	float maxRotF = 15;
	float maxRotH = 30;
	float animSpeedF = 1;
	float animSpeedH = 2.8;

	float scale = 1;


	////////////////////////////////////
	
	void playerKey() {

		
		//If alive do key checks
		if (life > 0) {
			
			//Movement
			if (IsKeyDown(KEY_LEFT_SHIFT)) {
				speed = speedOrignial * 2;
			}
			else {
				speed = speedOrignial;
			}
			if (IsKeyDown(KEY_W)) {
				v.y = -1;
			}
			else if (IsKeyDown(KEY_S)) {
				v.y = 1;
			}
			else {
				v.y = v.y / 4;
				if (v.y < .05) {
					v.y = 0;
				}
			}
			if (IsKeyDown(KEY_A)) {
				v.x = -2;
			}
			else if (IsKeyDown(KEY_D)) {
				v.x = 2;
			}
			else {
				v.x = v.x / 4;
				if (v.x < .05) {
					v.x = 0;
				}
			}
		}

	}
	void setUpPlayerAssassin() {

		player = true;
		civilian = false;

		MaxV = GetRandomValue(1500, 2000) / 1000;

		for (int i = 0; i < 8; i++) {
			characterColor[i] = BLACK;
			partRotation[i] = 0;
		}
	}
	void setUpAiAssassin() {
		civilian = false;
	}

	void SetVelocity() {
		if(life > 0){
			//AI set acceloration
			if (!player) {
				//Set new accelorations
				a = unitVec(goal.x - x, goal.y - y);
			}
			//Add it to velocity
			int breakx = 0;
			if (v.x * a.x < 0) { breakx = 1; }
			int breaky = 0;
			if (v.y * a.y < 0) { breaky = 1; }

			v.x += +a.x * (acceloration + breakingForce * breakx);
			v.y += +a.y * (acceloration + breakingForce * breaky);

			//Make it not move too fast
			float length = sqrt(v.x * v.x + v.y * v.y);
			if (length > MaxV) {
				v.x = v.x / length * MaxV;
				v.y = v.y / length * MaxV;
			}
			//Check to see if you've reached the goal! If so, re-randomize the goal
			if (!player && (sqrt(pow(goal.x - x, 2) + pow(goal.y - y, 2)) < 60)) {
				MaxV = GetRandomValue(1500, 2000) / 1000;
				//Set goal to move towards
				goal.x = GetRandomValue(0, GetScreenWidth() / scale);
				goal.y = GetRandomValue(0, GetScreenHeight()/ scale*.5) + 0.5*GetScreenHeight()/ scale;
			}
		}
	}

	void animateWhileWalking() {
		float speed = sqrt(v.x * v.x + v.y * v.y);
		if (speed > MaxV / 5) {
			//Animation rotatoin of feet
			partRotation[2] += dirAF * animSpeedF * speed;
			partRotation[4] += dirAF * animSpeedF * speed * .2;
			partRotation[5] += dirAH * animSpeedH * speed * .13;
			if (abs(partRotation[2]) > maxRotF) {
				partRotation[2] = maxRotF * dirAF;
				dirAF *= -1;
			}
			//amimation rotation of hands
			partRotation[3] += dirAH * animSpeedH * speed;
			if (abs(partRotation[3]) > maxRotH * speed/MaxV) {
				partRotation[3] = maxRotH * speed/MaxV * dirAH;
				dirAH *= -1;
			}
		}
	}
	
	void killed() {		
		life = -1;
		v.x = 0;
		v.y = 0;
		a.x = 0;
		a.y = 0;
		ranDeath = GetRandomValue(0, 1);
		if (ranDeath == 0) {
			ranDeath = -1;
		}
	}
	void animateDeath() {

		if (life <= -1) {
			for (int i = 0; i < 8; i++) {
				partRotation[i] += 4*sqrt(i)*pow(-1,i)*ranDeath;
			}
			life--;
			if (life < -20) {
				life = 0;
			}
		}
	}

	void StartCharacter() {


		life = 1;

		MaxV = GetRandomValue(1500, 3000) / 1000;
		//Set location
		x = GetRandomValue(0, GetScreenWidth()/scale);
		y = GetRandomValue(0, GetScreenHeight()/ scale * .4) + 0.6 * GetScreenHeight()/ scale;
		//Set goal to move towards
		goal.x = GetRandomValue(0, GetScreenWidth()/ scale);
		goal.y = GetRandomValue(0, GetScreenHeight()/ scale * .5) + 0.5 * GetScreenHeight()/ scale;
		
		setUpColor();

	}
	void setUpPictures() {
		if (pmanager != nullptr) {
			//Initialize pictures
			//Body,head,feet,hands,mouth,eyes, hair, hat
			characterParts[0] = &pmanager->body[GetRandomValue(1, pmanager->bodyN-1)];
			characterParts[1] = &pmanager->head[GetRandomValue(1, pmanager->headN-1)];
			characterParts[2] = &pmanager->feet[GetRandomValue(1, pmanager->feetN-1)];
			characterParts[3] = &pmanager->hands[GetRandomValue(0, pmanager->handsN-1)];
			characterParts[4] = &pmanager->mouth[GetRandomValue(0, pmanager->mouthN-1)];
			characterParts[5] = &pmanager->eyes[GetRandomValue(1, pmanager->eyesN-1)];
			characterParts[6] = &pmanager->hair[GetRandomValue(1, pmanager->hairN-1)];
			characterParts[7] = &pmanager->hat[GetRandomValue(0, pmanager->hatN-1)];
		}
		else {
			cout << "NUUUUUUUUUUUUUUUUUUUUUUULLLLLLLLLLLLLLLL\n";
		}
	}
	void setUpColor() {
		for (int i = 0; i < 8; i++) {

			partRotation[i] = 0;

			int ran = GetRandomValue(1, 10);
			if (i == 1) {
				//Make slight skin color change
				int r1 = 255 - GetRandomValue(0, 20);
				float r2 = 255 - GetRandomValue(0, 20);
				int r3 = 255 - GetRandomValue(0, 20);
				characterColor[i] = Color{ (uint8_t)r1,(uint8_t)r2,(uint8_t)r3,255 };
			}
			else if (i != 0 || ran == 1) {
				//make it so everythings colors white except for colored shirts
				characterColor[i] = WHITE;
			}
			else if (ran == 2) {
				characterColor[i] = DARKGRAY;
			}
			else if (ran == 3) {
				characterColor[i] = GREEN;
			}
			else if (ran == 4) {
				characterColor[i] = BLUE;
			}
			else if (ran == 5) {
				characterColor[i] = RED;
			}
			else if (ran == 6) {
				characterColor[i] = YELLOW;
			}
			else if (ran == 7) {
				characterColor[i] = ORANGE;
			}
			else if (ran == 8) {
				characterColor[i] = PURPLE;
			}
			else if (ran == 9) {
				characterColor[i] = GRAY;
			}
			else if (ran == 10) {
				characterColor[i] = DARKBROWN;
			}

		}
	}

	inline Vector2 unitVec(float x, float y) {
		// Get absolute value of each vector
		float ax = abs(x);
		float ay = abs(y);

		// Create a ratio
		float ratio = 1 / max(ax, ay);
		ratio = ratio * (1.29289 - (ax + ay) * ratio * 0.29289);

		// Multiply by ratio
		x = x * ratio;
		y = y * ratio;

		return { (x),(y) };
	}
};

