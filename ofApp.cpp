#include "ofApp.h"

Params param;	//Definition of global variable

vector<Particle> p;		//Вектор частиц

ofFbo fbo;
float history;	//Контролирует параметры следов, изменяется в пределах [0,1]
float time0;	//Начальное время

float bornRate;	//Сколько частиц породить за 1 секунду
float bornCount;//Кол-во частиц, порождаемых за 1 проход

void Params::setup() { 

	eCenter = ofPoint( ofGetWidth() / 2, ofGetHeight() / 2 );
	eRad = 300;
	velRad = 0;
	lifeTime = 1.0;
	rotate = 500;

	force = -1000;
	spinning = 1000;
	friction = 0.05;
}

Particle::Particle(){

	live = false;
}

ofPoint randomPointInCircle( float maxRad ){

	ofPoint pnt;
	float rad = ofRandom( 0, maxRad );
	float angle = ofRandom( 0, M_TWO_PI );
	pnt.x = cos( angle ) * rad;
	pnt.y = sin( angle ) * rad;
	return pnt;
}

void Particle::setup() {

	pos = param.eCenter + randomPointInCircle( param.eRad );
	vel = randomPointInCircle( param.velRad );
	time = 0;
	lifeTime = param.lifeTime;
	live = true;

	
}

void Particle::update( float dt ){

	if ( live ) 
	{
		//Rotate vel
		vel.rotate( 0, 0, param.rotate * dt );

		ofPoint acc;
		ofPoint delta = pos - param.eCenter;
		float len = delta.length();
		if (ofInRange(len,0,param.eRad))
		{
			delta.normalize();

			//Силы притяжения/отталкивания
			acc += delta * param.force;

			//Закручивающая сила
			acc.x += -delta.y * param.spinning;
			acc.y += delta.x * param.spinning;
		}
		vel += acc *dt;
		vel *= (1 - param.friction);

		//Update pos
		pos += vel * dt; //Euler method
		//Update time and check if particle should die
		time += dt;
		if ( time >= lifeTime )
		{
			live = false; //Particle is now considered as died
		}
	}
}

void Particle::draw(){

	if ( live )
	{
		//Compute size
		float size = ofMap( fabs(time - lifeTime/2), 0, lifeTime/2, 3, 1 );
		//Compute color
		ofColor color = ofColor::red;
		float hue = ofMap( time, 0, lifeTime, 128, 255 );
		color.setHue( hue );
		ofSetColor( color );
		ofCircle( pos, size ); //Draw particle
	}
}

//--------------------------------------------------------------
void ofApp::setup(){


	ofSetFrameRate(60);

	//Определяем графический буфер
	int w = ofGetWidth();
	int h = ofGetHeight();
	fbo.allocate(w,h,GL_RGB32F_ARB);

	//Заполняем буфер белым цветом
	fbo.begin();
	ofBackground(255,255,255);
	fbo.end();

	//Настраиваем параметры
	param.setup();
	history = 0.9;
	time0 = ofGetElapsedTimef();

	bornRate = 2500;
	bornCount = 0;
}

//--------------------------------------------------------------
void ofApp::update(){

	//Высчитываем время dt
	float time = ofGetElapsedTimef();
	float dt = ofClamp(time-time0,0,0.1);
	time0 = time;

	//Проверяем, активна ли частица, если нет - активируем
	int i=0;
	while (i<p.size())
	{
		if (!p[i].live)
		{
			p.erase(p.begin()+i);
		}
		else
		{
			++i;
		}
	}

	//Порождаем новые частицы
	bornCount += dt * bornRate;
	if (bornCount >= 1)
	{
		int bornN = int(bornCount);
		bornCount -= bornN; //Корректируем значение bornCount
		for (int i=0; i<bornN; ++i)
		{
			Particle newP;
			newP.setup();
			p.push_back(newP);
		}
	}



	//Обновляем частицу
	for (int i=0; i<p.size(); ++i)
	{
		p[i].update(dt);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackground(255,255,255); //Устанавливаем белый фон

	//1. Рисуем в буфер
	fbo.begin();

	//Рисуем полупрозрачный белый прямоугольник, который будет создавать эффект исчезновения
	ofEnableAlphaBlending(); 

	float alpha = (1-history) * 255;
	ofSetColor(255,255,255,alpha);
	ofFill();
	ofRect(0,0,ofGetWidth(),ofGetHeight());

	ofDisableAlphaBlending();

	//Рисуем частицу
	ofFill();
	for (int i=0; i<p.size(); ++i)
	{
		p[i].draw();
	}
	fbo.end();

	//2. Рисуем содержимое буфера на экране
	ofSetColor(255,255,255);
	fbo.draw(0,0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
