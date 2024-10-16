<?php	require_once(dirname(__FILE__).'/../inc/config.inc.php');
if($_SESSION['admin1'] == "")
	{
		$_SESSION = array();
		session_destroy();	
		echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
		exit();
	}

/*
**************************
(C)2010-2012 phpMyWind.com
update: 2012-6-20 8:58:54
person: Feng
**************************
*/


//初始化参数
$tbname = '#@__adminor';
$gourl  =  "home.php";


//添加列表信息
if($action == 'add')
{
	$pw=md5('zhang'.$pw);
	$sql = "INSERT INTO `$tbname` (id,username, passwrod, prem) VALUES ( NULL,'$adm','$pw', '$admintype')";
	if($dosql->ExecNoneQuery($sql))
	{
		echo "<meta http-equiv='refresh' content='0;url=home.php'><script>alert('成功')</script>";
		exit();
	}
}
if($action == "update")
{
	$pw=md5('zhang'.$pw);
	$sql = "update `$tbname` set username='$adm', passwrod='$pw', prem='$admintype' where id='$id'";
	if($dosql->ExecNoneQuery($sql))
	{
		echo "<meta http-equiv='refresh' content='0;url=home.php'><script>alert('成功')</script>";
		exit();
	}
}
if($action == "update1")
{
	$id=$_SESSION['id'];
	$_SESSION['pw']=$_POST['pw'];
	$pw1=$_POST['qr'];
	$pw1=md5('zhang'.$pw1);
	$sql = "update `$tbname` set  passwrod='$pw1' where id='$id'";
	if($dosql->ExecNoneQuery($sql))
	{
		echo "<meta http-equiv='refresh' content='0;url=home.php'><script>alert('成功')</script>";
		exit();
	}
}
else
{
	echo "<meta http-equiv='refresh' content='0;url=home.php'><script>alert('失败')</script>";
	exit();
}
?>