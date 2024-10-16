#pragma once

#include "GameCamera.h"

const unsigned int	ROTATE_PRE		= 10;
const float			ROTATE_SPEED	= 90.0f;
const float			PLUMB_SPEED		= 3000.0f;

const int			KEY_CAMERA_DOWN		= 'Q';
const int			KEY_CAMERA_UP		= 'E';
const int			KEY_CAMERA_LEFTR	= 'Z';
const int			KEY_CAMERA_RIGHTR	= 'C';
const int			KEY_CAMERA_UPR		= 'R';
const int			KEY_CAMERA_DOWNR	= 'F';

class FPSCameraUpdate : public IFollowCameraObserver
{
public:
	FPSCameraUpdate();
	virtual bool process(FollowCamera *camera, unsigned int tick);

private:
	float	m_angleH;
	float	m_angleV;
	int		m_nOrigin;
	unsigned int	m_uLastRotateTime;
};

extern FPSCameraUpdate fpsCameraUpdate;

//tolua_begin
void fps_open(float times = 1, int dist = -300);
void fps_close();

//tolua_end
