
echo "����������"
rd /s /q d:\myworldvers\%2
mkdir d:\myworldvers\%2
xcopy bin\*.* d:\myworldvers\%2\*.* /s

echo "����������ͻ��˴���"
xcopy client\Client\*.*      �Ϻ�����\client\Client\*.* /s
xcopy client\GameEngine\*.*  �Ϻ�����\client\GameEngine\*.* /s
xcopy client\include\*.*     �Ϻ�����\client\include\*.* /s
xcopy client\lib\*.*         �Ϻ�����\client\lib\*.* /s
xcopy client\OgreMain\*.h    �Ϻ�����\client\OgreMain\*.h
xcopy client\UILib\*.h       �Ϻ�����\client\UILib\*.h

echo "����������Դ"
xcopy \\perforce\share_write\������Դ\�汾����\bin\*.* d:\myworldvers\%2\*.* /s

echo "���ɰ汾patch"
d:
cd d:\myworldvers
makepatch %1 %2



