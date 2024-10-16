
echo "得到perforce代码"
p4 sync

echo "编译代码"
d:
cd D:\tools\vc8\Common7\IDE
devenv "D:\build_project\myworld\client\build\build.sln" /rebuild release  /out D:\build_project\myworld\rebuild.log