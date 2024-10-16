#pragma once

class FontString;
class Button;
class Frame;
class Texture;

//tolua_begin
extern void ShowUIPanel(const char *frameName);
extern void HideUIPanel(const char *frameName);
extern void UIBeginDrag(const char *path, int id1, int id2, int iNum, bool bCompose = true, int hotspotx = 0, int hotspoty = 0);
extern void UIEndDrag();
extern bool UIIsInDragState();
extern int UIGetDragUserData1();
extern int UIGetDragUserData2();
extern int UIGetDragUserData3();
extern Button* CreateButton( const char* name, const char* template_name = NULL, Frame* pParent = NULL, 
								bool isRoot = false );
extern FontString* FindFontString( const char* name );
extern Button* FindButton( const char* name );
extern bool  isAlreadyExistFrame( const char* name );
extern Texture* FindTexture( const char* name );
// 关闭所有的对话泡泡
extern void closePopWin();

extern void SetCurrentCursor(const char *cursorstate);
extern void ChangeCursorState(const char *cursorstate);
extern const char *GetCurrentCursor();
extern int GetCurrentCursorLevel();
extern int GetScreenWidth();
extern int GetScreenHeight();
extern void SetFocusFrame(const char *frameName);
extern void SetFocusFrame(Frame *frame);
// 获取当前点中控件的父窗口
extern const char *GetFocusFrameParent();
extern void ClearFocus();
extern int GetCursorPosX();
extern int GetCursorPosY();
extern void pushUnCloseWin(const char* frameName);
extern const char *GetCurrentDialogFrame();
extern bool isPointInFrame( const char* pszFrameName );
extern bool isPointInFrame( int	x, int	y, const char* pszFrameName );
extern bool isTwoFrameXConflict( LayoutFrame* pLeft, LayoutFrame* pRight );
void		CopyToMemory( const char* pszText );
bool		isFiliation( const char* pszParentFrame, const char* pszChildFrame ); 
void		addChangedFrames( const char* pszFrameName );
void		addChangedFrames( LayoutFrame* pFrame );
extern void playUISound( const char* pszSoundPath );
float	GetScreenScaleY();
float	GetScreenScaleX();
float	GetFontTextWidth( FontString* pFont );
// 判断文件是否存在
extern	bool IsInExistence( const char* szFileName ); 
// 判断一个控件是否在另一个控件的显示区域
bool isInVisibleArea( const LayoutFrame* pIncludedFrame, const LayoutFrame* pClipFrame );

extern bool isKeyPressed(int key);
extern int getRand( int nMinNum, int nMaxNum );

extern const char* GetFocusFrameName();
extern const char* replaceFaceString( const char* pszText );

extern const char* BigInt2Str( long long val );

void updateHeadBindingFrame( const char* pszFrameName, int x, int y, int nWidth, int mHeight );

void showUIFrames();
//tolua_end