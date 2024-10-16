#pragma once


namespace GhostSchema//tolua_export
{//tolua_export
	extern bool isGhost;
	extern int height;
	extern float times;

	//tolua_begin
	void enter(int i);
	void exit();
	//tolua_end
	void update();
}//tolua_export
