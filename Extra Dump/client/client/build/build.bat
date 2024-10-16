
echo "拷贝编译结果"
rd /s /q d:\myworldvers\%2
mkdir d:\myworldvers\%2
xcopy bin\*.* d:\myworldvers\%2\*.* /s

echo "拷贝并打包客户端代码"
xcopy client\Client\*.*      上海代码\client\Client\*.* /s
xcopy client\GameEngine\*.*  上海代码\client\GameEngine\*.* /s
xcopy client\include\*.*     上海代码\client\include\*.* /s
xcopy client\lib\*.*         上海代码\client\lib\*.* /s
xcopy client\OgreMain\*.h    上海代码\client\OgreMain\*.h
xcopy client\UILib\*.h       上海代码\client\UILib\*.h

echo "拷贝美术资源"
xcopy \\perforce\share_write\美术资源\版本进度\bin\*.* d:\myworldvers\%2\*.* /s

echo "生成版本patch"
d:
cd d:\myworldvers
makepatch %1 %2



