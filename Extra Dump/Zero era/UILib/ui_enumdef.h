
//layout anchor
enum FRAMEPOINT_T
{
	FP_TOPLEFT = 0,
	FP_TOPRIGHT,
	FP_BOTTOMLEFT,
	FP_BOTTOMRIGHT,
	FP_TOP,
	FP_BOTTOM,
	FP_LEFT,
	FP_RIGHT,
	FP_CENTER,

	FP_MAXPOINTPOS,
};

//Anchor的位置
static char *s_FramePoint[] =
{
	"TOPLEFT",
	"TOPRIGHT",
	"BOTTOMLEFT",
	"BOTTOMRIGHT",
	"TOP",
	"BOTTOM",
	"LEFT",
	"RIGHT",
	"CENTER",
};

//frame
enum FRAMELEVEL_T
{

};

//FRAMESTRATA
static char *s_FrameStrata[] =
{
	"NoDefined",
	"PARENT",
	"BACKGROUND",
	"LOW",
	"MEDIUM",
	"HIGH",
	"DIALOG",
	"FULLSCREEN",
	"FULLSCREEN_DIALOG",
	"TOOLTIP",
};

enum FRAMESTRATA_T
{
	FS_NO_DEFINED = 0,

	FS_PARENT,
	FS_BACKGROUND,
	FS_LOW,
	FS_MEDIUM,
	FS_HIGH,
	FS_DIALOG,
	FS_FULLSCREEN,
	FS_FULLSCREEN_DIALOG,
	FS_TOOLTIP,
	FS_MAX_STRATA,
};

//FRAMESTRATA
static char *s_Drawlayer[] =
{
	"background",
	"border",
	"artwork",
	"overlay",
	"highlight",
	"numlayers",
};

enum DRAWLAYER_T
{
	DL_BACKGROUND = 0,
	DL_BORDER,
	DL_ARTWORK,
	DL_OVERLAY,
	DL_HIGHLIGHT,

	DL_NUMLAYERS,
};

enum INSERT_MODE_T
{
	INSERT_TOP = 0,
	INSERT_BOTTOM,
};

//界面定义(消息)
enum UIMESSAGE_T
{
	//MESSAGE
	MSGTYPE_UPDATE,
	MSGTYPE_PAINT,
	MSGTYPE_CHAR,
	MSGTYPE_KEYDOWN,
	MSGTYPE_KEYUP,
	MSGTYPE_LEFTBTNDOWN,
	MSGTYPE_LEFTBTNUP,
	MSGTYPE_LBTNDBLCLK,
	MSGTYPE_RIGHTBTNDOWN,
	MSGTYPE_RIGHTBTNUP,
	MSGTYPE_MOUSEMOVE,
	MSGTYPE_MOUSEIN,
	MSGTYPE_MOUSEOUT,
	MSGTYPE_GAINFOCUS,
	MSGTYPE_LOSTFOCUS,
	MSGTYPE_TIMER,
	MSGTYPE_MOUSEWHEEL,
	MSGTYPE_MOUSEIDLE,
};

//界面定义(窗口类型)
enum eUIFrameType
{
	FRM_FRAME = 0,
	FRM_TEXTURE,
	FRM_FONTSTRING,
	FRM_BUTTON,
	FRM_CHECKBUTTON,
	FRM_COLORSELECT,
	FRM_EDITBOX,
	FRM_GAMETOOLTIP,
	FRM_LABEL,
	FRM_MESSAGEFRAME,
	FRM_RICHEDITTEXT,
	FRM_RICHEDITBOX,
	FRM_SCROLLFRAME,
	FRM_SLIDER,
	FRM_STATUSBAR, 

	//FRM_FRAME = 0,	FRM_ROOT,		FRM_SCREEN,
	//FRM_BUTTON,		FRM_CHECKButton,FRM_SPIN,		FRM_KEYEDIT,	
	//FRM_LABEL,		FRM_EDITBOX,	FRM_RICHEDIT,	FRM_MULTIEDIT,
	//FRM_LISTBASE,	FRM_LISTBOX,	FRM_LISTFILE,	FRM_COMBOBOX,
	//FRM_CHANGEBAR,	FRM_PROGRESSBAR,FRM_SCROLLBAR,	
	//FRM_MENU,		FRM_TABWIN,		FRM_TABLIST,
	//FRM_CONTAINER,	FRM_WIDGETLIST,	FRM_SWITCHER,
	//FRM_DIALOG,		FRM_SYSDIALOG,	FRM_MESSAGEBOX,
	//FRM_TEXTLIST,	FRM_TREECTRL,
	//FRM_CONSOLE,	FRM_PICKER,
};

//界面定义(窗口英文名称)
static const char* UI_TYPE_NAMES_ENG[]=
{
	"Frame",
	"Texture",
	"FontString",
	"Button",
	"CheckButton",
	"EditBox",
	"RichText",
	"RichEditBox",
	"ScrollFrame",
	"Slider",
};

