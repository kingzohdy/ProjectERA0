/*
** Lua binding: UITolua
** Generated automatically by tolua++-1.0.92 on 12/06/16 02:36:50.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_UITolua_open (lua_State* tolua_S);

#include "ui_common.h"
#include "ui_layoutframe.h"
#include "ui_fontstring.h"
#include "ui_texture.h"
#include "ui_modelview.h"
#include "ui_frame.h"
#include "ui_button.h"
#include "ui_checkbutton.h"
#include "ui_editbox.h"
#include "ui_richtext.h"
#include "ui_scrollframe.h"
#include "ui_slider.h"
#include "ui_gameui.h"
#include "ui_drawlineframe.h"
#include "ui_multieditbox.h"
#include "ui_webbrowerframe.h"
#include "ui_scriptfunc.h"
#include "ui_framemgr.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_Ogre__HUIRES (lua_State* tolua_S)
{
 Ogre::HUIRES* self = (Ogre::HUIRES*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_tagRect_ToLua (lua_State* tolua_S)
{
 tagRect_ToLua* self = (tagRect_ToLua*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"UIObject");
 tolua_usertype(tolua_S,"DrawLineFrame");
 tolua_usertype(tolua_S,"FRAMEPOINT_T");
 tolua_usertype(tolua_S,"FontInstance");
 tolua_usertype(tolua_S,"ModelView");
 tolua_usertype(tolua_S,"tagRect_ToLua");
 tolua_usertype(tolua_S,"Texture");
 tolua_usertype(tolua_S,"WebBrowerFrame");
 tolua_usertype(tolua_S,"Ogre::HUIRES");
 tolua_usertype(tolua_S,"FrameManager");
 tolua_usertype(tolua_S,"MultiEditBox");
 tolua_usertype(tolua_S,"std::deque<Ogre::Point2D>");
 tolua_usertype(tolua_S,"Slider");
 tolua_usertype(tolua_S,"EditBox");
 tolua_usertype(tolua_S,"ScrollFrame");
 tolua_usertype(tolua_S,"Ogre::LoadWrap");
 tolua_usertype(tolua_S,"CheckButton");
 tolua_usertype(tolua_S,"RichText");
 tolua_usertype(tolua_S,"LayoutFrame");
 tolua_usertype(tolua_S,"Frame");
 tolua_usertype(tolua_S,"Button");
 tolua_usertype(tolua_S,"FontString");
}

/* function: IsPrint */
#ifndef TOLUA_DISABLE_tolua_UITolua_IsPrint00
static int tolua_UITolua_IsPrint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  char c = ((char)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  IsPrint(c);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsPrint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: IsDBCSChar */
#ifndef TOLUA_DISABLE_tolua_UITolua_IsDBCSChar00
static int tolua_UITolua_IsDBCSChar00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* buffer = ((const char*)  tolua_tostring(tolua_S,1,0));
  int pos = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  IsDBCSChar(buffer,pos);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsDBCSChar'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: IsDBCSHead */
#ifndef TOLUA_DISABLE_tolua_UITolua_IsDBCSHead00
static int tolua_UITolua_IsDBCSHead00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* buffer = ((const char*)  tolua_tostring(tolua_S,1,0));
  int pos = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  IsDBCSHead(buffer,pos);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsDBCSHead'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: IsDBCSTail */
#ifndef TOLUA_DISABLE_tolua_UITolua_IsDBCSTail00
static int tolua_UITolua_IsDBCSTail00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* buffer = ((const char*)  tolua_tostring(tolua_S,1,0));
  int pos = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  IsDBCSTail(buffer,pos);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsDBCSTail'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetName of class  UIObject */
#ifndef TOLUA_DISABLE_tolua_UITolua_UIObject_SetName00
static int tolua_UITolua_UIObject_SetName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"UIObject",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  UIObject* self = (UIObject*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetName'",NULL);
#endif
  {
   self->SetName(name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetName of class  UIObject */
#ifndef TOLUA_DISABLE_tolua_UITolua_UIObject_GetName00
static int tolua_UITolua_UIObject_GetName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"UIObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  UIObject* self = (UIObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetName'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->GetName();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  tagRect_ToLua */
#ifndef TOLUA_DISABLE_tolua_UITolua_tagRect_ToLua_new00
static int tolua_UITolua_tagRect_ToLua_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"tagRect_ToLua",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   tagRect_ToLua* tolua_ret = (tagRect_ToLua*)  new tagRect_ToLua();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"tagRect_ToLua");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  tagRect_ToLua */
#ifndef TOLUA_DISABLE_tolua_UITolua_tagRect_ToLua_new00_local
static int tolua_UITolua_tagRect_ToLua_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"tagRect_ToLua",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   tagRect_ToLua* tolua_ret = (tagRect_ToLua*)  new tagRect_ToLua();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"tagRect_ToLua");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: empty of class  tagRect_ToLua */
#ifndef TOLUA_DISABLE_tolua_UITolua_tagRect_ToLua_empty00
static int tolua_UITolua_tagRect_ToLua_empty00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"tagRect_ToLua",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  tagRect_ToLua* self = (tagRect_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'empty'",NULL);
#endif
  {
   self->empty();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'empty'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWidth of class  tagRect_ToLua */
#ifndef TOLUA_DISABLE_tolua_UITolua_tagRect_ToLua_getWidth00
static int tolua_UITolua_tagRect_ToLua_getWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"tagRect_ToLua",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  tagRect_ToLua* self = (tagRect_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWidth'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getWidth();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getHeight of class  tagRect_ToLua */
#ifndef TOLUA_DISABLE_tolua_UITolua_tagRect_ToLua_getHeight00
static int tolua_UITolua_tagRect_ToLua_getHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"tagRect_ToLua",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  tagRect_ToLua* self = (tagRect_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getHeight'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getHeight();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_nLeft of class  tagRect_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagRect_ToLua_m_nLeft
static int tolua_get_tagRect_ToLua_m_nLeft(lua_State* tolua_S)
{
  tagRect_ToLua* self = (tagRect_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nLeft'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_nLeft);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_nLeft of class  tagRect_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagRect_ToLua_m_nLeft
static int tolua_set_tagRect_ToLua_m_nLeft(lua_State* tolua_S)
{
  tagRect_ToLua* self = (tagRect_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nLeft'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_nLeft = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_nRight of class  tagRect_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagRect_ToLua_m_nRight
static int tolua_get_tagRect_ToLua_m_nRight(lua_State* tolua_S)
{
  tagRect_ToLua* self = (tagRect_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nRight'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_nRight);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_nRight of class  tagRect_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagRect_ToLua_m_nRight
static int tolua_set_tagRect_ToLua_m_nRight(lua_State* tolua_S)
{
  tagRect_ToLua* self = (tagRect_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nRight'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_nRight = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_nTop of class  tagRect_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagRect_ToLua_m_nTop
static int tolua_get_tagRect_ToLua_m_nTop(lua_State* tolua_S)
{
  tagRect_ToLua* self = (tagRect_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nTop'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_nTop);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_nTop of class  tagRect_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagRect_ToLua_m_nTop
static int tolua_set_tagRect_ToLua_m_nTop(lua_State* tolua_S)
{
  tagRect_ToLua* self = (tagRect_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nTop'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_nTop = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_nBottom of class  tagRect_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagRect_ToLua_m_nBottom
static int tolua_get_tagRect_ToLua_m_nBottom(lua_State* tolua_S)
{
  tagRect_ToLua* self = (tagRect_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nBottom'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_nBottom);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_nBottom of class  tagRect_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagRect_ToLua_m_nBottom
static int tolua_set_tagRect_ToLua_m_nBottom(lua_State* tolua_S)
{
  tagRect_ToLua* self = (tagRect_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nBottom'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_nBottom = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: Show of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_Show00
static int tolua_UITolua_LayoutFrame_Show00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Show'",NULL);
#endif
  {
   self->Show();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Show'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Active of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_Active00
static int tolua_UITolua_LayoutFrame_Active00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Active'",NULL);
#endif
  {
   self->Active();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Active'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Hide of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_Hide00
static int tolua_UITolua_LayoutFrame_Hide00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Hide'",NULL);
#endif
  {
   self->Hide();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Hide'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsShown of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_IsShown00
static int tolua_UITolua_LayoutFrame_IsShown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsShown'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsShown();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsShown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetAnchorOffsetX of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_GetAnchorOffsetX00
static int tolua_UITolua_LayoutFrame_GetAnchorOffsetX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetAnchorOffsetX'",NULL);
#endif
  {
   float tolua_ret = (float)  self->GetAnchorOffsetX();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetAnchorOffsetX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetAnchorOffsetY of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_GetAnchorOffsetY00
static int tolua_UITolua_LayoutFrame_GetAnchorOffsetY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetAnchorOffsetY'",NULL);
#endif
  {
   float tolua_ret = (float)  self->GetAnchorOffsetY();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetAnchorOffsetY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeRelFrames of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_removeRelFrames00
static int tolua_UITolua_LayoutFrame_removeRelFrames00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
  LayoutFrame* pFrame = ((LayoutFrame*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeRelFrames'",NULL);
#endif
  {
   self->removeRelFrames(pFrame);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeRelFrames'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPoint of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_SetPoint00
static int tolua_UITolua_LayoutFrame_SetPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
  const char* strpoint = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* relFrame = ((const char*)  tolua_tostring(tolua_S,3,0));
  const char* strrelpoint = ((const char*)  tolua_tostring(tolua_S,4,0));
  int xoffset = ((int)  tolua_tonumber(tolua_S,5,0));
  int yoffset = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPoint'",NULL);
#endif
  {
   self->SetPoint(strpoint,relFrame,strrelpoint,xoffset,yoffset);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPointEx of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_SetPointEx00
static int tolua_UITolua_LayoutFrame_SetPointEx00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
  const char* strpoint = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* relFrame = ((const char*)  tolua_tostring(tolua_S,3,0));
  const char* strrelpoint = ((const char*)  tolua_tostring(tolua_S,4,0));
  int xoffset = ((int)  tolua_tonumber(tolua_S,5,0));
  int yoffset = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPointEx'",NULL);
#endif
  {
   self->SetPointEx(strpoint,relFrame,strrelpoint,xoffset,yoffset);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPointEx'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPoint of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_SetPoint01
static int tolua_UITolua_LayoutFrame_SetPoint01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"FRAMEPOINT_T",0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"FRAMEPOINT_T",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
  FRAMEPOINT_T myPoint = *((FRAMEPOINT_T*)  tolua_tousertype(tolua_S,2,0));
  const char* relFrame = ((const char*)  tolua_tostring(tolua_S,3,0));
  FRAMEPOINT_T relPoint = *((FRAMEPOINT_T*)  tolua_tousertype(tolua_S,4,0));
  int xoffset = ((int)  tolua_tonumber(tolua_S,5,0));
  int yoffset = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPoint'",NULL);
#endif
  {
   self->SetPoint(myPoint,relFrame,relPoint,xoffset,yoffset);
  }
 }
 return 0;
tolua_lerror:
 return tolua_UITolua_LayoutFrame_SetPoint00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPointAlwaysRecalc of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_SetPointAlwaysRecalc00
static int tolua_UITolua_LayoutFrame_SetPointAlwaysRecalc00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
  const char* strpoint = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* relFrame = ((const char*)  tolua_tostring(tolua_S,3,0));
  const char* strrelpoint = ((const char*)  tolua_tostring(tolua_S,4,0));
  int xoffset = ((int)  tolua_tonumber(tolua_S,5,0));
  int yoffset = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPointAlwaysRecalc'",NULL);
#endif
  {
   self->SetPointAlwaysRecalc(strpoint,relFrame,strrelpoint,xoffset,yoffset);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPointAlwaysRecalc'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetLeft of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_GetLeft00
static int tolua_UITolua_LayoutFrame_GetLeft00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetLeft'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetLeft();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetLeft'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRight of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_GetRight00
static int tolua_UITolua_LayoutFrame_GetRight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRight'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetRight();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTop of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_GetTop00
static int tolua_UITolua_LayoutFrame_GetTop00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTop'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetTop();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTop'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBottom of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_GetBottom00
static int tolua_UITolua_LayoutFrame_GetBottom00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBottom'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetBottom();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBottom'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetWidth of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_GetWidth00
static int tolua_UITolua_LayoutFrame_GetWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetWidth'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetWidth();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetWidth of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_SetWidth00
static int tolua_UITolua_LayoutFrame_SetWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
  int width = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetWidth'",NULL);
#endif
  {
   self->SetWidth(width);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetHeight of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_SetHeight00
static int tolua_UITolua_LayoutFrame_SetHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
  int height = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetHeight'",NULL);
#endif
  {
   self->SetHeight(height);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetHeight of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_GetHeight00
static int tolua_UITolua_LayoutFrame_GetHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHeight'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetHeight();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRealHeight of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_GetRealHeight00
static int tolua_UITolua_LayoutFrame_GetRealHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRealHeight'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetRealHeight();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRealHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRealWidth of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_GetRealWidth00
static int tolua_UITolua_LayoutFrame_GetRealWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRealWidth'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetRealWidth();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRealWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRealLeft of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_GetRealLeft00
static int tolua_UITolua_LayoutFrame_GetRealLeft00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRealLeft'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetRealLeft();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRealLeft'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRealRight of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_GetRealRight00
static int tolua_UITolua_LayoutFrame_GetRealRight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRealRight'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetRealRight();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRealRight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRealTop of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_GetRealTop00
static int tolua_UITolua_LayoutFrame_GetRealTop00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRealTop'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetRealTop();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRealTop'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRealBottom of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_GetRealBottom00
static int tolua_UITolua_LayoutFrame_GetRealBottom00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRealBottom'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetRealBottom();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRealBottom'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAbsRect of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_setAbsRect00
static int tolua_UITolua_LayoutFrame_setAbsRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
  float fLeft = ((float)  tolua_tonumber(tolua_S,2,0));
  float fTop = ((float)  tolua_tonumber(tolua_S,3,0));
  float fRight = ((float)  tolua_tonumber(tolua_S,4,0));
  float fBottom = ((float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAbsRect'",NULL);
#endif
  {
   self->setAbsRect(fLeft,fTop,fRight,fBottom);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAbsRect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: resizeRect of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_resizeRect00
static int tolua_UITolua_LayoutFrame_resizeRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
  int Width = ((int)  tolua_tonumber(tolua_S,2,0));
  int Height = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'resizeRect'",NULL);
#endif
  {
   self->resizeRect(Width,Height);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'resizeRect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: extendRect of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_extendRect00
static int tolua_UITolua_LayoutFrame_extendRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
  int Width = ((int)  tolua_tonumber(tolua_S,2,0));
  int Height = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'extendRect'",NULL);
#endif
  {
   self->extendRect(Width,Height);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'extendRect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetParent of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_GetParent00
static int tolua_UITolua_LayoutFrame_GetParent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetParent'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->GetParent();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetParent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetSizeNoRecal of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_SetSizeNoRecal00
static int tolua_UITolua_LayoutFrame_SetSizeNoRecal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSizeNoRecal'",NULL);
#endif
  {
   self->SetSizeNoRecal(x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetSizeNoRecal'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetSize of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_SetSize00
static int tolua_UITolua_LayoutFrame_SetSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSize'",NULL);
#endif
  {
   self->SetSize(x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTextExtentWidth of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_GetTextExtentWidth00
static int tolua_UITolua_LayoutFrame_GetTextExtentWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTextExtentWidth'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetTextExtentWidth(name);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTextExtentWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTextExtentHeight of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_GetTextExtentHeight00
static int tolua_UITolua_LayoutFrame_GetTextExtentHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTextExtentHeight'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetTextExtentHeight(name);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTextExtentHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetClientID of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_GetClientID00
static int tolua_UITolua_LayoutFrame_GetClientID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetClientID'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetClientID();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetClientID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetClientID of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_SetClientID00
static int tolua_UITolua_LayoutFrame_SetClientID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetClientID'",NULL);
#endif
  {
   self->SetClientID(id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetClientID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetClientUserData of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_SetClientUserData00
static int tolua_UITolua_LayoutFrame_SetClientUserData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  int data = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetClientUserData'",NULL);
#endif
  {
   self->SetClientUserData(index,data);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetClientUserData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetClientUserData of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_GetClientUserData00
static int tolua_UITolua_LayoutFrame_GetClientUserData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetClientUserData'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetClientUserData(index);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetClientUserData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetClientString of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_SetClientString00
static int tolua_UITolua_LayoutFrame_SetClientString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
  const char* str = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetClientString'",NULL);
#endif
  {
   self->SetClientString(str);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetClientString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetClientString of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_GetClientString00
static int tolua_UITolua_LayoutFrame_GetClientString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetClientString'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->GetClientString();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetClientString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFrameDrawLevel of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_getFrameDrawLevel00
static int tolua_UITolua_LayoutFrame_getFrameDrawLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFrameDrawLevel'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getFrameDrawLevel();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFrameDrawLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setInputTransparent of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_setInputTransparent00
static int tolua_UITolua_LayoutFrame_setInputTransparent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
  bool bTransparent = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setInputTransparent'",NULL);
#endif
  {
   self->setInputTransparent(bTransparent);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setInputTransparent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddRelFrame of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_AddRelFrame00
static int tolua_UITolua_LayoutFrame_AddRelFrame00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
  LayoutFrame* pRelFrame = ((LayoutFrame*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddRelFrame'",NULL);
#endif
  {
   self->AddRelFrame(pRelFrame);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddRelFrame'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetSelfScale of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_SetSelfScale00
static int tolua_UITolua_LayoutFrame_SetSelfScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
  float scale = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSelfScale'",NULL);
#endif
  {
   self->SetSelfScale(scale);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetSelfScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSelfScale of class  LayoutFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_LayoutFrame_GetSelfScale00
static int tolua_UITolua_LayoutFrame_GetSelfScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* self = (LayoutFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSelfScale'",NULL);
#endif
  {
   float tolua_ret = (float)  self->GetSelfScale();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSelfScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetText of class  FontString */
#ifndef TOLUA_DISABLE_tolua_UITolua_FontString_GetText00
static int tolua_UITolua_FontString_GetText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"FontString",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  FontString* self = (FontString*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetText'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->GetText();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetText of class  FontString */
#ifndef TOLUA_DISABLE_tolua_UITolua_FontString_SetText00
static int tolua_UITolua_FontString_SetText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"FontString",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  FontString* self = (FontString*)  tolua_tousertype(tolua_S,1,0);
  const char* str = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetText'",NULL);
#endif
  {
   self->SetText(str);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBufferTimer of class  FontString */
#ifndef TOLUA_DISABLE_tolua_UITolua_FontString_SetBufferTimer00
static int tolua_UITolua_FontString_SetBufferTimer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"FontString",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  FontString* self = (FontString*)  tolua_tousertype(tolua_S,1,0);
  float start = ((float)  tolua_tonumber(tolua_S,2,0));
  float total = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBufferTimer'",NULL);
#endif
  {
   self->SetBufferTimer(start,total);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBufferTimer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetTextColor of class  FontString */
#ifndef TOLUA_DISABLE_tolua_UITolua_FontString_SetTextColor00
static int tolua_UITolua_FontString_SetTextColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"FontString",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  FontString* self = (FontString*)  tolua_tousertype(tolua_S,1,0);
  int r = ((int)  tolua_tonumber(tolua_S,2,0));
  int g = ((int)  tolua_tonumber(tolua_S,3,0));
  int b = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetTextColor'",NULL);
#endif
  {
   self->SetTextColor(r,g,b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetTextColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBlendAlpha of class  FontString */
#ifndef TOLUA_DISABLE_tolua_UITolua_FontString_SetBlendAlpha00
static int tolua_UITolua_FontString_SetBlendAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"FontString",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  FontString* self = (FontString*)  tolua_tousertype(tolua_S,1,0);
  float alpha = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBlendAlpha'",NULL);
#endif
  {
   self->SetBlendAlpha(alpha);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBlendAlpha'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetScrollNumber of class  FontString */
#ifndef TOLUA_DISABLE_tolua_UITolua_FontString_SetScrollNumber00
static int tolua_UITolua_FontString_SetScrollNumber00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"FontString",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  FontString* self = (FontString*)  tolua_tousertype(tolua_S,1,0);
  unsigned int begin = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int end = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  float time = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetScrollNumber'",NULL);
#endif
  {
   self->SetScrollNumber(begin,end,time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetScrollNumber'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetScrollNumberWithUint of class  FontString */
#ifndef TOLUA_DISABLE_tolua_UITolua_FontString_SetScrollNumberWithUint00
static int tolua_UITolua_FontString_SetScrollNumberWithUint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"FontString",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  FontString* self = (FontString*)  tolua_tousertype(tolua_S,1,0);
  unsigned int begin = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int end = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  float time = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetScrollNumberWithUint'",NULL);
#endif
  {
   self->SetScrollNumberWithUint(begin,end,time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetScrollNumberWithUint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBuffStart of class  FontString */
#ifndef TOLUA_DISABLE_tolua_UITolua_FontString_GetBuffStart00
static int tolua_UITolua_FontString_GetBuffStart00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"FontString",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  FontString* self = (FontString*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBuffStart'",NULL);
#endif
  {
   float tolua_ret = (float)  self->GetBuffStart();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBuffStart'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBuffTotal of class  FontString */
#ifndef TOLUA_DISABLE_tolua_UITolua_FontString_GetBuffTotal00
static int tolua_UITolua_FontString_GetBuffTotal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"FontString",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  FontString* self = (FontString*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBuffTotal'",NULL);
#endif
  {
   float tolua_ret = (float)  self->GetBuffTotal();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBuffTotal'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTextExtentWidth of class  FontString */
#ifndef TOLUA_DISABLE_tolua_UITolua_FontString_GetTextExtentWidth00
static int tolua_UITolua_FontString_GetTextExtentWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"FontString",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  FontString* self = (FontString*)  tolua_tousertype(tolua_S,1,0);
  const char* pszContent = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTextExtentWidth'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetTextExtentWidth(pszContent);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTextExtentWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: __FontInstance__ of class  FontString */
#ifndef TOLUA_DISABLE_tolua_get_FontString___FontInstance__
static int tolua_get_FontString___FontInstance__(lua_State* tolua_S)
{
  FontString* self = (FontString*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable '__FontInstance__'",NULL);
#endif
#ifdef __cplusplus
  tolua_pushusertype(tolua_S,(void*)static_cast<FontInstance*>(self), "FontInstance");
#else
  tolua_pushusertype(tolua_S,(void*)((FontInstance*)self), "FontInstance");
#endif
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetUVAnimation of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_SetUVAnimation00
static int tolua_UITolua_Texture_SetUVAnimation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
  int timeslot = ((int)  tolua_tonumber(tolua_S,2,0));
  bool loopmode = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetUVAnimation'",NULL);
#endif
  {
   self->SetUVAnimation(timeslot,loopmode);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetUVAnimation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetTextureHuires of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_SetTextureHuires00
static int tolua_UITolua_Texture_SetTextureHuires00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::HUIRES",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
  Ogre::HUIRES hTex = *((Ogre::HUIRES*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetTextureHuires'",NULL);
#endif
  {
   self->SetTextureHuires(hTex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetTextureHuires'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTextureHuires of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_GetTextureHuires00
static int tolua_UITolua_Texture_GetTextureHuires00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTextureHuires'",NULL);
#endif
  {
   Ogre::HUIRES tolua_ret = (Ogre::HUIRES)  self->GetTextureHuires();
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::HUIRES(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::HUIRES");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::HUIRES));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::HUIRES");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTextureHuires'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetColor of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_SetColor00
static int tolua_UITolua_Texture_SetColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
  int r = ((int)  tolua_tonumber(tolua_S,2,0));
  int g = ((int)  tolua_tonumber(tolua_S,3,0));
  int b = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetColor'",NULL);
#endif
  {
   self->SetColor(r,g,b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetGray of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_SetGray00
static int tolua_UITolua_Texture_SetGray00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
  bool bGray = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetGray'",NULL);
#endif
  {
   self->SetGray(bGray);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetGray'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsGray of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_IsGray00
static int tolua_UITolua_Texture_IsGray00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsGray'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsGray();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsGray'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetTexture of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_SetTexture00
static int tolua_UITolua_Texture_SetTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
  const char* path = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetTexture'",NULL);
#endif
  {
   self->SetTexture(path);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTexture of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_GetTexture00
static int tolua_UITolua_Texture_GetTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTexture'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->GetTexture();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetAngle of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_SetAngle00
static int tolua_UITolua_Texture_SetAngle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
  float fAngle = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetAngle'",NULL);
#endif
  {
   self->SetAngle(fAngle);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetAngle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetAngle of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_GetAngle00
static int tolua_UITolua_Texture_GetAngle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetAngle'",NULL);
#endif
  {
   float tolua_ret = (float)  self->GetAngle();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetAngle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetTexUV of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_SetTexUV00
static int tolua_UITolua_Texture_SetTexUV00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  int width = ((int)  tolua_tonumber(tolua_S,4,0));
  int height = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetTexUV'",NULL);
#endif
  {
   self->SetTexUV(x,y,width,height);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetTexUV'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetTexRelUV of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_SetTexRelUV00
static int tolua_UITolua_Texture_SetTexRelUV00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
  float width = ((float)  tolua_tonumber(tolua_S,2,0));
  float height = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetTexRelUV'",NULL);
#endif
  {
   self->SetTexRelUV(width,height);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetTexRelUV'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetTexRelUV of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_SetTexRelUV01
static int tolua_UITolua_Texture_SetTexRelUV01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  float width = ((float)  tolua_tonumber(tolua_S,4,0));
  float height = ((float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetTexRelUV'",NULL);
#endif
  {
   self->SetTexRelUV(x,y,width,height);
  }
 }
 return 0;
tolua_lerror:
 return tolua_UITolua_Texture_SetTexRelUV00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRelWidth of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_getRelWidth00
static int tolua_UITolua_Texture_getRelWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRelWidth'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getRelWidth();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRelWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRelHeight of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_getRelHeight00
static int tolua_UITolua_Texture_getRelHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRelHeight'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getRelHeight();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRelHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTextOffsetY of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_setTextOffsetY00
static int tolua_UITolua_Texture_setTextOffsetY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
  float person = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTextOffsetY'",NULL);
#endif
  {
   self->setTextOffsetY(person);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTextOffsetY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTextOffsetX of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_setTextOffsetX00
static int tolua_UITolua_Texture_setTextOffsetX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
  float person = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTextOffsetX'",NULL);
#endif
  {
   self->setTextOffsetX(person);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTextOffsetX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTextOffsetY of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_getTextOffsetY00
static int tolua_UITolua_Texture_getTextOffsetY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTextOffsetY'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getTextOffsetY();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTextOffsetY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTextOffsetX of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_getTextOffsetX00
static int tolua_UITolua_Texture_getTextOffsetX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTextOffsetX'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getTextOffsetX();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTextOffsetX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBlendAlpha of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_SetBlendAlpha00
static int tolua_UITolua_Texture_SetBlendAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
  float alpha = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBlendAlpha'",NULL);
#endif
  {
   self->SetBlendAlpha(alpha);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBlendAlpha'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBlendAlpha of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_GetBlendAlpha00
static int tolua_UITolua_Texture_GetBlendAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBlendAlpha'",NULL);
#endif
  {
   float tolua_ret = (float)  self->GetBlendAlpha();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBlendAlpha'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMask of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_setMask00
static int tolua_UITolua_Texture_setMask00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
  const char* szmaskpath = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMask'",NULL);
#endif
  {
   self->setMask(szmaskpath);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMask'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setUvType of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_setUvType00
static int tolua_UITolua_Texture_setUvType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
  int nType = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setUvType'",NULL);
#endif
  {
   self->setUvType(nType);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setUvType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRotateTopLeft of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_setRotateTopLeft00
static int tolua_UITolua_Texture_setRotateTopLeft00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
  bool b = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotateTopLeft'",NULL);
#endif
  {
   self->setRotateTopLeft(b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRotateTopLeft'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ChangeTextureTemplate of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_ChangeTextureTemplate00
static int tolua_UITolua_Texture_ChangeTextureTemplate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
  const char* template_name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ChangeTextureTemplate'",NULL);
#endif
  {
   self->ChangeTextureTemplate(template_name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ChangeTextureTemplate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: StopUVAnim of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_StopUVAnim00
static int tolua_UITolua_Texture_StopUVAnim00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'StopUVAnim'",NULL);
#endif
  {
   self->StopUVAnim();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'StopUVAnim'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: StartAlphaAmin of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_StartAlphaAmin00
static int tolua_UITolua_Texture_StartAlphaAmin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
  float speed = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'StartAlphaAmin'",NULL);
#endif
  {
   self->StartAlphaAmin(speed);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'StartAlphaAmin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: StopAlphaAmin of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_StopAlphaAmin00
static int tolua_UITolua_Texture_StopAlphaAmin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'StopAlphaAmin'",NULL);
#endif
  {
   self->StopAlphaAmin();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'StopAlphaAmin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsPlayAlphaAnim of class  Texture */
#ifndef TOLUA_DISABLE_tolua_UITolua_Texture_IsPlayAlphaAnim00
static int tolua_UITolua_Texture_IsPlayAlphaAnim00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsPlayAlphaAnim'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsPlayAlphaAnim();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsPlayAlphaAnim'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: __Ogre of class  Texture */
#ifndef TOLUA_DISABLE_tolua_get_Texture___Ogre__LoadWrap__
static int tolua_get_Texture___Ogre__LoadWrap__(lua_State* tolua_S)
{
  Texture* self = (Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable '__Ogre'",NULL);
#endif
#ifdef __cplusplus
  tolua_pushusertype(tolua_S,(void*)static_cast<Ogre::LoadWrap*>(self), "Ogre::LoadWrap");
#else
  tolua_pushusertype(tolua_S,(void*)((Ogre::LoadWrap*)self), "Ogre::LoadWrap");
#endif
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: setBackground of class  ModelView */
#ifndef TOLUA_DISABLE_tolua_UITolua_ModelView_setBackground00
static int tolua_UITolua_ModelView_setBackground00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ModelView",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ModelView* self = (ModelView*)  tolua_tousertype(tolua_S,1,0);
  const char* path = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setBackground'",NULL);
#endif
  {
   self->setBackground(path);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setBackground'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addBackground of class  ModelView */
#ifndef TOLUA_DISABLE_tolua_UITolua_ModelView_addBackground00
static int tolua_UITolua_ModelView_addBackground00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ModelView",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ModelView* self = (ModelView*)  tolua_tousertype(tolua_S,1,0);
  const char* path = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addBackground'",NULL);
#endif
  {
   self->addBackground(path);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addBackground'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAvatarAttrib of class  ModelView */
#ifndef TOLUA_DISABLE_tolua_UITolua_ModelView_setAvatarAttrib00
static int tolua_UITolua_ModelView_setAvatarAttrib00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ModelView",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,6,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ModelView* self = (ModelView*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  int grid = ((int)  tolua_tonumber(tolua_S,3,0));
  int id = ((int)  tolua_tonumber(tolua_S,4,0));
  int starLv = ((int)  tolua_tonumber(tolua_S,5,0));
  bool bfirst = ((bool)  tolua_toboolean(tolua_S,6,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAvatarAttrib'",NULL);
#endif
  {
   self->setAvatarAttrib(index,grid,id,starLv,bfirst);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAvatarAttrib'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAvatarAttrib of class  ModelView */
#ifndef TOLUA_DISABLE_tolua_UITolua_ModelView_getAvatarAttrib00
static int tolua_UITolua_ModelView_getAvatarAttrib00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ModelView",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ModelView* self = (ModelView*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  int grid = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAvatarAttrib'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getAvatarAttrib(index,grid);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAvatarAttrib'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setEntityAvatar of class  ModelView */
#ifndef TOLUA_DISABLE_tolua_UITolua_ModelView_setEntityAvatar00
static int tolua_UITolua_ModelView_setEntityAvatar00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ModelView",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ModelView* self = (ModelView*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setEntityAvatar'",NULL);
#endif
  {
   self->setEntityAvatar(index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setEntityAvatar'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isPlayingAnim of class  ModelView */
#ifndef TOLUA_DISABLE_tolua_UITolua_ModelView_isPlayingAnim00
static int tolua_UITolua_ModelView_isPlayingAnim00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ModelView",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ModelView* self = (ModelView*)  tolua_tousertype(tolua_S,1,0);
  int nIndex = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isPlayingAnim'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isPlayingAnim(nIndex);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isPlayingAnim'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPetModel of class  ModelView */
#ifndef TOLUA_DISABLE_tolua_UITolua_ModelView_setPetModel00
static int tolua_UITolua_ModelView_setPetModel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ModelView",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isstring(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ModelView* self = (ModelView*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  int nAnimId = ((int)  tolua_tonumber(tolua_S,3,0));
  const char* szModelPath = ((const char*)  tolua_tostring(tolua_S,4,0));
  const char* szAppendPath = ((const char*)  tolua_tostring(tolua_S,5,NULL));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPetModel'",NULL);
#endif
  {
   self->setPetModel(index,nAnimId,szModelPath,szAppendPath);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPetModel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPetScale of class  ModelView */
#ifndef TOLUA_DISABLE_tolua_UITolua_ModelView_setPetScale00
static int tolua_UITolua_ModelView_setPetScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ModelView",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ModelView* self = (ModelView*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  float x = ((float)  tolua_tonumber(tolua_S,3,0));
  float y = ((float)  tolua_tonumber(tolua_S,4,0));
  float z = ((float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPetScale'",NULL);
#endif
  {
   self->setPetScale(index,x,y,z);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPetScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setEntityPosition of class  ModelView */
#ifndef TOLUA_DISABLE_tolua_UITolua_ModelView_setEntityPosition00
static int tolua_UITolua_ModelView_setEntityPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ModelView",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ModelView* self = (ModelView*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  float x = ((float)  tolua_tonumber(tolua_S,3,0));
  float y = ((float)  tolua_tonumber(tolua_S,4,0));
  float z = ((float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setEntityPosition'",NULL);
#endif
  {
   self->setEntityPosition(index,x,y,z);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setEntityPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setEntityRotate of class  ModelView */
#ifndef TOLUA_DISABLE_tolua_UITolua_ModelView_setEntityRotate00
static int tolua_UITolua_ModelView_setEntityRotate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ModelView",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ModelView* self = (ModelView*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  float angle = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setEntityRotate'",NULL);
#endif
  {
   self->setEntityRotate(index,angle);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setEntityRotate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setEntieyTextureByID of class  ModelView */
#ifndef TOLUA_DISABLE_tolua_UITolua_ModelView_setEntieyTextureByID00
static int tolua_UITolua_ModelView_setEntieyTextureByID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ModelView",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ModelView* self = (ModelView*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  int id = ((int)  tolua_tonumber(tolua_S,3,0));
  const char* szPath = ((const char*)  tolua_tostring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setEntieyTextureByID'",NULL);
#endif
  {
   self->setEntieyTextureByID(index,id,szPath);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setEntieyTextureByID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: playEntityAnim of class  ModelView */
#ifndef TOLUA_DISABLE_tolua_UITolua_ModelView_playEntityAnim00
static int tolua_UITolua_ModelView_playEntityAnim00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ModelView",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ModelView* self = (ModelView*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  int seq = ((int)  tolua_tonumber(tolua_S,3,0));
  const char* motion = ((const char*)  tolua_tostring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'playEntityAnim'",NULL);
#endif
  {
   self->playEntityAnim(index,seq,motion);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'playEntityAnim'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isHaveCameraAnim of class  ModelView */
#ifndef TOLUA_DISABLE_tolua_UITolua_ModelView_isHaveCameraAnim00
static int tolua_UITolua_ModelView_isHaveCameraAnim00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ModelView",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ModelView* self = (ModelView*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isHaveCameraAnim'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isHaveCameraAnim();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isHaveCameraAnim'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setCameraAnim of class  ModelView */
#ifndef TOLUA_DISABLE_tolua_UITolua_ModelView_setCameraAnim00
static int tolua_UITolua_ModelView_setCameraAnim00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ModelView",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ModelView* self = (ModelView*)  tolua_tousertype(tolua_S,1,0);
  const char* path = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCameraAnim'",NULL);
#endif
  {
   self->setCameraAnim(path);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setCameraAnim'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: playCameraAnim of class  ModelView */
#ifndef TOLUA_DISABLE_tolua_UITolua_ModelView_playCameraAnim00
static int tolua_UITolua_ModelView_playCameraAnim00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ModelView",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ModelView* self = (ModelView*)  tolua_tousertype(tolua_S,1,0);
  int seq = ((int)  tolua_tonumber(tolua_S,2,0));
  int fadetick = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'playCameraAnim'",NULL);
#endif
  {
   self->playCameraAnim(seq,fadetick);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'playCameraAnim'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RegisterEvent of class  Frame */
#ifndef TOLUA_DISABLE_tolua_UITolua_Frame_RegisterEvent00
static int tolua_UITolua_Frame_RegisterEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Frame",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Frame* self = (Frame*)  tolua_tousertype(tolua_S,1,0);
  const char* event = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RegisterEvent'",NULL);
#endif
  {
   self->RegisterEvent(event);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RegisterEvent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetFrameLevel of class  Frame */
#ifndef TOLUA_DISABLE_tolua_UITolua_Frame_SetFrameLevel00
static int tolua_UITolua_Frame_SetFrameLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Frame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Frame* self = (Frame*)  tolua_tousertype(tolua_S,1,0);
  int level = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetFrameLevel'",NULL);
#endif
  {
   self->SetFrameLevel(level);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFrameLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetFrameLevel of class  Frame */
#ifndef TOLUA_DISABLE_tolua_UITolua_Frame_GetFrameLevel00
static int tolua_UITolua_Frame_GetFrameLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Frame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Frame* self = (Frame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFrameLevel'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetFrameLevel();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFrameLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetClipState of class  Frame */
#ifndef TOLUA_DISABLE_tolua_UITolua_Frame_SetClipState00
static int tolua_UITolua_Frame_SetClipState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Frame",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Frame* self = (Frame*)  tolua_tousertype(tolua_S,1,0);
  bool bClip = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetClipState'",NULL);
#endif
  {
   self->SetClipState(bClip);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetClipState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBlendAlpha of class  Frame */
#ifndef TOLUA_DISABLE_tolua_UITolua_Frame_SetBlendAlpha00
static int tolua_UITolua_Frame_SetBlendAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Frame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Frame* self = (Frame*)  tolua_tousertype(tolua_S,1,0);
  float alpha = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBlendAlpha'",NULL);
#endif
  {
   self->SetBlendAlpha(alpha);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBlendAlpha'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setUpdateTime of class  Frame */
#ifndef TOLUA_DISABLE_tolua_UITolua_Frame_setUpdateTime00
static int tolua_UITolua_Frame_setUpdateTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Frame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Frame* self = (Frame*)  tolua_tousertype(tolua_S,1,0);
  float fUpdateTime = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setUpdateTime'",NULL);
#endif
  {
   self->setUpdateTime(fUpdateTime);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setUpdateTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBackDropBlendAlpha of class  Frame */
#ifndef TOLUA_DISABLE_tolua_UITolua_Frame_GetBackDropBlendAlpha00
static int tolua_UITolua_Frame_GetBackDropBlendAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Frame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Frame* self = (Frame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBackDropBlendAlpha'",NULL);
#endif
  {
   float tolua_ret = (float)  self->GetBackDropBlendAlpha();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBackDropBlendAlpha'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBackDropBlendAlpha of class  Frame */
#ifndef TOLUA_DISABLE_tolua_UITolua_Frame_SetBackDropBlendAlpha00
static int tolua_UITolua_Frame_SetBackDropBlendAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Frame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Frame* self = (Frame*)  tolua_tousertype(tolua_S,1,0);
  float alpha = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBackDropBlendAlpha'",NULL);
#endif
  {
   self->SetBackDropBlendAlpha(alpha);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBackDropBlendAlpha'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBackDropColor of class  Frame */
#ifndef TOLUA_DISABLE_tolua_UITolua_Frame_SetBackDropColor00
static int tolua_UITolua_Frame_SetBackDropColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Frame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Frame* self = (Frame*)  tolua_tousertype(tolua_S,1,0);
  unsigned int r = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int g = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  unsigned int b = ((unsigned int)  tolua_tonumber(tolua_S,4,0));
  unsigned int a = ((unsigned int)  tolua_tonumber(tolua_S,5,255));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBackDropColor'",NULL);
#endif
  {
   self->SetBackDropColor(r,g,b,a);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBackDropColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: MoveFrame of class  Frame */
#ifndef TOLUA_DISABLE_tolua_UITolua_Frame_MoveFrame00
static int tolua_UITolua_Frame_MoveFrame00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Frame",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Frame* self = (Frame*)  tolua_tousertype(tolua_S,1,0);
  const char* strMovePoint = ((const char*)  tolua_tostring(tolua_S,2,0));
  float nMoveDuration = ((float)  tolua_tonumber(tolua_S,3,0));
  int nOffsetX = ((int)  tolua_tonumber(tolua_S,4,0));
  int nOffsetY = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'MoveFrame'",NULL);
#endif
  {
   self->MoveFrame(strMovePoint,nMoveDuration,nOffsetX,nOffsetY);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MoveFrame'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: EndMoveFrame of class  Frame */
#ifndef TOLUA_DISABLE_tolua_UITolua_Frame_EndMoveFrame00
static int tolua_UITolua_Frame_EndMoveFrame00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Frame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Frame* self = (Frame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'EndMoveFrame'",NULL);
#endif
  {
   self->EndMoveFrame();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'EndMoveFrame'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBackDropBorderTex of class  Frame */
#ifndef TOLUA_DISABLE_tolua_UITolua_Frame_SetBackDropBorderTex00
static int tolua_UITolua_Frame_SetBackDropBorderTex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Frame",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Frame* self = (Frame*)  tolua_tousertype(tolua_S,1,0);
  const char* pszBorderTexPath = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBackDropBorderTex'",NULL);
#endif
  {
   self->SetBackDropBorderTex(pszBorderTexPath);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBackDropBorderTex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBackDropEdgeTex of class  Frame */
#ifndef TOLUA_DISABLE_tolua_UITolua_Frame_SetBackDropEdgeTex00
static int tolua_UITolua_Frame_SetBackDropEdgeTex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Frame",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Frame* self = (Frame*)  tolua_tousertype(tolua_S,1,0);
  const char* pszEdgeTexPath = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBackDropEdgeTex'",NULL);
#endif
  {
   self->SetBackDropEdgeTex(pszEdgeTexPath);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBackDropEdgeTex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBackDropAttr of class  Frame */
#ifndef TOLUA_DISABLE_tolua_UITolua_Frame_SetBackDropAttr00
static int tolua_UITolua_Frame_SetBackDropAttr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Frame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Frame* self = (Frame*)  tolua_tousertype(tolua_S,1,0);
  int nEdgeSize = ((int)  tolua_tonumber(tolua_S,2,0));
  int nInsetLeft = ((int)  tolua_tonumber(tolua_S,3,0));
  int nInsetRight = ((int)  tolua_tonumber(tolua_S,4,0));
  int nInsetTop = ((int)  tolua_tonumber(tolua_S,5,0));
  int nInsetBottom = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBackDropAttr'",NULL);
#endif
  {
   self->SetBackDropAttr(nEdgeSize,nInsetLeft,nInsetRight,nInsetTop,nInsetBottom);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBackDropAttr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetSelfScale of class  Frame */
#ifndef TOLUA_DISABLE_tolua_UITolua_Frame_SetSelfScale00
static int tolua_UITolua_Frame_SetSelfScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Frame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Frame* self = (Frame*)  tolua_tousertype(tolua_S,1,0);
  float scale = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSelfScale'",NULL);
#endif
  {
   self->SetSelfScale(scale);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetSelfScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddLevelRecursive of class  Frame */
#ifndef TOLUA_DISABLE_tolua_UITolua_Frame_AddLevelRecursive00
static int tolua_UITolua_Frame_AddLevelRecursive00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Frame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Frame* self = (Frame*)  tolua_tousertype(tolua_S,1,0);
  int iLevel = ((int)  tolua_tonumber(tolua_S,2,1));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddLevelRecursive'",NULL);
#endif
  {
   self->AddLevelRecursive(iLevel);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddLevelRecursive'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetCooldownTimer of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetCooldownTimer00
static int tolua_UITolua_Button_SetCooldownTimer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  float start = ((float)  tolua_tonumber(tolua_S,2,0));
  float duration = ((float)  tolua_tonumber(tolua_S,3,0));
  bool enable = ((bool)  tolua_toboolean(tolua_S,4,0));
  bool bFade = ((bool)  tolua_toboolean(tolua_S,5,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetCooldownTimer'",NULL);
#endif
  {
   self->SetCooldownTimer(start,duration,enable,bFade);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetCooldownTimer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsCooldown of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_IsCooldown00
static int tolua_UITolua_Button_IsCooldown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsCooldown'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsCooldown();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsCooldown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddLock of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_AddLock00
static int tolua_UITolua_Button_AddLock00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddLock'",NULL);
#endif
  {
   self->AddLock();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddLock'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetNormalRegionWidth of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetNormalRegionWidth00
static int tolua_UITolua_Button_SetNormalRegionWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  float width = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetNormalRegionWidth'",NULL);
#endif
  {
   self->SetNormalRegionWidth(width);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetNormalRegionWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetNormalRegionAbsWidth of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetNormalRegionAbsWidth00
static int tolua_UITolua_Button_SetNormalRegionAbsWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  float width = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetNormalRegionAbsWidth'",NULL);
#endif
  {
   self->SetNormalRegionAbsWidth(width);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetNormalRegionAbsWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNormalRegionWidth of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_GetNormalRegionWidth00
static int tolua_UITolua_Button_GetNormalRegionWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNormalRegionWidth'",NULL);
#endif
  {
   float tolua_ret = (float)  self->GetNormalRegionWidth();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNormalRegionWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetNormalRegionHight of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetNormalRegionHight00
static int tolua_UITolua_Button_SetNormalRegionHight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  float Hight = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetNormalRegionHight'",NULL);
#endif
  {
   self->SetNormalRegionHight(Hight);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetNormalRegionHight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNormalRegionHight of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_GetNormalRegionHight00
static int tolua_UITolua_Button_GetNormalRegionHight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNormalRegionHight'",NULL);
#endif
  {
   float tolua_ret = (float)  self->GetNormalRegionHight();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNormalRegionHight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetOverlayRegionAbsWidth of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetOverlayRegionAbsWidth00
static int tolua_UITolua_Button_SetOverlayRegionAbsWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  float width = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetOverlayRegionAbsWidth'",NULL);
#endif
  {
   self->SetOverlayRegionAbsWidth(width);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetOverlayRegionAbsWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetOverlayRegionWidth of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_GetOverlayRegionWidth00
static int tolua_UITolua_Button_GetOverlayRegionWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetOverlayRegionWidth'",NULL);
#endif
  {
   float tolua_ret = (float)  self->GetOverlayRegionWidth();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetOverlayRegionWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetNormalTexture of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetNormalTexture00
static int tolua_UITolua_Button_SetNormalTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  const char* path = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* strdrawmode = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetNormalTexture'",NULL);
#endif
  {
   self->SetNormalTexture(path,strdrawmode);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetNormalTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPushedTexture of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetPushedTexture00
static int tolua_UITolua_Button_SetPushedTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  const char* path = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* strdrawmode = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPushedTexture'",NULL);
#endif
  {
   self->SetPushedTexture(path,strdrawmode);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPushedTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetDisableTexture of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetDisableTexture00
static int tolua_UITolua_Button_SetDisableTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  const char* path = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* strdrawmode = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetDisableTexture'",NULL);
#endif
  {
   self->SetDisableTexture(path,strdrawmode);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetDisableTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNormalTexture of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_GetNormalTexture00
static int tolua_UITolua_Button_GetNormalTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNormalTexture'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->GetNormalTexture();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNormalTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetNormalTextureHuires of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetNormalTextureHuires00
static int tolua_UITolua_Button_SetNormalTextureHuires00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::HUIRES",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  Ogre::HUIRES h = *((Ogre::HUIRES*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetNormalTextureHuires'",NULL);
#endif
  {
   self->SetNormalTextureHuires(h);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetNormalTextureHuires'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetText of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetText00
static int tolua_UITolua_Button_SetText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  const char* buttontext = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetText'",NULL);
#endif
  {
   self->SetText(buttontext);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetText of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_GetText00
static int tolua_UITolua_Button_GetText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetText'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->GetText();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetTextColor of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetTextColor00
static int tolua_UITolua_Button_SetTextColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  int r = ((int)  tolua_tonumber(tolua_S,2,0));
  int g = ((int)  tolua_tonumber(tolua_S,3,0));
  int b = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetTextColor'",NULL);
#endif
  {
   self->SetTextColor(r,g,b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetTextColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetIntonateTimer of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetIntonateTimer00
static int tolua_UITolua_Button_SetIntonateTimer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  float start = ((float)  tolua_tonumber(tolua_S,2,0));
  float total = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetIntonateTimer'",NULL);
#endif
  {
   self->SetIntonateTimer(start,total);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetIntonateTimer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetHightlightTexture of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetHightlightTexture00
static int tolua_UITolua_Button_SetHightlightTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  const char* path = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* strdrawmode = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetHightlightTexture'",NULL);
#endif
  {
   self->SetHightlightTexture(path,strdrawmode);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetHightlightTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetDisableTextureHuires of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetDisableTextureHuires00
static int tolua_UITolua_Button_SetDisableTextureHuires00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::HUIRES",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  Ogre::HUIRES h = *((Ogre::HUIRES*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetDisableTextureHuires'",NULL);
#endif
  {
   self->SetDisableTextureHuires(h);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetDisableTextureHuires'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetDisableTextureHuires of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_GetDisableTextureHuires00
static int tolua_UITolua_Button_GetDisableTextureHuires00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetDisableTextureHuires'",NULL);
#endif
  {
   Ogre::HUIRES tolua_ret = (Ogre::HUIRES)  self->GetDisableTextureHuires();
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::HUIRES(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::HUIRES");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::HUIRES));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::HUIRES");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetDisableTextureHuires'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetHighlightTextureHuires of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetHighlightTextureHuires00
static int tolua_UITolua_Button_SetHighlightTextureHuires00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::HUIRES",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  Ogre::HUIRES h = *((Ogre::HUIRES*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetHighlightTextureHuires'",NULL);
#endif
  {
   self->SetHighlightTextureHuires(h);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetHighlightTextureHuires'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetHighlightTextureHuires of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_GetHighlightTextureHuires00
static int tolua_UITolua_Button_GetHighlightTextureHuires00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHighlightTextureHuires'",NULL);
#endif
  {
   Ogre::HUIRES tolua_ret = (Ogre::HUIRES)  self->GetHighlightTextureHuires();
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::HUIRES(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::HUIRES");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::HUIRES));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::HUIRES");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetHighlightTextureHuires'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBlendAlpha of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetBlendAlpha00
static int tolua_UITolua_Button_SetBlendAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  float alpha = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBlendAlpha'",NULL);
#endif
  {
   self->SetBlendAlpha(alpha);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBlendAlpha'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetGray of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetGray00
static int tolua_UITolua_Button_SetGray00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  bool bGray = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetGray'",NULL);
#endif
  {
   self->SetGray(bGray);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetGray'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetGrayEx of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetGrayEx00
static int tolua_UITolua_Button_SetGrayEx00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  bool bGray = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetGrayEx'",NULL);
#endif
  {
   self->SetGrayEx(bGray);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetGrayEx'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPushedState of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetPushedState00
static int tolua_UITolua_Button_SetPushedState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPushedState'",NULL);
#endif
  {
   self->SetPushedState();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPushedState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPushedState of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_GetPushedState00
static int tolua_UITolua_Button_GetPushedState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPushedState'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->GetPushedState();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPushedState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Disable of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_Disable00
static int tolua_UITolua_Button_Disable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Disable'",NULL);
#endif
  {
   self->Disable();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Disable'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Enable of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_Enable00
static int tolua_UITolua_Button_Enable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Enable'",NULL);
#endif
  {
   self->Enable();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Enable'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsEnable of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_IsEnable00
static int tolua_UITolua_Button_IsEnable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsEnable'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsEnable();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsEnable'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Highlight of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_Highlight00
static int tolua_UITolua_Button_Highlight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Highlight'",NULL);
#endif
  {
   self->Highlight();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Highlight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DisHighlight of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_DisHighlight00
static int tolua_UITolua_Button_DisHighlight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DisHighlight'",NULL);
#endif
  {
   self->DisHighlight();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DisHighlight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsHighlight of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_IsHighlight00
static int tolua_UITolua_Button_IsHighlight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsHighlight'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsHighlight();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsHighlight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Checked of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_Checked00
static int tolua_UITolua_Button_Checked00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Checked'",NULL);
#endif
  {
   self->Checked();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Checked'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DisChecked of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_DisChecked00
static int tolua_UITolua_Button_DisChecked00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DisChecked'",NULL);
#endif
  {
   self->DisChecked();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DisChecked'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsChecked of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_IsChecked00
static int tolua_UITolua_Button_IsChecked00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsChecked'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsChecked();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsChecked'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnLButtonDown of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_OnLButtonDown00
static int tolua_UITolua_Button_OnLButtonDown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OnLButtonDown'",NULL);
#endif
  {
   int tolua_ret = (int)  self->OnLButtonDown();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnLButtonDown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnLButtonUp of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_OnLButtonUp00
static int tolua_UITolua_Button_OnLButtonUp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OnLButtonUp'",NULL);
#endif
  {
   int tolua_ret = (int)  self->OnLButtonUp();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnLButtonUp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SwapCooldownMembers of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SwapCooldownMembers00
static int tolua_UITolua_Button_SwapCooldownMembers00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  Button* pdest = ((Button*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SwapCooldownMembers'",NULL);
#endif
  {
   self->SwapCooldownMembers(pdest);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SwapCooldownMembers'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CopyCooldownMembers of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_CopyCooldownMembers00
static int tolua_UITolua_Button_CopyCooldownMembers00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  Button* pdest = ((Button*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CopyCooldownMembers'",NULL);
#endif
  {
   self->CopyCooldownMembers(pdest);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CopyCooldownMembers'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetCooldownTextureRect of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetCooldownTextureRect00
static int tolua_UITolua_Button_SetCooldownTextureRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  int offsetleft = ((int)  tolua_tonumber(tolua_S,2,0));
  int offsettop = ((int)  tolua_tonumber(tolua_S,3,0));
  int width = ((int)  tolua_tonumber(tolua_S,4,0));
  int height = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetCooldownTextureRect'",NULL);
#endif
  {
   self->SetCooldownTextureRect(offsetleft,offsettop,width,height);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetCooldownTextureRect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetAngle of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetAngle00
static int tolua_UITolua_Button_SetAngle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  float fAngle = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetAngle'",NULL);
#endif
  {
   self->SetAngle(fAngle);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetAngle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetAngle of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_GetAngle00
static int tolua_UITolua_Button_GetAngle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetAngle'",NULL);
#endif
  {
   float tolua_ret = (float)  self->GetAngle();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetAngle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetNormalTextureSize of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetNormalTextureSize00
static int tolua_UITolua_Button_SetNormalTextureSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  int width = ((int)  tolua_tonumber(tolua_S,2,0));
  int height = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetNormalTextureSize'",NULL);
#endif
  {
   self->SetNormalTextureSize(width,height);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetNormalTextureSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ChangeNormalTexture of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_ChangeNormalTexture00
static int tolua_UITolua_Button_ChangeNormalTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  int width = ((int)  tolua_tonumber(tolua_S,4,0));
  int height = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ChangeNormalTexture'",NULL);
#endif
  {
   self->ChangeNormalTexture(x,y,width,height);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ChangeNormalTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetNormalTextureRelUV of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetNormalTextureRelUV00
static int tolua_UITolua_Button_SetNormalTextureRelUV00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetNormalTextureRelUV'",NULL);
#endif
  {
   self->SetNormalTextureRelUV(x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetNormalTextureRelUV'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ChangeHighlightTexture of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_ChangeHighlightTexture00
static int tolua_UITolua_Button_ChangeHighlightTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  int width = ((int)  tolua_tonumber(tolua_S,4,0));
  int height = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ChangeHighlightTexture'",NULL);
#endif
  {
   self->ChangeHighlightTexture(x,y,width,height);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ChangeHighlightTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ChangePushedTexture of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_ChangePushedTexture00
static int tolua_UITolua_Button_ChangePushedTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  int width = ((int)  tolua_tonumber(tolua_S,4,0));
  int height = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ChangePushedTexture'",NULL);
#endif
  {
   self->ChangePushedTexture(x,y,width,height);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ChangePushedTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ChangeDisableTexture of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_ChangeDisableTexture00
static int tolua_UITolua_Button_ChangeDisableTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  int width = ((int)  tolua_tonumber(tolua_S,4,0));
  int height = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ChangeDisableTexture'",NULL);
#endif
  {
   self->ChangeDisableTexture(x,y,width,height);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ChangeDisableTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetOverlayRegion of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetOverlayRegion00
static int tolua_UITolua_Button_SetOverlayRegion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  int offsetX = ((int)  tolua_tonumber(tolua_S,2,0));
  int offsetY = ((int)  tolua_tonumber(tolua_S,3,0));
  int width = ((int)  tolua_tonumber(tolua_S,4,0));
  int height = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetOverlayRegion'",NULL);
#endif
  {
   self->SetOverlayRegion(offsetX,offsetY,width,height);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetOverlayRegion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetNormalTextureTemplate of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetNormalTextureTemplate00
static int tolua_UITolua_Button_SetNormalTextureTemplate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  const char* template_name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetNormalTextureTemplate'",NULL);
#endif
  {
   self->SetNormalTextureTemplate(template_name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetNormalTextureTemplate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetHighlightTextureTemplate of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetHighlightTextureTemplate00
static int tolua_UITolua_Button_SetHighlightTextureTemplate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  const char* template_name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetHighlightTextureTemplate'",NULL);
#endif
  {
   self->SetHighlightTextureTemplate(template_name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetHighlightTextureTemplate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPushedTextureTemplate of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetPushedTextureTemplate00
static int tolua_UITolua_Button_SetPushedTextureTemplate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  const char* template_name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPushedTextureTemplate'",NULL);
#endif
  {
   self->SetPushedTextureTemplate(template_name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPushedTextureTemplate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetDisableTextureTemplate of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetDisableTextureTemplate00
static int tolua_UITolua_Button_SetDisableTextureTemplate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  const char* template_name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetDisableTextureTemplate'",NULL);
#endif
  {
   self->SetDisableTextureTemplate(template_name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetDisableTextureTemplate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetCoolStart of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_GetCoolStart00
static int tolua_UITolua_Button_GetCoolStart00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetCoolStart'",NULL);
#endif
  {
   float tolua_ret = (float)  self->GetCoolStart();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCoolStart'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetCoolTotal of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_GetCoolTotal00
static int tolua_UITolua_Button_GetCoolTotal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetCoolTotal'",NULL);
#endif
  {
   float tolua_ret = (float)  self->GetCoolTotal();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCoolTotal'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ClearPushState of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_ClearPushState00
static int tolua_UITolua_Button_ClearPushState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ClearPushState'",NULL);
#endif
  {
   self->ClearPushState();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ClearPushState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetNormalTextColor of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetNormalTextColor00
static int tolua_UITolua_Button_SetNormalTextColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  int r = ((int)  tolua_tonumber(tolua_S,2,0));
  int g = ((int)  tolua_tonumber(tolua_S,3,0));
  int b = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetNormalTextColor'",NULL);
#endif
  {
   self->SetNormalTextColor(r,g,b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetNormalTextColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPushedTextColor of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetPushedTextColor00
static int tolua_UITolua_Button_SetPushedTextColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  int r = ((int)  tolua_tonumber(tolua_S,2,0));
  int g = ((int)  tolua_tonumber(tolua_S,3,0));
  int b = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPushedTextColor'",NULL);
#endif
  {
   self->SetPushedTextColor(r,g,b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPushedTextColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetHighlightTextColor of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetHighlightTextColor00
static int tolua_UITolua_Button_SetHighlightTextColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  int r = ((int)  tolua_tonumber(tolua_S,2,0));
  int g = ((int)  tolua_tonumber(tolua_S,3,0));
  int b = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetHighlightTextColor'",NULL);
#endif
  {
   self->SetHighlightTextColor(r,g,b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetHighlightTextColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetDisableTextColor of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetDisableTextColor00
static int tolua_UITolua_Button_SetDisableTextColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  int r = ((int)  tolua_tonumber(tolua_S,2,0));
  int g = ((int)  tolua_tonumber(tolua_S,3,0));
  int b = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetDisableTextColor'",NULL);
#endif
  {
   self->SetDisableTextColor(r,g,b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetDisableTextColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetButtonColor of class  Button */
#ifndef TOLUA_DISABLE_tolua_UITolua_Button_SetButtonColor00
static int tolua_UITolua_Button_SetButtonColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Button",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Button* self = (Button*)  tolua_tousertype(tolua_S,1,0);
  int r = ((int)  tolua_tonumber(tolua_S,2,0));
  int g = ((int)  tolua_tonumber(tolua_S,3,0));
  int b = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetButtonColor'",NULL);
#endif
  {
   self->SetButtonColor(r,g,b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetButtonColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetCheckState of class  CheckButton */
#ifndef TOLUA_DISABLE_tolua_UITolua_CheckButton_GetCheckState00
static int tolua_UITolua_CheckButton_GetCheckState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CheckButton",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CheckButton* self = (CheckButton*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetCheckState'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->GetCheckState();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCheckState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setCheckState of class  CheckButton */
#ifndef TOLUA_DISABLE_tolua_UITolua_CheckButton_setCheckState00
static int tolua_UITolua_CheckButton_setCheckState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CheckButton",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CheckButton* self = (CheckButton*)  tolua_tousertype(tolua_S,1,0);
  bool bChecked = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCheckState'",NULL);
#endif
  {
   self->setCheckState(bChecked);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setCheckState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSelctTexLen of class  EditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_EditBox_GetSelctTexLen00
static int tolua_UITolua_EditBox_GetSelctTexLen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EditBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EditBox* self = (EditBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSelctTexLen'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetSelctTexLen();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSelctTexLen'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsAnyTextSelect of class  EditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_EditBox_IsAnyTextSelect00
static int tolua_UITolua_EditBox_IsAnyTextSelect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EditBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EditBox* self = (EditBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsAnyTextSelect'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsAnyTextSelect();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsAnyTextSelect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReplaceSelectText of class  EditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_EditBox_ReplaceSelectText00
static int tolua_UITolua_EditBox_ReplaceSelectText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EditBox",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EditBox* self = (EditBox*)  tolua_tousertype(tolua_S,1,0);
  const char* pszReplaceText = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReplaceSelectText'",NULL);
#endif
  {
   self->ReplaceSelectText(pszReplaceText);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReplaceSelectText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Clear of class  EditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_EditBox_Clear00
static int tolua_UITolua_EditBox_Clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EditBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EditBox* self = (EditBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Clear'",NULL);
#endif
  {
   self->Clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetText of class  EditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_EditBox_SetText00
static int tolua_UITolua_EditBox_SetText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EditBox",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EditBox* self = (EditBox*)  tolua_tousertype(tolua_S,1,0);
  const char* ptext = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetText'",NULL);
#endif
  {
   self->SetText(ptext);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddText of class  EditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_EditBox_AddText00
static int tolua_UITolua_EditBox_AddText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EditBox",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EditBox* self = (EditBox*)  tolua_tousertype(tolua_S,1,0);
  const char* ptext = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddText'",NULL);
#endif
  {
   self->AddText(ptext);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetText of class  EditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_EditBox_GetText00
static int tolua_UITolua_EditBox_GetText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EditBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EditBox* self = (EditBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetText'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->GetText();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPassWord of class  EditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_EditBox_GetPassWord00
static int tolua_UITolua_EditBox_GetPassWord00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EditBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EditBox* self = (EditBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPassWord'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->GetPassWord();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPassWord'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetTextColor of class  EditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_EditBox_SetTextColor00
static int tolua_UITolua_EditBox_SetTextColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EditBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EditBox* self = (EditBox*)  tolua_tousertype(tolua_S,1,0);
  int r = ((int)  tolua_tonumber(tolua_S,2,0));
  int g = ((int)  tolua_tonumber(tolua_S,3,0));
  int b = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetTextColor'",NULL);
#endif
  {
   self->SetTextColor(r,g,b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetTextColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddStringToHistory of class  EditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_EditBox_AddStringToHistory00
static int tolua_UITolua_EditBox_AddStringToHistory00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EditBox",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EditBox* self = (EditBox*)  tolua_tousertype(tolua_S,1,0);
  const char* text = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddStringToHistory'",NULL);
#endif
  {
   self->AddStringToHistory(text);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddStringToHistory'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clearHistory of class  EditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_EditBox_clearHistory00
static int tolua_UITolua_EditBox_clearHistory00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EditBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EditBox* self = (EditBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clearHistory'",NULL);
#endif
  {
   self->clearHistory();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clearHistory'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SelectAllText of class  EditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_EditBox_SelectAllText00
static int tolua_UITolua_EditBox_SelectAllText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EditBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EditBox* self = (EditBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SelectAllText'",NULL);
#endif
  {
   self->SelectAllText();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SelectAllText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: enableIME of class  EditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_EditBox_enableIME00
static int tolua_UITolua_EditBox_enableIME00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EditBox",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EditBox* self = (EditBox*)  tolua_tousertype(tolua_S,1,0);
  bool b = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'enableIME'",NULL);
#endif
  {
   self->enableIME(b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'enableIME'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPassword of class  EditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_EditBox_SetPassword00
static int tolua_UITolua_EditBox_SetPassword00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EditBox",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EditBox* self = (EditBox*)  tolua_tousertype(tolua_S,1,0);
  const char* ptext = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPassword'",NULL);
#endif
  {
   self->SetPassword(ptext);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPassword'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: enableEdit of class  EditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_EditBox_enableEdit00
static int tolua_UITolua_EditBox_enableEdit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EditBox",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EditBox* self = (EditBox*)  tolua_tousertype(tolua_S,1,0);
  bool b = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'enableEdit'",NULL);
#endif
  {
   self->enableEdit(b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'enableEdit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTextExtentWidth of class  EditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_EditBox_GetTextExtentWidth00
static int tolua_UITolua_EditBox_GetTextExtentWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EditBox",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EditBox* self = (EditBox*)  tolua_tousertype(tolua_S,1,0);
  const char* pszContent = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTextExtentWidth'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetTextExtentWidth(pszContent);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTextExtentWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMaxChar of class  EditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_EditBox_setMaxChar00
static int tolua_UITolua_EditBox_setMaxChar00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EditBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EditBox* self = (EditBox*)  tolua_tousertype(tolua_S,1,0);
  int nMax = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMaxChar'",NULL);
#endif
  {
   self->setMaxChar(nMax);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMaxChar'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCursorPos of class  EditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_EditBox_getCursorPos00
static int tolua_UITolua_EditBox_getCursorPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EditBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EditBox* self = (EditBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCursorPos'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getCursorPos();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCursorPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setCursorPos of class  EditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_EditBox_setCursorPos00
static int tolua_UITolua_EditBox_setCursorPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EditBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EditBox* self = (EditBox*)  tolua_tousertype(tolua_S,1,0);
  int nPos = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCursorPos'",NULL);
#endif
  {
   self->setCursorPos(nPos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setCursorPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSelBegin of class  EditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_EditBox_getSelBegin00
static int tolua_UITolua_EditBox_getSelBegin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EditBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EditBox* self = (EditBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSelBegin'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getSelBegin();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSelBegin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setSelBegin of class  EditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_EditBox_setSelBegin00
static int tolua_UITolua_EditBox_setSelBegin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EditBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EditBox* self = (EditBox*)  tolua_tousertype(tolua_S,1,0);
  int nPos = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setSelBegin'",NULL);
#endif
  {
   self->setSelBegin(nPos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setSelBegin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReplacePartialText of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_ReplacePartialText00
static int tolua_UITolua_RichText_ReplacePartialText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  const char* pszDestText = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* pszNewText = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReplacePartialText'",NULL);
#endif
  {
   self->ReplacePartialText(pszDestText,pszNewText);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReplacePartialText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTwoFaceInterval of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_setTwoFaceInterval00
static int tolua_UITolua_RichText_setTwoFaceInterval00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  float fXInterval = ((float)  tolua_tonumber(tolua_S,2,0));
  float fYInterval = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTwoFaceInterval'",NULL);
#endif
  {
   self->setTwoFaceInterval(fXInterval,fYInterval);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTwoFaceInterval'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLinkTextRect of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_getLinkTextRect00
static int tolua_UITolua_RichText_getLinkTextRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  const char* pszText = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLinkTextRect'",NULL);
#endif
  {
   tagRect_ToLua& tolua_ret = (tagRect_ToLua&)  self->getLinkTextRect(pszText);
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"tagRect_ToLua");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLinkTextRect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFaceWidth of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_getFaceWidth00
static int tolua_UITolua_RichText_getFaceWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFaceWidth'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getFaceWidth();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFaceWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFaceHeight of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_getFaceHeight00
static int tolua_UITolua_RichText_getFaceHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFaceHeight'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getFaceHeight();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFaceHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetFaceTexture of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_SetFaceTexture00
static int tolua_UITolua_RichText_SetFaceTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  const char* path = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetFaceTexture'",NULL);
#endif
  {
   self->SetFaceTexture(path);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFaceTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetFaceTexUV of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_SetFaceTexUV00
static int tolua_UITolua_RichText_SetFaceTexUV00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  int width = ((int)  tolua_tonumber(tolua_S,4,0));
  int height = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetFaceTexUV'",NULL);
#endif
  {
   self->SetFaceTexUV(x,y,width,height);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFaceTexUV'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetFaceTexRelUV of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_SetFaceTexRelUV00
static int tolua_UITolua_RichText_SetFaceTexRelUV00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetFaceTexRelUV'",NULL);
#endif
  {
   self->SetFaceTexRelUV(x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFaceTexRelUV'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetAlpha of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_SetAlpha00
static int tolua_UITolua_RichText_SetAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  float fAlpha = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetAlpha'",NULL);
#endif
  {
   self->SetAlpha(fAlpha);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetAlpha'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetAlpha of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_GetAlpha00
static int tolua_UITolua_RichText_GetAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetAlpha'",NULL);
#endif
  {
   float tolua_ret = (float)  self->GetAlpha();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetAlpha'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: resizeRect of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_resizeRect00
static int tolua_UITolua_RichText_resizeRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  int Width = ((int)  tolua_tonumber(tolua_S,2,0));
  int Height = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'resizeRect'",NULL);
#endif
  {
   self->resizeRect(Width,Height);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'resizeRect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: resizeRichWidth of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_resizeRichWidth00
static int tolua_UITolua_RichText_resizeRichWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  int Width = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'resizeRichWidth'",NULL);
#endif
  {
   self->resizeRichWidth(Width);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'resizeRichWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: resizeRichHeight of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_resizeRichHeight00
static int tolua_UITolua_RichText_resizeRichHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  int Height = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'resizeRichHeight'",NULL);
#endif
  {
   self->resizeRichHeight(Height);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'resizeRichHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: resizeText of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_resizeText00
static int tolua_UITolua_RichText_resizeText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  int Width = ((int)  tolua_tonumber(tolua_S,2,0));
  int Height = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'resizeText'",NULL);
#endif
  {
   self->resizeText(Width,Height);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'resizeText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddText of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_AddText00
static int tolua_UITolua_RichText_AddText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  const char* szText = ((const char*)  tolua_tostring(tolua_S,2,0));
  int r = ((int)  tolua_tonumber(tolua_S,3,0));
  int g = ((int)  tolua_tonumber(tolua_S,4,0));
  int b = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddText'",NULL);
#endif
  {
   self->AddText(szText,r,g,b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetText of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_SetText00
static int tolua_UITolua_RichText_SetText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  const char* szText = ((const char*)  tolua_tostring(tolua_S,2,0));
  int r = ((int)  tolua_tonumber(tolua_S,3,0));
  int g = ((int)  tolua_tonumber(tolua_S,4,0));
  int b = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetText'",NULL);
#endif
  {
   self->SetText(szText,r,g,b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetAutoExtend of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_SetAutoExtend00
static int tolua_UITolua_RichText_SetAutoExtend00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  bool bAutoExtend = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetAutoExtend'",NULL);
#endif
  {
   self->SetAutoExtend(bAutoExtend);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetAutoExtend'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetLinkTextColor of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_SetLinkTextColor00
static int tolua_UITolua_RichText_SetLinkTextColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  int r = ((int)  tolua_tonumber(tolua_S,2,0));
  int g = ((int)  tolua_tonumber(tolua_S,3,0));
  int b = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetLinkTextColor'",NULL);
#endif
  {
   self->SetLinkTextColor(r,g,b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetLinkTextColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetShadowColor of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_SetShadowColor00
static int tolua_UITolua_RichText_SetShadowColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  int r = ((int)  tolua_tonumber(tolua_S,2,0));
  int g = ((int)  tolua_tonumber(tolua_S,3,0));
  int b = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetShadowColor'",NULL);
#endif
  {
   self->SetShadowColor(r,g,b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetShadowColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetFontType of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_SetFontType00
static int tolua_UITolua_RichText_SetFontType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  int nType = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetFontType'",NULL);
#endif
  {
   self->SetFontType(nType);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFontType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Clear of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_Clear00
static int tolua_UITolua_RichText_Clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Clear'",NULL);
#endif
  {
   self->Clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clearHistory of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_clearHistory00
static int tolua_UITolua_RichText_clearHistory00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clearHistory'",NULL);
#endif
  {
   self->clearHistory();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clearHistory'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetDispPos of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_SetDispPos00
static int tolua_UITolua_RichText_SetDispPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  float fY = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetDispPos'",NULL);
#endif
  {
   self->SetDispPos(fY);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetDispPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IncrDispPos of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_IncrDispPos00
static int tolua_UITolua_RichText_IncrDispPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  float fY = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IncrDispPos'",NULL);
#endif
  {
   self->IncrDispPos(fY);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IncrDispPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetDispPos of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_GetDispPos00
static int tolua_UITolua_RichText_GetDispPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetDispPos'",NULL);
#endif
  {
   float tolua_ret = (float)  self->GetDispPos();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetDispPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetDispPosX of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_SetDispPosX00
static int tolua_UITolua_RichText_SetDispPosX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  float fY = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetDispPosX'",NULL);
#endif
  {
   self->SetDispPosX(fY);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetDispPosX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetDispPosX of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_GetDispPosX00
static int tolua_UITolua_RichText_GetDispPosX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetDispPosX'",NULL);
#endif
  {
   float tolua_ret = (float)  self->GetDispPosX();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetDispPosX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetStartDispPos of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_GetStartDispPos00
static int tolua_UITolua_RichText_GetStartDispPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetStartDispPos'",NULL);
#endif
  {
   float tolua_ret = (float)  self->GetStartDispPos();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetStartDispPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetEndDispPos of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_GetEndDispPos00
static int tolua_UITolua_RichText_GetEndDispPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEndDispPos'",NULL);
#endif
  {
   float tolua_ret = (float)  self->GetEndDispPos();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetEndDispPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ScrollUp of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_ScrollUp00
static int tolua_UITolua_RichText_ScrollUp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ScrollUp'",NULL);
#endif
  {
   self->ScrollUp();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ScrollUp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ScrollDown of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_ScrollDown00
static int tolua_UITolua_RichText_ScrollDown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ScrollDown'",NULL);
#endif
  {
   self->ScrollDown();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ScrollDown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ScrollFirst of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_ScrollFirst00
static int tolua_UITolua_RichText_ScrollFirst00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ScrollFirst'",NULL);
#endif
  {
   self->ScrollFirst();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ScrollFirst'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ScrollEnd of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_ScrollEnd00
static int tolua_UITolua_RichText_ScrollEnd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ScrollEnd'",NULL);
#endif
  {
   self->ScrollEnd();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ScrollEnd'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTextLines of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_GetTextLines00
static int tolua_UITolua_RichText_GetTextLines00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTextLines'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetTextLines();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTextLines'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLineRealWidth of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_getLineRealWidth00
static int tolua_UITolua_RichText_getLineRealWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  int nLine = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLineRealWidth'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getLineRealWidth(nLine);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLineRealWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLineWidth of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_getLineWidth00
static int tolua_UITolua_RichText_getLineWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  int nLine = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLineWidth'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getLineWidth(nLine);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLineWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetLineWidth of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_GetLineWidth00
static int tolua_UITolua_RichText_GetLineWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetLineWidth'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetLineWidth(index);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetLineWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetLineHeight of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_GetLineHeight00
static int tolua_UITolua_RichText_GetLineHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetLineHeight'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetLineHeight(index);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetLineHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTotalHeight of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_GetTotalHeight00
static int tolua_UITolua_RichText_GetTotalHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTotalHeight'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetTotalHeight();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTotalHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetViewLines of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_GetViewLines00
static int tolua_UITolua_RichText_GetViewLines00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetViewLines'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetViewLines();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetViewLines'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetAccurateViewLines of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_GetAccurateViewLines00
static int tolua_UITolua_RichText_GetAccurateViewLines00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetAccurateViewLines'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetAccurateViewLines();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetAccurateViewLines'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAbsRect of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_setAbsRect00
static int tolua_UITolua_RichText_setAbsRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  float fLeft = ((float)  tolua_tonumber(tolua_S,2,0));
  float fTop = ((float)  tolua_tonumber(tolua_S,3,0));
  float fRight = ((float)  tolua_tonumber(tolua_S,4,0));
  float fBottom = ((float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAbsRect'",NULL);
#endif
  {
   self->setAbsRect(fLeft,fTop,fRight,fBottom);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAbsRect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTextExtentWidth of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_GetTextExtentWidth00
static int tolua_UITolua_RichText_GetTextExtentWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  const char* pszContent = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTextExtentWidth'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetTextExtentWidth(pszContent);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTextExtentWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTextExtentHeight of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_GetTextExtentHeight00
static int tolua_UITolua_RichText_GetTextExtentHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  const char* pszContent = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTextExtentHeight'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetTextExtentHeight(pszContent);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTextExtentHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setCenterLine of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_setCenterLine00
static int tolua_UITolua_RichText_setCenterLine00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
  bool bCenter = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCenterLine'",NULL);
#endif
  {
   self->setCenterLine(bCenter);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setCenterLine'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setCursorNormal of class  RichText */
#ifndef TOLUA_DISABLE_tolua_UITolua_RichText_setCursorNormal00
static int tolua_UITolua_RichText_setCursorNormal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RichText* self = (RichText*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCursorNormal'",NULL);
#endif
  {
   self->setCursorNormal();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setCursorNormal'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CalVerticalScrollRange of class  ScrollFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_ScrollFrame_CalVerticalScrollRange00
static int tolua_UITolua_ScrollFrame_CalVerticalScrollRange00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScrollFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScrollFrame* self = (ScrollFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CalVerticalScrollRange'",NULL);
#endif
  {
   self->CalVerticalScrollRange();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CalVerticalScrollRange'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CalHorizonalScrollRange of class  ScrollFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_ScrollFrame_CalHorizonalScrollRange00
static int tolua_UITolua_ScrollFrame_CalHorizonalScrollRange00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScrollFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScrollFrame* self = (ScrollFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CalHorizonalScrollRange'",NULL);
#endif
  {
   self->CalHorizonalScrollRange();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CalHorizonalScrollRange'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetValueStep of class  ScrollFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_ScrollFrame_SetValueStep00
static int tolua_UITolua_ScrollFrame_SetValueStep00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScrollFrame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScrollFrame* self = (ScrollFrame*)  tolua_tousertype(tolua_S,1,0);
  int value = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetValueStep'",NULL);
#endif
  {
   self->SetValueStep(value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetValueStep'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetValueStep of class  ScrollFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_ScrollFrame_GetValueStep00
static int tolua_UITolua_ScrollFrame_GetValueStep00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScrollFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScrollFrame* self = (ScrollFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetValueStep'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetValueStep();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetValueStep'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetScrollChildRect of class  ScrollFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_ScrollFrame_SetScrollChildRect00
static int tolua_UITolua_ScrollFrame_SetScrollChildRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScrollFrame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScrollFrame* self = (ScrollFrame*)  tolua_tousertype(tolua_S,1,0);
  float fX = ((float)  tolua_tonumber(tolua_S,2,0));
  float fY = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetScrollChildRect'",NULL);
#endif
  {
   self->SetScrollChildRect(fX,fY);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetScrollChildRect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IncreaseScrollChildRect of class  ScrollFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_ScrollFrame_IncreaseScrollChildRect00
static int tolua_UITolua_ScrollFrame_IncreaseScrollChildRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScrollFrame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScrollFrame* self = (ScrollFrame*)  tolua_tousertype(tolua_S,1,0);
  float nOffsetX = ((float)  tolua_tonumber(tolua_S,2,0));
  float nOffsetY = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IncreaseScrollChildRect'",NULL);
#endif
  {
   self->IncreaseScrollChildRect(nOffsetX,nOffsetY);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IncreaseScrollChildRect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetVerticalScroll of class  ScrollFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_ScrollFrame_SetVerticalScroll00
static int tolua_UITolua_ScrollFrame_SetVerticalScroll00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScrollFrame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScrollFrame* self = (ScrollFrame*)  tolua_tousertype(tolua_S,1,0);
  float value = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetVerticalScroll'",NULL);
#endif
  {
   self->SetVerticalScroll(value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetVerticalScroll'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IncreaseVerticalScroll of class  ScrollFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_ScrollFrame_IncreaseVerticalScroll00
static int tolua_UITolua_ScrollFrame_IncreaseVerticalScroll00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScrollFrame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScrollFrame* self = (ScrollFrame*)  tolua_tousertype(tolua_S,1,0);
  float value = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IncreaseVerticalScroll'",NULL);
#endif
  {
   self->IncreaseVerticalScroll(value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IncreaseVerticalScroll'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetVerticalOffset of class  ScrollFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_ScrollFrame_GetVerticalOffset00
static int tolua_UITolua_ScrollFrame_GetVerticalOffset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScrollFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScrollFrame* self = (ScrollFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetVerticalOffset'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetVerticalOffset();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetVerticalOffset'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetVerticalScrollRange of class  ScrollFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_ScrollFrame_SetVerticalScrollRange00
static int tolua_UITolua_ScrollFrame_SetVerticalScrollRange00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScrollFrame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScrollFrame* self = (ScrollFrame*)  tolua_tousertype(tolua_S,1,0);
  int nRange = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetVerticalScrollRange'",NULL);
#endif
  {
   self->SetVerticalScrollRange(nRange);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetVerticalScrollRange'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetVerticalScrollRange of class  ScrollFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_ScrollFrame_GetVerticalScrollRange00
static int tolua_UITolua_ScrollFrame_GetVerticalScrollRange00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScrollFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScrollFrame* self = (ScrollFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetVerticalScrollRange'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetVerticalScrollRange();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetVerticalScrollRange'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetHorizonalScroll of class  ScrollFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_ScrollFrame_SetHorizonalScroll00
static int tolua_UITolua_ScrollFrame_SetHorizonalScroll00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScrollFrame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScrollFrame* self = (ScrollFrame*)  tolua_tousertype(tolua_S,1,0);
  float value = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetHorizonalScroll'",NULL);
#endif
  {
   self->SetHorizonalScroll(value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetHorizonalScroll'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IncreaseHorizonalScroll of class  ScrollFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_ScrollFrame_IncreaseHorizonalScroll00
static int tolua_UITolua_ScrollFrame_IncreaseHorizonalScroll00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScrollFrame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScrollFrame* self = (ScrollFrame*)  tolua_tousertype(tolua_S,1,0);
  float value = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IncreaseHorizonalScroll'",NULL);
#endif
  {
   self->IncreaseHorizonalScroll(value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IncreaseHorizonalScroll'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetHorizonalOffset of class  ScrollFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_ScrollFrame_GetHorizonalOffset00
static int tolua_UITolua_ScrollFrame_GetHorizonalOffset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScrollFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScrollFrame* self = (ScrollFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHorizonalOffset'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetHorizonalOffset();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetHorizonalOffset'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetHorizonalScrollRange of class  ScrollFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_ScrollFrame_SetHorizonalScrollRange00
static int tolua_UITolua_ScrollFrame_SetHorizonalScrollRange00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScrollFrame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScrollFrame* self = (ScrollFrame*)  tolua_tousertype(tolua_S,1,0);
  int nRange = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetHorizonalScrollRange'",NULL);
#endif
  {
   self->SetHorizonalScrollRange(nRange);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetHorizonalScrollRange'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetHorizonalScrollRange of class  ScrollFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_ScrollFrame_GetHorizonalScrollRange00
static int tolua_UITolua_ScrollFrame_GetHorizonalScrollRange00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScrollFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScrollFrame* self = (ScrollFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHorizonalScrollRange'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetHorizonalScrollRange();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetHorizonalScrollRange'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ResetChildAnchor of class  ScrollFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_ScrollFrame_ResetChildAnchor00
static int tolua_UITolua_ScrollFrame_ResetChildAnchor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScrollFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScrollFrame* self = (ScrollFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ResetChildAnchor'",NULL);
#endif
  {
   self->ResetChildAnchor();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ResetChildAnchor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetValue of class  Slider */
#ifndef TOLUA_DISABLE_tolua_UITolua_Slider_SetValue00
static int tolua_UITolua_Slider_SetValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Slider",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Slider* self = (Slider*)  tolua_tousertype(tolua_S,1,0);
  float value = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetValue'",NULL);
#endif
  {
   self->SetValue(value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetValue of class  Slider */
#ifndef TOLUA_DISABLE_tolua_UITolua_Slider_GetValue00
static int tolua_UITolua_Slider_GetValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Slider",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Slider* self = (Slider*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetValue'",NULL);
#endif
  {
   float tolua_ret = (float)  self->GetValue();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetLastValue of class  Slider */
#ifndef TOLUA_DISABLE_tolua_UITolua_Slider_GetLastValue00
static int tolua_UITolua_Slider_GetLastValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Slider",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Slider* self = (Slider*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetLastValue'",NULL);
#endif
  {
   float tolua_ret = (float)  self->GetLastValue();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetLastValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetMinValue of class  Slider */
#ifndef TOLUA_DISABLE_tolua_UITolua_Slider_SetMinValue00
static int tolua_UITolua_Slider_SetMinValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Slider",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Slider* self = (Slider*)  tolua_tousertype(tolua_S,1,0);
  float value = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetMinValue'",NULL);
#endif
  {
   self->SetMinValue(value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetMinValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMinValue of class  Slider */
#ifndef TOLUA_DISABLE_tolua_UITolua_Slider_GetMinValue00
static int tolua_UITolua_Slider_GetMinValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Slider",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Slider* self = (Slider*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMinValue'",NULL);
#endif
  {
   float tolua_ret = (float)  self->GetMinValue();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMinValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetMaxValue of class  Slider */
#ifndef TOLUA_DISABLE_tolua_UITolua_Slider_SetMaxValue00
static int tolua_UITolua_Slider_SetMaxValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Slider",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Slider* self = (Slider*)  tolua_tousertype(tolua_S,1,0);
  float value = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetMaxValue'",NULL);
#endif
  {
   self->SetMaxValue(value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetMaxValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMaxValue of class  Slider */
#ifndef TOLUA_DISABLE_tolua_UITolua_Slider_GetMaxValue00
static int tolua_UITolua_Slider_GetMaxValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Slider",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Slider* self = (Slider*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMaxValue'",NULL);
#endif
  {
   float tolua_ret = (float)  self->GetMaxValue();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMaxValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetValueStep of class  Slider */
#ifndef TOLUA_DISABLE_tolua_UITolua_Slider_SetValueStep00
static int tolua_UITolua_Slider_SetValueStep00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Slider",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Slider* self = (Slider*)  tolua_tousertype(tolua_S,1,0);
  float value = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetValueStep'",NULL);
#endif
  {
   self->SetValueStep(value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetValueStep'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetValueStep of class  Slider */
#ifndef TOLUA_DISABLE_tolua_UITolua_Slider_GetValueStep00
static int tolua_UITolua_Slider_GetValueStep00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Slider",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Slider* self = (Slider*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetValueStep'",NULL);
#endif
  {
   float tolua_ret = (float)  self->GetValueStep();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetValueStep'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetDrawPointList of class  DrawLineFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_DrawLineFrame_SetDrawPointList00
static int tolua_UITolua_DrawLineFrame_SetDrawPointList00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"DrawLineFrame",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"std::deque<Ogre::Point2D>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  DrawLineFrame* self = (DrawLineFrame*)  tolua_tousertype(tolua_S,1,0);
  std::deque<Ogre::Point2D> pPointList = *((std::deque<Ogre::Point2D>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetDrawPointList'",NULL);
#endif
  {
   self->SetDrawPointList(pPointList);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetDrawPointList'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ClearDrawPointList of class  DrawLineFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_DrawLineFrame_ClearDrawPointList00
static int tolua_UITolua_DrawLineFrame_ClearDrawPointList00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"DrawLineFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  DrawLineFrame* self = (DrawLineFrame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ClearDrawPointList'",NULL);
#endif
  {
   self->ClearDrawPointList();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ClearDrawPointList'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddPoint of class  DrawLineFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_DrawLineFrame_AddPoint00
static int tolua_UITolua_DrawLineFrame_AddPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"DrawLineFrame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  DrawLineFrame* self = (DrawLineFrame*)  tolua_tousertype(tolua_S,1,0);
  int nX = ((int)  tolua_tonumber(tolua_S,2,0));
  int nY = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddPoint'",NULL);
#endif
  {
   self->AddPoint(nX,nY);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddPoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetSliderValue of class  MultiEditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_MultiEditBox_SetSliderValue00
static int tolua_UITolua_MultiEditBox_SetSliderValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MultiEditBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MultiEditBox* self = (MultiEditBox*)  tolua_tousertype(tolua_S,1,0);
  int nValue = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSliderValue'",NULL);
#endif
  {
   self->SetSliderValue(nValue);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetSliderValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetText of class  MultiEditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_MultiEditBox_SetText00
static int tolua_UITolua_MultiEditBox_SetText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MultiEditBox",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MultiEditBox* self = (MultiEditBox*)  tolua_tousertype(tolua_S,1,0);
  const char* ptext = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetText'",NULL);
#endif
  {
   self->SetText(ptext);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddText of class  MultiEditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_MultiEditBox_AddText00
static int tolua_UITolua_MultiEditBox_AddText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MultiEditBox",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MultiEditBox* self = (MultiEditBox*)  tolua_tousertype(tolua_S,1,0);
  const char* ptext = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddText'",NULL);
#endif
  {
   self->AddText(ptext);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetTextColor of class  MultiEditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_MultiEditBox_SetTextColor00
static int tolua_UITolua_MultiEditBox_SetTextColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MultiEditBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MultiEditBox* self = (MultiEditBox*)  tolua_tousertype(tolua_S,1,0);
  int r = ((int)  tolua_tonumber(tolua_S,2,0));
  int g = ((int)  tolua_tonumber(tolua_S,3,0));
  int b = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetTextColor'",NULL);
#endif
  {
   self->SetTextColor(r,g,b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetTextColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetText of class  MultiEditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_MultiEditBox_GetText00
static int tolua_UITolua_MultiEditBox_GetText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MultiEditBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MultiEditBox* self = (MultiEditBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetText'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->GetText();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Clear of class  MultiEditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_MultiEditBox_Clear00
static int tolua_UITolua_MultiEditBox_Clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MultiEditBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MultiEditBox* self = (MultiEditBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Clear'",NULL);
#endif
  {
   self->Clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: enableIME of class  MultiEditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_MultiEditBox_enableIME00
static int tolua_UITolua_MultiEditBox_enableIME00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MultiEditBox",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MultiEditBox* self = (MultiEditBox*)  tolua_tousertype(tolua_S,1,0);
  bool b = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'enableIME'",NULL);
#endif
  {
   self->enableIME(b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'enableIME'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTextCount of class  MultiEditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_MultiEditBox_getTextCount00
static int tolua_UITolua_MultiEditBox_getTextCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MultiEditBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MultiEditBox* self = (MultiEditBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTextCount'",NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getTextCount();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTextCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMaxWordCount of class  MultiEditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_MultiEditBox_setMaxWordCount00
static int tolua_UITolua_MultiEditBox_setMaxWordCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MultiEditBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MultiEditBox* self = (MultiEditBox*)  tolua_tousertype(tolua_S,1,0);
  int nMax = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMaxWordCount'",NULL);
#endif
  {
   self->setMaxWordCount(nMax);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMaxWordCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SelectAllText of class  MultiEditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_MultiEditBox_SelectAllText00
static int tolua_UITolua_MultiEditBox_SelectAllText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MultiEditBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MultiEditBox* self = (MultiEditBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SelectAllText'",NULL);
#endif
  {
   self->SelectAllText();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SelectAllText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: enableEdit of class  MultiEditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_MultiEditBox_enableEdit00
static int tolua_UITolua_MultiEditBox_enableEdit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MultiEditBox",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MultiEditBox* self = (MultiEditBox*)  tolua_tousertype(tolua_S,1,0);
  bool b = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'enableEdit'",NULL);
#endif
  {
   self->enableEdit(b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'enableEdit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCursorPos of class  MultiEditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_MultiEditBox_getCursorPos00
static int tolua_UITolua_MultiEditBox_getCursorPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MultiEditBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MultiEditBox* self = (MultiEditBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCursorPos'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getCursorPos();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCursorPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setCursorPos of class  MultiEditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_MultiEditBox_setCursorPos00
static int tolua_UITolua_MultiEditBox_setCursorPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MultiEditBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MultiEditBox* self = (MultiEditBox*)  tolua_tousertype(tolua_S,1,0);
  int nPos = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCursorPos'",NULL);
#endif
  {
   self->setCursorPos(nPos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setCursorPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSelBegin of class  MultiEditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_MultiEditBox_getSelBegin00
static int tolua_UITolua_MultiEditBox_getSelBegin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MultiEditBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MultiEditBox* self = (MultiEditBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSelBegin'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getSelBegin();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSelBegin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setSelBegin of class  MultiEditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_MultiEditBox_setSelBegin00
static int tolua_UITolua_MultiEditBox_setSelBegin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MultiEditBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MultiEditBox* self = (MultiEditBox*)  tolua_tousertype(tolua_S,1,0);
  int nPos = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setSelBegin'",NULL);
#endif
  {
   self->setSelBegin(nPos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setSelBegin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTextBegin of class  MultiEditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_MultiEditBox_getTextBegin00
static int tolua_UITolua_MultiEditBox_getTextBegin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MultiEditBox",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MultiEditBox* self = (MultiEditBox*)  tolua_tousertype(tolua_S,1,0);
  const char* pszText = ((const char*)  tolua_tostring(tolua_S,2,0));
  int nFindPos = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTextBegin'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getTextBegin(pszText,nFindPos);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTextBegin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTextEnd of class  MultiEditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_MultiEditBox_getTextEnd00
static int tolua_UITolua_MultiEditBox_getTextEnd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MultiEditBox",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MultiEditBox* self = (MultiEditBox*)  tolua_tousertype(tolua_S,1,0);
  const char* pszText = ((const char*)  tolua_tostring(tolua_S,2,0));
  int nFindPos = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTextEnd'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getTextEnd(pszText,nFindPos);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTextEnd'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTextIndexFromRichCharIndex of class  MultiEditBox */
#ifndef TOLUA_DISABLE_tolua_UITolua_MultiEditBox_getTextIndexFromRichCharIndex00
static int tolua_UITolua_MultiEditBox_getTextIndexFromRichCharIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MultiEditBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MultiEditBox* self = (MultiEditBox*)  tolua_tousertype(tolua_S,1,0);
  int nRichCharIndex = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTextIndexFromRichCharIndex'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getTextIndexFromRichCharIndex(nRichCharIndex);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTextIndexFromRichCharIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OpenSite of class  WebBrowerFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_WebBrowerFrame_OpenSite00
static int tolua_UITolua_WebBrowerFrame_OpenSite00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"WebBrowerFrame",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  WebBrowerFrame* self = (WebBrowerFrame*)  tolua_tousertype(tolua_S,1,0);
  const char* szUrl = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OpenSite'",NULL);
#endif
  {
   self->OpenSite(szUrl);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OpenSite'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ShowWindow of class  WebBrowerFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_WebBrowerFrame_ShowWindow00
static int tolua_UITolua_WebBrowerFrame_ShowWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"WebBrowerFrame",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  WebBrowerFrame* self = (WebBrowerFrame*)  tolua_tousertype(tolua_S,1,0);
  bool bShow = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ShowWindow'",NULL);
#endif
  {
   self->ShowWindow(bShow);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ShowWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: ShowUIPanel */
#ifndef TOLUA_DISABLE_tolua_UITolua_ShowUIPanel00
static int tolua_UITolua_ShowUIPanel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* frameName = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   ShowUIPanel(frameName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ShowUIPanel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: HideUIPanel */
#ifndef TOLUA_DISABLE_tolua_UITolua_HideUIPanel00
static int tolua_UITolua_HideUIPanel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* frameName = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   HideUIPanel(frameName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HideUIPanel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: UIBeginDrag */
#ifndef TOLUA_DISABLE_tolua_UITolua_UIBeginDrag00
static int tolua_UITolua_UIBeginDrag00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* path = ((const char*)  tolua_tostring(tolua_S,1,0));
  int id1 = ((int)  tolua_tonumber(tolua_S,2,0));
  int id2 = ((int)  tolua_tonumber(tolua_S,3,0));
  int iNum = ((int)  tolua_tonumber(tolua_S,4,0));
  bool bCompose = ((bool)  tolua_toboolean(tolua_S,5,true));
  int hotspotx = ((int)  tolua_tonumber(tolua_S,6,0));
  int hotspoty = ((int)  tolua_tonumber(tolua_S,7,0));
  {
   UIBeginDrag(path,id1,id2,iNum,bCompose,hotspotx,hotspoty);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'UIBeginDrag'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: UIEndDrag */
#ifndef TOLUA_DISABLE_tolua_UITolua_UIEndDrag00
static int tolua_UITolua_UIEndDrag00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   UIEndDrag();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'UIEndDrag'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: UIIsInDragState */
#ifndef TOLUA_DISABLE_tolua_UITolua_UIIsInDragState00
static int tolua_UITolua_UIIsInDragState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   bool tolua_ret = (bool)  UIIsInDragState();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'UIIsInDragState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: UIGetDragUserData1 */
#ifndef TOLUA_DISABLE_tolua_UITolua_UIGetDragUserData100
static int tolua_UITolua_UIGetDragUserData100(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  UIGetDragUserData1();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'UIGetDragUserData1'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: UIGetDragUserData2 */
#ifndef TOLUA_DISABLE_tolua_UITolua_UIGetDragUserData200
static int tolua_UITolua_UIGetDragUserData200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  UIGetDragUserData2();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'UIGetDragUserData2'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: UIGetDragUserData3 */
#ifndef TOLUA_DISABLE_tolua_UITolua_UIGetDragUserData300
static int tolua_UITolua_UIGetDragUserData300(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  UIGetDragUserData3();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'UIGetDragUserData3'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: CreateButton */
#ifndef TOLUA_DISABLE_tolua_UITolua_CreateButton00
static int tolua_UITolua_CreateButton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,1,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Frame",1,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* name = ((const char*)  tolua_tostring(tolua_S,1,0));
  const char* template_name = ((const char*)  tolua_tostring(tolua_S,2,NULL));
  Frame* pParent = ((Frame*)  tolua_tousertype(tolua_S,3,NULL));
  bool isRoot = ((bool)  tolua_toboolean(tolua_S,4,false));
  {
   Button* tolua_ret = (Button*)  CreateButton(name,template_name,pParent,isRoot);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Button");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CreateButton'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: FindFontString */
#ifndef TOLUA_DISABLE_tolua_UITolua_FindFontString00
static int tolua_UITolua_FindFontString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* name = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   FontString* tolua_ret = (FontString*)  FindFontString(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"FontString");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FindFontString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: FindButton */
#ifndef TOLUA_DISABLE_tolua_UITolua_FindButton00
static int tolua_UITolua_FindButton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* name = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   Button* tolua_ret = (Button*)  FindButton(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Button");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FindButton'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: isAlreadyExistFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_isAlreadyExistFrame00
static int tolua_UITolua_isAlreadyExistFrame00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* name = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  isAlreadyExistFrame(name);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isAlreadyExistFrame'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: FindTexture */
#ifndef TOLUA_DISABLE_tolua_UITolua_FindTexture00
static int tolua_UITolua_FindTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* name = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   Texture* tolua_ret = (Texture*)  FindTexture(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Texture");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FindTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: closePopWin */
#ifndef TOLUA_DISABLE_tolua_UITolua_closePopWin00
static int tolua_UITolua_closePopWin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   closePopWin();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'closePopWin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: SetCurrentCursor */
#ifndef TOLUA_DISABLE_tolua_UITolua_SetCurrentCursor00
static int tolua_UITolua_SetCurrentCursor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* cursorstate = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   SetCurrentCursor(cursorstate);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetCurrentCursor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: ChangeCursorState */
#ifndef TOLUA_DISABLE_tolua_UITolua_ChangeCursorState00
static int tolua_UITolua_ChangeCursorState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* cursorstate = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   ChangeCursorState(cursorstate);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ChangeCursorState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetCurrentCursor */
#ifndef TOLUA_DISABLE_tolua_UITolua_GetCurrentCursor00
static int tolua_UITolua_GetCurrentCursor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   const char* tolua_ret = (const char*)  GetCurrentCursor();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCurrentCursor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetCurrentCursorLevel */
#ifndef TOLUA_DISABLE_tolua_UITolua_GetCurrentCursorLevel00
static int tolua_UITolua_GetCurrentCursorLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  GetCurrentCursorLevel();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCurrentCursorLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetScreenWidth */
#ifndef TOLUA_DISABLE_tolua_UITolua_GetScreenWidth00
static int tolua_UITolua_GetScreenWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  GetScreenWidth();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetScreenWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetScreenHeight */
#ifndef TOLUA_DISABLE_tolua_UITolua_GetScreenHeight00
static int tolua_UITolua_GetScreenHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  GetScreenHeight();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetScreenHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: SetFocusFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_SetFocusFrame00
static int tolua_UITolua_SetFocusFrame00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* frameName = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   SetFocusFrame(frameName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFocusFrame'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: SetFocusFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_SetFocusFrame01
static int tolua_UITolua_SetFocusFrame01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Frame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Frame* frame = ((Frame*)  tolua_tousertype(tolua_S,1,0));
  {
   SetFocusFrame(frame);
  }
 }
 return 0;
tolua_lerror:
 return tolua_UITolua_SetFocusFrame00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetFocusFrameParent */
#ifndef TOLUA_DISABLE_tolua_UITolua_GetFocusFrameParent00
static int tolua_UITolua_GetFocusFrameParent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   const char* tolua_ret = (const char*)  GetFocusFrameParent();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFocusFrameParent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: ClearFocus */
#ifndef TOLUA_DISABLE_tolua_UITolua_ClearFocus00
static int tolua_UITolua_ClearFocus00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ClearFocus();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ClearFocus'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetCursorPosX */
#ifndef TOLUA_DISABLE_tolua_UITolua_GetCursorPosX00
static int tolua_UITolua_GetCursorPosX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  GetCursorPosX();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCursorPosX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetCursorPosY */
#ifndef TOLUA_DISABLE_tolua_UITolua_GetCursorPosY00
static int tolua_UITolua_GetCursorPosY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  GetCursorPosY();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCursorPosY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: pushUnCloseWin */
#ifndef TOLUA_DISABLE_tolua_UITolua_pushUnCloseWin00
static int tolua_UITolua_pushUnCloseWin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* frameName = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   pushUnCloseWin(frameName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pushUnCloseWin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetCurrentDialogFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_GetCurrentDialogFrame00
static int tolua_UITolua_GetCurrentDialogFrame00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   const char* tolua_ret = (const char*)  GetCurrentDialogFrame();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCurrentDialogFrame'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: isPointInFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_isPointInFrame00
static int tolua_UITolua_isPointInFrame00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* pszFrameName = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  isPointInFrame(pszFrameName);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isPointInFrame'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: isPointInFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_isPointInFrame01
static int tolua_UITolua_isPointInFrame01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int x = ((int)  tolua_tonumber(tolua_S,1,0));
  int y = ((int)  tolua_tonumber(tolua_S,2,0));
  const char* pszFrameName = ((const char*)  tolua_tostring(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  isPointInFrame(x,y,pszFrameName);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_UITolua_isPointInFrame00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: isTwoFrameXConflict */
#ifndef TOLUA_DISABLE_tolua_UITolua_isTwoFrameXConflict00
static int tolua_UITolua_isTwoFrameXConflict00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LayoutFrame* pLeft = ((LayoutFrame*)  tolua_tousertype(tolua_S,1,0));
  LayoutFrame* pRight = ((LayoutFrame*)  tolua_tousertype(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  isTwoFrameXConflict(pLeft,pRight);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isTwoFrameXConflict'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: CopyToMemory */
#ifndef TOLUA_DISABLE_tolua_UITolua_CopyToMemory00
static int tolua_UITolua_CopyToMemory00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* pszText = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   CopyToMemory(pszText);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CopyToMemory'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: isFiliation */
#ifndef TOLUA_DISABLE_tolua_UITolua_isFiliation00
static int tolua_UITolua_isFiliation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* pszParentFrame = ((const char*)  tolua_tostring(tolua_S,1,0));
  const char* pszChildFrame = ((const char*)  tolua_tostring(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  isFiliation(pszParentFrame,pszChildFrame);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isFiliation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: addChangedFrames */
#ifndef TOLUA_DISABLE_tolua_UITolua_addChangedFrames00
static int tolua_UITolua_addChangedFrames00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* pszFrameName = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   addChangedFrames(pszFrameName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addChangedFrames'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: addChangedFrames */
#ifndef TOLUA_DISABLE_tolua_UITolua_addChangedFrames01
static int tolua_UITolua_addChangedFrames01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  LayoutFrame* pFrame = ((LayoutFrame*)  tolua_tousertype(tolua_S,1,0));
  {
   addChangedFrames(pFrame);
  }
 }
 return 0;
tolua_lerror:
 return tolua_UITolua_addChangedFrames00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: playUISound */
#ifndef TOLUA_DISABLE_tolua_UITolua_playUISound00
static int tolua_UITolua_playUISound00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* pszSoundPath = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   playUISound(pszSoundPath);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'playUISound'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetScreenScaleY */
#ifndef TOLUA_DISABLE_tolua_UITolua_GetScreenScaleY00
static int tolua_UITolua_GetScreenScaleY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   float tolua_ret = (float)  GetScreenScaleY();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetScreenScaleY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetScreenScaleX */
#ifndef TOLUA_DISABLE_tolua_UITolua_GetScreenScaleX00
static int tolua_UITolua_GetScreenScaleX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   float tolua_ret = (float)  GetScreenScaleX();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetScreenScaleX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetFontTextWidth */
#ifndef TOLUA_DISABLE_tolua_UITolua_GetFontTextWidth00
static int tolua_UITolua_GetFontTextWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"FontString",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  FontString* pFont = ((FontString*)  tolua_tousertype(tolua_S,1,0));
  {
   float tolua_ret = (float)  GetFontTextWidth(pFont);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFontTextWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: IsInExistence */
#ifndef TOLUA_DISABLE_tolua_UITolua_IsInExistence00
static int tolua_UITolua_IsInExistence00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* szFileName = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  IsInExistence(szFileName);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsInExistence'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: isInVisibleArea */
#ifndef TOLUA_DISABLE_tolua_UITolua_isInVisibleArea00
static int tolua_UITolua_isInVisibleArea00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const LayoutFrame",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const LayoutFrame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const LayoutFrame* pIncludedFrame = ((const LayoutFrame*)  tolua_tousertype(tolua_S,1,0));
  const LayoutFrame* pClipFrame = ((const LayoutFrame*)  tolua_tousertype(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  isInVisibleArea(pIncludedFrame,pClipFrame);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isInVisibleArea'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: isKeyPressed */
#ifndef TOLUA_DISABLE_tolua_UITolua_isKeyPressed00
static int tolua_UITolua_isKeyPressed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int key = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  isKeyPressed(key);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isKeyPressed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: getRand */
#ifndef TOLUA_DISABLE_tolua_UITolua_getRand00
static int tolua_UITolua_getRand00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int nMinNum = ((int)  tolua_tonumber(tolua_S,1,0));
  int nMaxNum = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   int tolua_ret = (int)  getRand(nMinNum,nMaxNum);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRand'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetFocusFrameName */
#ifndef TOLUA_DISABLE_tolua_UITolua_GetFocusFrameName00
static int tolua_UITolua_GetFocusFrameName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   const char* tolua_ret = (const char*)  GetFocusFrameName();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFocusFrameName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: replaceFaceString */
#ifndef TOLUA_DISABLE_tolua_UITolua_replaceFaceString00
static int tolua_UITolua_replaceFaceString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* pszText = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   const char* tolua_ret = (const char*)  replaceFaceString(pszText);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'replaceFaceString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: BigInt2Str */
#ifndef TOLUA_DISABLE_tolua_UITolua_BigInt2Str00
static int tolua_UITolua_BigInt2Str00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  long long val = ((long long)  tolua_tonumber(tolua_S,1,0));
  {
   const char* tolua_ret = (const char*)  BigInt2Str(val);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'BigInt2Str'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: updateHeadBindingFrame */
#ifndef TOLUA_DISABLE_tolua_UITolua_updateHeadBindingFrame00
static int tolua_UITolua_updateHeadBindingFrame00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* pszFrameName = ((const char*)  tolua_tostring(tolua_S,1,0));
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  int nWidth = ((int)  tolua_tonumber(tolua_S,4,0));
  int mHeight = ((int)  tolua_tonumber(tolua_S,5,0));
  {
   updateHeadBindingFrame(pszFrameName,x,y,nWidth,mHeight);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'updateHeadBindingFrame'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: showUIFrames */
#ifndef TOLUA_DISABLE_tolua_UITolua_showUIFrames00
static int tolua_UITolua_showUIFrames00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   showUIFrames();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showUIFrames'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: FindLayoutFrame of class  FrameManager */
#ifndef TOLUA_DISABLE_tolua_UITolua_FrameManager_FindLayoutFrame00
static int tolua_UITolua_FrameManager_FindLayoutFrame00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"FrameManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  FrameManager* self = (FrameManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FindLayoutFrame'",NULL);
#endif
  {
   UIObject* tolua_ret = (UIObject*)  self->FindLayoutFrame(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"UIObject");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FindLayoutFrame'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetEditMode of class  FrameManager */
#ifndef TOLUA_DISABLE_tolua_UITolua_FrameManager_GetEditMode00
static int tolua_UITolua_FrameManager_GetEditMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"FrameManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  FrameManager* self = (FrameManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEditMode'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetEditMode();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetEditMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: EnableAccelerator of class  FrameManager */
#ifndef TOLUA_DISABLE_tolua_UITolua_FrameManager_EnableAccelerator00
static int tolua_UITolua_FrameManager_EnableAccelerator00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"FrameManager",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  FrameManager* self = (FrameManager*)  tolua_tousertype(tolua_S,1,0);
  bool enable = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'EnableAccelerator'",NULL);
#endif
  {
   self->EnableAccelerator(enable);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'EnableAccelerator'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: IsCtrlPress */
#ifndef TOLUA_DISABLE_tolua_UITolua_IsCtrlPress00
static int tolua_UITolua_IsCtrlPress00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int code = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  IsCtrlPress(code);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsCtrlPress'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: IsShiftPress */
#ifndef TOLUA_DISABLE_tolua_UITolua_IsShiftPress00
static int tolua_UITolua_IsShiftPress00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int code = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  IsShiftPress(code);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsShiftPress'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: IsAltPress */
#ifndef TOLUA_DISABLE_tolua_UITolua_IsAltPress00
static int tolua_UITolua_IsAltPress00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int code = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  IsAltPress(code);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsAltPress'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetControlKeyCode */
#ifndef TOLUA_DISABLE_tolua_UITolua_GetControlKeyCode00
static int tolua_UITolua_GetControlKeyCode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isboolean(tolua_S,1,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  bool ctrl = ((bool)  tolua_toboolean(tolua_S,1,0));
  bool shift = ((bool)  tolua_toboolean(tolua_S,2,0));
  bool alt = ((bool)  tolua_toboolean(tolua_S,3,0));
  {
   int tolua_ret = (int)  GetControlKeyCode(ctrl,shift,alt);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetControlKeyCode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: IsAlpha */
#ifndef TOLUA_DISABLE_tolua_UITolua_IsAlpha00
static int tolua_UITolua_IsAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  char c = ((char)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  IsAlpha(c);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsAlpha'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: IsDigit */
#ifndef TOLUA_DISABLE_tolua_UITolua_IsDigit00
static int tolua_UITolua_IsDigit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  char c = ((char)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  IsDigit(c);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsDigit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_UITolua_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_function(tolua_S,"IsPrint",tolua_UITolua_IsPrint00);
  tolua_function(tolua_S,"IsDBCSChar",tolua_UITolua_IsDBCSChar00);
  tolua_function(tolua_S,"IsDBCSHead",tolua_UITolua_IsDBCSHead00);
  tolua_function(tolua_S,"IsDBCSTail",tolua_UITolua_IsDBCSTail00);
  tolua_cclass(tolua_S,"UIObject","UIObject","",NULL);
  tolua_beginmodule(tolua_S,"UIObject");
   tolua_function(tolua_S,"SetName",tolua_UITolua_UIObject_SetName00);
   tolua_function(tolua_S,"GetName",tolua_UITolua_UIObject_GetName00);
  tolua_endmodule(tolua_S);
  tolua_constant(tolua_S,"DEFAULT_UI_WIDTH",DEFAULT_UI_WIDTH);
  tolua_constant(tolua_S,"DEFAULT_UI_HEIGHT",DEFAULT_UI_HEIGHT);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"tagRect_ToLua","tagRect_ToLua","",tolua_collect_tagRect_ToLua);
  #else
  tolua_cclass(tolua_S,"tagRect_ToLua","tagRect_ToLua","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"tagRect_ToLua");
   tolua_function(tolua_S,"new",tolua_UITolua_tagRect_ToLua_new00);
   tolua_function(tolua_S,"new_local",tolua_UITolua_tagRect_ToLua_new00_local);
   tolua_function(tolua_S,".call",tolua_UITolua_tagRect_ToLua_new00_local);
   tolua_function(tolua_S,"empty",tolua_UITolua_tagRect_ToLua_empty00);
   tolua_function(tolua_S,"getWidth",tolua_UITolua_tagRect_ToLua_getWidth00);
   tolua_function(tolua_S,"getHeight",tolua_UITolua_tagRect_ToLua_getHeight00);
   tolua_variable(tolua_S,"m_nLeft",tolua_get_tagRect_ToLua_m_nLeft,tolua_set_tagRect_ToLua_m_nLeft);
   tolua_variable(tolua_S,"m_nRight",tolua_get_tagRect_ToLua_m_nRight,tolua_set_tagRect_ToLua_m_nRight);
   tolua_variable(tolua_S,"m_nTop",tolua_get_tagRect_ToLua_m_nTop,tolua_set_tagRect_ToLua_m_nTop);
   tolua_variable(tolua_S,"m_nBottom",tolua_get_tagRect_ToLua_m_nBottom,tolua_set_tagRect_ToLua_m_nBottom);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"LayoutFrame","LayoutFrame","UIObject",NULL);
  tolua_beginmodule(tolua_S,"LayoutFrame");
   tolua_function(tolua_S,"Show",tolua_UITolua_LayoutFrame_Show00);
   tolua_function(tolua_S,"Active",tolua_UITolua_LayoutFrame_Active00);
   tolua_function(tolua_S,"Hide",tolua_UITolua_LayoutFrame_Hide00);
   tolua_function(tolua_S,"IsShown",tolua_UITolua_LayoutFrame_IsShown00);
   tolua_function(tolua_S,"GetAnchorOffsetX",tolua_UITolua_LayoutFrame_GetAnchorOffsetX00);
   tolua_function(tolua_S,"GetAnchorOffsetY",tolua_UITolua_LayoutFrame_GetAnchorOffsetY00);
   tolua_function(tolua_S,"removeRelFrames",tolua_UITolua_LayoutFrame_removeRelFrames00);
   tolua_function(tolua_S,"SetPoint",tolua_UITolua_LayoutFrame_SetPoint00);
   tolua_function(tolua_S,"SetPointEx",tolua_UITolua_LayoutFrame_SetPointEx00);
   tolua_function(tolua_S,"SetPoint",tolua_UITolua_LayoutFrame_SetPoint01);
   tolua_function(tolua_S,"SetPointAlwaysRecalc",tolua_UITolua_LayoutFrame_SetPointAlwaysRecalc00);
   tolua_function(tolua_S,"GetLeft",tolua_UITolua_LayoutFrame_GetLeft00);
   tolua_function(tolua_S,"GetRight",tolua_UITolua_LayoutFrame_GetRight00);
   tolua_function(tolua_S,"GetTop",tolua_UITolua_LayoutFrame_GetTop00);
   tolua_function(tolua_S,"GetBottom",tolua_UITolua_LayoutFrame_GetBottom00);
   tolua_function(tolua_S,"GetWidth",tolua_UITolua_LayoutFrame_GetWidth00);
   tolua_function(tolua_S,"SetWidth",tolua_UITolua_LayoutFrame_SetWidth00);
   tolua_function(tolua_S,"SetHeight",tolua_UITolua_LayoutFrame_SetHeight00);
   tolua_function(tolua_S,"GetHeight",tolua_UITolua_LayoutFrame_GetHeight00);
   tolua_function(tolua_S,"GetRealHeight",tolua_UITolua_LayoutFrame_GetRealHeight00);
   tolua_function(tolua_S,"GetRealWidth",tolua_UITolua_LayoutFrame_GetRealWidth00);
   tolua_function(tolua_S,"GetRealLeft",tolua_UITolua_LayoutFrame_GetRealLeft00);
   tolua_function(tolua_S,"GetRealRight",tolua_UITolua_LayoutFrame_GetRealRight00);
   tolua_function(tolua_S,"GetRealTop",tolua_UITolua_LayoutFrame_GetRealTop00);
   tolua_function(tolua_S,"GetRealBottom",tolua_UITolua_LayoutFrame_GetRealBottom00);
   tolua_function(tolua_S,"setAbsRect",tolua_UITolua_LayoutFrame_setAbsRect00);
   tolua_function(tolua_S,"resizeRect",tolua_UITolua_LayoutFrame_resizeRect00);
   tolua_function(tolua_S,"extendRect",tolua_UITolua_LayoutFrame_extendRect00);
   tolua_function(tolua_S,"GetParent",tolua_UITolua_LayoutFrame_GetParent00);
   tolua_function(tolua_S,"SetSizeNoRecal",tolua_UITolua_LayoutFrame_SetSizeNoRecal00);
   tolua_function(tolua_S,"SetSize",tolua_UITolua_LayoutFrame_SetSize00);
   tolua_function(tolua_S,"GetTextExtentWidth",tolua_UITolua_LayoutFrame_GetTextExtentWidth00);
   tolua_function(tolua_S,"GetTextExtentHeight",tolua_UITolua_LayoutFrame_GetTextExtentHeight00);
   tolua_function(tolua_S,"GetClientID",tolua_UITolua_LayoutFrame_GetClientID00);
   tolua_function(tolua_S,"SetClientID",tolua_UITolua_LayoutFrame_SetClientID00);
   tolua_function(tolua_S,"SetClientUserData",tolua_UITolua_LayoutFrame_SetClientUserData00);
   tolua_function(tolua_S,"GetClientUserData",tolua_UITolua_LayoutFrame_GetClientUserData00);
   tolua_function(tolua_S,"SetClientString",tolua_UITolua_LayoutFrame_SetClientString00);
   tolua_function(tolua_S,"GetClientString",tolua_UITolua_LayoutFrame_GetClientString00);
   tolua_function(tolua_S,"getFrameDrawLevel",tolua_UITolua_LayoutFrame_getFrameDrawLevel00);
   tolua_function(tolua_S,"setInputTransparent",tolua_UITolua_LayoutFrame_setInputTransparent00);
   tolua_function(tolua_S,"AddRelFrame",tolua_UITolua_LayoutFrame_AddRelFrame00);
   tolua_function(tolua_S,"SetSelfScale",tolua_UITolua_LayoutFrame_SetSelfScale00);
   tolua_function(tolua_S,"GetSelfScale",tolua_UITolua_LayoutFrame_GetSelfScale00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"FontInstance","FontInstance","",NULL);
  tolua_beginmodule(tolua_S,"FontInstance");
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"FontString","FontString","LayoutFrame",NULL);
  tolua_beginmodule(tolua_S,"FontString");
   tolua_function(tolua_S,"GetText",tolua_UITolua_FontString_GetText00);
   tolua_function(tolua_S,"SetText",tolua_UITolua_FontString_SetText00);
   tolua_function(tolua_S,"SetBufferTimer",tolua_UITolua_FontString_SetBufferTimer00);
   tolua_function(tolua_S,"SetTextColor",tolua_UITolua_FontString_SetTextColor00);
   tolua_function(tolua_S,"SetBlendAlpha",tolua_UITolua_FontString_SetBlendAlpha00);
   tolua_function(tolua_S,"SetScrollNumber",tolua_UITolua_FontString_SetScrollNumber00);
   tolua_function(tolua_S,"SetScrollNumberWithUint",tolua_UITolua_FontString_SetScrollNumberWithUint00);
   tolua_function(tolua_S,"GetBuffStart",tolua_UITolua_FontString_GetBuffStart00);
   tolua_function(tolua_S,"GetBuffTotal",tolua_UITolua_FontString_GetBuffTotal00);
   tolua_function(tolua_S,"GetTextExtentWidth",tolua_UITolua_FontString_GetTextExtentWidth00);
   tolua_variable(tolua_S,"__FontInstance__",tolua_get_FontString___FontInstance__,NULL);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Texture","Texture","LayoutFrame",NULL);
  tolua_beginmodule(tolua_S,"Texture");
   tolua_function(tolua_S,"SetUVAnimation",tolua_UITolua_Texture_SetUVAnimation00);
   tolua_function(tolua_S,"SetTextureHuires",tolua_UITolua_Texture_SetTextureHuires00);
   tolua_function(tolua_S,"GetTextureHuires",tolua_UITolua_Texture_GetTextureHuires00);
   tolua_function(tolua_S,"SetColor",tolua_UITolua_Texture_SetColor00);
   tolua_function(tolua_S,"SetGray",tolua_UITolua_Texture_SetGray00);
   tolua_function(tolua_S,"IsGray",tolua_UITolua_Texture_IsGray00);
   tolua_function(tolua_S,"SetTexture",tolua_UITolua_Texture_SetTexture00);
   tolua_function(tolua_S,"GetTexture",tolua_UITolua_Texture_GetTexture00);
   tolua_function(tolua_S,"SetAngle",tolua_UITolua_Texture_SetAngle00);
   tolua_function(tolua_S,"GetAngle",tolua_UITolua_Texture_GetAngle00);
   tolua_function(tolua_S,"SetTexUV",tolua_UITolua_Texture_SetTexUV00);
   tolua_function(tolua_S,"SetTexRelUV",tolua_UITolua_Texture_SetTexRelUV00);
   tolua_function(tolua_S,"SetTexRelUV",tolua_UITolua_Texture_SetTexRelUV01);
   tolua_function(tolua_S,"getRelWidth",tolua_UITolua_Texture_getRelWidth00);
   tolua_function(tolua_S,"getRelHeight",tolua_UITolua_Texture_getRelHeight00);
   tolua_function(tolua_S,"setTextOffsetY",tolua_UITolua_Texture_setTextOffsetY00);
   tolua_function(tolua_S,"setTextOffsetX",tolua_UITolua_Texture_setTextOffsetX00);
   tolua_function(tolua_S,"getTextOffsetY",tolua_UITolua_Texture_getTextOffsetY00);
   tolua_function(tolua_S,"getTextOffsetX",tolua_UITolua_Texture_getTextOffsetX00);
   tolua_function(tolua_S,"SetBlendAlpha",tolua_UITolua_Texture_SetBlendAlpha00);
   tolua_function(tolua_S,"GetBlendAlpha",tolua_UITolua_Texture_GetBlendAlpha00);
   tolua_function(tolua_S,"setMask",tolua_UITolua_Texture_setMask00);
   tolua_function(tolua_S,"setUvType",tolua_UITolua_Texture_setUvType00);
   tolua_function(tolua_S,"setRotateTopLeft",tolua_UITolua_Texture_setRotateTopLeft00);
   tolua_function(tolua_S,"ChangeTextureTemplate",tolua_UITolua_Texture_ChangeTextureTemplate00);
   tolua_function(tolua_S,"StopUVAnim",tolua_UITolua_Texture_StopUVAnim00);
   tolua_function(tolua_S,"StartAlphaAmin",tolua_UITolua_Texture_StartAlphaAmin00);
   tolua_function(tolua_S,"StopAlphaAmin",tolua_UITolua_Texture_StopAlphaAmin00);
   tolua_function(tolua_S,"IsPlayAlphaAnim",tolua_UITolua_Texture_IsPlayAlphaAnim00);
   tolua_variable(tolua_S,"__Ogre__LoadWrap__",tolua_get_Texture___Ogre__LoadWrap__,NULL);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"ModelView","ModelView","LayoutFrame",NULL);
  tolua_beginmodule(tolua_S,"ModelView");
   tolua_function(tolua_S,"setBackground",tolua_UITolua_ModelView_setBackground00);
   tolua_function(tolua_S,"addBackground",tolua_UITolua_ModelView_addBackground00);
   tolua_function(tolua_S,"setAvatarAttrib",tolua_UITolua_ModelView_setAvatarAttrib00);
   tolua_function(tolua_S,"getAvatarAttrib",tolua_UITolua_ModelView_getAvatarAttrib00);
   tolua_function(tolua_S,"setEntityAvatar",tolua_UITolua_ModelView_setEntityAvatar00);
   tolua_function(tolua_S,"isPlayingAnim",tolua_UITolua_ModelView_isPlayingAnim00);
   tolua_function(tolua_S,"setPetModel",tolua_UITolua_ModelView_setPetModel00);
   tolua_function(tolua_S,"setPetScale",tolua_UITolua_ModelView_setPetScale00);
   tolua_function(tolua_S,"setEntityPosition",tolua_UITolua_ModelView_setEntityPosition00);
   tolua_function(tolua_S,"setEntityRotate",tolua_UITolua_ModelView_setEntityRotate00);
   tolua_function(tolua_S,"setEntieyTextureByID",tolua_UITolua_ModelView_setEntieyTextureByID00);
   tolua_function(tolua_S,"playEntityAnim",tolua_UITolua_ModelView_playEntityAnim00);
   tolua_function(tolua_S,"isHaveCameraAnim",tolua_UITolua_ModelView_isHaveCameraAnim00);
   tolua_function(tolua_S,"setCameraAnim",tolua_UITolua_ModelView_setCameraAnim00);
   tolua_function(tolua_S,"playCameraAnim",tolua_UITolua_ModelView_playCameraAnim00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Frame","Frame","LayoutFrame",NULL);
  tolua_beginmodule(tolua_S,"Frame");
   tolua_function(tolua_S,"RegisterEvent",tolua_UITolua_Frame_RegisterEvent00);
   tolua_function(tolua_S,"SetFrameLevel",tolua_UITolua_Frame_SetFrameLevel00);
   tolua_function(tolua_S,"GetFrameLevel",tolua_UITolua_Frame_GetFrameLevel00);
   tolua_function(tolua_S,"SetClipState",tolua_UITolua_Frame_SetClipState00);
   tolua_function(tolua_S,"SetBlendAlpha",tolua_UITolua_Frame_SetBlendAlpha00);
   tolua_function(tolua_S,"setUpdateTime",tolua_UITolua_Frame_setUpdateTime00);
   tolua_function(tolua_S,"GetBackDropBlendAlpha",tolua_UITolua_Frame_GetBackDropBlendAlpha00);
   tolua_function(tolua_S,"SetBackDropBlendAlpha",tolua_UITolua_Frame_SetBackDropBlendAlpha00);
   tolua_function(tolua_S,"SetBackDropColor",tolua_UITolua_Frame_SetBackDropColor00);
   tolua_function(tolua_S,"MoveFrame",tolua_UITolua_Frame_MoveFrame00);
   tolua_function(tolua_S,"EndMoveFrame",tolua_UITolua_Frame_EndMoveFrame00);
   tolua_function(tolua_S,"SetBackDropBorderTex",tolua_UITolua_Frame_SetBackDropBorderTex00);
   tolua_function(tolua_S,"SetBackDropEdgeTex",tolua_UITolua_Frame_SetBackDropEdgeTex00);
   tolua_function(tolua_S,"SetBackDropAttr",tolua_UITolua_Frame_SetBackDropAttr00);
   tolua_function(tolua_S,"SetSelfScale",tolua_UITolua_Frame_SetSelfScale00);
   tolua_function(tolua_S,"AddLevelRecursive",tolua_UITolua_Frame_AddLevelRecursive00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Button","Button","Frame",NULL);
  tolua_beginmodule(tolua_S,"Button");
   tolua_function(tolua_S,"SetCooldownTimer",tolua_UITolua_Button_SetCooldownTimer00);
   tolua_function(tolua_S,"IsCooldown",tolua_UITolua_Button_IsCooldown00);
   tolua_function(tolua_S,"AddLock",tolua_UITolua_Button_AddLock00);
   tolua_function(tolua_S,"SetNormalRegionWidth",tolua_UITolua_Button_SetNormalRegionWidth00);
   tolua_function(tolua_S,"SetNormalRegionAbsWidth",tolua_UITolua_Button_SetNormalRegionAbsWidth00);
   tolua_function(tolua_S,"GetNormalRegionWidth",tolua_UITolua_Button_GetNormalRegionWidth00);
   tolua_function(tolua_S,"SetNormalRegionHight",tolua_UITolua_Button_SetNormalRegionHight00);
   tolua_function(tolua_S,"GetNormalRegionHight",tolua_UITolua_Button_GetNormalRegionHight00);
   tolua_function(tolua_S,"SetOverlayRegionAbsWidth",tolua_UITolua_Button_SetOverlayRegionAbsWidth00);
   tolua_function(tolua_S,"GetOverlayRegionWidth",tolua_UITolua_Button_GetOverlayRegionWidth00);
   tolua_function(tolua_S,"SetNormalTexture",tolua_UITolua_Button_SetNormalTexture00);
   tolua_function(tolua_S,"SetPushedTexture",tolua_UITolua_Button_SetPushedTexture00);
   tolua_function(tolua_S,"SetDisableTexture",tolua_UITolua_Button_SetDisableTexture00);
   tolua_function(tolua_S,"GetNormalTexture",tolua_UITolua_Button_GetNormalTexture00);
   tolua_function(tolua_S,"SetNormalTextureHuires",tolua_UITolua_Button_SetNormalTextureHuires00);
   tolua_function(tolua_S,"SetText",tolua_UITolua_Button_SetText00);
   tolua_function(tolua_S,"GetText",tolua_UITolua_Button_GetText00);
   tolua_function(tolua_S,"SetTextColor",tolua_UITolua_Button_SetTextColor00);
   tolua_function(tolua_S,"SetIntonateTimer",tolua_UITolua_Button_SetIntonateTimer00);
   tolua_function(tolua_S,"SetHightlightTexture",tolua_UITolua_Button_SetHightlightTexture00);
   tolua_function(tolua_S,"SetDisableTextureHuires",tolua_UITolua_Button_SetDisableTextureHuires00);
   tolua_function(tolua_S,"GetDisableTextureHuires",tolua_UITolua_Button_GetDisableTextureHuires00);
   tolua_function(tolua_S,"SetHighlightTextureHuires",tolua_UITolua_Button_SetHighlightTextureHuires00);
   tolua_function(tolua_S,"GetHighlightTextureHuires",tolua_UITolua_Button_GetHighlightTextureHuires00);
   tolua_function(tolua_S,"SetBlendAlpha",tolua_UITolua_Button_SetBlendAlpha00);
   tolua_function(tolua_S,"SetGray",tolua_UITolua_Button_SetGray00);
   tolua_function(tolua_S,"SetGrayEx",tolua_UITolua_Button_SetGrayEx00);
   tolua_function(tolua_S,"SetPushedState",tolua_UITolua_Button_SetPushedState00);
   tolua_function(tolua_S,"GetPushedState",tolua_UITolua_Button_GetPushedState00);
   tolua_function(tolua_S,"Disable",tolua_UITolua_Button_Disable00);
   tolua_function(tolua_S,"Enable",tolua_UITolua_Button_Enable00);
   tolua_function(tolua_S,"IsEnable",tolua_UITolua_Button_IsEnable00);
   tolua_function(tolua_S,"Highlight",tolua_UITolua_Button_Highlight00);
   tolua_function(tolua_S,"DisHighlight",tolua_UITolua_Button_DisHighlight00);
   tolua_function(tolua_S,"IsHighlight",tolua_UITolua_Button_IsHighlight00);
   tolua_function(tolua_S,"Checked",tolua_UITolua_Button_Checked00);
   tolua_function(tolua_S,"DisChecked",tolua_UITolua_Button_DisChecked00);
   tolua_function(tolua_S,"IsChecked",tolua_UITolua_Button_IsChecked00);
   tolua_function(tolua_S,"OnLButtonDown",tolua_UITolua_Button_OnLButtonDown00);
   tolua_function(tolua_S,"OnLButtonUp",tolua_UITolua_Button_OnLButtonUp00);
   tolua_function(tolua_S,"SwapCooldownMembers",tolua_UITolua_Button_SwapCooldownMembers00);
   tolua_function(tolua_S,"CopyCooldownMembers",tolua_UITolua_Button_CopyCooldownMembers00);
   tolua_function(tolua_S,"SetCooldownTextureRect",tolua_UITolua_Button_SetCooldownTextureRect00);
   tolua_function(tolua_S,"SetAngle",tolua_UITolua_Button_SetAngle00);
   tolua_function(tolua_S,"GetAngle",tolua_UITolua_Button_GetAngle00);
   tolua_function(tolua_S,"SetNormalTextureSize",tolua_UITolua_Button_SetNormalTextureSize00);
   tolua_function(tolua_S,"ChangeNormalTexture",tolua_UITolua_Button_ChangeNormalTexture00);
   tolua_function(tolua_S,"SetNormalTextureRelUV",tolua_UITolua_Button_SetNormalTextureRelUV00);
   tolua_function(tolua_S,"ChangeHighlightTexture",tolua_UITolua_Button_ChangeHighlightTexture00);
   tolua_function(tolua_S,"ChangePushedTexture",tolua_UITolua_Button_ChangePushedTexture00);
   tolua_function(tolua_S,"ChangeDisableTexture",tolua_UITolua_Button_ChangeDisableTexture00);
   tolua_function(tolua_S,"SetOverlayRegion",tolua_UITolua_Button_SetOverlayRegion00);
   tolua_function(tolua_S,"SetNormalTextureTemplate",tolua_UITolua_Button_SetNormalTextureTemplate00);
   tolua_function(tolua_S,"SetHighlightTextureTemplate",tolua_UITolua_Button_SetHighlightTextureTemplate00);
   tolua_function(tolua_S,"SetPushedTextureTemplate",tolua_UITolua_Button_SetPushedTextureTemplate00);
   tolua_function(tolua_S,"SetDisableTextureTemplate",tolua_UITolua_Button_SetDisableTextureTemplate00);
   tolua_function(tolua_S,"GetCoolStart",tolua_UITolua_Button_GetCoolStart00);
   tolua_function(tolua_S,"GetCoolTotal",tolua_UITolua_Button_GetCoolTotal00);
   tolua_function(tolua_S,"ClearPushState",tolua_UITolua_Button_ClearPushState00);
   tolua_function(tolua_S,"SetNormalTextColor",tolua_UITolua_Button_SetNormalTextColor00);
   tolua_function(tolua_S,"SetPushedTextColor",tolua_UITolua_Button_SetPushedTextColor00);
   tolua_function(tolua_S,"SetHighlightTextColor",tolua_UITolua_Button_SetHighlightTextColor00);
   tolua_function(tolua_S,"SetDisableTextColor",tolua_UITolua_Button_SetDisableTextColor00);
   tolua_function(tolua_S,"SetButtonColor",tolua_UITolua_Button_SetButtonColor00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"CheckButton","CheckButton","Button",NULL);
  tolua_beginmodule(tolua_S,"CheckButton");
   tolua_function(tolua_S,"GetCheckState",tolua_UITolua_CheckButton_GetCheckState00);
   tolua_function(tolua_S,"setCheckState",tolua_UITolua_CheckButton_setCheckState00);
  tolua_endmodule(tolua_S);
  tolua_constant(tolua_S,"EDIT_EVENT_CHAR",EDIT_EVENT_CHAR);
  tolua_constant(tolua_S,"EDIT_EVENT_KEY",EDIT_EVENT_KEY);
  tolua_cclass(tolua_S,"EditBox","EditBox","Frame",NULL);
  tolua_beginmodule(tolua_S,"EditBox");
   tolua_function(tolua_S,"GetSelctTexLen",tolua_UITolua_EditBox_GetSelctTexLen00);
   tolua_function(tolua_S,"IsAnyTextSelect",tolua_UITolua_EditBox_IsAnyTextSelect00);
   tolua_function(tolua_S,"ReplaceSelectText",tolua_UITolua_EditBox_ReplaceSelectText00);
   tolua_function(tolua_S,"Clear",tolua_UITolua_EditBox_Clear00);
   tolua_function(tolua_S,"SetText",tolua_UITolua_EditBox_SetText00);
   tolua_function(tolua_S,"AddText",tolua_UITolua_EditBox_AddText00);
   tolua_function(tolua_S,"GetText",tolua_UITolua_EditBox_GetText00);
   tolua_function(tolua_S,"GetPassWord",tolua_UITolua_EditBox_GetPassWord00);
   tolua_function(tolua_S,"SetTextColor",tolua_UITolua_EditBox_SetTextColor00);
   tolua_function(tolua_S,"AddStringToHistory",tolua_UITolua_EditBox_AddStringToHistory00);
   tolua_function(tolua_S,"clearHistory",tolua_UITolua_EditBox_clearHistory00);
   tolua_function(tolua_S,"SelectAllText",tolua_UITolua_EditBox_SelectAllText00);
   tolua_function(tolua_S,"enableIME",tolua_UITolua_EditBox_enableIME00);
   tolua_function(tolua_S,"SetPassword",tolua_UITolua_EditBox_SetPassword00);
   tolua_function(tolua_S,"enableEdit",tolua_UITolua_EditBox_enableEdit00);
   tolua_function(tolua_S,"GetTextExtentWidth",tolua_UITolua_EditBox_GetTextExtentWidth00);
   tolua_function(tolua_S,"setMaxChar",tolua_UITolua_EditBox_setMaxChar00);
   tolua_function(tolua_S,"getCursorPos",tolua_UITolua_EditBox_getCursorPos00);
   tolua_function(tolua_S,"setCursorPos",tolua_UITolua_EditBox_setCursorPos00);
   tolua_function(tolua_S,"getSelBegin",tolua_UITolua_EditBox_getSelBegin00);
   tolua_function(tolua_S,"setSelBegin",tolua_UITolua_EditBox_setSelBegin00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"RichText","RichText","Frame",NULL);
  tolua_beginmodule(tolua_S,"RichText");
   tolua_function(tolua_S,"ReplacePartialText",tolua_UITolua_RichText_ReplacePartialText00);
   tolua_function(tolua_S,"setTwoFaceInterval",tolua_UITolua_RichText_setTwoFaceInterval00);
   tolua_function(tolua_S,"getLinkTextRect",tolua_UITolua_RichText_getLinkTextRect00);
   tolua_function(tolua_S,"getFaceWidth",tolua_UITolua_RichText_getFaceWidth00);
   tolua_function(tolua_S,"getFaceHeight",tolua_UITolua_RichText_getFaceHeight00);
   tolua_function(tolua_S,"SetFaceTexture",tolua_UITolua_RichText_SetFaceTexture00);
   tolua_function(tolua_S,"SetFaceTexUV",tolua_UITolua_RichText_SetFaceTexUV00);
   tolua_function(tolua_S,"SetFaceTexRelUV",tolua_UITolua_RichText_SetFaceTexRelUV00);
   tolua_function(tolua_S,"SetAlpha",tolua_UITolua_RichText_SetAlpha00);
   tolua_function(tolua_S,"GetAlpha",tolua_UITolua_RichText_GetAlpha00);
   tolua_function(tolua_S,"resizeRect",tolua_UITolua_RichText_resizeRect00);
   tolua_function(tolua_S,"resizeRichWidth",tolua_UITolua_RichText_resizeRichWidth00);
   tolua_function(tolua_S,"resizeRichHeight",tolua_UITolua_RichText_resizeRichHeight00);
   tolua_function(tolua_S,"resizeText",tolua_UITolua_RichText_resizeText00);
   tolua_function(tolua_S,"AddText",tolua_UITolua_RichText_AddText00);
   tolua_function(tolua_S,"SetText",tolua_UITolua_RichText_SetText00);
   tolua_function(tolua_S,"SetAutoExtend",tolua_UITolua_RichText_SetAutoExtend00);
   tolua_function(tolua_S,"SetLinkTextColor",tolua_UITolua_RichText_SetLinkTextColor00);
   tolua_function(tolua_S,"SetShadowColor",tolua_UITolua_RichText_SetShadowColor00);
   tolua_function(tolua_S,"SetFontType",tolua_UITolua_RichText_SetFontType00);
   tolua_function(tolua_S,"Clear",tolua_UITolua_RichText_Clear00);
   tolua_function(tolua_S,"clearHistory",tolua_UITolua_RichText_clearHistory00);
   tolua_function(tolua_S,"SetDispPos",tolua_UITolua_RichText_SetDispPos00);
   tolua_function(tolua_S,"IncrDispPos",tolua_UITolua_RichText_IncrDispPos00);
   tolua_function(tolua_S,"GetDispPos",tolua_UITolua_RichText_GetDispPos00);
   tolua_function(tolua_S,"SetDispPosX",tolua_UITolua_RichText_SetDispPosX00);
   tolua_function(tolua_S,"GetDispPosX",tolua_UITolua_RichText_GetDispPosX00);
   tolua_function(tolua_S,"GetStartDispPos",tolua_UITolua_RichText_GetStartDispPos00);
   tolua_function(tolua_S,"GetEndDispPos",tolua_UITolua_RichText_GetEndDispPos00);
   tolua_function(tolua_S,"ScrollUp",tolua_UITolua_RichText_ScrollUp00);
   tolua_function(tolua_S,"ScrollDown",tolua_UITolua_RichText_ScrollDown00);
   tolua_function(tolua_S,"ScrollFirst",tolua_UITolua_RichText_ScrollFirst00);
   tolua_function(tolua_S,"ScrollEnd",tolua_UITolua_RichText_ScrollEnd00);
   tolua_function(tolua_S,"GetTextLines",tolua_UITolua_RichText_GetTextLines00);
   tolua_function(tolua_S,"getLineRealWidth",tolua_UITolua_RichText_getLineRealWidth00);
   tolua_function(tolua_S,"getLineWidth",tolua_UITolua_RichText_getLineWidth00);
   tolua_function(tolua_S,"GetLineWidth",tolua_UITolua_RichText_GetLineWidth00);
   tolua_function(tolua_S,"GetLineHeight",tolua_UITolua_RichText_GetLineHeight00);
   tolua_function(tolua_S,"GetTotalHeight",tolua_UITolua_RichText_GetTotalHeight00);
   tolua_function(tolua_S,"GetViewLines",tolua_UITolua_RichText_GetViewLines00);
   tolua_function(tolua_S,"GetAccurateViewLines",tolua_UITolua_RichText_GetAccurateViewLines00);
   tolua_function(tolua_S,"setAbsRect",tolua_UITolua_RichText_setAbsRect00);
   tolua_function(tolua_S,"GetTextExtentWidth",tolua_UITolua_RichText_GetTextExtentWidth00);
   tolua_function(tolua_S,"GetTextExtentHeight",tolua_UITolua_RichText_GetTextExtentHeight00);
   tolua_function(tolua_S,"setCenterLine",tolua_UITolua_RichText_setCenterLine00);
   tolua_function(tolua_S,"setCursorNormal",tolua_UITolua_RichText_setCursorNormal00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"ScrollFrame","ScrollFrame","Frame",NULL);
  tolua_beginmodule(tolua_S,"ScrollFrame");
   tolua_function(tolua_S,"CalVerticalScrollRange",tolua_UITolua_ScrollFrame_CalVerticalScrollRange00);
   tolua_function(tolua_S,"CalHorizonalScrollRange",tolua_UITolua_ScrollFrame_CalHorizonalScrollRange00);
   tolua_function(tolua_S,"SetValueStep",tolua_UITolua_ScrollFrame_SetValueStep00);
   tolua_function(tolua_S,"GetValueStep",tolua_UITolua_ScrollFrame_GetValueStep00);
   tolua_function(tolua_S,"SetScrollChildRect",tolua_UITolua_ScrollFrame_SetScrollChildRect00);
   tolua_function(tolua_S,"IncreaseScrollChildRect",tolua_UITolua_ScrollFrame_IncreaseScrollChildRect00);
   tolua_function(tolua_S,"SetVerticalScroll",tolua_UITolua_ScrollFrame_SetVerticalScroll00);
   tolua_function(tolua_S,"IncreaseVerticalScroll",tolua_UITolua_ScrollFrame_IncreaseVerticalScroll00);
   tolua_function(tolua_S,"GetVerticalOffset",tolua_UITolua_ScrollFrame_GetVerticalOffset00);
   tolua_function(tolua_S,"SetVerticalScrollRange",tolua_UITolua_ScrollFrame_SetVerticalScrollRange00);
   tolua_function(tolua_S,"GetVerticalScrollRange",tolua_UITolua_ScrollFrame_GetVerticalScrollRange00);
   tolua_function(tolua_S,"SetHorizonalScroll",tolua_UITolua_ScrollFrame_SetHorizonalScroll00);
   tolua_function(tolua_S,"IncreaseHorizonalScroll",tolua_UITolua_ScrollFrame_IncreaseHorizonalScroll00);
   tolua_function(tolua_S,"GetHorizonalOffset",tolua_UITolua_ScrollFrame_GetHorizonalOffset00);
   tolua_function(tolua_S,"SetHorizonalScrollRange",tolua_UITolua_ScrollFrame_SetHorizonalScrollRange00);
   tolua_function(tolua_S,"GetHorizonalScrollRange",tolua_UITolua_ScrollFrame_GetHorizonalScrollRange00);
   tolua_function(tolua_S,"ResetChildAnchor",tolua_UITolua_ScrollFrame_ResetChildAnchor00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Slider","Slider","Frame",NULL);
  tolua_beginmodule(tolua_S,"Slider");
   tolua_function(tolua_S,"SetValue",tolua_UITolua_Slider_SetValue00);
   tolua_function(tolua_S,"GetValue",tolua_UITolua_Slider_GetValue00);
   tolua_function(tolua_S,"GetLastValue",tolua_UITolua_Slider_GetLastValue00);
   tolua_function(tolua_S,"SetMinValue",tolua_UITolua_Slider_SetMinValue00);
   tolua_function(tolua_S,"GetMinValue",tolua_UITolua_Slider_GetMinValue00);
   tolua_function(tolua_S,"SetMaxValue",tolua_UITolua_Slider_SetMaxValue00);
   tolua_function(tolua_S,"GetMaxValue",tolua_UITolua_Slider_GetMaxValue00);
   tolua_function(tolua_S,"SetValueStep",tolua_UITolua_Slider_SetValueStep00);
   tolua_function(tolua_S,"GetValueStep",tolua_UITolua_Slider_GetValueStep00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"DrawLineFrame","DrawLineFrame","LayoutFrame",NULL);
  tolua_beginmodule(tolua_S,"DrawLineFrame");
   tolua_function(tolua_S,"SetDrawPointList",tolua_UITolua_DrawLineFrame_SetDrawPointList00);
   tolua_function(tolua_S,"ClearDrawPointList",tolua_UITolua_DrawLineFrame_ClearDrawPointList00);
   tolua_function(tolua_S,"AddPoint",tolua_UITolua_DrawLineFrame_AddPoint00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"MultiEditBox","MultiEditBox","Frame",NULL);
  tolua_beginmodule(tolua_S,"MultiEditBox");
   tolua_function(tolua_S,"SetSliderValue",tolua_UITolua_MultiEditBox_SetSliderValue00);
   tolua_function(tolua_S,"SetText",tolua_UITolua_MultiEditBox_SetText00);
   tolua_function(tolua_S,"AddText",tolua_UITolua_MultiEditBox_AddText00);
   tolua_function(tolua_S,"SetTextColor",tolua_UITolua_MultiEditBox_SetTextColor00);
   tolua_function(tolua_S,"GetText",tolua_UITolua_MultiEditBox_GetText00);
   tolua_function(tolua_S,"Clear",tolua_UITolua_MultiEditBox_Clear00);
   tolua_function(tolua_S,"enableIME",tolua_UITolua_MultiEditBox_enableIME00);
   tolua_function(tolua_S,"getTextCount",tolua_UITolua_MultiEditBox_getTextCount00);
   tolua_function(tolua_S,"setMaxWordCount",tolua_UITolua_MultiEditBox_setMaxWordCount00);
   tolua_function(tolua_S,"SelectAllText",tolua_UITolua_MultiEditBox_SelectAllText00);
   tolua_function(tolua_S,"enableEdit",tolua_UITolua_MultiEditBox_enableEdit00);
   tolua_function(tolua_S,"getCursorPos",tolua_UITolua_MultiEditBox_getCursorPos00);
   tolua_function(tolua_S,"setCursorPos",tolua_UITolua_MultiEditBox_setCursorPos00);
   tolua_function(tolua_S,"getSelBegin",tolua_UITolua_MultiEditBox_getSelBegin00);
   tolua_function(tolua_S,"setSelBegin",tolua_UITolua_MultiEditBox_setSelBegin00);
   tolua_function(tolua_S,"getTextBegin",tolua_UITolua_MultiEditBox_getTextBegin00);
   tolua_function(tolua_S,"getTextEnd",tolua_UITolua_MultiEditBox_getTextEnd00);
   tolua_function(tolua_S,"getTextIndexFromRichCharIndex",tolua_UITolua_MultiEditBox_getTextIndexFromRichCharIndex00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"WebBrowerFrame","WebBrowerFrame","Frame",NULL);
  tolua_beginmodule(tolua_S,"WebBrowerFrame");
   tolua_function(tolua_S,"OpenSite",tolua_UITolua_WebBrowerFrame_OpenSite00);
   tolua_function(tolua_S,"ShowWindow",tolua_UITolua_WebBrowerFrame_ShowWindow00);
  tolua_endmodule(tolua_S);
  tolua_function(tolua_S,"ShowUIPanel",tolua_UITolua_ShowUIPanel00);
  tolua_function(tolua_S,"HideUIPanel",tolua_UITolua_HideUIPanel00);
  tolua_function(tolua_S,"UIBeginDrag",tolua_UITolua_UIBeginDrag00);
  tolua_function(tolua_S,"UIEndDrag",tolua_UITolua_UIEndDrag00);
  tolua_function(tolua_S,"UIIsInDragState",tolua_UITolua_UIIsInDragState00);
  tolua_function(tolua_S,"UIGetDragUserData1",tolua_UITolua_UIGetDragUserData100);
  tolua_function(tolua_S,"UIGetDragUserData2",tolua_UITolua_UIGetDragUserData200);
  tolua_function(tolua_S,"UIGetDragUserData3",tolua_UITolua_UIGetDragUserData300);
  tolua_function(tolua_S,"CreateButton",tolua_UITolua_CreateButton00);
  tolua_function(tolua_S,"FindFontString",tolua_UITolua_FindFontString00);
  tolua_function(tolua_S,"FindButton",tolua_UITolua_FindButton00);
  tolua_function(tolua_S,"isAlreadyExistFrame",tolua_UITolua_isAlreadyExistFrame00);
  tolua_function(tolua_S,"FindTexture",tolua_UITolua_FindTexture00);
  tolua_function(tolua_S,"closePopWin",tolua_UITolua_closePopWin00);
  tolua_function(tolua_S,"SetCurrentCursor",tolua_UITolua_SetCurrentCursor00);
  tolua_function(tolua_S,"ChangeCursorState",tolua_UITolua_ChangeCursorState00);
  tolua_function(tolua_S,"GetCurrentCursor",tolua_UITolua_GetCurrentCursor00);
  tolua_function(tolua_S,"GetCurrentCursorLevel",tolua_UITolua_GetCurrentCursorLevel00);
  tolua_function(tolua_S,"GetScreenWidth",tolua_UITolua_GetScreenWidth00);
  tolua_function(tolua_S,"GetScreenHeight",tolua_UITolua_GetScreenHeight00);
  tolua_function(tolua_S,"SetFocusFrame",tolua_UITolua_SetFocusFrame00);
  tolua_function(tolua_S,"SetFocusFrame",tolua_UITolua_SetFocusFrame01);
  tolua_function(tolua_S,"GetFocusFrameParent",tolua_UITolua_GetFocusFrameParent00);
  tolua_function(tolua_S,"ClearFocus",tolua_UITolua_ClearFocus00);
  tolua_function(tolua_S,"GetCursorPosX",tolua_UITolua_GetCursorPosX00);
  tolua_function(tolua_S,"GetCursorPosY",tolua_UITolua_GetCursorPosY00);
  tolua_function(tolua_S,"pushUnCloseWin",tolua_UITolua_pushUnCloseWin00);
  tolua_function(tolua_S,"GetCurrentDialogFrame",tolua_UITolua_GetCurrentDialogFrame00);
  tolua_function(tolua_S,"isPointInFrame",tolua_UITolua_isPointInFrame00);
  tolua_function(tolua_S,"isPointInFrame",tolua_UITolua_isPointInFrame01);
  tolua_function(tolua_S,"isTwoFrameXConflict",tolua_UITolua_isTwoFrameXConflict00);
  tolua_function(tolua_S,"CopyToMemory",tolua_UITolua_CopyToMemory00);
  tolua_function(tolua_S,"isFiliation",tolua_UITolua_isFiliation00);
  tolua_function(tolua_S,"addChangedFrames",tolua_UITolua_addChangedFrames00);
  tolua_function(tolua_S,"addChangedFrames",tolua_UITolua_addChangedFrames01);
  tolua_function(tolua_S,"playUISound",tolua_UITolua_playUISound00);
  tolua_function(tolua_S,"GetScreenScaleY",tolua_UITolua_GetScreenScaleY00);
  tolua_function(tolua_S,"GetScreenScaleX",tolua_UITolua_GetScreenScaleX00);
  tolua_function(tolua_S,"GetFontTextWidth",tolua_UITolua_GetFontTextWidth00);
  tolua_function(tolua_S,"IsInExistence",tolua_UITolua_IsInExistence00);
  tolua_function(tolua_S,"isInVisibleArea",tolua_UITolua_isInVisibleArea00);
  tolua_function(tolua_S,"isKeyPressed",tolua_UITolua_isKeyPressed00);
  tolua_function(tolua_S,"getRand",tolua_UITolua_getRand00);
  tolua_function(tolua_S,"GetFocusFrameName",tolua_UITolua_GetFocusFrameName00);
  tolua_function(tolua_S,"replaceFaceString",tolua_UITolua_replaceFaceString00);
  tolua_function(tolua_S,"BigInt2Str",tolua_UITolua_BigInt2Str00);
  tolua_function(tolua_S,"updateHeadBindingFrame",tolua_UITolua_updateHeadBindingFrame00);
  tolua_function(tolua_S,"showUIFrames",tolua_UITolua_showUIFrames00);
  tolua_cclass(tolua_S,"FrameManager","FrameManager","",NULL);
  tolua_beginmodule(tolua_S,"FrameManager");
   tolua_function(tolua_S,"FindLayoutFrame",tolua_UITolua_FrameManager_FindLayoutFrame00);
   tolua_function(tolua_S,"GetEditMode",tolua_UITolua_FrameManager_GetEditMode00);
   tolua_function(tolua_S,"EnableAccelerator",tolua_UITolua_FrameManager_EnableAccelerator00);
  tolua_endmodule(tolua_S);
  tolua_function(tolua_S,"IsCtrlPress",tolua_UITolua_IsCtrlPress00);
  tolua_function(tolua_S,"IsShiftPress",tolua_UITolua_IsShiftPress00);
  tolua_function(tolua_S,"IsAltPress",tolua_UITolua_IsAltPress00);
  tolua_function(tolua_S,"GetControlKeyCode",tolua_UITolua_GetControlKeyCode00);
  tolua_function(tolua_S,"IsAlpha",tolua_UITolua_IsAlpha00);
  tolua_function(tolua_S,"IsDigit",tolua_UITolua_IsDigit00);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_UITolua (lua_State* tolua_S) {
 return tolua_UITolua_open(tolua_S);
};
#endif

