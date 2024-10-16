
echo copy file begin

set DestDir=Autodesk3DMax\

mkdir %DestDir%\Scripts\OgreMaxScripts
mkdir %DestDir%\Scripts\Startup
mkdir %DestDir%\UI\Icons
mkdir %DestDir%\UI\MacroScripts
mkdir %DestDir%\db\client
mkdir %DestDir%\shaders
mkdir %DestDir%\shaders\test
mkdir %DestDir%\maps\fx\myogre
mkdir %DestDir%\plugins

copy OgreMaxScripts\*.* %DestDir%\Scripts\OgreMaxScripts\
copy OgreMaxScripts\Startup\OgreStartup.ms %DestDir%\Scripts\Startup\OgreStartup.ms
copy UI\OgreAnimationTools_Max8.cui %DestDir%\UI\OgreAnimationTools_Max8.cui
copy UI\Icons\*.* %DestDir%\UI\Icons\
copy UI\MacroScripts\*.* %DestDir%\UI\MacroScripts\
copy plugins\maxplugin.dlu %DestDir%\plugins

copy ..\..\..\bin\ogre_maxplugin.cfg %DestDir%\ogre_maxplugin.cfg
copy ..\..\..\bin\db\client\AnimMap.csv %DestDir%\db\client\AnimMap.csv
copy ..\..\..\bin\shaders\*.* %DestDir%\shaders\*.*
copy ..\..\..\bin\shaders\test\*.* %DestDir%\shaders\test\*.*
copy ..\..\..\bin\shaders\max_shaders\*.fx %DestDir%\maps\fx\myogre\*.fx
copy ..\..\..\bin\*.dll %DestDir%\

echo copy file end