//界面定义(窗口中文名称)
static const char* UI_TYPE_NAMES_CN[]=
{
	"窗口",
	"图片",
	"字体",
	"按钮",
	"检查框",
	"编辑框",
	"高级文本框",
	"高级编辑框",
	"滚动框",
	"滑动条",
};

//绘制标志
enum eUIDrawFlag
{	
	DRAWFLAG_NONE				=0,
	DRAWFLAG_WIN_EDGE			=1,
	DRAWFLAG_WIN_FILL			=1<<1,
	DRAWFLAG_CLIENT_EDGE		=1<<2,
	DRAWFLAG_CLIENT_FILL		=1<<3,
	DRAWFLAG_CAPTION_EDGE		=1<<4,
	DRAWFLAG_CAPTION_FILL		=1<<5,
	DRAWFLAG_DRAG_EDGE			=1<<6,
	DRAWFLAG_DRAG_FILL			=1<<7,
	DRAWFLAG_ENABLE_EFFECT		=1<<8,//只是在enable的时候绘制effect图片
	DRAWFLAG_FOCUS_EFFECT		=1<<9,//只是在focus的时候绘制effect图片
	DRAWFLAG_CHECK_EFFECT		=1<<10,//只是在check的时候绘制effect图片
	DRAWFLAG_DRAW_EFFECT		=1<<11,//总是绘制effect图片
	DRAWFLAG_CLIENTPRIOR		=1<<12,//客户端绘制(custombuffer)优先于UI绘制
	DRAWFLAG_FOCUSCAPTION		=1<<13,//窗口Focus的时候，使用m_FocusColor代替m_DrawColor绘制窗口caption
	DRAWFLAG_HOTCAPTION			=1<<14,//窗口mouseover的时候，使用m_FocusColor代替m_DrawColor绘制窗口caption
};

//界面定义(窗口状态)
enum eUIFrameState
{
	FRMSTATE_NORMAL			= 0,
	FRMSTATE_MOUSEOVER		= 1,	  //highlighted
	FRMSTATE_CLICKDOWN		= 1<<1,   //pushed
	FRMSTATE_FOCUSED		= 1<<2,   //focused
	FRMSTATE_DISABLE		= 1<<3,   //self disabled
	FRMSTATE_CHECKED		= 1<<4,   //checked
	FRMSTATE_DISCARDED		= 1<<5,   //discarded
	FRMSTATE_DRAGGING		= 1<<6,   //dragging
	FRMSTATE_VISIBLE		= 1<<7,   //自身是否能见
	FRMSTATE_UPDATE_SORTED	= 1<<8,   //已经排序完毕	
	FRMSTATE_UPDATE_ALIGNED	= 1<<9,   //子控件已经布局完毕
	FRMSTATE_UPDATE_AJUSTED	= 1<<10,  //窗口大小调整完毕了
	FRMSTATE_LOADED			= 1<<11,  //窗口载入完毕了
	FRMSTATE_ACTIVED		= 1<<12,  //窗口激活了
	FRMSTATE_UPDATE_DIRTY	= 1<<13,  //需要执行窗口的自定义更新了	
	FRMSTATE_ACCEPT_IDLE	= 1<<14,  //接受idle消息
	FRMSTATE_PATHMOVE		= 1<<15,  //沿着路径点的移动
	FRMSTATE_FLIPMOVE		= 1<<16,  //停止沿着路径点的移动
	FRMSTATE_ALIGN_DIRTY	= 1<<17,  //窗口大小调整后，需要重新布局孩子
	FRMSTATE_IME_ENABLE		= 1<<18,  //窗口中输入法是否打开
};

