#pragma once

#include <iostream>
#include <raylib.h>
#include <string>

using namespace std;

class PictureManager
{
public:
	//Body,head,feet,hands,mouth,eyes, hair, hat
	static const int bodyN = 11;
	static const int headN = 7;
	static const int feetN = 5;
	static const int handsN = 5;
	static const int mouthN = 8;
	static const int eyesN = 9;
	static const int hairN = 11;
	static const int hatN = 5;



	Texture2D body[bodyN];
	Texture2D head[headN];
	Texture2D feet[feetN];
	Texture2D hands[handsN];
	Texture2D mouth[mouthN];
	Texture2D eyes[eyesN];
	Texture2D hair[hairN];
	Texture2D hat[hatN];

	PictureManager() {
		//Initialize pictures
		//Body,head,feet,hands,mouth,eyes, hair, hat
		// 
		//BODY
		for (int i = 0; i < bodyN; i++) {
			string bodyP = "./assets/Body" + std::to_string(i) + ".png";
			body[i] = LoadTexture(bodyP.c_str());
		}
		//HEAD
		for (int i = 0; i < headN; i++) {
			string headP = "./assets/Head" + std::to_string(i) + ".png";
			head[i] = LoadTexture(headP.c_str());
		}
		//FEET
		for (int i = 0; i < feetN; i++) {
			string feetP = "./assets/Feet" + std::to_string(i) + ".png";
			feet[i] = LoadTexture(feetP.c_str());
		}
		//HANDS
		for (int i = 0; i < handsN; i++) {
			string handsP = "./assets/Hands" + std::to_string(i) + ".png";
			hands[i] = LoadTexture(handsP.c_str());
		}
		//MOUTH
		for (int i = 0; i < mouthN; i++) {
			string mouthP = "./assets/Mouth" + std::to_string(i) + ".png";
			mouth[i] = LoadTexture(mouthP.c_str());
		}
		//EYES
		for (int i = 0; i < eyesN; i++) {
			string eyesP = "./assets/Eyes" + std::to_string(i) + ".png";
			eyes[i] = LoadTexture(eyesP.c_str());
		}
		//HAIR
		for (int i = 0; i < hairN; i++) {
			string hairP = "./assets/Hair" + std::to_string(i) + ".png";
			hair[i] = LoadTexture(hairP.c_str());
		}
		//HAT
		for (int i = 0; i < hatN; i++) {
			string hatP = "./assets/Hat" + std::to_string(i) + ".png";
			hat[i] = LoadTexture(hatP.c_str());
		}
	}


};

