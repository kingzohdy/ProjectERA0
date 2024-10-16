; 安装程序初始定义常量
!define PRODUCT_NAME "零纪元"
!define PRODUCT_VERSION "3.6.5"
!define PRODUCT_PUBLISHER "万游引力"
!define PRODUCT_WEB_SITE "http://www.ljy0.com"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\Era Zero"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"
!define SHORTCUT_MAIN "零纪元"
!define SHORTCUT_OFFLINE "零纪元Offline"
!define SHORTCUT_WEBSITE "访问《零纪元》官网"
!define SHORTCUT_UNINSTALL "卸载"
!define INPUT_PATH ".\${PRODUCT_VERSION}"


;http://www.ljy0.com/other/execstatistics.php?t=1&w=0
;t:1（完整客户端）或 2（微端）,99网吧安装包
;w:0（安装）或 1（卸载）
!define INSTALL_COMPLETE_PAGE "http://www.ljy0.com/other/execstatistics_cb.php?t=1&w=0"
!define UNINST_RESULT_PAGE "http://www.ljy0.com/other/execstatistics_cb.php?t=1&w=1"

!define COUNT_INSTALL

SetCompressor lzma
SetDatablockOptimize on
SetCompressorDictSize 64
SetCompress auto
AllowSkipFiles on
CRCCheck off

; ------ MUI 现代界面定义 ------
!include "MUI.nsh"

; MUI 预定义常量
!define MUI_ABORTWARNING
!define MUI_WELCOMEPAGE_TITLE "\r\n 欢迎使用${PRODUCT_NAME}安装向导"
!define MUI_ICON "零纪元.ico"
!define MUI_UNICON "零纪元.ico"
!define MUI_WELCOMEFINISHPAGE_BITMAP "游戏安装界面图1.bmp";165×292
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "游戏安装界面图1.bmp"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "游戏安装界面图2.bmp";151×55
!define MUI_HEADERIMAGE_UNBITMAP "游戏安装界面图2.bmp"
!define MUI_HEADERIMAGE_RIGHT

!define MUI_WELCOMEPAGE_TEXT "这是《${PRODUCT_NAME}》的安装向导，该向导将指引你完成《${PRODUCT_NAME}》的安装过程。\r\n\r\n在开始安装之前，建议您先关闭其他所有正在运行的程序。\r\n\r\n单击 [取消(C)] 退出向导，或单击 [下一步(N)] 继续安装。"

; 欢迎页面
!insertmacro MUI_PAGE_WELCOME
ShowInstDetails hide
; 许可协议页面
!define MUI_LICENSEPAGE_CHECKBOX
!insertmacro MUI_PAGE_LICENSE "许可协议.txt"
; 安装目录选择页面
!define MUI_DIRECTORYPAGE_TEXT_TOP "请选择《${PRODUCT_NAME}》的安装目录"
!insertmacro MUI_PAGE_DIRECTORY
; 开始菜单设置页面
var ICONS_GROUP
;!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "${PRODUCT_NAME}"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${PRODUCT_STARTMENU_REGVAL}"
!define MUI_STARTMENUPAGE_TEXT_CHECKBOX "不创建开始菜单"
!insertmacro MUI_PAGE_STARTMENU Application $ICONS_GROUP
; 安装过程页面
!insertmacro MUI_PAGE_INSTFILES
; 安装完成页面
!define MUI_FINISHPAGE_RUN "$INSTDIR\Launcher.exe"
!define MUI_FINISHPAGE_RUN_TEXT "启动 ${PRODUCT_NAME}"
!define MUI_FINISHPAGE_SHOWREADME
!define MUI_FINISHPAGE_SHOWREADME_FUNCTION CreateDesktop
!define MUI_FINISHPAGE_SHOWREADME_TEXT "创建桌面快捷方式"
!insertmacro MUI_PAGE_FINISH

; 安装卸载过程页面
!insertmacro MUI_UNPAGE_INSTFILES

; 安装界面包含的语言设置
!insertmacro MUI_LANGUAGE "SimpChinese"

