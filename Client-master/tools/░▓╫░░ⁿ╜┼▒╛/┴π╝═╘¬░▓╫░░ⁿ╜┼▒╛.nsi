; ��װ�����ʼ���峣��
!define PRODUCT_NAME "���Ԫ"
!define PRODUCT_VERSION "3.6.5"
!define PRODUCT_PUBLISHER "��������"
!define PRODUCT_WEB_SITE "http://www.ljy0.com"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\Era Zero"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"
!define SHORTCUT_MAIN "���Ԫ"
!define SHORTCUT_OFFLINE "���ԪOffline"
!define SHORTCUT_WEBSITE "���ʡ����Ԫ������"
!define SHORTCUT_UNINSTALL "ж��"
!define INPUT_PATH ".\${PRODUCT_VERSION}"


;http://www.ljy0.com/other/execstatistics.php?t=1&w=0
;t:1�������ͻ��ˣ��� 2��΢�ˣ�,99���ɰ�װ��
;w:0����װ���� 1��ж�أ�
!define INSTALL_COMPLETE_PAGE "http://www.ljy0.com/other/execstatistics_cb.php?t=1&w=0"
!define UNINST_RESULT_PAGE "http://www.ljy0.com/other/execstatistics_cb.php?t=1&w=1"

!define COUNT_INSTALL

SetCompressor lzma
SetDatablockOptimize on
SetCompressorDictSize 64
SetCompress auto
AllowSkipFiles on
CRCCheck off

; ------ MUI �ִ����涨�� ------
!include "MUI.nsh"

; MUI Ԥ���峣��
!define MUI_ABORTWARNING
!define MUI_WELCOMEPAGE_TITLE "\r\n ��ӭʹ��${PRODUCT_NAME}��װ��"
!define MUI_ICON "���Ԫ.ico"
!define MUI_UNICON "���Ԫ.ico"
!define MUI_WELCOMEFINISHPAGE_BITMAP "��Ϸ��װ����ͼ1.bmp";165��292
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "��Ϸ��װ����ͼ1.bmp"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "��Ϸ��װ����ͼ2.bmp";151��55
!define MUI_HEADERIMAGE_UNBITMAP "��Ϸ��װ����ͼ2.bmp"
!define MUI_HEADERIMAGE_RIGHT

!define MUI_WELCOMEPAGE_TEXT "���ǡ�${PRODUCT_NAME}���İ�װ�򵼣����򵼽�ָ������ɡ�${PRODUCT_NAME}���İ�װ���̡�\r\n\r\n�ڿ�ʼ��װ֮ǰ���������ȹر����������������еĳ���\r\n\r\n���� [ȡ��(C)] �˳��򵼣��򵥻� [��һ��(N)] ������װ��"

; ��ӭҳ��
!insertmacro MUI_PAGE_WELCOME
ShowInstDetails hide
; ���Э��ҳ��
!define MUI_LICENSEPAGE_CHECKBOX
!insertmacro MUI_PAGE_LICENSE "���Э��.txt"
; ��װĿ¼ѡ��ҳ��
!define MUI_DIRECTORYPAGE_TEXT_TOP "��ѡ��${PRODUCT_NAME}���İ�װĿ¼"
!insertmacro MUI_PAGE_DIRECTORY
; ��ʼ�˵�����ҳ��
var ICONS_GROUP
;!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "${PRODUCT_NAME}"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${PRODUCT_STARTMENU_REGVAL}"
!define MUI_STARTMENUPAGE_TEXT_CHECKBOX "��������ʼ�˵�"
!insertmacro MUI_PAGE_STARTMENU Application $ICONS_GROUP
; ��װ����ҳ��
!insertmacro MUI_PAGE_INSTFILES
; ��װ���ҳ��
!define MUI_FINISHPAGE_RUN "$INSTDIR\Launcher.exe"
!define MUI_FINISHPAGE_RUN_TEXT "���� ${PRODUCT_NAME}"
!define MUI_FINISHPAGE_SHOWREADME
!define MUI_FINISHPAGE_SHOWREADME_FUNCTION CreateDesktop
!define MUI_FINISHPAGE_SHOWREADME_TEXT "���������ݷ�ʽ"
!insertmacro MUI_PAGE_FINISH