//界面定义(窗口风格)
enum eUIFrameStyle
{	
	FRMSTYLE_NONE				= 0,
	FRMSTYLE_FLASH				= 1,//闪烁
	FRMSTYLE_VERTICAL			= 1<<1,//竖直
	FRMSTYLE_PRIMARY			= 1<<2, //成对窗口中的主窗口（例如：一对spin窗口中的up spin）
	FRMSTYLE_CLIPSELF			= 1<<3, 
	FRMSTYLE_CLIPCHILD_WINDOW   = 1<<4,
	FRMSTYLE_CLIPCHILD_CLIENT   = 1<<5,
	FRMSTYLE_SYSTEMDRAW			= 1<<6,
	FRMSTYLE_ADVANCE			= 1<<7,
	FRMSTYLE_POPUP				= 1<<8,
	FRMSTYLE_SELF_XMOVE			= 1<<9,//可否被鼠标拖动沿着X轴移动
	FRMSTYLE_SELF_YMOVE			= 1<<10,//可否被鼠标拖动沿着Y轴移动		
	FRMSTYLE_SELF_XSIZE			= 1<<11,//可否沿着X轴调整大小
	FRMSTYLE_SELF_YSIZE			= 1<<12,//可否沿着Y轴调整大小
	FRMSTYLE_STATIC				= 1<<13,//是否是静态窗口
	FRMSTYLE_TOPMOST			= 1<<14,//是否是顶层窗口
	FRMSTYLE_ALPHAWIN			= 1<<15,//是否是透明检测窗口(透明区域不算窗口区)
	FRMSTYLE_FLIPDRAG			= 1<<16,//是否是翻转拖动区窗口(拖动区实际是不可拖动区)
	FRMSTYLE_TEMPLATE			= 1<<17,//是否是模板窗口
	FRMSTYLE_ADJUSTSELF			= 1<<18,//是否裁减自己的绘制内容
	FRMSTYLE_AJUSTFOLLOW		= 1<<19,//当父亲ajustwidth或者ajustheight时，自己也ajust
	FRMSTYLE_CLEANWIN			= 1<<20,//不响应左键点击，避免干扰玩家操作的窗口,点击,仍然会让角色走动
	FRMSTYLE_EDITABLE			= 1<<21,//是否可以编辑
	FRMSTYLE_COLORTEST			= 1<<22,//是否探测绘制颜色（置位：探测callback函数颜色，不置位：探测背景图颜色）
	FRMSTYLE_CUSTOMCLOSE		= 1<<23,//是否取消onok,oncancel,endwidget时候的自动关闭
	FRMSTYLE_CUSTOMALPHA		= 1<<24,//是否取消透明度的自动传递
	FRMSTYLE_RENDERBUFFER		= 1<<25,//是否绘制CustomBuffer
	FRMSTYLE_RENDERITEM			= 1<<26,//是否绘制CustomItem
	FRMSTYLE_AREAWAKE			= 1<<27,//是否警觉鼠标区域移动	
	FRMSTYLE_FATHERENABLE		= 1<<28,//是否关注父窗口的enable状态	
	FRMSTYLE_ALIGNFOLLOW		= 1<<29,//是否在ajustdirty时，重新align孩子
	FRMSTYLE_AUTOACTIVE			= 1<<30,//是否在mouseover时，activeself	
};

//界面定义(窗口扩展风格)
enum eUIFramePattern
{
	FRMPATTERN_NONE				= 0,
	FRMPATTERN_RAWVISIBLE		= 1,//当窗口视觉不可见的时候,不交互
	FRMPATTERN_ALPHAVISIBLE		= 2,//当窗口视觉可见的时候,是否绘制由renderfactor决定	
};

//fontstring
enum OUTLINE_T
{
	NONE = 0,
	NORMAL,
	THICK,
};

enum JUSTIFYV_T
{
	JUSTIFYV_TOP = 0,
	JUSTIFYV_MIDDLE,
	JUSTIFYV_BOTTOM,
};

enum JUSTIFYH_T
{
	JUSTIFYH_LEFT = 0,
	JUSTIFYH_CENTER,
	JUSTIFYH_RIGHT,
};

enum UIEDITMETHOD_T
{
	UI_EDITMETHOD_ENGLISH	= 1,//英文
	UI_EDITMETHOD_NUMBER	= 2,//数字
	UI_EDITMETHOD_SYMBOL	= 4,//符号
	UI_EDITMETHOD_CHINESE	= 8,//中文

	UI_EDITMETHOD_VALUE		= 16,//数值
	UI_EDITMETHOD_CODER		= 32,//密码
	UI_EDITMETHOD_DISABLE	= 64,//禁止输入
	UI_EDITMETHOD_DISABLE_COPY= 128,//禁止复制
	UI_EDITMOTHOD_DISABLE_PASTE = 256,//禁止粘贴
	UI_EDITMOTHOD_UNDERLINE	= 512,	//下划线
};

//控件排列方式:水平/垂直
enum ORIENTATION_T
{
	ORIENTATION_HORIZONTAL = 1,
	ORIENTATION_VERTICAL   = 2,
};

//编辑模式
enum UIMODE_T
{
	UI_MODE_DEFAULT	=0,
	UI_MODE_DEBUG   =1,

};

//消息类型
static char *s_EventName[] = 
{
	"onanimfinisher",
	"onchar",
	"onfinishchar",
	"onquerychar",
	"onclick",
	"oncolorselect",
	"oncursorchanger",
	"ondoubleclick",
	"ondragstart",
	"ondragstop",
	"oneditfocusgained",
	"oneditfocuslost",
	"onenter",
	"onenterpressed",
	"onctrlenterpressed",
	"onescapepressed",
	"onevent",
	"onfocusgained",
	"onfocuslost",
	"onhide",
	"onhorizontalscroll",
	"onhyperlinkclick",
	"onhyperlinkenter",
	"onhyperlinkleave",
	"oninputlanguagechanged",
	"onkeydown",
	"onkeyup",
	"onleave",
	"onload",
	"onmessagescrollchanged",
	"onmousedown",
	"onmouseup",
	"onmousewheel",
	"onmousemove",
	"onmoviefinished",
	"onmoviehidesubtitle",
	"onmovieshowsubtitle",
	"onreceivedrag",
	"onscrollrangechanged",
	"onshow",
	"onsizechanged",
	"onspacepressed",
	"ontabpressed",
	"ontextchanged",
	"ontextset",
	"onupdate",
	"onupdatemodel",
	"onvaluechanged",
	"onverticalscroll",
	"onbegindrag",
	"onbuttonclick",
	"onwebeven",
	"onendmove"
};