; 安装预释放文件
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
; ------ MUI 现代界面定义结束 ------

!ifdef COUNT_INSTALL
  ReserveFile "${NSISDIR}\Plugins\InstallCount.dll"
!endif
ReserveFile "${NSISDIR}\Plugins\advsplash.dll"
ReserveFile "登陆图.bmp"
ReserveFile "游戏安装界面图1.bmp"
ReserveFile "游戏安装界面图2.bmp"

Name "${PRODUCT_NAME}"
OutFile "LJY_Setup_${PRODUCT_VERSION}.exe"
InstallDir "$PROGRAMFILES\${PRODUCT_NAME}"
InstallDirRegKey HKLM "Software\WanYou YL\Era Zero" "RootPath"
ShowInstDetails show
ShowUnInstDetails show
BrandingText "万游引力. Inc"

RequestExecutionLevel admin
Section "MainSection" SEC01
Find:
  InstallCount::StartUninstall $INSTDIR
    strCmp $R0 1 Running Uninst
 Running:
	MessageBox MB_ICONQUESTION|MB_RETRYCANCEL|MB_DEFBUTTON1 "请先关闭客户端，然后再重试！" IDRETRY Find
	Abort
 Uninst:
  SetOutPath "$INSTDIR"
  ;Delete "$INSTDIR\data\update.pkg"
  RMDir /r "$INSTDIR\data"
  SetOverwrite ifdiff
  File /a /r /x "*.cfg" /x "log" "${INPUT_PATH}\*"
  ;SetOverwrite off
  File /nonfatal /a /r "${INPUT_PATH}\*.cfg"
  CreateDirectory "$INSTDIR\log"

; 创建开始菜单快捷方式
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  CreateDirectory "$SMPROGRAMS\$ICONS_GROUP"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\${SHORTCUT_MAIN}.lnk" "$INSTDIR\Launcher.exe"
  ClearErrors
  FileOpen $0 "$INSTDIR\OffLine2.exe" r
  IfErrors continue
  FileClose $0
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\${SHORTCUT_OFFLINE}.lnk" "$INSTDIR\OffLine2.exe"
  continue:
  !insertmacro MUI_STARTMENU_WRITE_END
  !ifdef COUNT_INSTALL
    InstallCount::FinishInstall ${INSTALL_COMPLETE_PAGE}
  !endif
SectionEnd

Section -AdditionalIcons
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\${SHORTCUT_WEBSITE}.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\${SHORTCUT_UNINSTALL}.lnk" "$INSTDIR\uninst.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section -Post
  WriteRegStr HKLM "Software\WanYou YL\Era Zero" "RootPath" $INSTDIR
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "${PRODUCT_NAME}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

#-- 根据 NSIS 脚本编辑规则，所有 Function 区段必须放置在 Section 区段之后编写，以避免安装程序出现未可预知的问题。--#

Function .onInit
  System::Call 'kernel32::CreateMutexA(i 0, i 0, t "wanyouyl") i .r1 ?e'
  Pop $R0

  StrCmp $R0 0 +3
    MessageBox MB_OK|MB_ICONEXCLAMATION "安装程序已经在运行。"
    Abort
    
  InitPluginsDir
  File "/oname=$PLUGINSDIR\Splash.bmp" "登陆图.bmp"
  IfSilent +3
  advsplash::show 1000 600 400 -1 "$PLUGINSDIR\Splash" ; 使用闪屏插件显示闪屏
  Pop $0 ; $0 返回 '1' 表示用户提前关闭闪屏, 返回 '0' 表示闪屏正常结束, 返回 '-1' 表示闪屏显示出错
  
  !ifdef COUNT_INSTALL
    InstallCount::StartInstall ${PRODUCT_VERSION}
  !endif
FunctionEnd

Function .onInstSuccess
  IfSilent 0 +2
  CreateShortCut "$DESKTOP\零纪元.lnk" "$INSTDIR\Launcher.exe"
  
FunctionEnd

