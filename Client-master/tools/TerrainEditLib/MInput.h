#pragma once

namespace TE
{

class MInput
{
public:
	MInput(void);
	~MInput(void);

	int   KeyDown( char  key );
    
	int   LControlKeyDown();
	int   RControlKeyDown();

	int   LMenuKeyDown();
	int   RMenuKeyDown();

	int   LShiftKeyDown();
	int   RShiftKeyDown();
};


};