<?php
require_once('../include/common.inc.php');
if(isset($_POST['username'])){
	@session_start();
	$usr=$_POST['username'];
	$pw=md5('zhang'.$_POST['password']);
	$sql = "SELECT * FROM `adminor` WHERE `username`='$usr' AND `passwrod`='$pw'";
	$a = mysql_query ($sql);
	$b = mysql_fetch_array($a);
	if ("" == $b) {
		session_destroy();
		echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
		exit;
	}
	if($b['prem']==2){
		$_SESSION['type']="2";
		$_SESSION['admin1'] = "超级游戏管理员";
		$_SESSION['pw'] = $_POST['password'];
		$_SESSION['id'] = $b[0];
		echo '<script type="text/javascript">window.top.location.href="index.php";</script>';
		exit;
	}
	if($b['prem']==3){
		$_SESSION['type']="2";
		$_SESSION['admin1'] = "高级游戏管理员";
		$_SESSION['pw'] = $_POST['password'];
		$_SESSION['id'] = $b[0];
		header('location:index.php');
		exit;
	}
	if($b['prem']==4){
		$_SESSION['type']="2";
		$_SESSION['admin1'] = "游戏管理员";
		$_SESSION['pw'] = $_POST['password'];
		$_SESSION['id'] = $b[0];
		header('location:index.php');
		exit;
	}
	if($b['prem']==5){
		$_SESSION['type']="5";
		$_SESSION['admin1'] = "x";
		$_SESSION['pw'] = $_POST['password'];
		$_SESSION['id'] = $b[0];
		header('location:index2.php');
		exit;
	}
	if($b['prem']==99){
		session_destroy();
		echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
		exit;
	}
	else
	{
		session_destroy();
		echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
		exit;
	}
}
?>