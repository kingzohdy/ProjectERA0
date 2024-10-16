#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>
#include "MoverSin.h"

MoveSin::MoveSin(const GameXYZ &start, unsigned int radius, unsigned int time, int hway, int vway /*= 0*/, unsigned int loop /*= 1*/, float initPhase /*= 0*/) :
	IGameMover(start), cursor(start), swing(radius), during(time), cost(0), angle(hway * M_PI / 180), dgree(vway * M_PI / 180), phaseoffset(initPhase * M_PI / 180)
{
	swingoffset = -swing * sin(phaseoffset);
	period = time * 1.0f / loop;
	lastTime = timeGetTime();
}

void MoveSin::update()
{
	unsigned int now = timeGetTime();
	unsigned int dtick = now - lastTime;
	if (dtick == 0)
	{
		return;
	}
	lastTime = now;
	cost += dtick;
	setpos(cursor);
	if (cost >= during)
	{
		reach();
	}
	else
	{
		float phase = cost / period * M_PI * 2 + phaseoffset;
		float offset = sin(phase) * swing + swingoffset;
		float x = cos(dgree) * offset;
		float y = sin(dgree) * offset;
		sety(gety() + y);
		setx(getx() + cos(angle) * x);
		setz(getz() - sin(angle) * x);
	}
}
