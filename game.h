#ifndef I_GAME_H
#define I_GAME_H

#define SCRWIDTH	800
#define SCRHEIGHT	600

namespace Tmpl8 {

class Surface;
class vector3;
class Sprite;
class Game
{
public:
	void SetTarget( Surface* a_Surface ) { screen = a_Surface; }
	void Init();
	void Shutdown() {};
	void DrawBall( int bx, int by );
	void DrawBackdrop();
	void Tick( float a_DT );
	void MouseUp( int a_Nr ) {}
	void MouseDown( int a_Nr ) {}
	void MouseMove( int a_X, int a_Y ) {}
	void KeyUp( int a_Key ) {}
	void KeyDown( int a_Key ) {}
private:
	Surface* screen, *image;
};

}; // namespace Tmpl8

#endif