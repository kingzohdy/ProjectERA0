
echo "�õ�perforce����"
p4 sync

echo "�������"
d:
cd D:\tools\vc8\Common7\IDE
devenv "D:\build_project\myworld\client\build\build.sln" /rebuild release  /out D:\build_project\myworld\rebuild.log