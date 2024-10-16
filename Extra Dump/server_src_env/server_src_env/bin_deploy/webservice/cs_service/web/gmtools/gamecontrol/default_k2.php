<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta http-equiv="Page-Enter" content="blendTrans(Duration=0.3)" />
<meta http-equiv="Page-Exit" content="blendTrans(Duration=0.3)" />
<title>零纪元后台管理系统</title>
<link href="style/frame_k.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="js/frame.js"></script>
<style type="text/css">        
.div_topbar { height: 40px; line-height:30px; background: url(images/f2.gif) repeat-x center; background-position: 0px -50px;}
.div_left { float: left; width: 600px; }
.h_title { margin-left: 10px; font-size: 18px; font-weight: bold; }        
.div_right {  height: 30px; line-height:30px; text-align: right; }
.span_account { margin-right: 30px; }
.span_action { margin-right: 10px; }
</style>
<?php 
if($_SESSION['type']!=="5"){
	echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
	exit();
} 
?>
</head>

<body class="showmenu">
<div class="header">
	<div class="div_topbar">
        <div class="div_left">
            <h1 class="h_title">万游引力 -- 游戏管理</h1>
        </div>
        <div class="div_right">
            <span class="span_account">&nbsp;</span>
            <span class="span_action">         
                <a href="loginOut.php">注销</a>
            </span>
        </div>
    </div>
	<div class="header_bottom"> <a href="javascript:;" class="togglemenu">隐藏菜单</a> </div>
</div>
<div class="left">
	<div class="menu" id="menu">
		<iframe src="left_menu2.php" frameborder="0"></iframe>
	</div>
</div>
<div class="right">
	<div class="main">
		<iframe id="main" name="main" frameborder="0" src="home.php" >
		<?php 
		if(strpos($_SERVER['HTTP_USER_AGENT'],'MSIE')) echo 'scrolling="yes"'; 
		?>
		</iframe>
	</div>
</div>
</body>
</html>