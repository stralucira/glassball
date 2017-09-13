#include "template.h"

#define BALLS	20

float ballx[BALLS], bally[BALLS], vx[BALLS], vy[BALLS];

// -----------------------------------------------------------
// Scale a color (range: 0..128, where 128 is 100%)
// -----------------------------------------------------------
inline unsigned int ScaleColor( unsigned int orig, char scale )
{
	const Pixel rb = ((scale * (orig & ((255 << 16) + 255))) >> 7) & ((255 << 16) + 255);
	const Pixel g = ((scale * (orig & (255 << 8))) >> 7) & (255 << 8);
	return (Pixel)rb + g;
}

// -----------------------------------------------------------
// Draw a glass ball using fake reflection & refraction
// -----------------------------------------------------------
void Game::DrawBall( int bx, int by )
{
	Pixel* dst = screen->GetBuffer() + bx + by * screen->GetPitch();
	Pixel* src = image->GetBuffer();
	for ( int y = 0; y < 128; y++ )
	{
		float dy = (float)(y - 64);
		for ( int x = 0; x < 128; x++ )
		{
			float dx = (float)(x - 64);
			float dist = sqrtf( dx * dx + dy * dy );
			if (dist < 64)
			{
				int xoffs = (int)((dist / 2 + 10) * sinf( 0.025f * (float)(x - 50) ) );
				int yoffs = (int)((dist / 2 + 10) * sinf( 0.025f * (float)(y - 50) ) );
				int u1 = ((bx + x) - 4 * xoffs) % SCRWIDTH;
				int v1 = ((by + y) - 4 * yoffs) % SCRHEIGHT;
				int u2 = ((bx + x) + 2 * xoffs) % SCRWIDTH;
				int v2 = ((by + y) + 2 * yoffs) % SCRHEIGHT;
				Pixel refl = src[max( 0, u1 ) + max( 0, v1 ) * image->GetWidth()];
				Pixel refr = src[max( 0, u2 ) + max( 0, v2 ) * image->GetWidth()];
				int reflscale = min( 127, (int)(127.0f - 0.015f * (1 - dist) * (1 - dist)) );
				int refrscale = min( 127, (int)(0.030f * (1 - dist) * (1 - dist)) );
				Pixel color1 = ScaleColor( refl, 127 - reflscale );
				Pixel color2 = ScaleColor( refr, 127 - refrscale );
				unsigned int red = MIN( 0xff0000, (color1 & 0xff0000) + (color2 & 0xff0000) );
				unsigned int grn = MIN( 0x00ff00, (color1 & 0x00ff00) + (color2 & 0x00ff00) );
				unsigned int blu = MIN( 0x0000ff, (color1 & 0x0000ff) + (color2 & 0x0000ff) );
				dst[x + y * screen->GetPitch()] = red + grn + blu;
			}
		}
	}
}

// -----------------------------------------------------------
// Initialize the game
// -----------------------------------------------------------
void Game::Init()
{
	image = new Surface( "testdata/mountains.png" );
	for( int i = 0; i < BALLS; i++ ) 
	{
		ballx[i] = 100 + Rand( 500 );
		bally[i] = 200 - Rand( 150 );
		vx[i] = (Rand( 1.0f ) > 0.5f) ? 1.6f : -1.6f;
		vy[i] = 0;
	}
}

// -----------------------------------------------------------
// Draw the backdrop and make it a bit darker
// -----------------------------------------------------------
void Game::DrawBackdrop()
{
	image->CopyTo( screen, 0, 0 );
	Pixel* src = screen->GetBuffer();
	unsigned int count = screen->GetPitch() * screen->GetHeight();
	for ( unsigned int i = 0; i < count; i++ ) src[i] = ScaleColor( src[i], 40 );
}

// -----------------------------------------------------------
// Main game tick function
// -----------------------------------------------------------
void Game::Tick( float a_DT )
{
	screen->Clear( 0 );
	DrawBackdrop();
	for( int i = 0; i < BALLS; i++ )
	{
		DrawBall( (int)ballx[i], (int)bally[i] );
		bally[i] += vy[i];
		ballx[i] += vx[i];
		vy[i] += 0.2f;
		if (bally[i] > (SCRHEIGHT - 128))
		{
			bally[i] = SCRHEIGHT - 128;
			vy[i] = -0.96f * vy[i];
		}
		if (ballx[i] > (SCRWIDTH - 128))
		{
			ballx[i] = SCRWIDTH - 128;
			vx[i] = -vx[i];
		}
		if (ballx[i] < 0)
		{
			ballx[i] = 0;
			vx[i] = -vx[i];
		}
	}
}












