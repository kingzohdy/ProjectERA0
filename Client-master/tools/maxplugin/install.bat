
echo copy file begin

mkdir Scripts\OgreMaxScripts
mkdir Scripts\Startup
mkdir UI\Icons
mkdir UI\MacroScripts
mkdir db\client
mkdir shaders
mkdir shaders\test
mkdir maps\fx\myogre

copy Autodesk3DMax\Scripts\OgreMaxScripts\*.* .\Scripts\OgreMaxScripts\
copy Autodesk3DMax\Scripts\Startup\OgreStartup.ms .\Scripts\Startup\OgreStartup.ms
copy Autodesk3DMax\UI\OgreAnimationTools_Max8.cui .\UI\OgreAnimationTools_Max8.cui
copy Autodesk3DMax\UI\Icons\*.* .\UI\Icons\
copy Autodesk3DMax\UI\MacroScripts\*.* .\UI\MacroScripts\
copy Autodesk3DMax\plugins\maxplugin.dlu .\plugins\maxplugin.dlu

copy Autodesk3DMax\ogre_maxplugin.cfg .\ogre_maxplugin.cfg
copy Autodesk3DMax\db\client\AnimMap.csv .\db\client\AnimMap.csv
copy Autodesk3DMax\shaders\*.* .\shaders\*.*
copy Autodesk3DMax\shaders\test\*.* .\shaders\test\*.*
copy Autodesk3DMax\maps\fx\myogre\*.fx .\maps\fx\myogre\*.fx
copy Autodesk3DMax\*.dll .\*.dll

echo copy file end
