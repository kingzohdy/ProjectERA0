
#ifndef _GAMEKEYMOVEOPERATE_H_
#define _GAMEKEYMOVEOPERATE_H_

//tolua_begin
enum
{
	KEYMOVE_UP,
	KEYMOVE_DOWN,
	KEYMOVE_LEFT,
	KEYMOVE_RIGHT,
};
//tolua_end

class GameKeyMoveOperate//tolua_export
{//tolua_export
private:
	unsigned int	m_uLastMoveTime;
	unsigned int	m_uLastChangeTime;
	double			m_fLastAngle;
	double			m_lastKeyCmd;
	bool			m_isStop;
	bool			m_fps;
	bool			m_god;

	int				KEY_UP;
	int				KEY_DOWN;
	int				KEY_LEFT;
	int				KEY_RIGHT;

protected:
	GameKeyMoveOperate();
public:
	~GameKeyMoveOperate();

public:
	static GameKeyMoveOperate & getSingleton();

public:
	void update();
	void reset();

public:
	bool MoveStep(float angle);
	void MoveStop();
	void SetFPS(bool b = true);
	bool IsFPS();
	void SetGod(bool b = true);
	bool IsGod();

	//tolua_begin
	void StartMove(int type, int key);
	void StopMove(int type);
	//tolua_end
};//tolua_export

#endif
