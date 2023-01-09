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
	
	float scale = 1.5;
	int characterNum = 100;
	float xOffset = 0;
	float yOffset = 0;

	//Gamemode stuff
	enum GameMode { assassin, contact, police, thugs };
	GameMode gameMode = assassin;

	bool singlePlayer = false;

	int badguys = 7;
	int victimsToWin = 8;
	int hints = 3;

	//
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
	Texture2D arrowT;

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
	int someonesAttacking = -1;

	//Menu stuff
	bool exitToMenu = false;
	bool menu = true;
	int clickedButton = 0;
	float clickedButtonY = 0;
	float arrowRot = 0;

	//Cutscene
	int scene = 0;
	int continues = 0;
	int stage = 0;
	float timeCS = 0;
	string message = "";
	float charactersOfMessage = 0;
	float messageSpeed = .32;
	int choices = 0;
	Character* characterStore;
	int useCutSceneCharacter = -1;

	//early initialize
	//bool touching(Character* c1, Character* c2);
	//bool locationInside(Character* c, float x, float y, float aReq);

	void InitializeGame() {

		//Make the window
		InitWindow(0, 0, "GameScreen");
		SetTargetFPS(100);
		//Create random seed
		SetRandomSeed(time(0));

		HideCursor();

		//Set window icon
		Image icon = LoadImage("./assets/iconWindow.png");
		SetWindowIcon(icon);

		//Load all images
		PictureManager pmanager;


		//Load pictures for game
		Texture2D streetT = LoadTexture("./assets/streat.png");
		Texture2D scopeT = LoadTexture("./assets/scope3.png");
		darkScopeT = LoadTexture("./assets/scope4.png");
		bloodT = LoadTexture("./assets/blood.png");
		gunShotT = LoadTexture("./assets/gunShot.png");
		arrowT = LoadTexture("./assets/arrow.png");

		MenuLoop(&pmanager, &streetT, &scopeT);
	}
	void MenuLoop(PictureManager* pmanager, Texture2D* streetT, Texture2D* scopeT) {


		//Load menu picture
		Texture2D menuBackdrop = LoadTexture("./assets/buildings.png");
		Texture2D menuTitle = LoadTexture("./assets/TitlePic.png");
		Texture2D menuSinglePlayer = LoadTexture("./assets/singlePlayerButton.png");
		Texture2D menuMultiPlayer = LoadTexture("./assets/multiPlayerButton.png");
		Texture2D targetT = LoadTexture("./assets/Target.png");
		Texture2D howToPlayT = LoadTexture("./assets/howtoplay.png");
		Texture2D multiplayerT = LoadTexture("./assets/multiplayer.png");

		while (menu) {

			//Get sniper aim
			int  xc = (GetMouseX() + xOffsetS) - xOffset;
			int  yc = (GetMouseY() + yOffsetS) - yOffset;

			//Draw menu
			//Begin drawing
			BeginDrawing();
			//Clear the background
			ClearBackground(BLACK);

			//What to draw:
			DrawPic(&menuBackdrop, GetScreenWidth(), GetScreenHeight(), 0, 0, 0, WHITE, 0);
			//Title
			DrawPic(&menuTitle, 800, 160, GetScreenWidth() / 2, 200, 1, WHITE, 0);
			//Buttons

			int buttonHovering = 0;
			//Check to see if mouse is hovering SINGLEPLAYER button
			int spRot = 0;
			float yMod = 0;
			if (clickedButton == 1) {
				clickedButtonY += 15;
				yMod = clickedButtonY;
				spRot = clickedButtonY;
			}
			else if (insidePic(239, 100, GetScreenWidth() / 2, 410, xc, yc, 1)) {
				spRot = GetRandomValue(-3, 3);
				buttonHovering = 1;
			}
			DrawPic(&menuSinglePlayer, 239, 100, GetScreenWidth() / 2, 410 + yMod, 1, WHITE, spRot);
			//Check to see if mouse is hovering MULTIPLAYER button
			spRot = 0;
			yMod = 0;
			if (clickedButton == 2) {
				clickedButtonY += 15;
				yMod = clickedButtonY;
				spRot = clickedButtonY;
			}
			else if (insidePic(239, 100, GetScreenWidth() / 2, 550, xc, yc, 1)) {
				spRot = GetRandomValue(-3, 3);
				buttonHovering = 2;
			}
			DrawPic(&menuMultiPlayer, 239, 100, GetScreenWidth() / 2, 550 + yMod, 1, WHITE, spRot);
			//Check to see if mouse is hovering SETTINGS button
			spRot = 0;
			yMod = 0;
			if (clickedButton == 3) {
				clickedButtonY += 15;
				yMod = clickedButtonY;
				spRot = clickedButtonY;
			}
			else if (insidePic(3 * 80, 3.6 * 80, GetScreenWidth() / 5, GetScreenHeight() * 2 / 4, xc, yc, 1)) {
				spRot = GetRandomValue(-4, 4);
				buttonHovering = 3;
			}
			DrawPic(&targetT, 3 * 80, 7.8 * 80, GetScreenWidth() / 5, GetScreenHeight() * 2 / 4 + yMod, 1, WHITE, spRot);
			arrowRot += .3;
			DrawPic(&arrowT, 500, 500, GetScreenWidth() / 5, GetScreenHeight() * 2 / 4, 1, WHITE, arrowRot);


			DrawScope(scopeT);

			//end the drawing
			EndDrawing();

			//Check for clicks
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				clickedButton = buttonHovering;
			}
			if (clickedButtonY > GetScreenHeight() / 2) {
				if (clickedButton == 3) {
					//SETTINGS
					while (true) {
						if (IsKeyPressed(KEY_ENTER)) {
							break;
						}

						//Draw menu
						//Begin drawing
						BeginDrawing();
						//Clear the background
						ClearBackground(BLACK);

						DrawPic(&howToPlayT, GetScreenWidth(), GetScreenHeight(), 0, 0, 0, WHITE, 0);

						//end the drawing
						EndDrawing();
					}
					clickedButton = 0;
					clickedButtonY = 0;
					arrowRot = 0;
				}
				else if (clickedButton == 2) {
					int breakType = -1;
					//SETTINGS
					while (true) {
						if (IsKeyPressed(KEY_ENTER)) {
							breakType = 1;
							break;
						}
						else if (IsKeyPressed(KEY_KP_1)) {
							breakType = 2;
							break;
						}
						else if (IsKeyPressed(KEY_KP_2)) {
							breakType = 3;
							break;
						}
						else if (IsKeyPressed(KEY_KP_3)) {
							breakType = 4;
							break;
						}

						//Draw menu
						//Begin drawing
						BeginDrawing();
						//Clear the background
						ClearBackground(BLACK);

						DrawPic(&multiplayerT, GetScreenWidth(), GetScreenHeight(), 0, 0, 0, WHITE, 0);

						//end the drawing
						EndDrawing();
					}
					if (breakType == 1) {
						clickedButton = 0;
						clickedButtonY = 0;
						arrowRot = 0;
					}
					else if (breakType == 2) {
						characterNum = 30;
						victimsToWin = 6;
						scale = 2.1;

						gameMode = assassin;
						menu = false;
						singlePlayer = false;
						break;
					}
					else if (breakType == 3) {

						characterNum = 90;
						scale = 1.7;

						gameMode = contact;
						menu = false;
						singlePlayer = false;
						break;
					}
					else if (breakType == 4) {
						characterNum = 90;
						victimsToWin = 10;
						scale = 1.8;

						gameMode = thugs;
						menu = false;
						singlePlayer = false;
						break;
					}

				}
				else if (clickedButton == 1) {
					menu = false;
					singlePlayer = true;
					cutSceneFirst(pmanager, streetT,scopeT);
					break;
				}
			}
			//Check for escape
			if (IsKeyPressed(KEY_ESCAPE)) { exit(0); }
		}

		//Launch the standard game
		GameLoop(pmanager, streetT, scopeT);
		
	}
	void GameLoop(PictureManager* pmanager, Texture2D* streetT, Texture2D* scopeT) {
		
			//Make character
			Character* characters = new Character[characterNum];
			//Set the picture manager
			for (int i = 0; i < characterNum; i++) {
				characters[i].pmanager = pmanager;
			}


			//Game restart loop
			while (!exitToMenu) {

				if (restartGame) {
					restartGame = false;
					//Set varibles
					paused = true;
					winner = 0;
					timeTaken = 0;
					shotsTaken = 0;
					//Start game
					PlayGame(pmanager, characters, streetT, scopeT);
				}
			}
			
			
		
		
		
	}
	void PlayGame(PictureManager * pmanager,Character characters[], Texture2D *streetT, Texture2D * scopeT) {


		for (int i = 0; i < characterNum; i++) {
			characters[i].scale = scale;
			characters[i].StartCharacter();
			characters[i].setUpPictures();
		}
		if (useCutSceneCharacter != -1) {
			for (int i = 0; i < 8; i++) {
				characters[useCutSceneCharacter].characterParts[i] = characterStore->characterParts[i];
				characters[useCutSceneCharacter].characterColor[i] = characterStore->characterColor[i];
			}
		}

		LaunchGameMode(characters);

		//Set up player character
		if (!singlePlayer) {
			characters[0].setUpPlayerAssassin();
		}

		//Load hints
		int h1 = GetRandomValue(0, 7);
		int h2 = getHintTwo(h1);
		Texture2D* info1 = characters[0].characterParts[h1];
		Texture2D* info2 = characters[0].characterParts[h2];
		Texture2D* info3 = (characters[0].characterParts[getHintThree(h1,h2)]);
		
		//std::this_thread::sleep_for(std::chrono::milliseconds(150));

		while (!restartGame) {

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

			if (paused && (winner != 0 || !singlePlayer)) {
				DrawPic(&arrowT, 1400*zscale(&characters[0]), 1400*zscale(&characters[0]), (characters[0].x + xOffset) * scale, (characters[0].y + yOffset) * scale, 1, WHITE, 0);
			}

			DrawInfo(characters, info1, info2, info3);

			//end the drawing
			EndDrawing();

			//Check for escape
			if (IsKeyPressed(KEY_ESCAPE)) {
				//Go back to main menu
				clickedButton = 0;
				clickedButtonY = 0;
				arrowRot = 0;
				menu = true;
				//exitToMenu = true;
				//restartGame = true;
				//If you got here, we must be going back to the main menu
				//exitToMenu = false;
				//restartGame = false;
				GamManager theGame;
				theGame.InitializeGame();
				exit(0);
			}
		}

	}

	void LaunchGameMode(Character ch[]) {

		if (gameMode == assassin) {

			if (singlePlayer) {
				ch[0].setUpAiAssassin();
			}else{
				ch[0].setUpPlayerAssassin();
			}
		}
		else if (gameMode == contact) {

			ch[0].setUpPlayerAssassin();
			
		}
		else if (gameMode == police) {

		}
		else if (gameMode == thugs) {
			//In THUG there are multiple badguys that all look the same and try to kill people!

			int includeFirst = 1;
			//If it is single player make the 0 character a badguy too
			if (singlePlayer) { includeFirst = 0; }

			for (int p = includeFirst; p < badguys; p++) {

				ch[p].civilian = false;

				for (int i = 0; i < 8; i++) {
					ch[p].characterParts[i] = ch[0].characterParts[i];
				}
			}
		}

	}

	void cutSceneFirst(PictureManager* pmanager, Texture2D* streetT, Texture2D* scopeT) {

		float smod = 1;
		int theMugger = -1;

		Color textColor = WHITE;

		Texture2D menuBackdrop = LoadTexture("./assets/streat.png");
		Texture2D roomT = LoadTexture("./assets/room2.png");
		Texture2D pointerT = LoadTexture("./assets/pointer.png");
		//Make character
		scale = 6;
		characterNum = 6;
		Character* ch = new Character[characterNum];
		//Set the picture manager
		for (int i = 0; i < characterNum; i++) {
			ch[i].pmanager = pmanager;
			ch[i].scale = scale;
			ch[i].StartCharacter();
			ch[i].setUpPictures();
			ch[i].x = -100*i;
			ch[i].y = GetScreenHeight() / 5 * 4 / scale;
			
		}

		//Set starting locations
		ch[0].x = GetScreenWidth() / 2/scale;
		ch[0].y = GetScreenHeight() / 5*4 / scale;
		ch[1].x = -130 / scale;
		ch[1].y = GetScreenHeight() / 5 * 4 / scale;
		ch[1].v.x = 1;
		ch[0].v.x = .5;

		ch[0].characterParts[4] = &(pmanager->mouth[6]);

		characterStore = &ch[1];
		useCutSceneCharacter = 0;
		hints = 0;

		timeCS = 0;
		continues = 1;
		while(true){
			if (IsKeyDown(KEY_SPACE)) {
				SetTargetFPS(1000);
				smod = 10;
			}
			else {
				SetTargetFPS(100);
				smod = 1;
			}
			//============================Cutscene
			
			if (timeCS < 3) {
				timeCS += GetFrameTime()* smod;
				if (stage == 0) { message = "Your day was going terribly.\nYou had just got fired from your job.";stage++; charactersOfMessage = 0;
				}
				if ((int)charactersOfMessage < message.length()) {
					charactersOfMessage += messageSpeed;
				}
			}
			else if (timeCS < 6) {
				timeCS += GetFrameTime() * smod;
				if (stage == 1) { message = "The $50 in your pocket was all you had to your name."; stage++; charactersOfMessage = 0;
				}
				if ((int)charactersOfMessage < message.length()) {
					charactersOfMessage += messageSpeed;
				}
			}
			else if (timeCS < 11) {
				timeCS += GetFrameTime() * smod;
				if (stage == 2) { message = "But you decided to try to be happy anyway.\nAnd you started to smile."; stage++; charactersOfMessage = 0; }
				if ((int)charactersOfMessage < message.length()) {
					charactersOfMessage += messageSpeed;
				}
				if (ch[0].partRotation[4] < 520) {
					ch[0].partRotation[4] += GetFrameTime() * 180;
				}
			}
			else if (timeCS < 15 && continues > 0) {
				timeCS += GetFrameTime() * smod;
				if (stage == 3) { message = "You felt better for a little while...\nBut then a man began approuching..."; stage++; charactersOfMessage = 0; }
				if ((int)charactersOfMessage < message.length()) {
					charactersOfMessage += messageSpeed/2;
				}
				if (ch[1].x < ch[0].x-20) {
					ch[1].x += GetFrameTime() * smod * ((ch[0].x - 20)-ch[1].x);
				}
			}
			else if (timeCS < 20 && continues > 0) {
				timeCS += GetFrameTime() * smod;
				if (stage == 4) { message = "'GIVE ME ALL YOUR MONEY!' He yelled.             \nYou Felt your smile disappearing."; stage++; charactersOfMessage = 0; }
				if ((int)charactersOfMessage < message.length()) {
					charactersOfMessage += messageSpeed;
				}
				if (ch[0].partRotation[4] > 0) {
					ch[0].partRotation[4] -= GetFrameTime() * smod * 180;
				}

				ch[1].partRotation[4] = GetRandomValue(-10,10);
				
			}
			else if (timeCS < 26 && continues > 0) {
				timeCS += GetFrameTime() * smod;
				if (stage == 5) { message = "Feeling his hard hand against your chest, anger flared up inside of you.\n What had you done to deserve how fate treated you!!!!!!!!!!!"; stage++; charactersOfMessage = 0; }
				if ((int)charactersOfMessage < message.length()) {
					charactersOfMessage += messageSpeed;
				}

				for (int pa = 0; pa < 8; pa++) {
					if (ch[0].partRotation[0] < 90) {
						ch[0].partRotation[pa] += GetFrameTime() * smod * 90*4;
					}
				}
				if (ch[0].partRotation[0] < 90) {
					ch[0].y += GetFrameTime() * smod * 90;
				}
			}
			else if (timeCS < 36 && continues > 0) {
				timeCS += GetFrameTime() * smod;
				if (stage == 6) { message = "'Come back! I need that money!' You heard your faint voice call after him...\nHe dissapeared into the distance.\nYou Feel bruised.\nYou struggle to stand..."; stage++; charactersOfMessage = 0; }
				if ((int)charactersOfMessage < message.length()) {
					charactersOfMessage += messageSpeed / 3;
				}
				if (ch[1].x < GetScreenWidth()/scale+200 && ch[1].v.y == 0) {
					ch[1].x -= GetFrameTime() * smod * (-5-(ch[0].x));
				}
				else {
					ch[1].v.y = .1;
					ch[1].x = -100;
					ch[1].y = ch[2].y;
				}
				ch[0].partRotation[4] = GetRandomValue(100, 80);
			}
			else if (timeCS < 40 && continues > 0) {
				timeCS += GetFrameTime() * smod;
				if (stage == 7) { message = "Struggling to your feet, you feel the angle pounding in your chest.\n'What now?' you ask yourself."; stage++; charactersOfMessage = 0; }
				if ((int)charactersOfMessage < message.length()) {
					charactersOfMessage += messageSpeed;
				}

				for (int pa = 0; pa < 8; pa++) {
					if (ch[0].partRotation[0] > 0) {
						ch[0].partRotation[pa] -= GetFrameTime() * smod * 90 * 4;
					}
				}
				if (ch[0].partRotation[0] > 0) {
					ch[0].y += GetFrameTime() * smod * 90;
				}
			}
			else if (timeCS < 44 && continues > 0) {
				timeCS += GetFrameTime() * smod;
				if (stage == 8) { message = "You pull out your phone to call the Police.        \nBut, you pause,      \nYou remember the hunting rifle in your appartment...\nThe police probably won't catch him.\nEven if they do, they won't give him much of a sentence..."; stage++; charactersOfMessage = 0; }
				if ((int)charactersOfMessage < message.length()) {
					charactersOfMessage += messageSpeed;
				}

				ch[0].partRotation[5] = GetRandomValue(-2.5,2.5);
				
			}
			else if (timeCS < 46 && continues > 0) {
				continues = 1;
				if (stage == 9) { message = "You have to decide...\nPress [P] to call the police.\nPress [S] to get your hunting rifle..."; stage++; charactersOfMessage = 0; }
				if ((int)charactersOfMessage < message.length()) {
					charactersOfMessage += messageSpeed;
				}
				else {
					timeCS += GetFrameTime() * smod;
				}

				ch[0].partRotation[5] = GetRandomValue(-2.5, 2.5);

			}
			else if (continues == 1) {
				if (stage == 10) { stage++; choices = 0;}

				if(IsKeyPressed(KEY_P) && choices == 0){ choices = 1; }
				else if (IsKeyPressed(KEY_S) && choices == 0) { choices = 2; }

				if (IsKeyPressed(KEY_ENTER) && choices != 0) { continues = 2; }

				if (choices == 1) {
					if (stage == 11) { message = "You quickly dile 111, hoping that they can catch this guy.\nYou know this was the right thing to do...\n[Press ENTER to continue]"; stage++; charactersOfMessage = 0; }
					if ((int)charactersOfMessage < message.length()) {
						charactersOfMessage += messageSpeed;
					}
				}else if (choices == 2) {
					if (stage == 11) { message = "You run home and get your sniper rifle.\nYou'll teach that guy to mess with you!\n[Press ENTER to continue]"; stage++; charactersOfMessage = 0; }
					if ((int)charactersOfMessage < message.length()) {
						charactersOfMessage += messageSpeed;
					}
					
					ch[0].x += GetFrameTime() * smod *30;
					
				}
			}
			else if (continues == 2) {
			
				if (timeCS < 60 && choices == 1 ) {
					timeCS += GetFrameTime() * smod;

					if (IsKeyPressed(KEY_ENTER)) { continues = 3; }

					if (stage == 12) { message = "The next day, you are called to the police station.\n'We think we caught him!' A kind police officer tells you.\nYou just have to pick him out of the line up, just to make sure.\n[Press ENTER to continue]"; stage++; charactersOfMessage = 0;
						menuBackdrop = roomT;
						textColor = BLACK;
						//SET UP LINE UP
						//Make it so all the other characteres look the same, except for 1 part
						theMugger = GetRandomValue(1, characterNum - 1);

						for (int i = 1; i < characterNum; i++) {
							
							ch[i].v.x = 1;
							//Make them all look the same (except for shirt color)
							if (i != 1) {
								for (int pr = 0; pr < 8; pr++) {
									ch[i].characterParts[pr] = ch[1].characterParts[pr];
								}
							}

						}
						for (int i = 1; i < characterNum; i++) {
							int differences = GetRandomValue(2, 3);
							//Set NUM amount of parts to look different if you aren't the mugger
							for (int num = 0; num < differences; num++) {
								if (theMugger != i) {
									int difference = GetRandomValue(0, 7);

									//Body,head,feet,hands,mouth,eyes, hair, hat

									if (difference == 0) { ch[i].characterParts[0] = &pmanager->body[GetRandomValue(1, pmanager->bodyN - 1)]; }
									if (difference == 1) { ch[i].characterParts[1] = &pmanager->head[GetRandomValue(1, pmanager->headN - 1)]; }
									if (difference == 2) { ch[i].characterParts[2] = &pmanager->feet[GetRandomValue(1, pmanager->feetN - 1)]; }
									if (difference == 3) { ch[i].characterParts[3] = &pmanager->hands[GetRandomValue(0, pmanager->handsN - 1)]; }
									if (difference == 4) { ch[i].characterParts[4] = &pmanager->mouth[GetRandomValue(0, pmanager->mouthN - 1)]; }
									if (difference == 5) { ch[i].characterParts[5] = &pmanager->eyes[GetRandomValue(1, pmanager->eyesN - 1)]; }
									if (difference == 6) { ch[i].characterParts[6] = &pmanager->hair[GetRandomValue(1, pmanager->hairN - 1)]; }
									if (difference == 7) { ch[i].characterParts[7] = &pmanager->hat[GetRandomValue(0, pmanager->hatN - 1)]; }

								}
							}

						}
					}
					if ((int)charactersOfMessage < message.length()) {
						charactersOfMessage += messageSpeed;
					}
				}
				else if (timeCS < 60 && choices == 2) {
					timeCS += GetFrameTime() * smod;
					if (stage == 12) { message = "You get a ping from your credit card. You know where he is. But can you remember what he looks like..?\nPress [ENTER] to continue."; stage++; charactersOfMessage = 0; }
					if ((int)charactersOfMessage < message.length()) {
						charactersOfMessage += messageSpeed;
					}
					else {
						if (IsKeyPressed(KEY_ENTER)) {
							break;
						}
					}
				}
			}
			else if (continues == 3) {
				choices = 0;

				if (IsKeyPressed(KEY_ENTER)) { continues = 4; }

				if (stage == 13) {
					message = "You watch as a bunch of people walk into the room. You struggle to tell which one was your mugger...\n'Which one is it?' The officer asks.\n[Press ENTER to continue]"; stage++; charactersOfMessage = 0;
				}
				if ((int)charactersOfMessage < message.length()) {
					charactersOfMessage += messageSpeed;
				}
				for (int i = 1; i < characterNum; i++) {
					if (ch[i].x < GetScreenWidth() / scale / 7 * i) {
						ch[i].x += GetFrameTime() * GetScreenWidth() / scale * 2 / 3 / 2;
					}
				}
				ch[0].x += GetFrameTime() * GetScreenWidth() / scale / 2;

				
			}
			else if (continues == 4) {

				if (stage == 14) {
					choices = -1;
					message = "Click on the person who mugged you.\nThey might have changed their shirt, so ignore their colors..."; stage++; charactersOfMessage = 0;
				}
				if ((int)charactersOfMessage < message.length()) {
					charactersOfMessage += messageSpeed;
				}
				//Make them shake
				for (int i = 1; i < characterNum; i++) {
					if (insidePic(ch[i].partSizes.x/3, ch[i].partSizes.y/3, ch[i].x, ch[i].y, (GetMouseX() + xOffsetS) / scale, (GetMouseY() + yOffsetS) / scale, 1)) {
						for (int part = 1; part < 8; part++) {
							ch[i].partRotation[part] = GetRandomValue(-3, 3);
						}
					}
				}
				//
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
					for (int i = 1; i < characterNum; i++) {
						if (insidePic(ch[i].partSizes.x/3, ch[i].partSizes.y/3, ch[i].x, ch[i].y, (GetMouseX()+xOffsetS)/scale, (GetMouseY() + yOffsetS) /scale, 1)) {
							choices = i;
						}
					}
				}

				if (choices == theMugger) {
					if (stage == 15) { message = "You managed to reconize him, and he was arrested.\nThats when you realized what job you were going to try to get next...\nYou are going to apply to the POLICE COLLEGE!!\n[Press Enter to continue]"; stage++; charactersOfMessage = 0; }
					if ((int)charactersOfMessage < message.length()) {
						charactersOfMessage += messageSpeed;
					}
					if (IsKeyPressed(KEY_ENTER)) { break; }

				}
				else if(choices != theMugger && choices != -1) {
					if (stage == 15) { message = "'No, sorry.' The officer said, looking sad. 'That wasn't the man we brought in.\nLooks like we didn't catch him after all...'\nYOU LOOSE"; stage++; charactersOfMessage = 0; }
					if ((int)charactersOfMessage < message.length()) {
						charactersOfMessage += messageSpeed;
					}
					else {
						break;
					}
				}
			
			}
			else {
				if (IsKeyPressed(KEY_ENTER)) {
					continues++;
				}
			}


			//============================
			for (int i = 1; i < characterNum; i++) {
				ch[i].animateWhileWalking();
			}

			//Begin drawing
			BeginDrawing();
			//Clear the background
			ClearBackground(BLACK);


			//What to draw:
			DrawPic(&menuBackdrop, GetScreenWidth(), GetScreenHeight(), 0, 0, 0, WHITE, 0);
			//Draw Characters
			DrawInHeightOrder(ch);

			if (stage == 15 && continues == 4) {
				DrawScope(&pointerT);
			}

			//Draw label
			string text = message.substr(0,(int)charactersOfMessage);
			char arr[400];
			strcpy_s(arr, text.c_str());
			DrawText(arr, 100, 100, 30, textColor);

			//end the drawing
			EndDrawing();
		}
		//LOAD GAME
		if (choices == 1) {
			scale = 3;
			characterNum = 5;

			//Launch the standard game
			GameLoop(pmanager, streetT, scopeT);
		}
		else if (choices == 2) {

			gameMode = assassin;
			characterNum = 22;
			victimsToWin = 3;
			scale = 2.1;

			//Launch the standard game
			GameLoop(pmanager, streetT, scopeT);
		}

	}

	int getHintTwo(int h1) {

		int h2 = GetRandomValue(0, 7);

		if (h2 == h1) {
			return getHintTwo(h1);
		}
		else {
			return h2;
		}
	}
	int getHintThree(int h1, int h2) {

		if (h1 == 2 || h1 == 3 || h2 == 2 || h2 == 3) {
			return getHintTwo(2);
		}
		return 2;
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
				winner = 4;
				paused = true;

			}
			else {
				//Failed to contact the agent
				winner = 5;
				paused = true;
			}
		}
		//-------------------
		//================Attack stuff
		doAssassination(ch);

		//================PLAYER ATTACKS
		if (ch[0].player == true) {

			int attackType = -1;

			if (IsKeyPressed(KEY_F)) { attackType = 2; }
			else if (IsKeyPressed(KEY_G)) { attackType = 1; }
			else if (IsKeyPressed(KEY_H)) { attackType = 3; }

			if (attackType != -1 && someonesAttacking == -1) {
				someonesAttacking = 0;
				AttackChecks(ch, attackType);
			}
		}
		
		//================AI ATTACKS
		if (gameMode == assassin || gameMode == thugs && !paused) {
			for (int p = 0; p < badguys; p++) {
				//Anyone who is not a civilian in an assassin in Assassin gamemode
				if (!ch[p].civilian && !ch[p].player) {

					int attackType = -1;

					attackType = GetRandomValue(1, 63);//Only attacks between 1-3, so 61 out of 63 times it doesn't attack
					
					if (attackType <= 3 && someonesAttacking == -1) {
						someonesAttacking = p;
						AttackChecks(ch, attackType);
					}
				}
			}
		}
		

	}
	void AttackChecks(Character ch[], int attackT) {
		
		//ATTACkS!!!!!!!!!!!!!!!!!!!
		if (attackT == 1) {
			if (canAttack <= 0) {
				canAttack = 3;
				attack = shoot;
				//shoot
				if (attack == shoot) {
					timeLeftKill = 3;
					shootDelay = (float)(GetRandomValue(2000, maxShootDelay * 10000)) / 10000;
					shotLoc.x = ch[someonesAttacking].x + GetRandomValue(-140, 140);
					shotLoc.y = ch[someonesAttacking].y + GetRandomValue(-100, 100);

				}
			}
		}
		else if (attackT == 2) {
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
		else if (attackT == 3) {
			if (canAttack <= 0) {
				canAttack = 4;
				attack = poison;
				//poison
				if (attack == poison) {
					bool touch = false;
					for (int i = 0; i < characterNum; i++) {
						if (ch[i].civilian) {
							if (touching(&ch[someonesAttacking], &ch[i], .2)) {
								if (ch[i].life > 0) {
									poisoned = &ch[i];
									touch = true;
									break;
								}
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

			float wPicture = characters->characterParts[i]->width;
			float hPicture = characters->characterParts[i]->height;
			float drawW = characters->partSizes.x * scale * zscale(characters);
			float drawH = characters->partSizes.y * scale * zscale(characters);


			DrawTexturePro(*(characters->characterParts[i]), Rectangle{ 0,0,wPicture,hPicture }, Rectangle{ (characters->x + xOffset) * scale,(characters->y + yOffset) * scale,drawW,drawH }, Vector2{ drawW / 2,drawH / 2 }, characters->partRotation[i], characters->characterColor[i]);


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

	void DrawInfo(Character ch[], Texture2D * info1, Texture2D * info2, Texture2D* info3) {
		
		int theDead = 0;
		for (int i = 0; i < characterNum; i++) {
			if (ch[i].life <= 0 && ch[i].civilian) {
				theDead++;
			}
		}
		
		//Draw label for shots taken
		const string bullets = "SHOTS TAKEN: " + to_string(shotsTaken);
		char arr[30];
		strcpy_s(arr, bullets.c_str());
		DrawText(arr, 100, 50, 30, RED);
		//Time taken
		const string time = "VICTIMS: " + to_string((int)theDead) + "\nEliminate the agent:";
		char arrT[100];
		strcpy_s(arrT, time.c_str());
		DrawText(arrT, 100, 100, 30, RED);

		//HINTS to contact
		if (gameMode == assassin || gameMode == thugs) {
			if (canAttack <= 0) {
				const string cT = "KILL EVERYONE!\n[F]: Stab\n[G]: Shoot\n[H]: Poison";
				char arrCT[100];
				strcpy_s(arrCT, cT.c_str());
				DrawText(arrCT, GetScreenWidth() - 400, 100, 30, RED);
			}
		}
		else if (gameMode == contact) {
			if (canAttack <= 0) {
				const string cT = "Find your contact in the crowd!\nHit [SPACE] to contact them before it's too late!";
				char arrCT[100];
				strcpy_s(arrCT, cT.c_str());
				DrawText(arrCT, GetScreenWidth() - 500, 100, 30, BLUE);
			}
		}

		//Draw the two clues
		if (!paused) {
			if (gameMode == assassin || gameMode == contact) {

				if (hints > 0) { DrawClue(info1, 0); }
				if (hints > 1) {DrawClue(info2, 0);}
				if (hints > 2) {DrawClue(info3, 0);}
			}
			//
			if (gameMode == contact) {
				for (int i = 0; i < 8; i++) {
					//Draw the contact agent on the right
					DrawClue(ch[1].characterParts[i], GetScreenWidth() - 400);
				}
			}
			else if (gameMode == thugs) {
				for (int i = 0; i < 8; i++) {
					int clueN = 1;
					if (gameMode) { clueN = 0; }
					DrawClue(ch[1].characterParts[i], GetScreenWidth() - 400);
				}
			}
		}


		/////////WIN CONDITIONS
		DrawWinConditions(ch, theDead);
	}
	void DrawPic(Texture2D* t, float w, float h, float x, float y, int center, Color color, float rotation) {

		float wPicture = t->width;
		float hPicture = t->height;
		float drawW = w;
		float drawH = h;
		DrawTexturePro(*t, Rectangle{ 0,0,wPicture,hPicture }, Rectangle{ x,y,drawW,drawH }, Vector2{ drawW/2* center,drawH/2*center }, rotation, color);

	}
	void DrawClue(Texture2D* sT, float xoff) {
		
		DrawPic(sT, 200, 300, (float)(100) + xoff, 230, 0, WHITE, 0);
	}
	void DrawWinConditions(Character ch[], int theDead) {
		//Win conditions
		if (gameMode == assassin || gameMode == contact) {
			if ((ch[0].life <= 0) && winner == 0) {
				winner = 1;
				paused = true;
			}
			else if (timeTaken > 80 && winner == 0) {
				winner = 2;
				paused = true;
			}
			else if ((theDead >= victimsToWin) && winner == 0) {
				winner = 3;
				paused = true;
			}
		}
		else if (gameMode == thugs) {
			bool anyAlive = false;
			for (int p = 0; p < badguys; p++) {
				//If any badguys are alive
				if (ch[p].life > 0 && ch[p].civilian == false) {
					anyAlive = true;
				}
			}
			if (winner == 0 && !anyAlive) {
				//If no badguys are alive
				winner = 6;
				paused = true;
			}
			if ((theDead >= victimsToWin) && winner == 0) {
				winner = 7;
				paused = true;
			}

		}
		
		//Draw the winner
		if(winner == 1){
			const string winner = "SNIPER WINS!\nAssassin was shot.";
			char arr[100];
			strcpy_s(arr, winner.c_str());
			DrawText(arr, 300, 400, 90, GREEN);
		}
		else if (winner == 2) {
			const string winner = "SNIPER WINS!\nAssassin ran out of time.";
			char arr[100];
			strcpy_s(arr, winner.c_str());
			DrawText(arr, 300, 400, 90, GREEN);
		}
		else if (winner == 3) {
			const string winner = "THE ASSASSIN WINS!\n" + to_string(theDead) + " civilians are dead!";
			char arr[100];
			strcpy_s(arr, winner.c_str());
			DrawText(arr, 100, 400, 90, RED);
		}
		else if (winner == 4) {
			const string winner = "THE AGENT WINS!\nHe handed over the info!";
			char arr[100];
			strcpy_s(arr, winner.c_str());
			DrawText(arr, 100, 400, 90, ORANGE);
		}
		else if (winner == 5) {
			const string winner = "THE SNIPER WINS!\nThe agent contacted the wrong person!";
			char arr[100];
			strcpy_s(arr, winner.c_str());
			DrawText(arr, 100, 400, 90, GREEN);
		}
		else if (winner == 6) {
			const string winner = "THE SNIPER WINS!\nAll thugs were killed!";
			char arr[100];
			strcpy_s(arr, winner.c_str());
			DrawText(arr, 100, 400, 90, GREEN);
		}
		else if (winner == 7) {
			const string winner = "THE THUGS WINS!\n" + to_string(theDead) + " civilians are dead!";
			char arr[100];
			strcpy_s(arr, winner.c_str());
			DrawText(arr, 100, 400, 90, GREEN);
		}
	}
	void doAssassination(Character c[]) {

		int p = someonesAttacking;

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
					shotLoc.x = c[p].x + GetRandomValue(-140, 140);
					shotLoc.y = c[p].y + GetRandomValue(-100, 100);
					//shoot
					for (int i = 0; i < characterNum; i++) {
						if (c[i].civilian) {
							if (locationInside(&c[i], shotLoc.x, shotLoc.y, .3)) {
								if (c[i].life > 0 && GetRandomValue(0, 100) > 60) {
									c[i].killed();
									break;
								}
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
					for (int i = 0; i < characterNum; i++) {
						if (c[i].civilian) {
							if (touching(&c[p], &c[i], .3)) {
								if (c[i].life > 0) {
									c[i].killed();
									break;
								}
							}
						}
					}
				}
			}

		}
		else {
			someonesAttacking = -1;
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
	bool insidePic(float wp, float hp, float xp, float yp, float x, float y, int centred) {
		if (centred == 1) {
			if (x > xp - wp / 2 && x < xp + wp / 2 && y > yp - hp / 2 && y < yp + hp / 2) {
				return true;
			}
		}
		else {
			if (x > xp && y > yp && x < xp + wp && y < yp + hp) {
				return true;
			}
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

