
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

//Anchor��λ��
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

//���涨��(��Ϣ)
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

//���涨��(��������)
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

//���涨��(����Ӣ������)
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

//���涨��(������������)
static const char* UI_TYPE_NAMES_CN[]=
{
	"����",
	"ͼƬ",
	"����",
	"��ť",
	"����",
	"�༭��",
	"�߼��ı���",
	"�߼��༭��",
	"������",
	"������",
};

//���Ʊ�־
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
	DRAWFLAG_ENABLE_EFFECT		=1<<8,//ֻ����enable��ʱ�����effectͼƬ
	DRAWFLAG_FOCUS_EFFECT		=1<<9,//ֻ����focus��ʱ�����effectͼƬ
	DRAWFLAG_CHECK_EFFECT		=1<<10,//ֻ����check��ʱ�����effectͼƬ
	DRAWFLAG_DRAW_EFFECT		=1<<11,//���ǻ���effectͼƬ
	DRAWFLAG_CLIENTPRIOR		=1<<12,//�ͻ��˻���(custombuffer)������UI����
	DRAWFLAG_FOCUSCAPTION		=1<<13,//����Focus��ʱ��ʹ��m_FocusColor����m_DrawColor���ƴ���caption
	DRAWFLAG_HOTCAPTION			=1<<14,//����mouseover��ʱ��ʹ��m_FocusColor����m_DrawColor���ƴ���caption
};

//���涨��(����״̬)
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
	FRMSTATE_VISIBLE		= 1<<7,   //�����Ƿ��ܼ�
	FRMSTATE_UPDATE_SORTED	= 1<<8,   //�Ѿ��������	
	FRMSTATE_UPDATE_ALIGNED	= 1<<9,   //�ӿؼ��Ѿ��������
	FRMSTATE_UPDATE_AJUSTED	= 1<<10,  //���ڴ�С���������
	FRMSTATE_LOADED			= 1<<11,  //�������������
	FRMSTATE_ACTIVED		= 1<<12,  //���ڼ�����
	FRMSTATE_UPDATE_DIRTY	= 1<<13,  //��Ҫִ�д��ڵ��Զ��������	
	FRMSTATE_ACCEPT_IDLE	= 1<<14,  //����idle��Ϣ
	FRMSTATE_PATHMOVE		= 1<<15,  //����·������ƶ�
	FRMSTATE_FLIPMOVE		= 1<<16,  //ֹͣ����·������ƶ�
	FRMSTATE_ALIGN_DIRTY	= 1<<17,  //���ڴ�С��������Ҫ���²��ֺ���
	FRMSTATE_IME_ENABLE		= 1<<18,  //���������뷨�Ƿ��
};

