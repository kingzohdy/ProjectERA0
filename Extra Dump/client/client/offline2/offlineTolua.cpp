/*
** Lua binding: offlineTolua
** Generated automatically by tolua++-1.0.92 on 07/11/14 20:49:06.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_offlineTolua_open (lua_State* tolua_S);

#include "OfflineManager.h"
#include "MudNetMgr.h"
#include "FlickerManager.h"
#include "OfflineChatManager.h"
#include "OfflineWatchPlayer.h"
#include "RelationManager.h"
#include "ChatRoomManager.h"
#include "OfflineMainPlayer.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_tagChatLinkData_ToLua (lua_State* tolua_S)
{
 tagChatLinkData_ToLua* self = (tagChatLinkData_ToLua*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_DirBigWorldInfo (lua_State* tolua_S)
{
 DirBigWorldInfo* self = (DirBigWorldInfo*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_tdr_longlong (lua_State* tolua_S)
{
 tdr_longlong* self = (tdr_longlong*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"MudNetMgr");
 tolua_usertype(tolua_S,"tagSecondarySkillLink_ToLua");
 tolua_usertype(tolua_S,"std::vector<DirWorldInfo>");
 tolua_usertype(tolua_S,"tagArmLink_ToLua");
 tolua_usertype(tolua_S,"tagPostionLink_ToLua");
 tolua_usertype(tolua_S,"tagRoleArm_ToLua");
 tolua_usertype(tolua_S,"DirWorldInfo");
 tolua_usertype(tolua_S,"tdr_ulonglong");
 tolua_usertype(tolua_S,"DirBigWorldInfo");
 tolua_usertype(tolua_S,"GameActor");
 tolua_usertype(tolua_S,"tagPetLink_ToLua");
 tolua_usertype(tolua_S,"tagLinkData_ToLua");
 tolua_usertype(tolua_S,"OfflinePlayer");
 tolua_usertype(tolua_S,"RelationManager");
 tolua_usertype(tolua_S,"OfflineWatchPlayer");
 tolua_usertype(tolua_S,"CSDIRWORLDINFO");
 tolua_usertype(tolua_S,"OfflineMainPlayer");
 tolua_usertype(tolua_S,"Item");
 tolua_usertype(tolua_S,"tagChatLinkData_ToLua");
 tolua_usertype(tolua_S,"ChatManager");
 tolua_usertype(tolua_S,"tagItemLink_ToLua");
 tolua_usertype(tolua_S,"tagChatLink_ToLua");
 tolua_usertype(tolua_S,"tagMissionLink_ToLua");
 tolua_usertype(tolua_S,"ChatRoomManager");
 tolua_usertype(tolua_S,"tdr_longlong");
 tolua_usertype(tolua_S,"OfflineManager");
}

/* method: bitAnd of class  OfflineManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineManager_bitAnd00
static int tolua_offlineTolua_OfflineManager_bitAnd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineManager* self = (OfflineManager*)  tolua_tousertype(tolua_S,1,0);
  int nLhs = ((int)  tolua_tonumber(tolua_S,2,0));
  int nRhs = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'bitAnd'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->bitAnd(nLhs,nRhs);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'bitAnd'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: end1 of class  OfflineManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineManager_end100
static int tolua_offlineTolua_OfflineManager_end100(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineManager* self = (OfflineManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'end1'",NULL);
#endif
  {
   self->end1();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'end1'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: end of class  OfflineManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineManager_end00
static int tolua_offlineTolua_OfflineManager_end00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineManager* self = (OfflineManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'end'",NULL);
#endif
  {
   self->end();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'end'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setsize of class  OfflineManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineManager_setsize00
static int tolua_offlineTolua_OfflineManager_setsize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineManager* self = (OfflineManager*)  tolua_tousertype(tolua_S,1,0);
  int i = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setsize'",NULL);
#endif
  {
   self->setsize(i);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setsize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: MoveWindowDown of class  OfflineManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineManager_MoveWindowDown00
static int tolua_offlineTolua_OfflineManager_MoveWindowDown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineManager* self = (OfflineManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'MoveWindowDown'",NULL);
#endif
  {
   self->MoveWindowDown();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MoveWindowDown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: MoveWindowUp of class  OfflineManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineManager_MoveWindowUp00
static int tolua_offlineTolua_OfflineManager_MoveWindowUp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineManager* self = (OfflineManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'MoveWindowUp'",NULL);
#endif
  {
   self->MoveWindowUp();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MoveWindowUp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendUIEvent of class  OfflineManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineManager_SendUIEvent00
static int tolua_offlineTolua_OfflineManager_SendUIEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineManager* self = (OfflineManager*)  tolua_tousertype(tolua_S,1,0);
  const char* pevent = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendUIEvent'",NULL);
#endif
  {
   self->SendUIEvent(pevent);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendUIEvent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: MiniWindow of class  OfflineManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineManager_MiniWindow00
static int tolua_offlineTolua_OfflineManager_MiniWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineManager* self = (OfflineManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'MiniWindow'",NULL);
#endif
  {
   self->MiniWindow();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MiniWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: saveusername of class  OfflineManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineManager_saveusername00
static int tolua_offlineTolua_OfflineManager_saveusername00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineManager* self = (OfflineManager*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'saveusername'",NULL);
#endif
  {
   self->saveusername(name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'saveusername'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clearusername of class  OfflineManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineManager_clearusername00
static int tolua_offlineTolua_OfflineManager_clearusername00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineManager* self = (OfflineManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clearusername'",NULL);
#endif
  {
   self->clearusername();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clearusername'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getlastname of class  OfflineManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineManager_getlastname00
static int tolua_offlineTolua_OfflineManager_getlastname00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineManager* self = (OfflineManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getlastname'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getlastname();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getlastname'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getlastusercheck of class  OfflineManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineManager_getlastusercheck00
static int tolua_offlineTolua_OfflineManager_getlastusercheck00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineManager* self = (OfflineManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getlastusercheck'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getlastusercheck();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getlastusercheck'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OpenUrl of class  OfflineManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineManager_OpenUrl00
static int tolua_offlineTolua_OfflineManager_OpenUrl00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineManager* self = (OfflineManager*)  tolua_tousertype(tolua_S,1,0);
  const char* url = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OpenUrl'",NULL);
#endif
  {
   self->OpenUrl(url);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OpenUrl'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: name of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_get_DirWorldInfo_name
static int tolua_get_DirWorldInfo_name(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'name'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: name of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_set_DirWorldInfo_name
static int tolua_set_DirWorldInfo_name(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'name'",NULL);
  if (!tolua_istable(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 strncpy(self->name,tolua_tostring(tolua_S,2,0),32-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: recommend of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_get_DirWorldInfo_recommend
static int tolua_get_DirWorldInfo_recommend(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'recommend'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->recommend);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: recommend of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_set_DirWorldInfo_recommend
static int tolua_set_DirWorldInfo_recommend(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'recommend'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->recommend = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: busy of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_get_DirWorldInfo_busy
static int tolua_get_DirWorldInfo_busy(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'busy'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->busy);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: busy of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_set_DirWorldInfo_busy
static int tolua_set_DirWorldInfo_busy(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'busy'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->busy = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ip of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_get_DirWorldInfo_ip
static int tolua_get_DirWorldInfo_ip(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ip'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->ip);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: ip of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_set_DirWorldInfo_ip
static int tolua_set_DirWorldInfo_ip(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ip'",NULL);
  if (!tolua_istable(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 strncpy(self->ip,tolua_tostring(tolua_S,2,0),48-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Ip_Bak1 of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_get_DirWorldInfo_Ip_Bak1
static int tolua_get_DirWorldInfo_Ip_Bak1(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Ip_Bak1'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->Ip_Bak1);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Ip_Bak1 of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_set_DirWorldInfo_Ip_Bak1
static int tolua_set_DirWorldInfo_Ip_Bak1(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Ip_Bak1'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Ip_Bak1 = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Ip_Bak2 of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_get_DirWorldInfo_Ip_Bak2
static int tolua_get_DirWorldInfo_Ip_Bak2(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Ip_Bak2'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->Ip_Bak2);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Ip_Bak2 of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_set_DirWorldInfo_Ip_Bak2
static int tolua_set_DirWorldInfo_Ip_Bak2(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Ip_Bak2'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Ip_Bak2 = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: port of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_get_DirWorldInfo_unsigned_port
static int tolua_get_DirWorldInfo_unsigned_port(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'port'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->port);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: port of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_set_DirWorldInfo_unsigned_port
static int tolua_set_DirWorldInfo_unsigned_port(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'port'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->port = ((unsigned short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: worldid of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_get_DirWorldInfo_unsigned_worldid
static int tolua_get_DirWorldInfo_unsigned_worldid(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'worldid'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->worldid);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: worldid of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_set_DirWorldInfo_unsigned_worldid
static int tolua_set_DirWorldInfo_unsigned_worldid(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'worldid'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->worldid = ((unsigned short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: RegionID of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_get_DirWorldInfo_unsigned_RegionID
static int tolua_get_DirWorldInfo_unsigned_RegionID(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'RegionID'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->RegionID);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: RegionID of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_set_DirWorldInfo_unsigned_RegionID
static int tolua_set_DirWorldInfo_unsigned_RegionID(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'RegionID'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->RegionID = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: AuthIP of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_get_DirWorldInfo_AuthIP
static int tolua_get_DirWorldInfo_AuthIP(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AuthIP'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->AuthIP);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: AuthIP of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_set_DirWorldInfo_AuthIP
static int tolua_set_DirWorldInfo_AuthIP(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AuthIP'",NULL);
  if (!tolua_istable(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 strncpy(self->AuthIP,tolua_tostring(tolua_S,2,0),48-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: authIP_Bak1 of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_get_DirWorldInfo_authIP_Bak1
static int tolua_get_DirWorldInfo_authIP_Bak1(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'authIP_Bak1'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->authIP_Bak1);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: authIP_Bak1 of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_set_DirWorldInfo_authIP_Bak1
static int tolua_set_DirWorldInfo_authIP_Bak1(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'authIP_Bak1'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->authIP_Bak1 = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: authIP_Bak2 of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_get_DirWorldInfo_authIP_Bak2
static int tolua_get_DirWorldInfo_authIP_Bak2(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'authIP_Bak2'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->authIP_Bak2);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: authIP_Bak2 of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_set_DirWorldInfo_authIP_Bak2
static int tolua_set_DirWorldInfo_authIP_Bak2(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'authIP_Bak2'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->authIP_Bak2 = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: AuthPort of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_get_DirWorldInfo_unsigned_AuthPort
static int tolua_get_DirWorldInfo_unsigned_AuthPort(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AuthPort'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->AuthPort);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: AuthPort of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_set_DirWorldInfo_unsigned_AuthPort
static int tolua_set_DirWorldInfo_unsigned_AuthPort(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AuthPort'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->AuthPort = ((unsigned short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mudIp of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_get_DirWorldInfo_mudIp
static int tolua_get_DirWorldInfo_mudIp(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mudIp'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->mudIp);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: mudIp of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_set_DirWorldInfo_mudIp
static int tolua_set_DirWorldInfo_mudIp(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mudIp'",NULL);
  if (!tolua_istable(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 strncpy(self->mudIp,tolua_tostring(tolua_S,2,0),48-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mudPort of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_get_DirWorldInfo_unsigned_mudPort
static int tolua_get_DirWorldInfo_unsigned_mudPort(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mudPort'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->mudPort);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: mudPort of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_set_DirWorldInfo_unsigned_mudPort
static int tolua_set_DirWorldInfo_unsigned_mudPort(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mudPort'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->mudPort = ((unsigned short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: MudIp_Bak1 of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_get_DirWorldInfo_MudIp_Bak1
static int tolua_get_DirWorldInfo_MudIp_Bak1(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'MudIp_Bak1'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->MudIp_Bak1);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: MudIp_Bak1 of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_set_DirWorldInfo_MudIp_Bak1
static int tolua_set_DirWorldInfo_MudIp_Bak1(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'MudIp_Bak1'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->MudIp_Bak1 = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: MudIp_Bak2 of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_get_DirWorldInfo_MudIp_Bak2
static int tolua_get_DirWorldInfo_MudIp_Bak2(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'MudIp_Bak2'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->MudIp_Bak2);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: MudIp_Bak2 of class  DirWorldInfo */
#ifndef TOLUA_DISABLE_tolua_set_DirWorldInfo_MudIp_Bak2
static int tolua_set_DirWorldInfo_MudIp_Bak2(lua_State* tolua_S)
{
  DirWorldInfo* self = (DirWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'MudIp_Bak2'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->MudIp_Bak2 = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  DirBigWorldInfo */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_DirBigWorldInfo_new00
static int tolua_offlineTolua_DirBigWorldInfo_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"DirBigWorldInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   DirBigWorldInfo* tolua_ret = (DirBigWorldInfo*)  new DirBigWorldInfo();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"DirBigWorldInfo");
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

/* method: new_local of class  DirBigWorldInfo */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_DirBigWorldInfo_new00_local
static int tolua_offlineTolua_DirBigWorldInfo_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"DirBigWorldInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   DirBigWorldInfo* tolua_ret = (DirBigWorldInfo*)  new DirBigWorldInfo();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"DirBigWorldInfo");
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

/* method: getName of class  DirBigWorldInfo */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_DirBigWorldInfo_getName00
static int tolua_offlineTolua_DirBigWorldInfo_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"DirBigWorldInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  DirBigWorldInfo* self = (DirBigWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getName'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getName();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getNumWorld of class  DirBigWorldInfo */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_DirBigWorldInfo_getNumWorld00
static int tolua_offlineTolua_DirBigWorldInfo_getNumWorld00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"DirBigWorldInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  DirBigWorldInfo* self = (DirBigWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getNumWorld'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getNumWorld();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getNumWorld'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFlag of class  DirBigWorldInfo */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_DirBigWorldInfo_getFlag00
static int tolua_offlineTolua_DirBigWorldInfo_getFlag00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"DirBigWorldInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  DirBigWorldInfo* self = (DirBigWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFlag'",NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getFlag();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFlag'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getIthWorld of class  DirBigWorldInfo */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_DirBigWorldInfo_getIthWorld00
static int tolua_offlineTolua_DirBigWorldInfo_getIthWorld00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"DirBigWorldInfo",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  DirBigWorldInfo* self = (DirBigWorldInfo*)  tolua_tousertype(tolua_S,1,0);
  int i = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getIthWorld'",NULL);
#endif
  {
   DirWorldInfo& tolua_ret = (DirWorldInfo&)  self->getIthWorld(i);
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"DirWorldInfo");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getIthWorld'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addWorldInfo of class  DirBigWorldInfo */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_DirBigWorldInfo_addWorldInfo00
static int tolua_offlineTolua_DirBigWorldInfo_addWorldInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"DirBigWorldInfo",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const CSDIRWORLDINFO",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  DirBigWorldInfo* self = (DirBigWorldInfo*)  tolua_tousertype(tolua_S,1,0);
  const CSDIRWORLDINFO* srcinfo = ((const CSDIRWORLDINFO*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addWorldInfo'",NULL);
#endif
  {
   self->addWorldInfo(*srcinfo);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addWorldInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addWorldInfo of class  DirBigWorldInfo */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_DirBigWorldInfo_addWorldInfo01
static int tolua_offlineTolua_DirBigWorldInfo_addWorldInfo01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"DirBigWorldInfo",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const DirWorldInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  DirBigWorldInfo* self = (DirBigWorldInfo*)  tolua_tousertype(tolua_S,1,0);
  const DirWorldInfo* src = ((const DirWorldInfo*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addWorldInfo'",NULL);
#endif
  {
   self->addWorldInfo(*src);
  }
 }
 return 0;
tolua_lerror:
 return tolua_offlineTolua_DirBigWorldInfo_addWorldInfo00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_Name of class  DirBigWorldInfo */
#ifndef TOLUA_DISABLE_tolua_get_DirBigWorldInfo_m_Name
static int tolua_get_DirBigWorldInfo_m_Name(lua_State* tolua_S)
{
  DirBigWorldInfo* self = (DirBigWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Name'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->m_Name);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_Name of class  DirBigWorldInfo */
#ifndef TOLUA_DISABLE_tolua_set_DirBigWorldInfo_m_Name
static int tolua_set_DirBigWorldInfo_m_Name(lua_State* tolua_S)
{
  DirBigWorldInfo* self = (DirBigWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Name'",NULL);
  if (!tolua_istable(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 strncpy(self->m_Name,tolua_tostring(tolua_S,2,0),64-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_strUrl of class  DirBigWorldInfo */
#ifndef TOLUA_DISABLE_tolua_get_DirBigWorldInfo_m_strUrl
static int tolua_get_DirBigWorldInfo_m_strUrl(lua_State* tolua_S)
{
  DirBigWorldInfo* self = (DirBigWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_strUrl'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->m_strUrl);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_strUrl of class  DirBigWorldInfo */
#ifndef TOLUA_DISABLE_tolua_set_DirBigWorldInfo_m_strUrl
static int tolua_set_DirBigWorldInfo_m_strUrl(lua_State* tolua_S)
{
  DirBigWorldInfo* self = (DirBigWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_strUrl'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_strUrl = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_Worlds of class  DirBigWorldInfo */
#ifndef TOLUA_DISABLE_tolua_get_DirBigWorldInfo_m_Worlds
static int tolua_get_DirBigWorldInfo_m_Worlds(lua_State* tolua_S)
{
  DirBigWorldInfo* self = (DirBigWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Worlds'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->m_Worlds,"std::vector<DirWorldInfo>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_Worlds of class  DirBigWorldInfo */
#ifndef TOLUA_DISABLE_tolua_set_DirBigWorldInfo_m_Worlds
static int tolua_set_DirBigWorldInfo_m_Worlds(lua_State* tolua_S)
{
  DirBigWorldInfo* self = (DirBigWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Worlds'",NULL);
  if (!tolua_isusertype(tolua_S,2,"std::vector<DirWorldInfo>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_Worlds = *((std::vector<DirWorldInfo>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_szServer of class  DirBigWorldInfo */
#ifndef TOLUA_DISABLE_tolua_get_DirBigWorldInfo_m_szServer
static int tolua_get_DirBigWorldInfo_m_szServer(lua_State* tolua_S)
{
  DirBigWorldInfo* self = (DirBigWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_szServer'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->m_szServer);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_szServer of class  DirBigWorldInfo */
#ifndef TOLUA_DISABLE_tolua_set_DirBigWorldInfo_m_szServer
static int tolua_set_DirBigWorldInfo_m_szServer(lua_State* tolua_S)
{
  DirBigWorldInfo* self = (DirBigWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_szServer'",NULL);
  if (!tolua_istable(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 strncpy(self->m_szServer,tolua_tostring(tolua_S,2,0),128-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_uStatusFlag of class  DirBigWorldInfo */
#ifndef TOLUA_DISABLE_tolua_get_DirBigWorldInfo_unsigned_m_uStatusFlag
static int tolua_get_DirBigWorldInfo_unsigned_m_uStatusFlag(lua_State* tolua_S)
{
  DirBigWorldInfo* self = (DirBigWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_uStatusFlag'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_uStatusFlag);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_uStatusFlag of class  DirBigWorldInfo */
#ifndef TOLUA_DISABLE_tolua_set_DirBigWorldInfo_unsigned_m_uStatusFlag
static int tolua_set_DirBigWorldInfo_unsigned_m_uStatusFlag(lua_State* tolua_S)
{
  DirBigWorldInfo* self = (DirBigWorldInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_uStatusFlag'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_uStatusFlag = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBigWorldNum of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getBigWorldNum00
static int tolua_offlineTolua_MudNetMgr_getBigWorldNum00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBigWorldNum'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getBigWorldNum();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBigWorldNum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBidWorldInfo of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getBidWorldInfo00
static int tolua_offlineTolua_MudNetMgr_getBidWorldInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
  int nIndex = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBidWorldInfo'",NULL);
#endif
  {
   DirBigWorldInfo& tolua_ret = (DirBigWorldInfo&)  self->getBidWorldInfo(nIndex);
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"DirBigWorldInfo");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBidWorldInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isDirConnected of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_isDirConnected00
static int tolua_offlineTolua_MudNetMgr_isDirConnected00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isDirConnected'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isDirConnected();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isDirConnected'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSevernums of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getSevernums00
static int tolua_offlineTolua_MudNetMgr_getSevernums00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSevernums'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getSevernums();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSevernums'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSeverName of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getSeverName00
static int tolua_offlineTolua_MudNetMgr_getSeverName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
  int i = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSeverName'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getSeverName(i);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSeverName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBigWorldindex of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getBigWorldindex00
static int tolua_offlineTolua_MudNetMgr_getBigWorldindex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBigWorldindex'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getBigWorldindex();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBigWorldindex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWorldNum of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getWorldNum00
static int tolua_offlineTolua_MudNetMgr_getWorldNum00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
  int i = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWorldNum'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getWorldNum(i);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWorldNum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWorldName of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getWorldName00
static int tolua_offlineTolua_MudNetMgr_getWorldName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
  int i = ((int)  tolua_tonumber(tolua_S,2,0));
  int j = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWorldName'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getWorldName(i,j);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWorldName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWorldState of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getWorldState00
static int tolua_offlineTolua_MudNetMgr_getWorldState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
  int i = ((int)  tolua_tonumber(tolua_S,2,0));
  int j = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWorldState'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getWorldState(i,j);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWorldState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWorldindex of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getWorldindex00
static int tolua_offlineTolua_MudNetMgr_getWorldindex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWorldindex'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getWorldindex();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWorldindex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWorldIp of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getWorldIp00
static int tolua_offlineTolua_MudNetMgr_getWorldIp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWorldIp'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getWorldIp();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWorldIp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWorldPort of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getWorldPort00
static int tolua_offlineTolua_MudNetMgr_getWorldPort00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWorldPort'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getWorldPort();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWorldPort'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWorldId of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getWorldId00
static int tolua_offlineTolua_MudNetMgr_getWorldId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWorldId'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getWorldId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWorldId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSelectWorldState of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getSelectWorldState00
static int tolua_offlineTolua_MudNetMgr_getSelectWorldState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSelectWorldState'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getSelectWorldState();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSelectWorldState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSelectWorldName of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getSelectWorldName00
static int tolua_offlineTolua_MudNetMgr_getSelectWorldName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSelectWorldName'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getSelectWorldName();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSelectWorldName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSelectBigWorldName of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getSelectBigWorldName00
static int tolua_offlineTolua_MudNetMgr_getSelectBigWorldName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSelectBigWorldName'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getSelectBigWorldName();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSelectBigWorldName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getChatmsgtype of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getChatmsgtype00
static int tolua_offlineTolua_MudNetMgr_getChatmsgtype00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getChatmsgtype'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getChatmsgtype();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getChatmsgtype'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getChatmsgrolename of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getChatmsgrolename00
static int tolua_offlineTolua_MudNetMgr_getChatmsgrolename00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getChatmsgrolename'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getChatmsgrolename();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getChatmsgrolename'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getChatmsgContent of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getChatmsgContent00
static int tolua_offlineTolua_MudNetMgr_getChatmsgContent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getChatmsgContent'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getChatmsgContent();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getChatmsgContent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTextTipsnums of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getTextTipsnums00
static int tolua_offlineTolua_MudNetMgr_getTextTipsnums00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTextTipsnums'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getTextTipsnums();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTextTipsnums'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTextTip of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getTextTip00
static int tolua_offlineTolua_MudNetMgr_getTextTip00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
  int i = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTextTip'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getTextTip(i);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTextTip'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMessageNums of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getMessageNums00
static int tolua_offlineTolua_MudNetMgr_getMessageNums00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMessageNums'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getMessageNums();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMessageNums'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMessagestr of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getMessagestr00
static int tolua_offlineTolua_MudNetMgr_getMessagestr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMessagestr'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getMessagestr();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMessagestr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMessagestyle of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getMessagestyle00
static int tolua_offlineTolua_MudNetMgr_getMessagestyle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMessagestyle'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getMessagestyle();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMessagestyle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLastSelectArea of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getLastSelectArea00
static int tolua_offlineTolua_MudNetMgr_getLastSelectArea00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLastSelectArea'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getLastSelectArea();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLastSelectArea'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLastSelectWorld of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getLastSelectWorld00
static int tolua_offlineTolua_MudNetMgr_getLastSelectWorld00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLastSelectWorld'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getLastSelectWorld();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLastSelectWorld'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addMessagestr of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_addMessagestr00
static int tolua_offlineTolua_MudNetMgr_addMessagestr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
  const char* str = ((const char*)  tolua_tostring(tolua_S,2,0));
  int style = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addMessagestr'",NULL);
#endif
  {
   self->addMessagestr(str,style);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addMessagestr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delMessage of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_delMessage00
static int tolua_offlineTolua_MudNetMgr_delMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delMessage'",NULL);
#endif
  {
   self->delMessage();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setBigworldindex of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_setBigworldindex00
static int tolua_offlineTolua_MudNetMgr_setBigworldindex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
  const char* str = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setBigworldindex'",NULL);
#endif
  {
   self->setBigworldindex(str);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setBigworldindex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setWorldindex of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_setWorldindex00
static int tolua_offlineTolua_MudNetMgr_setWorldindex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
  const char* str = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setWorldindex'",NULL);
#endif
  {
   self->setWorldindex(str);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setWorldindex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: requestLoginGameServer of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_requestLoginGameServer00
static int tolua_offlineTolua_MudNetMgr_requestLoginGameServer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isstring(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
  const char* ip = ((const char*)  tolua_tostring(tolua_S,2,0));
  unsigned short port = ((unsigned short)  tolua_tonumber(tolua_S,3,0));
  const char* username = ((const char*)  tolua_tostring(tolua_S,4,0));
  const char* passwd = ((const char*)  tolua_tostring(tolua_S,5,0));
  int worldId = ((int)  tolua_tonumber(tolua_S,6,0));
  const char* worldName = ((const char*)  tolua_tostring(tolua_S,7,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'requestLoginGameServer'",NULL);
#endif
  {
   self->requestLoginGameServer(ip,port,username,passwd,worldId,worldName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'requestLoginGameServer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: requestSendChat of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_requestSendChat00
static int tolua_offlineTolua_MudNetMgr_requestSendChat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
  int channel = ((int)  tolua_tonumber(tolua_S,2,0));
  const char* szName = ((const char*)  tolua_tostring(tolua_S,3,0));
  const char* szContent = ((const char*)  tolua_tostring(tolua_S,4,0));
  int onlinetype = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'requestSendChat'",NULL);
#endif
  {
   self->requestSendChat(channel,szName,szContent,onlinetype);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'requestSendChat'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: requestChannelSet of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_requestChannelSet00
static int tolua_offlineTolua_MudNetMgr_requestChannelSet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
  int channel = ((int)  tolua_tonumber(tolua_S,2,0));
  bool sign = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'requestChannelSet'",NULL);
#endif
  {
   self->requestChannelSet(channel,sign);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'requestChannelSet'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: requestLogout of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_requestLogout00
static int tolua_offlineTolua_MudNetMgr_requestLogout00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'requestLogout'",NULL);
#endif
  {
   self->requestLogout();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'requestLogout'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ClearTexttip of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_ClearTexttip00
static int tolua_offlineTolua_MudNetMgr_ClearTexttip00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ClearTexttip'",NULL);
#endif
  {
   self->ClearTexttip();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ClearTexttip'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: startZone of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_startZone00
static int tolua_offlineTolua_MudNetMgr_startZone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'startZone'",NULL);
#endif
  {
   self->startZone();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'startZone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ResetRfreshTime of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_ResetRfreshTime00
static int tolua_offlineTolua_MudNetMgr_ResetRfreshTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ResetRfreshTime'",NULL);
#endif
  {
   self->ResetRfreshTime();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ResetRfreshTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLoginResult of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_getLoginResult00
static int tolua_offlineTolua_MudNetMgr_getLoginResult00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLoginResult'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getLoginResult();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLoginResult'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReFreshServer of class  MudNetMgr */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_MudNetMgr_ReFreshServer00
static int tolua_offlineTolua_MudNetMgr_ReFreshServer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MudNetMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MudNetMgr* self = (MudNetMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReFreshServer'",NULL);
#endif
  {
   self->ReFreshServer();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReFreshServer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_nItemId of class  tagItemLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagItemLink_ToLua_m_nItemId
static int tolua_get_tagItemLink_ToLua_m_nItemId(lua_State* tolua_S)
{
  tagItemLink_ToLua* self = (tagItemLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nItemId'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_nItemId);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_nItemId of class  tagItemLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagItemLink_ToLua_m_nItemId
static int tolua_set_tagItemLink_ToLua_m_nItemId(lua_State* tolua_S)
{
  tagItemLink_ToLua* self = (tagItemLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nItemId'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_nItemId = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_nItemDur of class  tagItemLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagItemLink_ToLua_m_nItemDur
static int tolua_get_tagItemLink_ToLua_m_nItemDur(lua_State* tolua_S)
{
  tagItemLink_ToLua* self = (tagItemLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nItemDur'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_nItemDur);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_nItemDur of class  tagItemLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagItemLink_ToLua_m_nItemDur
static int tolua_set_tagItemLink_ToLua_m_nItemDur(lua_State* tolua_S)
{
  tagItemLink_ToLua* self = (tagItemLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nItemDur'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_nItemDur = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: InstVal1 of class  tagItemLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagItemLink_ToLua_InstVal1
static int tolua_get_tagItemLink_ToLua_InstVal1(lua_State* tolua_S)
{
  tagItemLink_ToLua* self = (tagItemLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'InstVal1'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->InstVal1);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: InstVal1 of class  tagItemLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagItemLink_ToLua_InstVal1
static int tolua_set_tagItemLink_ToLua_InstVal1(lua_State* tolua_S)
{
  tagItemLink_ToLua* self = (tagItemLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'InstVal1'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->InstVal1 = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: InstVal2 of class  tagItemLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagItemLink_ToLua_InstVal2
static int tolua_get_tagItemLink_ToLua_InstVal2(lua_State* tolua_S)
{
  tagItemLink_ToLua* self = (tagItemLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'InstVal2'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->InstVal2);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: InstVal2 of class  tagItemLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagItemLink_ToLua_InstVal2
static int tolua_set_tagItemLink_ToLua_InstVal2(lua_State* tolua_S)
{
  tagItemLink_ToLua* self = (tagItemLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'InstVal2'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->InstVal2 = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: InstVal3 of class  tagItemLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagItemLink_ToLua_InstVal3
static int tolua_get_tagItemLink_ToLua_InstVal3(lua_State* tolua_S)
{
  tagItemLink_ToLua* self = (tagItemLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'InstVal3'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->InstVal3);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: InstVal3 of class  tagItemLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagItemLink_ToLua_InstVal3
static int tolua_set_tagItemLink_ToLua_InstVal3(lua_State* tolua_S)
{
  tagItemLink_ToLua* self = (tagItemLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'InstVal3'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->InstVal3 = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Flag of class  tagItemLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagItemLink_ToLua_Flag
static int tolua_get_tagItemLink_ToLua_Flag(lua_State* tolua_S)
{
  tagItemLink_ToLua* self = (tagItemLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Flag'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->Flag);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Flag of class  tagItemLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagItemLink_ToLua_Flag
static int tolua_set_tagItemLink_ToLua_Flag(lua_State* tolua_S)
{
  tagItemLink_ToLua* self = (tagItemLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Flag'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Flag = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_arm of class  tagArmLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagArmLink_ToLua_m_arm
static int tolua_get_tagArmLink_ToLua_m_arm(lua_State* tolua_S)
{
  tagArmLink_ToLua* self = (tagArmLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_arm'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->m_arm,"tagRoleArm_ToLua");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_arm of class  tagArmLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagArmLink_ToLua_m_arm
static int tolua_set_tagArmLink_ToLua_m_arm(lua_State* tolua_S)
{
  tagArmLink_ToLua* self = (tagArmLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_arm'",NULL);
  if (!tolua_isusertype(tolua_S,2,"tagRoleArm_ToLua",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_arm = *((tagRoleArm_ToLua*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_MissionId of class  tagMissionLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagMissionLink_ToLua_m_MissionId
static int tolua_get_tagMissionLink_ToLua_m_MissionId(lua_State* tolua_S)
{
  tagMissionLink_ToLua* self = (tagMissionLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_MissionId'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_MissionId);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_MissionId of class  tagMissionLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagMissionLink_ToLua_m_MissionId
static int tolua_set_tagMissionLink_ToLua_m_MissionId(lua_State* tolua_S)
{
  tagMissionLink_ToLua* self = (tagMissionLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_MissionId'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_MissionId = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_nMapId of class  tagPostionLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagPostionLink_ToLua_m_nMapId
static int tolua_get_tagPostionLink_ToLua_m_nMapId(lua_State* tolua_S)
{
  tagPostionLink_ToLua* self = (tagPostionLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nMapId'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_nMapId);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_nMapId of class  tagPostionLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagPostionLink_ToLua_m_nMapId
static int tolua_set_tagPostionLink_ToLua_m_nMapId(lua_State* tolua_S)
{
  tagPostionLink_ToLua* self = (tagPostionLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nMapId'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_nMapId = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_nX of class  tagPostionLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagPostionLink_ToLua_m_nX
static int tolua_get_tagPostionLink_ToLua_m_nX(lua_State* tolua_S)
{
  tagPostionLink_ToLua* self = (tagPostionLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nX'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_nX);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_nX of class  tagPostionLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagPostionLink_ToLua_m_nX
static int tolua_set_tagPostionLink_ToLua_m_nX(lua_State* tolua_S)
{
  tagPostionLink_ToLua* self = (tagPostionLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nX'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_nX = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_nY of class  tagPostionLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagPostionLink_ToLua_m_nY
static int tolua_get_tagPostionLink_ToLua_m_nY(lua_State* tolua_S)
{
  tagPostionLink_ToLua* self = (tagPostionLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nY'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_nY);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_nY of class  tagPostionLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagPostionLink_ToLua_m_nY
static int tolua_set_tagPostionLink_ToLua_m_nY(lua_State* tolua_S)
{
  tagPostionLink_ToLua* self = (tagPostionLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nY'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_nY = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: SkillType of class  tagSecondarySkillLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagSecondarySkillLink_ToLua_SkillType
static int tolua_get_tagSecondarySkillLink_ToLua_SkillType(lua_State* tolua_S)
{
  tagSecondarySkillLink_ToLua* self = (tagSecondarySkillLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SkillType'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->SkillType);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: SkillType of class  tagSecondarySkillLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagSecondarySkillLink_ToLua_SkillType
static int tolua_set_tagSecondarySkillLink_ToLua_SkillType(lua_State* tolua_S)
{
  tagSecondarySkillLink_ToLua* self = (tagSecondarySkillLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SkillType'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->SkillType = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: SkillLevel of class  tagSecondarySkillLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagSecondarySkillLink_ToLua_SkillLevel
static int tolua_get_tagSecondarySkillLink_ToLua_SkillLevel(lua_State* tolua_S)
{
  tagSecondarySkillLink_ToLua* self = (tagSecondarySkillLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SkillLevel'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->SkillLevel);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: SkillLevel of class  tagSecondarySkillLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagSecondarySkillLink_ToLua_SkillLevel
static int tolua_set_tagSecondarySkillLink_ToLua_SkillLevel(lua_State* tolua_S)
{
  tagSecondarySkillLink_ToLua* self = (tagSecondarySkillLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SkillLevel'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->SkillLevel = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: SkillVal of class  tagSecondarySkillLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagSecondarySkillLink_ToLua_SkillVal
static int tolua_get_tagSecondarySkillLink_ToLua_SkillVal(lua_State* tolua_S)
{
  tagSecondarySkillLink_ToLua* self = (tagSecondarySkillLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SkillVal'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->SkillVal);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: SkillVal of class  tagSecondarySkillLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagSecondarySkillLink_ToLua_SkillVal
static int tolua_set_tagSecondarySkillLink_ToLua_SkillVal(lua_State* tolua_S)
{
  tagSecondarySkillLink_ToLua* self = (tagSecondarySkillLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SkillVal'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->SkillVal = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: RecipeInfo of class  tagSecondarySkillLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagSecondarySkillLink_ToLua_RecipeInfo
static int tolua_get_tagSecondarySkillLink_ToLua_RecipeInfo(lua_State* tolua_S)
{
  tagSecondarySkillLink_ToLua* self = (tagSecondarySkillLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'RecipeInfo'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->RecipeInfo);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: RecipeInfo of class  tagSecondarySkillLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagSecondarySkillLink_ToLua_RecipeInfo
static int tolua_set_tagSecondarySkillLink_ToLua_RecipeInfo(lua_State* tolua_S)
{
  tagSecondarySkillLink_ToLua* self = (tagSecondarySkillLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'RecipeInfo'",NULL);
  if (!tolua_istable(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 strncpy(self->RecipeInfo,tolua_tostring(tolua_S,2,0),MAX_SECONDARY_SKILL_RECIPE-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: PetWid of class  tagPetLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagPetLink_ToLua_PetWid
static int tolua_get_tagPetLink_ToLua_PetWid(lua_State* tolua_S)
{
  tagPetLink_ToLua* self = (tagPetLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'PetWid'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->PetWid,"tdr_longlong");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: PetWid of class  tagPetLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagPetLink_ToLua_PetWid
static int tolua_set_tagPetLink_ToLua_PetWid(lua_State* tolua_S)
{
  tagPetLink_ToLua* self = (tagPetLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'PetWid'",NULL);
  if (!tolua_isusertype(tolua_S,2,"tdr_longlong",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->PetWid = *((tdr_longlong*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: PetName of class  tagPetLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagPetLink_ToLua_PetName
static int tolua_get_tagPetLink_ToLua_PetName(lua_State* tolua_S)
{
  tagPetLink_ToLua* self = (tagPetLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'PetName'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->PetName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: PetName of class  tagPetLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagPetLink_ToLua_PetName
static int tolua_set_tagPetLink_ToLua_PetName(lua_State* tolua_S)
{
  tagPetLink_ToLua* self = (tagPetLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'PetName'",NULL);
  if (!tolua_istable(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 strncpy(self->PetName,tolua_tostring(tolua_S,2,0),NAME_LEN-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: OwnerName of class  tagPetLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagPetLink_ToLua_OwnerName
static int tolua_get_tagPetLink_ToLua_OwnerName(lua_State* tolua_S)
{
  tagPetLink_ToLua* self = (tagPetLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'OwnerName'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->OwnerName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: OwnerName of class  tagPetLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagPetLink_ToLua_OwnerName
static int tolua_set_tagPetLink_ToLua_OwnerName(lua_State* tolua_S)
{
  tagPetLink_ToLua* self = (tagPetLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'OwnerName'",NULL);
  if (!tolua_istable(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 strncpy(self->OwnerName,tolua_tostring(tolua_S,2,0),ROLE_NAME_LEN-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_SystemLink of class  tagLinkData_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagLinkData_ToLua_m_SystemLink
static int tolua_get_tagLinkData_ToLua_m_SystemLink(lua_State* tolua_S)
{
  tagLinkData_ToLua* self = (tagLinkData_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_SystemLink'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->m_SystemLink,"tagPostionLink_ToLua");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_SystemLink of class  tagLinkData_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagLinkData_ToLua_m_SystemLink
static int tolua_set_tagLinkData_ToLua_m_SystemLink(lua_State* tolua_S)
{
  tagLinkData_ToLua* self = (tagLinkData_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_SystemLink'",NULL);
  if (!tolua_isusertype(tolua_S,2,"tagPostionLink_ToLua",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_SystemLink = *((tagPostionLink_ToLua*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_ItemLink of class  tagLinkData_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagLinkData_ToLua_m_ItemLink
static int tolua_get_tagLinkData_ToLua_m_ItemLink(lua_State* tolua_S)
{
  tagLinkData_ToLua* self = (tagLinkData_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ItemLink'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->m_ItemLink,"tagItemLink_ToLua");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_ItemLink of class  tagLinkData_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagLinkData_ToLua_m_ItemLink
static int tolua_set_tagLinkData_ToLua_m_ItemLink(lua_State* tolua_S)
{
  tagLinkData_ToLua* self = (tagLinkData_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ItemLink'",NULL);
  if (!tolua_isusertype(tolua_S,2,"tagItemLink_ToLua",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_ItemLink = *((tagItemLink_ToLua*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_RoleArmLink of class  tagLinkData_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagLinkData_ToLua_m_RoleArmLink
static int tolua_get_tagLinkData_ToLua_m_RoleArmLink(lua_State* tolua_S)
{
  tagLinkData_ToLua* self = (tagLinkData_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_RoleArmLink'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->m_RoleArmLink,"tagArmLink_ToLua");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_RoleArmLink of class  tagLinkData_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagLinkData_ToLua_m_RoleArmLink
static int tolua_set_tagLinkData_ToLua_m_RoleArmLink(lua_State* tolua_S)
{
  tagLinkData_ToLua* self = (tagLinkData_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_RoleArmLink'",NULL);
  if (!tolua_isusertype(tolua_S,2,"tagArmLink_ToLua",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_RoleArmLink = *((tagArmLink_ToLua*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_MissionLink of class  tagLinkData_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagLinkData_ToLua_m_MissionLink
static int tolua_get_tagLinkData_ToLua_m_MissionLink(lua_State* tolua_S)
{
  tagLinkData_ToLua* self = (tagLinkData_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_MissionLink'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->m_MissionLink,"tagMissionLink_ToLua");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_MissionLink of class  tagLinkData_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagLinkData_ToLua_m_MissionLink
static int tolua_set_tagLinkData_ToLua_m_MissionLink(lua_State* tolua_S)
{
  tagLinkData_ToLua* self = (tagLinkData_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_MissionLink'",NULL);
  if (!tolua_isusertype(tolua_S,2,"tagMissionLink_ToLua",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_MissionLink = *((tagMissionLink_ToLua*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_PostionLink of class  tagLinkData_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagLinkData_ToLua_m_PostionLink
static int tolua_get_tagLinkData_ToLua_m_PostionLink(lua_State* tolua_S)
{
  tagLinkData_ToLua* self = (tagLinkData_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PostionLink'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->m_PostionLink,"tagPostionLink_ToLua");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_PostionLink of class  tagLinkData_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagLinkData_ToLua_m_PostionLink
static int tolua_set_tagLinkData_ToLua_m_PostionLink(lua_State* tolua_S)
{
  tagLinkData_ToLua* self = (tagLinkData_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PostionLink'",NULL);
  if (!tolua_isusertype(tolua_S,2,"tagPostionLink_ToLua",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_PostionLink = *((tagPostionLink_ToLua*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_SecondarySkillLink of class  tagLinkData_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagLinkData_ToLua_m_SecondarySkillLink
static int tolua_get_tagLinkData_ToLua_m_SecondarySkillLink(lua_State* tolua_S)
{
  tagLinkData_ToLua* self = (tagLinkData_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_SecondarySkillLink'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->m_SecondarySkillLink,"tagSecondarySkillLink_ToLua");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_SecondarySkillLink of class  tagLinkData_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagLinkData_ToLua_m_SecondarySkillLink
static int tolua_set_tagLinkData_ToLua_m_SecondarySkillLink(lua_State* tolua_S)
{
  tagLinkData_ToLua* self = (tagLinkData_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_SecondarySkillLink'",NULL);
  if (!tolua_isusertype(tolua_S,2,"tagSecondarySkillLink_ToLua",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_SecondarySkillLink = *((tagSecondarySkillLink_ToLua*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_PetLink of class  tagLinkData_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagLinkData_ToLua_m_PetLink
static int tolua_get_tagLinkData_ToLua_m_PetLink(lua_State* tolua_S)
{
  tagLinkData_ToLua* self = (tagLinkData_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PetLink'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->m_PetLink,"tagPetLink_ToLua");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_PetLink of class  tagLinkData_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagLinkData_ToLua_m_PetLink
static int tolua_set_tagLinkData_ToLua_m_PetLink(lua_State* tolua_S)
{
  tagLinkData_ToLua* self = (tagLinkData_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PetLink'",NULL);
  if (!tolua_isusertype(tolua_S,2,"tagPetLink_ToLua",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_PetLink = *((tagPetLink_ToLua*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Type of class  tagChatLinkData_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagChatLinkData_ToLua_Type
static int tolua_get_tagChatLinkData_ToLua_Type(lua_State* tolua_S)
{
  tagChatLinkData_ToLua* self = (tagChatLinkData_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Type'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->Type);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Type of class  tagChatLinkData_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagChatLinkData_ToLua_Type
static int tolua_set_tagChatLinkData_ToLua_Type(lua_State* tolua_S)
{
  tagChatLinkData_ToLua* self = (tagChatLinkData_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Type'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Type = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Data of class  tagChatLinkData_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagChatLinkData_ToLua_Data
static int tolua_get_tagChatLinkData_ToLua_Data(lua_State* tolua_S)
{
  tagChatLinkData_ToLua* self = (tagChatLinkData_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Data'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->Data,"tagLinkData_ToLua");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Data of class  tagChatLinkData_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagChatLinkData_ToLua_Data
static int tolua_set_tagChatLinkData_ToLua_Data(lua_State* tolua_S)
{
  tagChatLinkData_ToLua* self = (tagChatLinkData_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Data'",NULL);
  if (!tolua_isusertype(tolua_S,2,"tagLinkData_ToLua",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Data = *((tagLinkData_ToLua*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_nLinkNum of class  tagChatLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_tagChatLink_ToLua_m_nLinkNum
static int tolua_get_tagChatLink_ToLua_m_nLinkNum(lua_State* tolua_S)
{
  tagChatLink_ToLua* self = (tagChatLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nLinkNum'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_nLinkNum);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_nLinkNum of class  tagChatLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_tagChatLink_ToLua_m_nLinkNum
static int tolua_set_tagChatLink_ToLua_m_nLinkNum(lua_State* tolua_S)
{
  tagChatLink_ToLua* self = (tagChatLink_ToLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nLinkNum'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_nLinkNum = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_LinkData of class  tagChatLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_get_offlineTolua_tagChatLink_ToLua_m_LinkData
static int tolua_get_offlineTolua_tagChatLink_ToLua_m_LinkData(lua_State* tolua_S)
{
 int tolua_index;
  tagChatLink_ToLua* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (tagChatLink_ToLua*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=MAX_CHAT_LINK_NUM)
  tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->m_LinkData[tolua_index],"tagChatLinkData_ToLua");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_LinkData of class  tagChatLink_ToLua */
#ifndef TOLUA_DISABLE_tolua_set_offlineTolua_tagChatLink_ToLua_m_LinkData
static int tolua_set_offlineTolua_tagChatLink_ToLua_m_LinkData(lua_State* tolua_S)
{
 int tolua_index;
  tagChatLink_ToLua* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (tagChatLink_ToLua*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=MAX_CHAT_LINK_NUM)
  tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
  self->m_LinkData[tolua_index] = *((tagChatLinkData_ToLua*)  tolua_tousertype(tolua_S,3,0));
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getItemLink of class  ChatManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_ChatManager_getItemLink00
static int tolua_offlineTolua_ChatManager_getItemLink00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ChatManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ChatManager* self = (ChatManager*)  tolua_tousertype(tolua_S,1,0);
  int linkId = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getItemLink'",NULL);
#endif
  {
   tagItemLink_ToLua& tolua_ret = (tagItemLink_ToLua&)  self->getItemLink(linkId);
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"tagItemLink_ToLua");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getItemLink'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getArmLink of class  ChatManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_ChatManager_getArmLink00
static int tolua_offlineTolua_ChatManager_getArmLink00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ChatManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ChatManager* self = (ChatManager*)  tolua_tousertype(tolua_S,1,0);
  int linkId = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getArmLink'",NULL);
#endif
  {
   const tagArmLink_ToLua& tolua_ret = (const tagArmLink_ToLua&)  self->getArmLink(linkId);
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const tagArmLink_ToLua");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getArmLink'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: strReplace of class  ChatManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_ChatManager_strReplace00
static int tolua_offlineTolua_ChatManager_strReplace00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ChatManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ChatManager* self = (ChatManager*)  tolua_tousertype(tolua_S,1,0);
  std::string szText = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  const char* src = ((const char*)  tolua_tostring(tolua_S,3,0));
  const char* des = ((const char*)  tolua_tostring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'strReplace'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->strReplace(szText,src,des);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)szText);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'strReplace'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getItem of class  ChatManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_ChatManager_getItem00
static int tolua_offlineTolua_ChatManager_getItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ChatManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const tagRoleArm_ToLua",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ChatManager* self = (ChatManager*)  tolua_tousertype(tolua_S,1,0);
  const tagRoleArm_ToLua* arm = ((const tagRoleArm_ToLua*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getItem'",NULL);
#endif
  {
   Item* tolua_ret = (Item*)  self->getItem(arm);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Item");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getChatmsgtype of class  ChatManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_ChatManager_getChatmsgtype00
static int tolua_offlineTolua_ChatManager_getChatmsgtype00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ChatManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ChatManager* self = (ChatManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getChatmsgtype'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getChatmsgtype();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getChatmsgtype'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getChatmsgrolename of class  ChatManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_ChatManager_getChatmsgrolename00
static int tolua_offlineTolua_ChatManager_getChatmsgrolename00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ChatManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ChatManager* self = (ChatManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getChatmsgrolename'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getChatmsgrolename();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getChatmsgrolename'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getChatmsgContent of class  ChatManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_ChatManager_getChatmsgContent00
static int tolua_offlineTolua_ChatManager_getChatmsgContent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ChatManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ChatManager* self = (ChatManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getChatmsgContent'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getChatmsgContent();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getChatmsgContent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getChatreqrolename of class  ChatManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_ChatManager_getChatreqrolename00
static int tolua_offlineTolua_ChatManager_getChatreqrolename00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ChatManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ChatManager* self = (ChatManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getChatreqrolename'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getChatreqrolename();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getChatreqrolename'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getChatreqContent of class  ChatManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_ChatManager_getChatreqContent00
static int tolua_offlineTolua_ChatManager_getChatreqContent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ChatManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ChatManager* self = (ChatManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getChatreqContent'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getChatreqContent();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getChatreqContent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getChatreqType of class  ChatManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_ChatManager_getChatreqType00
static int tolua_offlineTolua_ChatManager_getChatreqType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ChatManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ChatManager* self = (ChatManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getChatreqType'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getChatreqType();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getChatreqType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendWatchRequest of class  OfflineWatchPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineWatchPlayer_sendWatchRequest00
static int tolua_offlineTolua_OfflineWatchPlayer_sendWatchRequest00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineWatchPlayer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineWatchPlayer* self = (OfflineWatchPlayer*)  tolua_tousertype(tolua_S,1,0);
  const char* szName = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendWatchRequest'",NULL);
#endif
  {
   self->sendWatchRequest(szName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendWatchRequest'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWatchPlayerMemId of class  OfflineWatchPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineWatchPlayer_getWatchPlayerMemId00
static int tolua_offlineTolua_OfflineWatchPlayer_getWatchPlayerMemId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineWatchPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineWatchPlayer* self = (OfflineWatchPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWatchPlayerMemId'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getWatchPlayerMemId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWatchPlayerMemId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getItem of class  OfflineWatchPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineWatchPlayer_getItem00
static int tolua_offlineTolua_OfflineWatchPlayer_getItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineWatchPlayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineWatchPlayer* self = (OfflineWatchPlayer*)  tolua_tousertype(tolua_S,1,0);
  int GridIndex = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getItem'",NULL);
#endif
  {
   Item& tolua_ret = (Item&)  self->getItem(GridIndex);
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"Item");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAvatarAttrib of class  OfflineWatchPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineWatchPlayer_setAvatarAttrib00
static int tolua_offlineTolua_OfflineWatchPlayer_setAvatarAttrib00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineWatchPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineWatchPlayer* self = (OfflineWatchPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAvatarAttrib'",NULL);
#endif
  {
   self->setAvatarAttrib();
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

/* method: IsSawArmItem of class  OfflineWatchPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineWatchPlayer_IsSawArmItem00
static int tolua_offlineTolua_OfflineWatchPlayer_IsSawArmItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineWatchPlayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineWatchPlayer* self = (OfflineWatchPlayer*)  tolua_tousertype(tolua_S,1,0);
  int nArmId = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsSawArmItem'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsSawArmItem(nArmId);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsSawArmItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWatchPlayer of class  OfflineWatchPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineWatchPlayer_getWatchPlayer00
static int tolua_offlineTolua_OfflineWatchPlayer_getWatchPlayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineWatchPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineWatchPlayer* self = (OfflineWatchPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWatchPlayer'",NULL);
#endif
  {
   GameActor* tolua_ret = (GameActor*)  self->getWatchPlayer();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GameActor");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWatchPlayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_RoleID of class  OfflinePlayer */
#ifndef TOLUA_DISABLE_tolua_get_OfflinePlayer_m_RoleID
static int tolua_get_OfflinePlayer_m_RoleID(lua_State* tolua_S)
{
  OfflinePlayer* self = (OfflinePlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_RoleID'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->m_RoleID,"tdr_ulonglong");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_RoleID of class  OfflinePlayer */
#ifndef TOLUA_DISABLE_tolua_set_OfflinePlayer_m_RoleID
static int tolua_set_OfflinePlayer_m_RoleID(lua_State* tolua_S)
{
  OfflinePlayer* self = (OfflinePlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_RoleID'",NULL);
  if (!tolua_isusertype(tolua_S,2,"tdr_ulonglong",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_RoleID = *((tdr_ulonglong*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_RoleName of class  OfflinePlayer */
#ifndef TOLUA_DISABLE_tolua_get_OfflinePlayer_m_RoleName
static int tolua_get_OfflinePlayer_m_RoleName(lua_State* tolua_S)
{
  OfflinePlayer* self = (OfflinePlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_RoleName'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->m_RoleName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_RoleName of class  OfflinePlayer */
#ifndef TOLUA_DISABLE_tolua_set_OfflinePlayer_m_RoleName
static int tolua_set_OfflinePlayer_m_RoleName(lua_State* tolua_S)
{
  OfflinePlayer* self = (OfflinePlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_RoleName'",NULL);
  if (!tolua_istable(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 strncpy(self->m_RoleName,tolua_tostring(tolua_S,2,0),NAME_LEN-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_Emotion of class  OfflinePlayer */
#ifndef TOLUA_DISABLE_tolua_get_OfflinePlayer_m_Emotion
static int tolua_get_OfflinePlayer_m_Emotion(lua_State* tolua_S)
{
  OfflinePlayer* self = (OfflinePlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Emotion'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->m_Emotion);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_Emotion of class  OfflinePlayer */
#ifndef TOLUA_DISABLE_tolua_set_OfflinePlayer_m_Emotion
static int tolua_set_OfflinePlayer_m_Emotion(lua_State* tolua_S)
{
  OfflinePlayer* self = (OfflinePlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Emotion'",NULL);
  if (!tolua_istable(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 strncpy(self->m_Emotion,tolua_tostring(tolua_S,2,0),EMOTION_LEN-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_uCareer of class  OfflinePlayer */
#ifndef TOLUA_DISABLE_tolua_get_OfflinePlayer_m_uCareer
static int tolua_get_OfflinePlayer_m_uCareer(lua_State* tolua_S)
{
  OfflinePlayer* self = (OfflinePlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_uCareer'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_uCareer);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_uCareer of class  OfflinePlayer */
#ifndef TOLUA_DISABLE_tolua_set_OfflinePlayer_m_uCareer
static int tolua_set_OfflinePlayer_m_uCareer(lua_State* tolua_S)
{
  OfflinePlayer* self = (OfflinePlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_uCareer'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_uCareer = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_uLevel of class  OfflinePlayer */
#ifndef TOLUA_DISABLE_tolua_get_OfflinePlayer_m_uLevel
static int tolua_get_OfflinePlayer_m_uLevel(lua_State* tolua_S)
{
  OfflinePlayer* self = (OfflinePlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_uLevel'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_uLevel);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_uLevel of class  OfflinePlayer */
#ifndef TOLUA_DISABLE_tolua_set_OfflinePlayer_m_uLevel
static int tolua_set_OfflinePlayer_m_uLevel(lua_State* tolua_S)
{
  OfflinePlayer* self = (OfflinePlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_uLevel'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_uLevel = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_uGender of class  OfflinePlayer */
#ifndef TOLUA_DISABLE_tolua_get_OfflinePlayer_m_uGender
static int tolua_get_OfflinePlayer_m_uGender(lua_State* tolua_S)
{
  OfflinePlayer* self = (OfflinePlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_uGender'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_uGender);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_uGender of class  OfflinePlayer */
#ifndef TOLUA_DISABLE_tolua_set_OfflinePlayer_m_uGender
static int tolua_set_OfflinePlayer_m_uGender(lua_State* tolua_S)
{
  OfflinePlayer* self = (OfflinePlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_uGender'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_uGender = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_Onlinetype of class  OfflinePlayer */
#ifndef TOLUA_DISABLE_tolua_get_OfflinePlayer_m_Onlinetype
static int tolua_get_OfflinePlayer_m_Onlinetype(lua_State* tolua_S)
{
  OfflinePlayer* self = (OfflinePlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Onlinetype'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_Onlinetype);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_Onlinetype of class  OfflinePlayer */
#ifndef TOLUA_DISABLE_tolua_set_OfflinePlayer_m_Onlinetype
static int tolua_set_OfflinePlayer_m_Onlinetype(lua_State* tolua_S)
{
  OfflinePlayer* self = (OfflinePlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Onlinetype'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_Onlinetype = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_nhead of class  OfflinePlayer */
#ifndef TOLUA_DISABLE_tolua_get_OfflinePlayer_m_nhead
static int tolua_get_OfflinePlayer_m_nhead(lua_State* tolua_S)
{
  OfflinePlayer* self = (OfflinePlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nhead'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_nhead);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_nhead of class  OfflinePlayer */
#ifndef TOLUA_DISABLE_tolua_set_OfflinePlayer_m_nhead
static int tolua_set_OfflinePlayer_m_nhead(lua_State* tolua_S)
{
  OfflinePlayer* self = (OfflinePlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nhead'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_nhead = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_nface of class  OfflinePlayer */
#ifndef TOLUA_DISABLE_tolua_get_OfflinePlayer_m_nface
static int tolua_get_OfflinePlayer_m_nface(lua_State* tolua_S)
{
  OfflinePlayer* self = (OfflinePlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nface'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_nface);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_nface of class  OfflinePlayer */
#ifndef TOLUA_DISABLE_tolua_set_OfflinePlayer_m_nface
static int tolua_set_OfflinePlayer_m_nface(lua_State* tolua_S)
{
  OfflinePlayer* self = (OfflinePlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nface'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_nface = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_nhair of class  OfflinePlayer */
#ifndef TOLUA_DISABLE_tolua_get_OfflinePlayer_m_nhair
static int tolua_get_OfflinePlayer_m_nhair(lua_State* tolua_S)
{
  OfflinePlayer* self = (OfflinePlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nhair'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_nhair);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_nhair of class  OfflinePlayer */
#ifndef TOLUA_DISABLE_tolua_set_OfflinePlayer_m_nhair
static int tolua_set_OfflinePlayer_m_nhair(lua_State* tolua_S)
{
  OfflinePlayer* self = (OfflinePlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_nhair'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_nhair = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_Relationtype of class  OfflinePlayer */
#ifndef TOLUA_DISABLE_tolua_get_OfflinePlayer_m_Relationtype
static int tolua_get_OfflinePlayer_m_Relationtype(lua_State* tolua_S)
{
  OfflinePlayer* self = (OfflinePlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Relationtype'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_Relationtype);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_Relationtype of class  OfflinePlayer */
#ifndef TOLUA_DISABLE_tolua_set_OfflinePlayer_m_Relationtype
static int tolua_set_OfflinePlayer_m_Relationtype(lua_State* tolua_S)
{
  OfflinePlayer* self = (OfflinePlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Relationtype'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_Relationtype = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFriendInfo of class  RelationManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_RelationManager_getFriendInfo00
static int tolua_offlineTolua_RelationManager_getFriendInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RelationManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RelationManager* self = (RelationManager*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFriendInfo'",NULL);
#endif
  {
   OfflinePlayer* tolua_ret = (OfflinePlayer*)  self->getFriendInfo(index);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"OfflinePlayer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFriendInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFriendInfoByName of class  RelationManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_RelationManager_getFriendInfoByName00
static int tolua_offlineTolua_RelationManager_getFriendInfoByName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RelationManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RelationManager* self = (RelationManager*)  tolua_tousertype(tolua_S,1,0);
  const char* szName = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFriendInfoByName'",NULL);
#endif
  {
   OfflinePlayer* tolua_ret = (OfflinePlayer*)  self->getFriendInfoByName(szName);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"OfflinePlayer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFriendInfoByName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getClanMemberInfo of class  RelationManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_RelationManager_getClanMemberInfo00
static int tolua_offlineTolua_RelationManager_getClanMemberInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RelationManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RelationManager* self = (RelationManager*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getClanMemberInfo'",NULL);
#endif
  {
   OfflinePlayer* tolua_ret = (OfflinePlayer*)  self->getClanMemberInfo(index);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"OfflinePlayer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getClanMemberInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: requestAddFriend of class  RelationManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_RelationManager_requestAddFriend00
static int tolua_offlineTolua_RelationManager_requestAddFriend00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RelationManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RelationManager* self = (RelationManager*)  tolua_tousertype(tolua_S,1,0);
  const char* szName = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* Reqinfo = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'requestAddFriend'",NULL);
#endif
  {
   self->requestAddFriend(szName,Reqinfo);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'requestAddFriend'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: requestDeleteFriend of class  RelationManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_RelationManager_requestDeleteFriend00
static int tolua_offlineTolua_RelationManager_requestDeleteFriend00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RelationManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"tdr_ulonglong",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RelationManager* self = (RelationManager*)  tolua_tousertype(tolua_S,1,0);
  tdr_ulonglong LongId = *((tdr_ulonglong*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'requestDeleteFriend'",NULL);
#endif
  {
   self->requestDeleteFriend(LongId);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'requestDeleteFriend'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: responseAddFriend of class  RelationManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_RelationManager_responseAddFriend00
static int tolua_offlineTolua_RelationManager_responseAddFriend00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RelationManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RelationManager* self = (RelationManager*)  tolua_tousertype(tolua_S,1,0);
  const char* szName = ((const char*)  tolua_tostring(tolua_S,2,0));
  int isaccept = ((int)  tolua_tonumber(tolua_S,3,0));
  int Onlinetype = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'responseAddFriend'",NULL);
#endif
  {
   self->responseAddFriend(szName,isaccept,Onlinetype);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'responseAddFriend'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFriendNum of class  RelationManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_RelationManager_getFriendNum00
static int tolua_offlineTolua_RelationManager_getFriendNum00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RelationManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RelationManager* self = (RelationManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFriendNum'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getFriendNum();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFriendNum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getClanMemeberNum of class  RelationManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_RelationManager_getClanMemeberNum00
static int tolua_offlineTolua_RelationManager_getClanMemeberNum00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RelationManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RelationManager* self = (RelationManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getClanMemeberNum'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getClanMemeberNum();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getClanMemeberNum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getReqName of class  RelationManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_RelationManager_getReqName00
static int tolua_offlineTolua_RelationManager_getReqName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RelationManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RelationManager* self = (RelationManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getReqName'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getReqName();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getReqName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getReqInfo of class  RelationManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_RelationManager_getReqInfo00
static int tolua_offlineTolua_RelationManager_getReqInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RelationManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RelationManager* self = (RelationManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getReqInfo'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getReqInfo();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getReqInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getReqOnlinetype of class  RelationManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_RelationManager_getReqOnlinetype00
static int tolua_offlineTolua_RelationManager_getReqOnlinetype00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RelationManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RelationManager* self = (RelationManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getReqOnlinetype'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getReqOnlinetype();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getReqOnlinetype'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsFriend of class  RelationManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_RelationManager_IsFriend00
static int tolua_offlineTolua_RelationManager_IsFriend00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RelationManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RelationManager* self = (RelationManager*)  tolua_tousertype(tolua_S,1,0);
  const char* szName = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsFriend'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsFriend(szName);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsFriend'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: requestgetFriendinfo of class  RelationManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_RelationManager_requestgetFriendinfo00
static int tolua_offlineTolua_RelationManager_requestgetFriendinfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RelationManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RelationManager* self = (RelationManager*)  tolua_tousertype(tolua_S,1,0);
  const char* szName = ((const char*)  tolua_tostring(tolua_S,2,0));
  int Onlinetype = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'requestgetFriendinfo'",NULL);
#endif
  {
   self->requestgetFriendinfo(szName,Onlinetype);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'requestgetFriendinfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: requestLeaveRoom of class  ChatRoomManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_ChatRoomManager_requestLeaveRoom00
static int tolua_offlineTolua_ChatRoomManager_requestLeaveRoom00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ChatRoomManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ChatRoomManager* self = (ChatRoomManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'requestLeaveRoom'",NULL);
#endif
  {
   self->requestLeaveRoom();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'requestLeaveRoom'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: requestEnterRoom of class  ChatRoomManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_ChatRoomManager_requestEnterRoom00
static int tolua_offlineTolua_ChatRoomManager_requestEnterRoom00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ChatRoomManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ChatRoomManager* self = (ChatRoomManager*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'requestEnterRoom'",NULL);
#endif
  {
   self->requestEnterRoom(index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'requestEnterRoom'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPlayerInfo of class  ChatRoomManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_ChatRoomManager_getPlayerInfo00
static int tolua_offlineTolua_ChatRoomManager_getPlayerInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ChatRoomManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ChatRoomManager* self = (ChatRoomManager*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPlayerInfo'",NULL);
#endif
  {
   OfflinePlayer* tolua_ret = (OfflinePlayer*)  self->getPlayerInfo(index);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"OfflinePlayer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPlayerInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCurrentRoomrolenums of class  ChatRoomManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_ChatRoomManager_getCurrentRoomrolenums00
static int tolua_offlineTolua_ChatRoomManager_getCurrentRoomrolenums00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ChatRoomManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ChatRoomManager* self = (ChatRoomManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCurrentRoomrolenums'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getCurrentRoomrolenums();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCurrentRoomrolenums'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCurrentChatRoomIndex of class  ChatRoomManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_ChatRoomManager_getCurrentChatRoomIndex00
static int tolua_offlineTolua_ChatRoomManager_getCurrentChatRoomIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ChatRoomManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ChatRoomManager* self = (ChatRoomManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCurrentChatRoomIndex'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getCurrentChatRoomIndex();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCurrentChatRoomIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getChatRoomNums of class  ChatRoomManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_ChatRoomManager_getChatRoomNums00
static int tolua_offlineTolua_ChatRoomManager_getChatRoomNums00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ChatRoomManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ChatRoomManager* self = (ChatRoomManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getChatRoomNums'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getChatRoomNums();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getChatRoomNums'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRoomPlayerNums of class  ChatRoomManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_ChatRoomManager_getRoomPlayerNums00
static int tolua_offlineTolua_ChatRoomManager_getRoomPlayerNums00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ChatRoomManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ChatRoomManager* self = (ChatRoomManager*)  tolua_tousertype(tolua_S,1,0);
  int i = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRoomPlayerNums'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getRoomPlayerNums(i);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRoomPlayerNums'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clear of class  ChatRoomManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_ChatRoomManager_clear00
static int tolua_offlineTolua_ChatRoomManager_clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ChatRoomManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ChatRoomManager* self = (ChatRoomManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clear'",NULL);
#endif
  {
   self->clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCanConnnect of class  ChatRoomManager */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_ChatRoomManager_getCanConnnect00
static int tolua_offlineTolua_ChatRoomManager_getCanConnnect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ChatRoomManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ChatRoomManager* self = (ChatRoomManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCanConnnect'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getCanConnnect();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCanConnnect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getName of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_getName00
static int tolua_offlineTolua_OfflineMainPlayer_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getName'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getName();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLevel of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_getLevel00
static int tolua_offlineTolua_OfflineMainPlayer_getLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLevel'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getLevel();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCareer of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_getCareer00
static int tolua_offlineTolua_OfflineMainPlayer_getCareer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCareer'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getCareer();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCareer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getGender of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_getGender00
static int tolua_offlineTolua_OfflineMainPlayer_getGender00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getGender'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getGender();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getGender'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getOfflineExp of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_getOfflineExp00
static int tolua_offlineTolua_OfflineMainPlayer_getOfflineExp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getOfflineExp'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getOfflineExp();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getOfflineExp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMaxOfflineExp of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_getMaxOfflineExp00
static int tolua_offlineTolua_OfflineMainPlayer_getMaxOfflineExp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMaxOfflineExp'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getMaxOfflineExp();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMaxOfflineExp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEmotion of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_getEmotion00
static int tolua_offlineTolua_OfflineMainPlayer_getEmotion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEmotion'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getEmotion();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEmotion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getHead of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_getHead00
static int tolua_offlineTolua_OfflineMainPlayer_getHead00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getHead'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getHead();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getHead'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getHair of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_getHair00
static int tolua_offlineTolua_OfflineMainPlayer_getHair00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getHair'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getHair();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getHair'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFace of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_getFace00
static int tolua_offlineTolua_OfflineMainPlayer_getFace00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFace'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getFace();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFace'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRoleId of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_getRoleId00
static int tolua_offlineTolua_OfflineMainPlayer_getRoleId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRoleId'",NULL);
#endif
  {
   tdr_longlong tolua_ret = (tdr_longlong)  self->getRoleId();
   {
#ifdef __cplusplus
    void* tolua_obj = new tdr_longlong(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"tdr_longlong");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(tdr_longlong));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"tdr_longlong");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRoleId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRoleListNums of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_getRoleListNums00
static int tolua_offlineTolua_OfflineMainPlayer_getRoleListNums00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRoleListNums'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getRoleListNums();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRoleListNums'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRoleName of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_getRoleName00
static int tolua_offlineTolua_OfflineMainPlayer_getRoleName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
  int i = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRoleName'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getRoleName(i);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRoleName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRoleLevel of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_getRoleLevel00
static int tolua_offlineTolua_OfflineMainPlayer_getRoleLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
  int i = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRoleLevel'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getRoleLevel(i);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRoleLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRoleCareer of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_getRoleCareer00
static int tolua_offlineTolua_OfflineMainPlayer_getRoleCareer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
  int i = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRoleCareer'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getRoleCareer(i);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRoleCareer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getOffExplength of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_getOffExplength00
static int tolua_offlineTolua_OfflineMainPlayer_getOffExplength00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getOffExplength'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getOffExplength();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getOffExplength'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setSelectRoleIndex of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_setSelectRoleIndex00
static int tolua_offlineTolua_OfflineMainPlayer_setSelectRoleIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
  int i = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setSelectRoleIndex'",NULL);
#endif
  {
   self->setSelectRoleIndex(i);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setSelectRoleIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSelectRoleIndex of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_getSelectRoleIndex00
static int tolua_offlineTolua_OfflineMainPlayer_getSelectRoleIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSelectRoleIndex'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getSelectRoleIndex();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSelectRoleIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: requestEnterGameMudWorld of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_requestEnterGameMudWorld00
static int tolua_offlineTolua_OfflineMainPlayer_requestEnterGameMudWorld00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'requestEnterGameMudWorld'",NULL);
#endif
  {
   self->requestEnterGameMudWorld();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'requestEnterGameMudWorld'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: requestRoomList of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_requestRoomList00
static int tolua_offlineTolua_OfflineMainPlayer_requestRoomList00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'requestRoomList'",NULL);
#endif
  {
   self->requestRoomList();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'requestRoomList'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: requestEmotion of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_requestEmotion00
static int tolua_offlineTolua_OfflineMainPlayer_requestEmotion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
  const char* szEmotion = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'requestEmotion'",NULL);
#endif
  {
   self->requestEmotion(szEmotion);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'requestEmotion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: requestRoleLogout of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_requestRoleLogout00
static int tolua_offlineTolua_OfflineMainPlayer_requestRoleLogout00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'requestRoleLogout'",NULL);
#endif
  {
   self->requestRoleLogout();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'requestRoleLogout'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: requestStealexp of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_requestStealexp00
static int tolua_offlineTolua_OfflineMainPlayer_requestStealexp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"tdr_ulonglong",0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
  tdr_ulonglong RoleID = *((tdr_ulonglong*)  tolua_tousertype(tolua_S,2,0));
  const char* szName = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'requestStealexp'",NULL);
#endif
  {
   self->requestStealexp(RoleID,szName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'requestStealexp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setGameState of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_setGameState00
static int tolua_offlineTolua_OfflineMainPlayer_setGameState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
  int i = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setGameState'",NULL);
#endif
  {
   self->setGameState(i);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setGameState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getGameState of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_getGameState00
static int tolua_offlineTolua_OfflineMainPlayer_getGameState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getGameState'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getGameState();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getGameState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsSamePlayer of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_IsSamePlayer00
static int tolua_offlineTolua_OfflineMainPlayer_IsSamePlayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
  int i = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsSamePlayer'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsSamePlayer(i);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsSamePlayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: requestLeaveChatRoom of class  OfflineMainPlayer */
#ifndef TOLUA_DISABLE_tolua_offlineTolua_OfflineMainPlayer_requestLeaveChatRoom00
static int tolua_offlineTolua_OfflineMainPlayer_requestLeaveChatRoom00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"OfflineMainPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  OfflineMainPlayer* self = (OfflineMainPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'requestLeaveChatRoom'",NULL);
#endif
  {
   self->requestLeaveChatRoom();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'requestLeaveChatRoom'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_offlineTolua_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_constant(tolua_S,"OFFLINE",OFFLINE);
  tolua_constant(tolua_S,"CLIENT_ONLINE",CLIENT_ONLINE);
  tolua_constant(tolua_S,"MUD_ONLINE",MUD_ONLINE);
  tolua_cclass(tolua_S,"OfflineManager","OfflineManager","",NULL);
  tolua_beginmodule(tolua_S,"OfflineManager");
   tolua_function(tolua_S,"bitAnd",tolua_offlineTolua_OfflineManager_bitAnd00);
   tolua_function(tolua_S,"end1",tolua_offlineTolua_OfflineManager_end100);
   tolua_function(tolua_S,"end",tolua_offlineTolua_OfflineManager_end00);
   tolua_function(tolua_S,"setsize",tolua_offlineTolua_OfflineManager_setsize00);
   tolua_function(tolua_S,"MoveWindowDown",tolua_offlineTolua_OfflineManager_MoveWindowDown00);
   tolua_function(tolua_S,"MoveWindowUp",tolua_offlineTolua_OfflineManager_MoveWindowUp00);
   tolua_function(tolua_S,"SendUIEvent",tolua_offlineTolua_OfflineManager_SendUIEvent00);
   tolua_function(tolua_S,"MiniWindow",tolua_offlineTolua_OfflineManager_MiniWindow00);
   tolua_function(tolua_S,"saveusername",tolua_offlineTolua_OfflineManager_saveusername00);
   tolua_function(tolua_S,"clearusername",tolua_offlineTolua_OfflineManager_clearusername00);
   tolua_function(tolua_S,"getlastname",tolua_offlineTolua_OfflineManager_getlastname00);
   tolua_function(tolua_S,"getlastusercheck",tolua_offlineTolua_OfflineManager_getlastusercheck00);
   tolua_function(tolua_S,"OpenUrl",tolua_offlineTolua_OfflineManager_OpenUrl00);
  tolua_endmodule(tolua_S);
  tolua_constant(tolua_S,"Colligation_Channel",Colligation_Channel);
  tolua_constant(tolua_S,"World_Channel",World_Channel);
  tolua_constant(tolua_S,"Private_Channel",Private_Channel);
  tolua_constant(tolua_S,"Clan_Channel",Clan_Channel);
  tolua_cclass(tolua_S,"DirWorldInfo","DirWorldInfo","",NULL);
  tolua_beginmodule(tolua_S,"DirWorldInfo");
   tolua_variable(tolua_S,"name",tolua_get_DirWorldInfo_name,tolua_set_DirWorldInfo_name);
   tolua_variable(tolua_S,"recommend",tolua_get_DirWorldInfo_recommend,tolua_set_DirWorldInfo_recommend);
   tolua_variable(tolua_S,"busy",tolua_get_DirWorldInfo_busy,tolua_set_DirWorldInfo_busy);
   tolua_variable(tolua_S,"ip",tolua_get_DirWorldInfo_ip,tolua_set_DirWorldInfo_ip);
   tolua_variable(tolua_S,"Ip_Bak1",tolua_get_DirWorldInfo_Ip_Bak1,tolua_set_DirWorldInfo_Ip_Bak1);
   tolua_variable(tolua_S,"Ip_Bak2",tolua_get_DirWorldInfo_Ip_Bak2,tolua_set_DirWorldInfo_Ip_Bak2);
   tolua_variable(tolua_S,"port",tolua_get_DirWorldInfo_unsigned_port,tolua_set_DirWorldInfo_unsigned_port);
   tolua_variable(tolua_S,"worldid",tolua_get_DirWorldInfo_unsigned_worldid,tolua_set_DirWorldInfo_unsigned_worldid);
   tolua_variable(tolua_S,"RegionID",tolua_get_DirWorldInfo_unsigned_RegionID,tolua_set_DirWorldInfo_unsigned_RegionID);
   tolua_variable(tolua_S,"AuthIP",tolua_get_DirWorldInfo_AuthIP,tolua_set_DirWorldInfo_AuthIP);
   tolua_variable(tolua_S,"authIP_Bak1",tolua_get_DirWorldInfo_authIP_Bak1,tolua_set_DirWorldInfo_authIP_Bak1);
   tolua_variable(tolua_S,"authIP_Bak2",tolua_get_DirWorldInfo_authIP_Bak2,tolua_set_DirWorldInfo_authIP_Bak2);
   tolua_variable(tolua_S,"AuthPort",tolua_get_DirWorldInfo_unsigned_AuthPort,tolua_set_DirWorldInfo_unsigned_AuthPort);
   tolua_variable(tolua_S,"mudIp",tolua_get_DirWorldInfo_mudIp,tolua_set_DirWorldInfo_mudIp);
   tolua_variable(tolua_S,"mudPort",tolua_get_DirWorldInfo_unsigned_mudPort,tolua_set_DirWorldInfo_unsigned_mudPort);
   tolua_variable(tolua_S,"MudIp_Bak1",tolua_get_DirWorldInfo_MudIp_Bak1,tolua_set_DirWorldInfo_MudIp_Bak1);
   tolua_variable(tolua_S,"MudIp_Bak2",tolua_get_DirWorldInfo_MudIp_Bak2,tolua_set_DirWorldInfo_MudIp_Bak2);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"DirBigWorldInfo","DirBigWorldInfo","",tolua_collect_DirBigWorldInfo);
  #else
  tolua_cclass(tolua_S,"DirBigWorldInfo","DirBigWorldInfo","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"DirBigWorldInfo");
   tolua_function(tolua_S,"new",tolua_offlineTolua_DirBigWorldInfo_new00);
   tolua_function(tolua_S,"new_local",tolua_offlineTolua_DirBigWorldInfo_new00_local);
   tolua_function(tolua_S,".call",tolua_offlineTolua_DirBigWorldInfo_new00_local);
   tolua_function(tolua_S,"getName",tolua_offlineTolua_DirBigWorldInfo_getName00);
   tolua_function(tolua_S,"getNumWorld",tolua_offlineTolua_DirBigWorldInfo_getNumWorld00);
   tolua_function(tolua_S,"getFlag",tolua_offlineTolua_DirBigWorldInfo_getFlag00);
   tolua_function(tolua_S,"getIthWorld",tolua_offlineTolua_DirBigWorldInfo_getIthWorld00);
   tolua_function(tolua_S,"addWorldInfo",tolua_offlineTolua_DirBigWorldInfo_addWorldInfo00);
   tolua_function(tolua_S,"addWorldInfo",tolua_offlineTolua_DirBigWorldInfo_addWorldInfo01);
   tolua_variable(tolua_S,"m_Name",tolua_get_DirBigWorldInfo_m_Name,tolua_set_DirBigWorldInfo_m_Name);
   tolua_variable(tolua_S,"m_strUrl",tolua_get_DirBigWorldInfo_m_strUrl,tolua_set_DirBigWorldInfo_m_strUrl);
   tolua_variable(tolua_S,"m_Worlds",tolua_get_DirBigWorldInfo_m_Worlds,tolua_set_DirBigWorldInfo_m_Worlds);
   tolua_variable(tolua_S,"m_szServer",tolua_get_DirBigWorldInfo_m_szServer,tolua_set_DirBigWorldInfo_m_szServer);
   tolua_variable(tolua_S,"m_uStatusFlag",tolua_get_DirBigWorldInfo_unsigned_m_uStatusFlag,tolua_set_DirBigWorldInfo_unsigned_m_uStatusFlag);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"MudNetMgr","MudNetMgr","",NULL);
  tolua_beginmodule(tolua_S,"MudNetMgr");
   tolua_function(tolua_S,"getBigWorldNum",tolua_offlineTolua_MudNetMgr_getBigWorldNum00);
   tolua_function(tolua_S,"getBidWorldInfo",tolua_offlineTolua_MudNetMgr_getBidWorldInfo00);
   tolua_function(tolua_S,"isDirConnected",tolua_offlineTolua_MudNetMgr_isDirConnected00);
   tolua_function(tolua_S,"getSevernums",tolua_offlineTolua_MudNetMgr_getSevernums00);
   tolua_function(tolua_S,"getSeverName",tolua_offlineTolua_MudNetMgr_getSeverName00);
   tolua_function(tolua_S,"getBigWorldindex",tolua_offlineTolua_MudNetMgr_getBigWorldindex00);
   tolua_function(tolua_S,"getWorldNum",tolua_offlineTolua_MudNetMgr_getWorldNum00);
   tolua_function(tolua_S,"getWorldName",tolua_offlineTolua_MudNetMgr_getWorldName00);
   tolua_function(tolua_S,"getWorldState",tolua_offlineTolua_MudNetMgr_getWorldState00);
   tolua_function(tolua_S,"getWorldindex",tolua_offlineTolua_MudNetMgr_getWorldindex00);
   tolua_function(tolua_S,"getWorldIp",tolua_offlineTolua_MudNetMgr_getWorldIp00);
   tolua_function(tolua_S,"getWorldPort",tolua_offlineTolua_MudNetMgr_getWorldPort00);
   tolua_function(tolua_S,"getWorldId",tolua_offlineTolua_MudNetMgr_getWorldId00);
   tolua_function(tolua_S,"getSelectWorldState",tolua_offlineTolua_MudNetMgr_getSelectWorldState00);
   tolua_function(tolua_S,"getSelectWorldName",tolua_offlineTolua_MudNetMgr_getSelectWorldName00);
   tolua_function(tolua_S,"getSelectBigWorldName",tolua_offlineTolua_MudNetMgr_getSelectBigWorldName00);
   tolua_function(tolua_S,"getChatmsgtype",tolua_offlineTolua_MudNetMgr_getChatmsgtype00);
   tolua_function(tolua_S,"getChatmsgrolename",tolua_offlineTolua_MudNetMgr_getChatmsgrolename00);
   tolua_function(tolua_S,"getChatmsgContent",tolua_offlineTolua_MudNetMgr_getChatmsgContent00);
   tolua_function(tolua_S,"getTextTipsnums",tolua_offlineTolua_MudNetMgr_getTextTipsnums00);
   tolua_function(tolua_S,"getTextTip",tolua_offlineTolua_MudNetMgr_getTextTip00);
   tolua_function(tolua_S,"getMessageNums",tolua_offlineTolua_MudNetMgr_getMessageNums00);
   tolua_function(tolua_S,"getMessagestr",tolua_offlineTolua_MudNetMgr_getMessagestr00);
   tolua_function(tolua_S,"getMessagestyle",tolua_offlineTolua_MudNetMgr_getMessagestyle00);
   tolua_function(tolua_S,"getLastSelectArea",tolua_offlineTolua_MudNetMgr_getLastSelectArea00);
   tolua_function(tolua_S,"getLastSelectWorld",tolua_offlineTolua_MudNetMgr_getLastSelectWorld00);
   tolua_function(tolua_S,"addMessagestr",tolua_offlineTolua_MudNetMgr_addMessagestr00);
   tolua_function(tolua_S,"delMessage",tolua_offlineTolua_MudNetMgr_delMessage00);
   tolua_function(tolua_S,"setBigworldindex",tolua_offlineTolua_MudNetMgr_setBigworldindex00);
   tolua_function(tolua_S,"setWorldindex",tolua_offlineTolua_MudNetMgr_setWorldindex00);
   tolua_function(tolua_S,"requestLoginGameServer",tolua_offlineTolua_MudNetMgr_requestLoginGameServer00);
   tolua_function(tolua_S,"requestSendChat",tolua_offlineTolua_MudNetMgr_requestSendChat00);
   tolua_function(tolua_S,"requestChannelSet",tolua_offlineTolua_MudNetMgr_requestChannelSet00);
   tolua_function(tolua_S,"requestLogout",tolua_offlineTolua_MudNetMgr_requestLogout00);
   tolua_function(tolua_S,"ClearTexttip",tolua_offlineTolua_MudNetMgr_ClearTexttip00);
   tolua_function(tolua_S,"startZone",tolua_offlineTolua_MudNetMgr_startZone00);
   tolua_function(tolua_S,"ResetRfreshTime",tolua_offlineTolua_MudNetMgr_ResetRfreshTime00);
   tolua_function(tolua_S,"getLoginResult",tolua_offlineTolua_MudNetMgr_getLoginResult00);
   tolua_function(tolua_S,"ReFreshServer",tolua_offlineTolua_MudNetMgr_ReFreshServer00);
  tolua_endmodule(tolua_S);
  tolua_constant(tolua_S,"SERVER_LINK",SERVER_LINK);
  tolua_constant(tolua_S,"ITEM_LINK",ITEM_LINK);
  tolua_constant(tolua_S,"ARM_LINK",ARM_LINK);
  tolua_constant(tolua_S,"MISSION_LINK",MISSION_LINK);
  tolua_constant(tolua_S,"POSTION_LINK",POSTION_LINK);
  tolua_constant(tolua_S,"SECONDARY_SKILL_LINK",SECONDARY_SKILL_LINK);
  tolua_constant(tolua_S,"PET_LINK",PET_LINK);
  tolua_constant(tolua_S,"ZONGHE_CHANNEL",ZONGHE_CHANNEL);
  tolua_constant(tolua_S,"CHATROOM_CHANNEL",CHATROOM_CHANNEL);
  tolua_constant(tolua_S,"WORLD_CHANNEL",WORLD_CHANNEL);
  tolua_constant(tolua_S,"CLAN_CHANNEL",CLAN_CHANNEL);
  tolua_constant(tolua_S,"PRIVATE_CHANNEL",PRIVATE_CHANNEL);
  tolua_constant(tolua_S,"CL_CHAT_SAY",CL_CHAT_SAY);
  tolua_constant(tolua_S,"CL_CHAT_TEAM",CL_CHAT_TEAM);
  tolua_constant(tolua_S,"CL_CHAT_PRIVATE",CL_CHAT_PRIVATE);
  tolua_constant(tolua_S,"CL_CHAT_CLAN",CL_CHAT_CLAN);
  tolua_constant(tolua_S,"CL_CHAT_WORLD",CL_CHAT_WORLD);
  tolua_constant(tolua_S,"CL_CHAT_ITEM",CL_CHAT_ITEM);
  tolua_constant(tolua_S,"CL_CHAT_FRIEND",CL_CHAT_FRIEND);
  tolua_constant(tolua_S,"CL_CHAT_BATTLE",CL_CHAT_BATTLE);
  tolua_constant(tolua_S,"CL_CHAT_MUD_ROOM",CL_CHAT_MUD_ROOM);
  tolua_constant(tolua_S,"CL_CHAT_WORLD_LEVEL",CL_CHAT_WORLD_LEVEL);
  tolua_constant(tolua_S,"CL_CHAT_WORLD_COOL",CL_CHAT_WORLD_COOL);
  tolua_cclass(tolua_S,"tagItemLink_ToLua","tagItemLink_ToLua","",NULL);
  tolua_beginmodule(tolua_S,"tagItemLink_ToLua");
   tolua_variable(tolua_S,"m_nItemId",tolua_get_tagItemLink_ToLua_m_nItemId,tolua_set_tagItemLink_ToLua_m_nItemId);
   tolua_variable(tolua_S,"m_nItemDur",tolua_get_tagItemLink_ToLua_m_nItemDur,tolua_set_tagItemLink_ToLua_m_nItemDur);
   tolua_variable(tolua_S,"InstVal1",tolua_get_tagItemLink_ToLua_InstVal1,tolua_set_tagItemLink_ToLua_InstVal1);
   tolua_variable(tolua_S,"InstVal2",tolua_get_tagItemLink_ToLua_InstVal2,tolua_set_tagItemLink_ToLua_InstVal2);
   tolua_variable(tolua_S,"InstVal3",tolua_get_tagItemLink_ToLua_InstVal3,tolua_set_tagItemLink_ToLua_InstVal3);
   tolua_variable(tolua_S,"Flag",tolua_get_tagItemLink_ToLua_Flag,tolua_set_tagItemLink_ToLua_Flag);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"tagArmLink_ToLua","tagArmLink_ToLua","",NULL);
  tolua_beginmodule(tolua_S,"tagArmLink_ToLua");
   tolua_variable(tolua_S,"m_arm",tolua_get_tagArmLink_ToLua_m_arm,tolua_set_tagArmLink_ToLua_m_arm);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"tagMissionLink_ToLua","tagMissionLink_ToLua","",NULL);
  tolua_beginmodule(tolua_S,"tagMissionLink_ToLua");
   tolua_variable(tolua_S,"m_MissionId",tolua_get_tagMissionLink_ToLua_m_MissionId,tolua_set_tagMissionLink_ToLua_m_MissionId);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"tagPostionLink_ToLua","tagPostionLink_ToLua","",NULL);
  tolua_beginmodule(tolua_S,"tagPostionLink_ToLua");
   tolua_variable(tolua_S,"m_nMapId",tolua_get_tagPostionLink_ToLua_m_nMapId,tolua_set_tagPostionLink_ToLua_m_nMapId);
   tolua_variable(tolua_S,"m_nX",tolua_get_tagPostionLink_ToLua_m_nX,tolua_set_tagPostionLink_ToLua_m_nX);
   tolua_variable(tolua_S,"m_nY",tolua_get_tagPostionLink_ToLua_m_nY,tolua_set_tagPostionLink_ToLua_m_nY);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"tagSecondarySkillLink_ToLua","tagSecondarySkillLink_ToLua","",NULL);
  tolua_beginmodule(tolua_S,"tagSecondarySkillLink_ToLua");
   tolua_variable(tolua_S,"SkillType",tolua_get_tagSecondarySkillLink_ToLua_SkillType,tolua_set_tagSecondarySkillLink_ToLua_SkillType);
   tolua_variable(tolua_S,"SkillLevel",tolua_get_tagSecondarySkillLink_ToLua_SkillLevel,tolua_set_tagSecondarySkillLink_ToLua_SkillLevel);
   tolua_variable(tolua_S,"SkillVal",tolua_get_tagSecondarySkillLink_ToLua_SkillVal,tolua_set_tagSecondarySkillLink_ToLua_SkillVal);
   tolua_variable(tolua_S,"RecipeInfo",tolua_get_tagSecondarySkillLink_ToLua_RecipeInfo,tolua_set_tagSecondarySkillLink_ToLua_RecipeInfo);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"tagPetLink_ToLua","tagPetLink_ToLua","",NULL);
  tolua_beginmodule(tolua_S,"tagPetLink_ToLua");
   tolua_variable(tolua_S,"PetWid",tolua_get_tagPetLink_ToLua_PetWid,tolua_set_tagPetLink_ToLua_PetWid);
   tolua_variable(tolua_S,"PetName",tolua_get_tagPetLink_ToLua_PetName,tolua_set_tagPetLink_ToLua_PetName);
   tolua_variable(tolua_S,"OwnerName",tolua_get_tagPetLink_ToLua_OwnerName,tolua_set_tagPetLink_ToLua_OwnerName);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"tagLinkData_ToLua","tagLinkData_ToLua","",NULL);
  tolua_beginmodule(tolua_S,"tagLinkData_ToLua");
   tolua_variable(tolua_S,"m_SystemLink",tolua_get_tagLinkData_ToLua_m_SystemLink,tolua_set_tagLinkData_ToLua_m_SystemLink);
   tolua_variable(tolua_S,"m_ItemLink",tolua_get_tagLinkData_ToLua_m_ItemLink,tolua_set_tagLinkData_ToLua_m_ItemLink);
   tolua_variable(tolua_S,"m_RoleArmLink",tolua_get_tagLinkData_ToLua_m_RoleArmLink,tolua_set_tagLinkData_ToLua_m_RoleArmLink);
   tolua_variable(tolua_S,"m_MissionLink",tolua_get_tagLinkData_ToLua_m_MissionLink,tolua_set_tagLinkData_ToLua_m_MissionLink);
   tolua_variable(tolua_S,"m_PostionLink",tolua_get_tagLinkData_ToLua_m_PostionLink,tolua_set_tagLinkData_ToLua_m_PostionLink);
   tolua_variable(tolua_S,"m_SecondarySkillLink",tolua_get_tagLinkData_ToLua_m_SecondarySkillLink,tolua_set_tagLinkData_ToLua_m_SecondarySkillLink);
   tolua_variable(tolua_S,"m_PetLink",tolua_get_tagLinkData_ToLua_m_PetLink,tolua_set_tagLinkData_ToLua_m_PetLink);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"tagChatLinkData_ToLua","tagChatLinkData_ToLua","",tolua_collect_tagChatLinkData_ToLua);
  #else
  tolua_cclass(tolua_S,"tagChatLinkData_ToLua","tagChatLinkData_ToLua","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"tagChatLinkData_ToLua");
   tolua_variable(tolua_S,"Type",tolua_get_tagChatLinkData_ToLua_Type,tolua_set_tagChatLinkData_ToLua_Type);
   tolua_variable(tolua_S,"Data",tolua_get_tagChatLinkData_ToLua_Data,tolua_set_tagChatLinkData_ToLua_Data);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"tagChatLink_ToLua","tagChatLink_ToLua","",NULL);
  tolua_beginmodule(tolua_S,"tagChatLink_ToLua");
   tolua_variable(tolua_S,"m_nLinkNum",tolua_get_tagChatLink_ToLua_m_nLinkNum,tolua_set_tagChatLink_ToLua_m_nLinkNum);
   tolua_array(tolua_S,"m_LinkData",tolua_get_offlineTolua_tagChatLink_ToLua_m_LinkData,tolua_set_offlineTolua_tagChatLink_ToLua_m_LinkData);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"ChatManager","ChatManager","",NULL);
  tolua_beginmodule(tolua_S,"ChatManager");
   tolua_function(tolua_S,"getItemLink",tolua_offlineTolua_ChatManager_getItemLink00);
   tolua_function(tolua_S,"getArmLink",tolua_offlineTolua_ChatManager_getArmLink00);
   tolua_function(tolua_S,"strReplace",tolua_offlineTolua_ChatManager_strReplace00);
   tolua_function(tolua_S,"getItem",tolua_offlineTolua_ChatManager_getItem00);
   tolua_function(tolua_S,"getChatmsgtype",tolua_offlineTolua_ChatManager_getChatmsgtype00);
   tolua_function(tolua_S,"getChatmsgrolename",tolua_offlineTolua_ChatManager_getChatmsgrolename00);
   tolua_function(tolua_S,"getChatmsgContent",tolua_offlineTolua_ChatManager_getChatmsgContent00);
   tolua_function(tolua_S,"getChatreqrolename",tolua_offlineTolua_ChatManager_getChatreqrolename00);
   tolua_function(tolua_S,"getChatreqContent",tolua_offlineTolua_ChatManager_getChatreqContent00);
   tolua_function(tolua_S,"getChatreqType",tolua_offlineTolua_ChatManager_getChatreqType00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"OfflineWatchPlayer","OfflineWatchPlayer","",NULL);
  tolua_beginmodule(tolua_S,"OfflineWatchPlayer");
   tolua_function(tolua_S,"sendWatchRequest",tolua_offlineTolua_OfflineWatchPlayer_sendWatchRequest00);
   tolua_function(tolua_S,"getWatchPlayerMemId",tolua_offlineTolua_OfflineWatchPlayer_getWatchPlayerMemId00);
   tolua_function(tolua_S,"getItem",tolua_offlineTolua_OfflineWatchPlayer_getItem00);
   tolua_function(tolua_S,"setAvatarAttrib",tolua_offlineTolua_OfflineWatchPlayer_setAvatarAttrib00);
   tolua_function(tolua_S,"IsSawArmItem",tolua_offlineTolua_OfflineWatchPlayer_IsSawArmItem00);
   tolua_function(tolua_S,"getWatchPlayer",tolua_offlineTolua_OfflineWatchPlayer_getWatchPlayer00);
  tolua_endmodule(tolua_S);
  tolua_constant(tolua_S,"CHATROOMTYPE",CHATROOMTYPE);
  tolua_constant(tolua_S,"CLANTYPE",CLANTYPE);
  tolua_constant(tolua_S,"FRIENDTYPE",FRIENDTYPE);
  tolua_cclass(tolua_S,"OfflinePlayer","OfflinePlayer","",NULL);
  tolua_beginmodule(tolua_S,"OfflinePlayer");
   tolua_variable(tolua_S,"m_RoleID",tolua_get_OfflinePlayer_m_RoleID,tolua_set_OfflinePlayer_m_RoleID);
   tolua_variable(tolua_S,"m_RoleName",tolua_get_OfflinePlayer_m_RoleName,tolua_set_OfflinePlayer_m_RoleName);
   tolua_variable(tolua_S,"m_Emotion",tolua_get_OfflinePlayer_m_Emotion,tolua_set_OfflinePlayer_m_Emotion);
   tolua_variable(tolua_S,"m_uCareer",tolua_get_OfflinePlayer_m_uCareer,tolua_set_OfflinePlayer_m_uCareer);
   tolua_variable(tolua_S,"m_uLevel",tolua_get_OfflinePlayer_m_uLevel,tolua_set_OfflinePlayer_m_uLevel);
   tolua_variable(tolua_S,"m_uGender",tolua_get_OfflinePlayer_m_uGender,tolua_set_OfflinePlayer_m_uGender);
   tolua_variable(tolua_S,"m_Onlinetype",tolua_get_OfflinePlayer_m_Onlinetype,tolua_set_OfflinePlayer_m_Onlinetype);
   tolua_variable(tolua_S,"m_nhead",tolua_get_OfflinePlayer_m_nhead,tolua_set_OfflinePlayer_m_nhead);
   tolua_variable(tolua_S,"m_nface",tolua_get_OfflinePlayer_m_nface,tolua_set_OfflinePlayer_m_nface);
   tolua_variable(tolua_S,"m_nhair",tolua_get_OfflinePlayer_m_nhair,tolua_set_OfflinePlayer_m_nhair);
   tolua_variable(tolua_S,"m_Relationtype",tolua_get_OfflinePlayer_m_Relationtype,tolua_set_OfflinePlayer_m_Relationtype);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"RelationManager","RelationManager","",NULL);
  tolua_beginmodule(tolua_S,"RelationManager");
   tolua_function(tolua_S,"getFriendInfo",tolua_offlineTolua_RelationManager_getFriendInfo00);
   tolua_function(tolua_S,"getFriendInfoByName",tolua_offlineTolua_RelationManager_getFriendInfoByName00);
   tolua_function(tolua_S,"getClanMemberInfo",tolua_offlineTolua_RelationManager_getClanMemberInfo00);
   tolua_function(tolua_S,"requestAddFriend",tolua_offlineTolua_RelationManager_requestAddFriend00);
   tolua_function(tolua_S,"requestDeleteFriend",tolua_offlineTolua_RelationManager_requestDeleteFriend00);
   tolua_function(tolua_S,"responseAddFriend",tolua_offlineTolua_RelationManager_responseAddFriend00);
   tolua_function(tolua_S,"getFriendNum",tolua_offlineTolua_RelationManager_getFriendNum00);
   tolua_function(tolua_S,"getClanMemeberNum",tolua_offlineTolua_RelationManager_getClanMemeberNum00);
   tolua_function(tolua_S,"getReqName",tolua_offlineTolua_RelationManager_getReqName00);
   tolua_function(tolua_S,"getReqInfo",tolua_offlineTolua_RelationManager_getReqInfo00);
   tolua_function(tolua_S,"getReqOnlinetype",tolua_offlineTolua_RelationManager_getReqOnlinetype00);
   tolua_function(tolua_S,"IsFriend",tolua_offlineTolua_RelationManager_IsFriend00);
   tolua_function(tolua_S,"requestgetFriendinfo",tolua_offlineTolua_RelationManager_requestgetFriendinfo00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"ChatRoomManager","ChatRoomManager","",NULL);
  tolua_beginmodule(tolua_S,"ChatRoomManager");
   tolua_function(tolua_S,"requestLeaveRoom",tolua_offlineTolua_ChatRoomManager_requestLeaveRoom00);
   tolua_function(tolua_S,"requestEnterRoom",tolua_offlineTolua_ChatRoomManager_requestEnterRoom00);
   tolua_function(tolua_S,"getPlayerInfo",tolua_offlineTolua_ChatRoomManager_getPlayerInfo00);
   tolua_function(tolua_S,"getCurrentRoomrolenums",tolua_offlineTolua_ChatRoomManager_getCurrentRoomrolenums00);
   tolua_function(tolua_S,"getCurrentChatRoomIndex",tolua_offlineTolua_ChatRoomManager_getCurrentChatRoomIndex00);
   tolua_function(tolua_S,"getChatRoomNums",tolua_offlineTolua_ChatRoomManager_getChatRoomNums00);
   tolua_function(tolua_S,"getRoomPlayerNums",tolua_offlineTolua_ChatRoomManager_getRoomPlayerNums00);
   tolua_function(tolua_S,"clear",tolua_offlineTolua_ChatRoomManager_clear00);
   tolua_function(tolua_S,"getCanConnnect",tolua_offlineTolua_ChatRoomManager_getCanConnnect00);
  tolua_endmodule(tolua_S);
  tolua_constant(tolua_S,"LOGIN_STATE",LOGIN_STATE);
  tolua_constant(tolua_S,"ROOMLIST_STATE",ROOMLIST_STATE);
  tolua_constant(tolua_S,"CHATROOM_STATE",CHATROOM_STATE);
  tolua_constant(tolua_S,"ROLELIST_STATE",ROLELIST_STATE);
  tolua_cclass(tolua_S,"OfflineMainPlayer","OfflineMainPlayer","",NULL);
  tolua_beginmodule(tolua_S,"OfflineMainPlayer");
   tolua_function(tolua_S,"getName",tolua_offlineTolua_OfflineMainPlayer_getName00);
   tolua_function(tolua_S,"getLevel",tolua_offlineTolua_OfflineMainPlayer_getLevel00);
   tolua_function(tolua_S,"getCareer",tolua_offlineTolua_OfflineMainPlayer_getCareer00);
   tolua_function(tolua_S,"getGender",tolua_offlineTolua_OfflineMainPlayer_getGender00);
   tolua_function(tolua_S,"getOfflineExp",tolua_offlineTolua_OfflineMainPlayer_getOfflineExp00);
   tolua_function(tolua_S,"getMaxOfflineExp",tolua_offlineTolua_OfflineMainPlayer_getMaxOfflineExp00);
   tolua_function(tolua_S,"getEmotion",tolua_offlineTolua_OfflineMainPlayer_getEmotion00);
   tolua_function(tolua_S,"getHead",tolua_offlineTolua_OfflineMainPlayer_getHead00);
   tolua_function(tolua_S,"getHair",tolua_offlineTolua_OfflineMainPlayer_getHair00);
   tolua_function(tolua_S,"getFace",tolua_offlineTolua_OfflineMainPlayer_getFace00);
   tolua_function(tolua_S,"getRoleId",tolua_offlineTolua_OfflineMainPlayer_getRoleId00);
   tolua_function(tolua_S,"getRoleListNums",tolua_offlineTolua_OfflineMainPlayer_getRoleListNums00);
   tolua_function(tolua_S,"getRoleName",tolua_offlineTolua_OfflineMainPlayer_getRoleName00);
   tolua_function(tolua_S,"getRoleLevel",tolua_offlineTolua_OfflineMainPlayer_getRoleLevel00);
   tolua_function(tolua_S,"getRoleCareer",tolua_offlineTolua_OfflineMainPlayer_getRoleCareer00);
   tolua_function(tolua_S,"getOffExplength",tolua_offlineTolua_OfflineMainPlayer_getOffExplength00);
   tolua_function(tolua_S,"setSelectRoleIndex",tolua_offlineTolua_OfflineMainPlayer_setSelectRoleIndex00);
   tolua_function(tolua_S,"getSelectRoleIndex",tolua_offlineTolua_OfflineMainPlayer_getSelectRoleIndex00);
   tolua_function(tolua_S,"requestEnterGameMudWorld",tolua_offlineTolua_OfflineMainPlayer_requestEnterGameMudWorld00);
   tolua_function(tolua_S,"requestRoomList",tolua_offlineTolua_OfflineMainPlayer_requestRoomList00);
   tolua_function(tolua_S,"requestEmotion",tolua_offlineTolua_OfflineMainPlayer_requestEmotion00);
   tolua_function(tolua_S,"requestRoleLogout",tolua_offlineTolua_OfflineMainPlayer_requestRoleLogout00);
   tolua_function(tolua_S,"requestStealexp",tolua_offlineTolua_OfflineMainPlayer_requestStealexp00);
   tolua_function(tolua_S,"setGameState",tolua_offlineTolua_OfflineMainPlayer_setGameState00);
   tolua_function(tolua_S,"getGameState",tolua_offlineTolua_OfflineMainPlayer_getGameState00);
   tolua_function(tolua_S,"IsSamePlayer",tolua_offlineTolua_OfflineMainPlayer_IsSamePlayer00);
   tolua_function(tolua_S,"requestLeaveChatRoom",tolua_offlineTolua_OfflineMainPlayer_requestLeaveChatRoom00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_offlineTolua (lua_State* tolua_S) {
 return tolua_offlineTolua_open(tolua_S);
};
#endif

