#pragma once
#include <raylib.h>
#include <iostream>
#include "Character.h"
#include <string>
#include "PictureManager.h"
#include <chrono>
#include <thread>
using namespace std;


class GamManager
{


public:
	
	float scale = 1;
	int characterNum = 500;
	float xOffset = 0;
	float yOffset = 0;

	//Scope stuff
	float xOffsetS = 0;
	float yOffsetS = 0;
	int dirSY = 1;
	int dirSX = 1;
	int maxSX = 30;
	int maxSY = 30;
	float swaySpeed = .1;
	int recoil = 0;
	bool darkScope = false;


	int winner = 0;

	bool paused = true;

	int shotsTaken = 0;
	float timeTaken = 0;

	bool restartGame = true;

	Texture2D bloodT;
	Texture2D darkScopeT;
	Texture2D gunShotT;

	//Assassin varibles
	enum killTypes{stab,shoot,poison };
	killTypes attack = poison;
	float timeLeftKill = 0;
	Vector2 shotLoc = { -1,-1 };
	float maxShootDelay = 1;
	float shootDelay = 0;
	bool shooting = false;
	float canAttack = 0;
	bool stabbed = false;
	Character* poisoned;
	float poisonDelay = 0;


	//early initialize
	//bool touching(Character* c1, Character* c2);
	//bool locationInside(Character* c, float x, float y, float aReq);

	void InitializeGame() {

		//Make the window
		InitWindow(0, 0, "GameScreen");
		SetTargetFPS(100);
		//Create random seed
		SetRandomSeed(time(0));



		//Load all images
		PictureManager pmanager;

		//Make character
		Character* characters = new Character[characterNum];
		//Set the picture manager
		for (int i = 0; i < characterNum; i++) {
			characters[i].pmanager = &pmanager;
		}


		//Make background scope and blood
		Texture2D streetT = LoadTexture("./assets/streat.png");
		Texture2D scopeT = LoadTexture("./assets/scope3.png");
		darkScopeT = LoadTexture("./assets/scope4.png");
		bloodT = LoadTexture("./assets/blood.png");
		gunShotT = LoadTexture("./assets/gunShot.png");

		//Game restart loop
		while (!WindowShouldClose()) {

			if (restartGame) {
				restartGame = false;
				//Set varibles
				paused = true;
				winner = 0;
				timeTaken = 0;
				shotsTaken = 0;
				//Start game
				PlayGame(&pmanager, characters, &streetT, &scopeT);
			}
		}

	}
	void PlayGame(PictureManager * pmanager,Character characters[], Texture2D *streetT, Texture2D * scopeT) {


		for (int i = 0; i < characterNum; i++) {
			characters[i].scale = scale;
			characters[i].StartCharacter();
			characters[i].setUpPictures();
		}

		//Set up player character
		//characters[0].setUpPlayer();

		//Load hints
		Texture2D* info1 = (characters[0].characerParts[GetRandomValue(0, 7)]);//
		Texture2D* info2 = (characters[0].characerParts[GetRandomValue(0, 7)]);
		//Contact person
		Texture2D* info3 = (characters[1].characerParts[GetRandomValue(0, 7)]);//
		Texture2D* info4 = (characters[1].characerParts[GetRandomValue(0, 7)]);
		
		//std::this_thread::sleep_for(std::chrono::milliseconds(150));

		while (!WindowShouldClose() && !restartGame) {

			DoLogic(characters);

			//Begin drawing
			BeginDrawing();
			//Clear the background
			ClearBackground(BLACK);

			//What to draw:

			DrawStreat(streetT);

			DrawInHeightOrder(characters);

			DrawShots();

			if (darkScope) {
				DrawScope(&darkScopeT);
			}
			else {
				DrawScope(scopeT);
			}


			DrawInfo(characters, info1, info2, info3, info4);

			//end the drawing
			EndDrawing();


		}

	}

