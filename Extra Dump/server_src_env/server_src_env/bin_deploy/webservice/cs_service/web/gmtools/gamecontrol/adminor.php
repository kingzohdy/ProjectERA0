<?php require_once(dirname(__FILE__).'/../inc/config.inc.php');
if($_SESSION['admin1'] != "超级游戏管理员")
	{
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
<title>列表信息管理</title>
<link href="../templates/style/admin.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="js/listajax.js"></script>
<script type="text/javascript" src="js/mgr.func.js"></script>
</head>
<body onload="GetList('adminorlist','<?php echo ($tid = isset($tid) ? $tid : ''); ?>','')">
<div class="mgr_header"> <span class="title">列表信息管理</span>
    <div class="alltype" onmouseover="ShowAllType();" onmouseout="HideAllType();">
    </div>
    <span class="reload"><a href="javascript:location.reload();">刷新</a></span>
</div>
<!--/header end-->
<form name="form" id="form" method="post">
    <div id="list_area">
    	<div class="loading"><img src="../templates/images/loading.gif" />读取列表中...</div>
    </div>
</form>
<!--/list end-->
</body>
</html>