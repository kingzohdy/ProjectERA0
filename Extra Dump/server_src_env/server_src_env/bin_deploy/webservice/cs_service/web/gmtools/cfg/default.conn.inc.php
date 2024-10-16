<?php	if(!defined('IN_PHPMYWIND')) exit('Request Error!');

/*
**************************
(C)2010-2012 phpMyWind.com
update: 2012-2-14 17:19:08
**************************
*/

//数据库服务器
$db_host = '192.168.0.77';
//$GLOBALS[];

//数据库用户名
$db_user = 'server';

//数据库密码
$db_pwd = 'tulin#1234';

//数据库名
$db_name = 'gmtools';

//数据表前缀
$db_tablepre = '';

//数据表编码
$db_charset = 'utf8';

//连接wanyou数据库(同上)
$hostW = '192.168.0.165';
$userW = 'mmog';
$pwdW = 'mmog';
$nameW = 'wanyou';
$charsetW = 'utf8';

//连接bill所在数据库(需有可读权限的用户访问bill数据库)
$hostBill = '192.168.0.57';
$userBill = 'chengjun';
$pwdBill = 'chengjun';

//连接gmlog所在数据库(需有可读权限的用户访问bill数据库)
$hostGM = '192.168.0.77';
$userGM = 'server';
$pwdGM = 'tulin#1234';
$dbGM = 'gmtools';
$charsetGM = 'utf8';

//连接shopdef(所在数据库)
$link = mysql_connect('192.168.0.77','mmog','mmog');
?>