	void DoLogic(Character characters[]) {

		//xOffset = -characters[0].x + GetScreenWidth() / 2 / scale;
		//yOffset = -characters[0].y + GetScreenHeight() / 2 / scale;

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			fireSniper(characters);
		}

		//Do player key stuff
		if (characters[0].player) {
			characters[0].playerKey();
		}
		CheckKeyInput(characters);

		if (!paused) {

			timeTaken += GetFrameTime();

			//Do velocity and movement
			for (int ch = 0; ch < characterNum; ch++) {
				//Set velocity
				characters[ch].SetVelocity();
				//Move character
				MoveCharacter(&characters[ch]);
				//Do animation character
				characters[ch].animateWhileWalking();
				characters[ch].animateDeath();
			}
		}
	}

	void DrawInHeightOrder(Character characters[]) {
		
		int * order = new int[characterNum];
		//Initialize distances
		for (int d = 0; d < characterNum; d++) {
			order[d] = -1;
		}
		//Make order for drawing
		for (int c = 0; c < characterNum; c++) {
			
			for (int d = 0; d < characterNum; d++) {
				//See if the order is avalable
				if (order[d] != -1) {
					//Check to see if the character is farther down then the d character
					if (characters[c].y > characters[order[d]].y) {
						//Loop backwards threw the array, setting every eliment equal to the previous element
						for (int rep = characterNum-1; rep > d; rep--) {
							order[rep] = order[rep-1];
						}
						//Then set the new order d
						order[d] = c;
						break;
					}
				}
				else {
					order[d] = c;
					break;
				}
			}


		}
		for (int c = characterNum-1; c >= 0 ; c--) {
			//Draw them in reverse order
			if(order[c] != -1) {
				DrawCharacter(&characters[order[c]]);
			}
		}
		
	}
	
	void CheckKeyInput(Character ch[]) {

		//Restart game
		if (IsKeyPressed(KEY_BACKSPACE)) {
			restartGame = true;
			paused = true;
		}
		//Start game
		if (IsKeyPressed(KEY_ENTER)) {
			ch[0].setUpColor();
			paused = false;
		}
		//Deliver intel
		if (IsKeyPressed(KEY_SPACE)) {
			//Check to see if your ontop of the contact agent
			if (touching(&ch[0], &ch[1], 1.1)) {
				//You contacted the agent!
				winner = 2;

			}
			else {
				//Failed to contact the agent
				winner = 3;
			}
		}

		//ATTACkS!!!!!!!!!!!!!!!!!!!
		doAssassination(ch);
		int attackT = -1;
		//AI kill
		if (ch[0].player != true && !paused) {
			attackT = GetRandomValue(1, 63);
		}
		if (IsKeyPressed(KEY_G) || attackT == 1) {
			if (canAttack <= 0) {
				canAttack = 3;
				attack = shoot;
				//shoot
				if (attack == shoot) {
					timeLeftKill = 3;
					shootDelay = (float)(GetRandomValue(2000, maxShootDelay * 10000)) / 10000;
					shotLoc.x = ch[0].x + GetRandomValue(-140, 140);
					shotLoc.y = ch[0].y + GetRandomValue(-100, 100);

				}
			}
		}
		else if (IsKeyPressed(KEY_F) || attackT == 2) {
			if (canAttack <= 0) {
				canAttack = 5;
				attack = stab;
					//stab
				if (attack == stab) {
					timeLeftKill = 2.3;
					darkScope = true;
					stabbed = false;
				}
			}
		}
		else if (IsKeyPressed(KEY_H) || attackT == 3) {
			if (canAttack <= 0) {
				canAttack = 4;
				attack = poison;
				//poison
				if (attack == poison) {
					bool touch = false;
					for (int i = 1; i < characterNum; i++) {
						if (touching(&ch[0], &ch[i], .3)) {
							if (ch[i].life > 0) {
								poisoned = &ch[i];
								touch = true;
								break;
							}
						}
					}
					if (touch == true) {
						timeLeftKill = 5;
						poisonDelay = 1.2;
					}
				}

			}
		}
	}

	float zscale(Character* c) {
		return (c->y - 0.5 * (GetScreenHeight() / scale)) / (GetScreenHeight() / scale);
	}
	void DrawCharacter(Character * characters) {

		//Draw blood
		if (characters->life <= 0) {
			float wPicture = bloodT.width;
			float hPicture = bloodT.height;
			float drawW = characters->partSizes.x * scale * zscale(characters);
			float drawH = characters->partSizes.y * scale * zscale(characters);
			DrawTexturePro(bloodT, Rectangle{ 0,0,wPicture,hPicture }, Rectangle{ (characters->x+xOffset) * scale,(characters->y+yOffset) * scale,drawW,drawH }, Vector2{ drawW / 2,drawH / 2 }, 0, WHITE);

		}
		//Second draw body
		for (int i = 0; i < 8; i++) {

			float wPicture = characters->characerParts[i]->width;
			float hPicture = characters->characerParts[i]->height;
			float drawW = characters->partSizes.x * scale * zscale(characters);
			float drawH = characters->partSizes.y * scale * zscale(characters);


			DrawTexturePro(*(characters->characerParts[i]), Rectangle{ 0,0,wPicture,hPicture }, Rectangle{ (characters->x + xOffset) * scale,(characters->y + yOffset) * scale,drawW,drawH }, Vector2{ drawW / 2,drawH / 2 }, characters->partRotation[i], characters->characterColor[i]);


		}
		
		
	}
	void DrawStreat(Texture2D * sT) {
		float wPicture = sT->width;
		float hPicture = sT->height;
		float drawW = GetScreenWidth()*2;
		float drawH = GetScreenHeight()*2;
		DrawTexturePro(*sT, Rectangle{ 0,0,wPicture,hPicture }, Rectangle{ (float)( - .5*GetScreenWidth()),(float)(-GetScreenHeight()),drawW,drawH}, Vector2{0,0}, 0, WHITE);

	}
	void DrawScope(Texture2D * sT) {

		float wPicture = sT->width;
		float hPicture = sT->height;
		float drawW = GetScreenWidth() * 2;
		float drawH = GetScreenWidth() * 2;

		float xd = (float)GetMouseX() + xOffsetS;
		if (xd > GetScreenWidth()) {
			xd = GetScreenWidth();
		}else if (xd < 0) {
			xd = 0;
		}
		float yd = (float)GetMouseY() + yOffsetS;
		if (yd > GetScreenHeight()) {
			yd = GetScreenHeight();
		}
		else if (yd < 0) {
			yd = 0;
		}
		DrawTexturePro(*sT, Rectangle{ 0,0,wPicture,hPicture }, Rectangle{ xd,yd,drawW,drawH }, Vector2{ drawW/2,drawH/2 }, 0, WHITE);

		xOffsetS += dirSX * swaySpeed;
		if (abs(xOffsetS) > maxSX) {
			dirSX *= -1;
			maxSX = GetRandomValue(0, 40);
		}
		yOffsetS += dirSY * swaySpeed;
		if (abs(yOffsetS) > maxSY) {
			dirSY *= -1;
			maxSY = GetRandomValue(0, 40);
		}
		swaySpeed += GetRandomValue(-100, 100) / 10000;

		if (recoil > 980) {
			recoil--;
			swaySpeed = 20;
			dirSY = -1;
		}else if (yOffsetS < 0 && recoil > 1) {
			recoil--;
			swaySpeed = 12;
			dirSY = 1;
		}
		else if (yOffsetS >= 0 && recoil > 1) {
			recoil = 0;
			swaySpeed = .1;
		}
	}

	void MoveCharacter(Character* c) {
		c->x += c->v.x * c->speed * sqrt(zscale(c));
		c->y += c->v.y * c->speed * zscale(c) * 2;
	}

	void DrawInfo(Character ch[], Texture2D * info1, Texture2D * info2, Texture2D* info3, Texture2D* info4) {
		
		int theDead = 0;
		for (int i = 0; i < characterNum; i++) {
			if (ch[i].life <= 0) {
				theDead++;
			}
		}
		
		//Draw label for shots taken
		const string bullets = "SHOTS TAKEN: " + to_string(shotsTaken);
		char arr[20];
		strcpy_s(arr, bullets.c_str());
		DrawText(arr, 100, 50, 30, RED);
		//Time taken
		const string time = "VICTIMS: " + to_string((int)theDead) + "\nEliminate the agent:";
		char arrT[50];
		strcpy_s(arrT, time.c_str());
		DrawText(arrT, 100, 100, 30, RED);

		//HINTS to contact
		if (canAttack <= 0) {
			const string cT = "[F]: Stab\n[G]: Shoot\n[H]: Poison";
			char arrCT[40];
			strcpy_s(arrCT, cT.c_str());
			DrawText(arrCT, GetScreenWidth() - 400, 100, 30, GREEN);
		}

		//Draw the two clues
		if (!paused) {
			DrawClue(info1, 0);
			DrawClue(info2, 0);
			for (int i = 0; i < 8; i++) {
				DrawClue(ch[0].characerParts[i], GetScreenWidth() - 400);

			}
		}


		/////////WIN CONDITIONS
		DrawWinConditions(ch, theDead);
	}
	void DrawClue(Texture2D* sT, float xoff) {
		
		float wPicture = sT->width;
		float hPicture = sT->height;
		float drawW = 200;
		float drawH = 300;
		DrawTexturePro(*sT, Rectangle{ 0,0,wPicture,hPicture }, Rectangle{ (float)(100) + xoff,(float)(230),drawW,drawH }, Vector2{ 0,0 }, 0, WHITE);
		
	}
	void DrawWinConditions(Character ch[], int theDead) {
		//SNIPER WINS!!
		if ((ch[0].life <= 0 || timeTaken > 100) && winner == 0) {
			winner = 1;
			paused = true;
		}
		else if ((theDead > 7) && winner == 0) {
			winner = 2;
			paused = true;
		}
		//Draw the winner
		if(winner == 1){
			const string winner = "THE SNIPER WINS!";
			char arr[20];
			strcpy_s(arr, winner.c_str());
			DrawText(arr, 300, 400, 120, RED);
		}
		//AGENT WINS!!
		else if (winner == 2) {
			const string winner = "THE AGENT WINS!";
			char arr[20];
			strcpy_s(arr, winner.c_str());
			DrawText(arr, 300, 400, 120, GREEN);
		}
		//SNIPER WINS!! WRONG CONTACT
		else if (winner == 3) {
			const string winner = "THE AGENT CONTACTED THE WRONG PERSON!";
			char arr[60];
			strcpy_s(arr, winner.c_str());
			DrawText(arr, 100, 400, 100, DARKPURPLE);
		}
	}
	void doAssassination(Character c[]) {

		canAttack -= GetFrameTime();

		if (timeLeftKill > 0) {
			timeLeftKill -= GetFrameTime();
			//===============================SHOOT
			//Fire every time the shoot delay expires
			if (attack == shoot) {
				shootDelay -= GetFrameTime();

				if (shootDelay <= maxShootDelay / 12) {
					shooting = false;
				}
				if (shootDelay <= 0) {
					shooting = true;
					//Reset the shoot delay
					shootDelay = (float)(GetRandomValue(2000, maxShootDelay * 10000)) / 10000;
					shotLoc.x = c[0].x + GetRandomValue(-140, 140);
					shotLoc.y = c[0].y + GetRandomValue(-100, 100);
					//shoot
					for (int i = 1; i < characterNum; i++) {
						if (locationInside(&c[i], shotLoc.x, shotLoc.y, .3)) {
							if (c[i].life > 0 && GetRandomValue(0, 100) > 60) {
								c[i].killed();
								break;
							}
						}
					}
					if (timeLeftKill <= 0) {
						attack = stab;
					}
				}
			}
			else {
				shootDelay = -1;
			}
			//=================================POISON
			if (attack == poison) {
				poisonDelay -= GetFrameTime();

				if (poisonDelay <= 0) {
					//Reset the shoot delay
					poisonDelay = (float)(GetRandomValue(2000, 5000)) / 10000;
					//shoot
					//for (int i = 1; i < 8; i++) {
						if (poisoned->life > 0) {
							poisoned->life -= .126;
							poisoned->characterColor[GetRandomValue(0, 7)] = GREEN;
						}else{
							poisoned->killed();
						}
					//}
				}
			}
			//Then see if it is now less than 0
			if (timeLeftKill <= 2.1 && !stabbed) {

				//===============================STAB
				if (attack == stab) {
					stabbed = true;
					//attack = shoot;
					for (int i = 1; i < characterNum; i++) {
						if (touching(&c[0], &c[i], .3)) {
							if (c[i].life > 0) {
								c[i].killed();
								break;
							}
						}
					}
				}
			}

		}
		else {
			shooting = false;
			darkScope = false;
		}
		

	}
	void DrawShots() {
		if (shooting || recoil > 980) {
			float wPicture = gunShotT.width;
			float hPicture = gunShotT.height;
			float drawW = 120;
			float drawH = 120;
			DrawTexturePro(gunShotT, Rectangle{ 0,0,wPicture,hPicture }, Rectangle{( shotLoc.x + xOffset )* scale,(shotLoc.y + yOffset) * scale,drawW,drawH }, Vector2{ drawW / 2,drawH / 2 }, GetRandomValue(-90, 90), WHITE);
		}
	}

	void fireSniper(Character c[]) {

		recoil = 1000;
		shotsTaken++;

		int  x = (GetMouseX()+ xOffsetS)/scale - xOffset;
		int  y = (GetMouseY()+ yOffsetS)/scale - yOffset;

		shotLoc.x = x;
		shotLoc.y = y;

		float aReq = .4; //accuracy required
		int shot = -1;
		for (int i = 0; i < characterNum; i++) {
			if (c[i].life > 0) {

				float W = c[i].partSizes.x * scale * zscale(&c[i]);
				float H = c[i].partSizes.y * scale * zscale(&c[i]);

				//Check to see if you clicked on a character
				if (locationInside(&c[i],x,y,aReq)) {
					if (shot == -1) {
						shot = i;
					}
					else if (c[i].y > c[shot].y) {
						shot = i;
					}
				}
			}
		}
		if (shot != -1) {
			c[shot].killed();
		}
	}
	bool locationInside(Character * c, float x, float y, float aReq) {

		float W = c->partSizes.x * scale * zscale(c);
		float H = c->partSizes.y * scale * zscale(c);

		//Check to see if you clicked on a character
		if (x > c->x - W / 2 * aReq && x < c->x + W / 2 * aReq && y > c->y - H / 2 * aReq && y < c->y + H / 2 * aReq) {
			return true;
		}
		return false;
	}

	bool touching(Character * c1, Character * c2, float aReq) {
		float z1 = zscale(c1);
		float z2 = zscale(c2);
		//get width relative to z 
		float WZ = c1->partSizes.x * scale * z1;
		float HZ = c1->partSizes.y * scale * z1;
		float W = c2->partSizes.x * scale * z2;
		float H = c2->partSizes.y * scale * z2;

		//see if they are touching
		if (c1->x + WZ / 2 * aReq > c2->x - W / 2 * aReq && c1->x - WZ / 2 * aReq < c2->x + W / 2 * aReq && c1->y + HZ / 2 * aReq > c2->y - H / 2 * aReq && c1->y - HZ / 2 * aReq < c2->y + H / 2 * aReq) {
			if (abs(z1-z2) < aReq/4) {
			//You are touching
			return true;
			}
		}
		//Return false if got here, because that means they aren't touching
		return false;
	}
};

