
echo copy file begin
rem set DestDir=..\..\..\bin\maxplugins
set DestDir=v:

mkdir %DestDir%\Scripts\OgreMaxScripts
mkdir %DestDir%\Scripts\Startup
mkdir %DestDir%\UI\Icons
mkdir %DestDir%\UI\MacroScripts
mkdir %DestDir%\db\client
mkdir %DestDir%\shaders
mkdir %DestDir%\maps\fx\myogre

copy OgreMaxScripts\*.* %DestDir%\Scripts\OgreMaxScripts\
copy OgreMaxScripts\Startup\OgreStartup.ms %DestDir%\Scripts\Startup\OgreStartup.ms
copy UI\OgreAnimationTools_Max8.cui %DestDir%\UI\OgreAnimationTools_Max8.cui
copy UI\Icons\*.* %DestDir%\UI\Icons\
copy UI\MacroScripts\*.* %DestDir%\UI\MacroScripts\

copy ..\..\..\bin\ogre_maxplugin.cfg %DestDir%\ogre_maxplugin.cfg
copy ..\..\..\bin\db\client\AnimMap.csv %DestDir%\db\client\AnimMap.csv
copy ..\..\..\bin\shaders\*.* %DestDir%\shaders\*.*
copy ..\..\..\bin\shaders\max_shaders\*.fx %DestDir%\maps\fx\myogre\*.fx

copy ..\..\..\bin\OgreMain_d.dll %DestDir%\OgreMain_d.dll
copy ..\..\..\bin\RenderSystem_Direct3D9_d.dll %DestDir%\RenderSystem_Direct3D9_d.dll

copy ..\..\..\bin\OgreMain.dll %DestDir%\OgreMain.dll
copy ..\..\..\bin\RenderSystem_Direct3D9.dll %DestDir%\RenderSystem_Direct3D9.dll

echo copy file end
