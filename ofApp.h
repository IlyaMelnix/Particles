#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};

class Particle
{
public:
	Particle();
	void setup();
	void update(float dt);
	void draw();

	ofPoint pos;
	ofPoint vel;
	float time;
	float lifeTime;
	bool live;

};

class Params 
{
public:
	void setup();
	ofPoint eCenter; //Emitter center
	float eRad; //Emitter radius
	float velRad; //Initial velocity limit
	float lifeTime; //Lifetime in seconds
	float rotate; //Direction rotation speed in angles per second

	float force;	//Силы притяжения/отталкивания внутри emitter
	float spinning;	//Закручивающая сила
	float friction; //Трение в пределах [0,1]
};


extern Params param;