Function CreateDesktop
  CreateShortCut "$DESKTOP\零纪元.lnk" "$INSTDIR\Launcher.exe"
FunctionEnd

/******************************
 *  以下是安装程序的卸载部分  *
 ******************************/
Section Uninstall  
  !insertmacro MUI_STARTMENU_GETFOLDER "Application" $ICONS_GROUP
  
  Delete "$DESKTOP\零纪元.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\${SHORTCUT_UNINSTALL}.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\${SHORTCUT_WEBSITE}.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\${SHORTCUT_MAIN}.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\${SHORTCUT_OFFLINE}.lnk"
  RMDir "$SMPROGRAMS\$ICONS_GROUP\"

  RMDir /r "$INSTDIR\data"
  RMDir /r "$INSTDIR\resource"
  RMDir /r "$INSTDIR\sounds"
  RMDir /r "$INSTDIR\users"
  RMDir /r "$INSTDIR\log"
  Delete "$INSTDIR\client.cfg"
  Delete "$INSTDIR\Client.exe"
  Delete "$INSTDIR\d3dx9_36.dll"
  Delete "$INSTDIR\devil.dll"
  Delete "$INSTDIR\DirSvr.xml"
  Delete "$INSTDIR\fmodex.dll"
  Delete "$INSTDIR\ilu.dll"
  Delete "$INSTDIR\ilut.dll"
  Delete "$INSTDIR\Launcher.exe"
  Delete "$INSTDIR\libmysql.dll"
  Delete "$INSTDIR\luadll.dll"
  Delete "$INSTDIR\mfc80.dll"
  Delete "$INSTDIR\Microsoft.VC80.CRT.manifest"
  Delete "$INSTDIR\Microsoft.VC80.MFC.manifest"
  Delete "$INSTDIR\msvcp80.dll"
  Delete "$INSTDIR\msvcr80.dll"
  Delete "$INSTDIR\OgreMain.dll"
  Delete "$INSTDIR\RenderSystem_Direct3D9.dll"
  Delete "$INSTDIR\serverlist.data"
  Delete "$INSTDIR\shadercache.dat"
  Delete "$INSTDIR\version.data"
  Delete "$INSTDIR\zlib1.dll"
  Delete "$INSTDIR\offline2.cfg"
  Delete "$INSTDIR\offline2.exe"
  Delete "$INSTDIR\UserInfoLoginRecord.ini"
  Delete "$INSTDIR\avcodec.dll"
  Delete "$INSTDIR\avformat.dll"
  Delete "$INSTDIR\avutil.dll"
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\CrashRpt1300.dll"
  Delete "$INSTDIR\crashrpt_lang.ini"
  Delete "$INSTDIR\CrashSender1300.exe"
  Delete "$INSTDIR\SmartDownloader.exe"
  Delete "$INSTDIR\UserInfoLoginRecord.xml"
  Delete "$INSTDIR\dirsvr.xml"
  Delete "$INSTDIR\uninst.exe"

  RMDir  "$INSTDIR"  
  
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "Software\WanYou YL\Era Zero"
  DeleteRegKey /ifempty HKLM "Software\WanYou YL"
  SetAutoClose true
SectionEnd

#-- 根据 NSIS 脚本编辑规则，所有 Function 区段必须放置在 Section 区段之后编写，以避免安装程序出现未可预知的问题。--#

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "你确实要完全移除${PRODUCT_NAME}？" IDYES +2
  Abort
Find:
  InstallCount::StartUninstall $INSTDIR
    strCmp $R0 1 Running Uninst
 Running:
	MessageBox MB_ICONQUESTION|MB_RETRYCANCEL|MB_DEFBUTTON1 "请先关闭客户端，然后再重试！" IDRETRY Find
	Abort
 Uninst:
  !ifdef COUNT_INSTALL
    InstallCount::Uninstall ${UNINST_RESULT_PAGE}
  !endif
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "${PRODUCT_NAME} 已成功地从你的计算机移除。"
FunctionEnd
