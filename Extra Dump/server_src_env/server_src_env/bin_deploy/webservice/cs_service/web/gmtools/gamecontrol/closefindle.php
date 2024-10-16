<?php require_once(dirname(__FILE__).'/../inc/config.inc.php');
//检测用户是否有权限
if($_SESSION['admin1'] == "")
{
	$_SESSION = array();
	session_destroy();	
	echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
	exit();
}
if(isset($_POST['colsefid']))
{
require_once('soap.php');
 $result="";
	$FindId = $_GET['fid'];
	for($i=0;$i<20;$i++)
	{
		HandleClose($i);
	}
	echo "<script type='text/javascript'>alert('关闭成功')</script>";
	echo '<script type="text/javascript">window.top.location.href="index.php";</script>';
	exit;
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>关闭句柄</title>
<link href="style/admin.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="js/serverlist.js"></script>
</head>
<body> 
<div class="gray_header"> <span class="title">关闭句柄</span> <span class="reload"><a href="javascript:location.reload();">刷新</a></span> </div>
<form name="form" id="form" method="post" action="closefindle.php" onsubmit="return cfm_infolm();">
	<table width="100%" border="0" cellspacing="0" cellpadding="0" class="form_table">
    	<tr height="32">
        	<td width="15%">&nbsp;</td>
            <td width="85%" style="font-size:14px;">当你在某个时间段不停操作&nbsp;帐号查询&nbsp;或&nbsp;角色查询&nbsp;菜单时，</td>
        </tr>
        <tr height="32">
        	<td width="15%">&nbsp;</td>
            <td width="85%" style="font-size:14px;">可能出现（Array ( [ErrorCode] => -1 [ErrorText] => no free find handle )）错误</td>
        </tr>
        <tr height="32">
        	<td width="15%">&nbsp;</td>
            <td width="85%" style="font-size:14px;">你将不能在一段时间内再次使用&nbsp;帐号查询&nbsp;或&nbsp;角色查询&nbsp;菜单，此时若需再次使用，则需关闭一些句柄。</td>
        </tr>
        <tr height="32">
        	<td width="15%">&nbsp;</td>
            <td width="85%" style="font-size:14px;" ><a href="javascript:;" ><input type="submit" style="color:#FFF; font-weight:bold; width: 80px; height: 22px; border:1;background:url(images/on_db_action.gif) no-repeat;" value="关闭句柄" /></a></td>
        </tr>
	</table>	
        <input type="hidden" name="colsefid" />
</form>
</body>
</html>