<?php require_once(dirname(__FILE__).'/../inc/config.inc.php');
//检测是否登录
if($_SESSION['type']!="5"){

	$_SESSION = array();
	session_destroy();	
	echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
	exit();
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>左侧菜单</title>
<link href="style/menu.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="js/leftmenu.js"></script>
</head>
<body>
<div class="quickbtn"> 
	<span class="quickbtn_left"><a href="changepasswrod.php" target="main" >修改密码</a></span> 
    <span class="quickbtn_right"><a>【零纪元】</a></span> 
</div>
<div class="tGradient"></div>
<div id="scrollmenu">
	<div class="scrollbar">
		<div class="track">
		</div>
	</div>
	<div class="viewport">
		<div class="overview">
			<div class="menubox">
				<div class="title" ><a href="systemTrade.php" target="main" style="color:#F00;"> 金银币交易工具 </a></div>
			</div>
            <div class="menubox">
				<div class="title" ><a href="systemlog.php" target="main" style="color:#F00;"> 操作记录 </a></div>
			</div>
            <div class="menubox">
				<div class="title" ><a href="OpTradeInfolog.php" target="main" style="color:#F00;"> 系统交易日志 </a></div>
			</div>
		</div>
	</div>
</div>
<div class="bGradient"></div>
<div class="hr_5"></div>
</body>
</html>