//���涨��(���ڷ��)
enum eUIFrameStyle
{	
	FRMSTYLE_NONE				= 0,
	FRMSTYLE_FLASH				= 1,//��˸
	FRMSTYLE_VERTICAL			= 1<<1,//��ֱ
	FRMSTYLE_PRIMARY			= 1<<2, //�ɶԴ����е������ڣ����磺һ��spin�����е�up spin��
	FRMSTYLE_CLIPSELF			= 1<<3, 
	FRMSTYLE_CLIPCHILD_WINDOW   = 1<<4,
	FRMSTYLE_CLIPCHILD_CLIENT   = 1<<5,
	FRMSTYLE_SYSTEMDRAW			= 1<<6,
	FRMSTYLE_ADVANCE			= 1<<7,
	FRMSTYLE_POPUP				= 1<<8,
	FRMSTYLE_SELF_XMOVE			= 1<<9,//�ɷ�����϶�����X���ƶ�
	FRMSTYLE_SELF_YMOVE			= 1<<10,//�ɷ�����϶�����Y���ƶ�		
	FRMSTYLE_SELF_XSIZE			= 1<<11,//�ɷ�����X�������С
	FRMSTYLE_SELF_YSIZE			= 1<<12,//�ɷ�����Y�������С
	FRMSTYLE_STATIC				= 1<<13,//�Ƿ��Ǿ�̬����
	FRMSTYLE_TOPMOST			= 1<<14,//�Ƿ��Ƕ��㴰��
	FRMSTYLE_ALPHAWIN			= 1<<15,//�Ƿ���͸����ⴰ��(͸�������㴰����)
	FRMSTYLE_FLIPDRAG			= 1<<16,//�Ƿ��Ƿ�ת�϶�������(�϶���ʵ���ǲ����϶���)
	FRMSTYLE_TEMPLATE			= 1<<17,//�Ƿ���ģ�崰��
	FRMSTYLE_ADJUSTSELF			= 1<<18,//�Ƿ�ü��Լ��Ļ�������
	FRMSTYLE_AJUSTFOLLOW		= 1<<19,//������ajustwidth����ajustheightʱ���Լ�Ҳajust
	FRMSTYLE_CLEANWIN			= 1<<20,//����Ӧ�����������������Ҳ����Ĵ���,���,��Ȼ���ý�ɫ�߶�
	FRMSTYLE_EDITABLE			= 1<<21,//�Ƿ���Ա༭
	FRMSTYLE_COLORTEST			= 1<<22,//�Ƿ�̽�������ɫ����λ��̽��callback������ɫ������λ��̽�ⱳ��ͼ��ɫ��
	FRMSTYLE_CUSTOMCLOSE		= 1<<23,//�Ƿ�ȡ��onok,oncancel,endwidgetʱ����Զ��ر�
	FRMSTYLE_CUSTOMALPHA		= 1<<24,//�Ƿ�ȡ��͸���ȵ��Զ�����
	FRMSTYLE_RENDERBUFFER		= 1<<25,//�Ƿ����CustomBuffer
	FRMSTYLE_RENDERITEM			= 1<<26,//�Ƿ����CustomItem
	FRMSTYLE_AREAWAKE			= 1<<27,//�Ƿ񾯾���������ƶ�	
	FRMSTYLE_FATHERENABLE		= 1<<28,//�Ƿ��ע�����ڵ�enable״̬	
	FRMSTYLE_ALIGNFOLLOW		= 1<<29,//�Ƿ���ajustdirtyʱ������align����
	FRMSTYLE_AUTOACTIVE			= 1<<30,//�Ƿ���mouseoverʱ��activeself	
};

//���涨��(������չ���)
enum eUIFramePattern
{
	FRMPATTERN_NONE				= 0,
	FRMPATTERN_RAWVISIBLE		= 1,//�������Ӿ����ɼ���ʱ��,������
	FRMPATTERN_ALPHAVISIBLE		= 2,//�������Ӿ��ɼ���ʱ��,�Ƿ������renderfactor����	
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
	UI_EDITMETHOD_ENGLISH	= 1,//Ӣ��
	UI_EDITMETHOD_NUMBER	= 2,//����
	UI_EDITMETHOD_SYMBOL	= 4,//����
	UI_EDITMETHOD_CHINESE	= 8,//����

	UI_EDITMETHOD_VALUE		= 16,//��ֵ
	UI_EDITMETHOD_CODER		= 32,//����
	UI_EDITMETHOD_DISABLE	= 64,//��ֹ����
	UI_EDITMETHOD_DISABLE_COPY= 128,//��ֹ����
	UI_EDITMOTHOD_DISABLE_PASTE = 256,//��ֹճ��
	UI_EDITMOTHOD_UNDERLINE	= 512,	//�»���
};

//�ؼ����з�ʽ:ˮƽ/��ֱ
enum ORIENTATION_T
{
	ORIENTATION_HORIZONTAL = 1,
	ORIENTATION_VERTICAL   = 2,
};

//�༭ģʽ
enum UIMODE_T
{
	UI_MODE_DEFAULT	=0,
	UI_MODE_DEBUG   =1,

};

//��Ϣ����
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