; ��װж�ع���ҳ��
!insertmacro MUI_UNPAGE_INSTFILES

; ��װ�����������������
!insertmacro MUI_LANGUAGE "SimpChinese"

; ��װԤ�ͷ��ļ�
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
; ------ MUI �ִ����涨����� ------

!ifdef COUNT_INSTALL
  ReserveFile "${NSISDIR}\Plugins\InstallCount.dll"
!endif
ReserveFile "${NSISDIR}\Plugins\advsplash.dll"
ReserveFile "��½ͼ.bmp"
ReserveFile "��Ϸ��װ����ͼ1.bmp"
ReserveFile "��Ϸ��װ����ͼ2.bmp"

Name "${PRODUCT_NAME}"
OutFile "LJY_Setup_${PRODUCT_VERSION}.exe"
InstallDir "$PROGRAMFILES\${PRODUCT_NAME}"
InstallDirRegKey HKLM "Software\WanYou YL\Era Zero" "RootPath"
ShowInstDetails show
ShowUnInstDetails show
BrandingText "��������. Inc"

RequestExecutionLevel admin
Section "MainSection" SEC01
Find:
  InstallCount::StartUninstall $INSTDIR
    strCmp $R0 1 Running Uninst
 Running:
	MessageBox MB_ICONQUESTION|MB_RETRYCANCEL|MB_DEFBUTTON1 "���ȹرտͻ��ˣ�Ȼ�������ԣ�" IDRETRY Find
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

; ������ʼ�˵���ݷ�ʽ
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

#-- ���� NSIS �ű��༭�������� Function ���α�������� Section ����֮���д���Ա��ⰲװ�������δ��Ԥ֪�����⡣--#

Function .onInit
  System::Call 'kernel32::CreateMutexA(i 0, i 0, t "wanyouyl") i .r1 ?e'
  Pop $R0

  StrCmp $R0 0 +3
    MessageBox MB_OK|MB_ICONEXCLAMATION "��װ�����Ѿ������С�"
    Abort
    
  InitPluginsDir
  File "/oname=$PLUGINSDIR\Splash.bmp" "��½ͼ.bmp"
  IfSilent +3
  advsplash::show 1000 600 400 -1 "$PLUGINSDIR\Splash" ; ʹ�����������ʾ����
  Pop $0 ; $0 ���� '1' ��ʾ�û���ǰ�ر�����, ���� '0' ��ʾ������������, ���� '-1' ��ʾ������ʾ����
  
  !ifdef COUNT_INSTALL
    InstallCount::StartInstall ${PRODUCT_VERSION}
  !endif
FunctionEnd

Function .onInstSuccess
  IfSilent 0 +2
  CreateShortCut "$DESKTOP\���Ԫ.lnk" "$INSTDIR\Launcher.exe"
  
FunctionEnd

Function CreateDesktop
  CreateShortCut "$DESKTOP\���Ԫ.lnk" "$INSTDIR\Launcher.exe"
FunctionEnd

/******************************
 *  �����ǰ�װ�����ж�ز���  *
 ******************************/
Section Uninstall  
  !insertmacro MUI_STARTMENU_GETFOLDER "Application" $ICONS_GROUP
  
  Delete "$DESKTOP\���Ԫ.lnk"
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

#-- ���� NSIS �ű��༭�������� Function ���α�������� Section ����֮���д���Ա��ⰲװ�������δ��Ԥ֪�����⡣--#

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "��ȷʵҪ��ȫ�Ƴ�${PRODUCT_NAME}��" IDYES +2
  Abort
Find:
  InstallCount::StartUninstall $INSTDIR
    strCmp $R0 1 Running Uninst
 Running:
	MessageBox MB_ICONQUESTION|MB_RETRYCANCEL|MB_DEFBUTTON1 "���ȹرտͻ��ˣ�Ȼ�������ԣ�" IDRETRY Find
	Abort
 Uninst:
  !ifdef COUNT_INSTALL
    InstallCount::Uninstall ${UNINST_RESULT_PAGE}
  !endif
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "${PRODUCT_NAME} �ѳɹ��ش���ļ�����Ƴ���"
FunctionEnd
