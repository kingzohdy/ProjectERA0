<?php require_once(dirname(__FILE__).'/../inc/config.inc.php');
//检测用户是否有权限
if($_SESSION['admin1'] == "游戏管理员")
{
	$_SESSION = array();
	session_destroy();	
	echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
	exit();
}
if(isset($_POST['acc']))
{
	require_once('soap_1.php');
	$AccName = $_POST['acc'];
	$uid = $_SESSION['id']; 
	$sql = "select * from `adminor` where `id` = '$uid' ";
	$dosql->Execute($sql);
	$user = $dosql->GetArray();
	
	//设置时区
	date_default_timezone_set('PRC');
	//获取当前时间
	$dqtime = date('Y-m-d H:i:s');
	//日志记录
	$log1 = "<b>修改玩家安全邮箱</b><hr class=dotline>操作者：".$user['username']."&nbsp;&nbsp;（玩家账号：".$AccName."）<hr class=dotline>操作时间为：".$dqtime."<hr class=dotline>";
	
	$result="";
	$result = lossmibao($AccName);
	if($result->ErrorCode==0 )
	{
		$GLOBALS['db_host'] = $hostW;
		$GLOBALS['db_user'] = $userW;
		$GLOBALS['db_name'] = $nameW;
		$GLOBALS['db_pwd'] = $pwdW;

		$dosql->Close(true);
		$dosql = new MySql(false);
		
		$sql =  "SELECT `uid` from`user_pwdcard` where `username` = '$AccName' ";
		$dosql->Execute($sql);
		$rowdb = $dosql->GetArray();
		$uid = $rowdb['uid'];
		$sql2 = "DELETE FROM `user_pwdcard` WHERE `uid`='$uid' ";
		$sql3 = "DELETE FROM `user_pwdcardlist` WHERE `uid`='$uid'";
		
		$m1 = $dosql->ExecNoneQuery($sql2);
		$m2 = $dosql->ExecNoneQuery($sql3);
		
		if($m1)
		{
			if($m2)
			{
				$GLOBALS['db_host'] = $hostGM;
				$GLOBALS['db_user'] = $userGM;
				$GLOBALS['db_name'] = $dbGM;
				$GLOBALS['db_pwd'] = $pwdGM;
				
				$dosql->Close(true);
				$dosql = new MySql(false);
				
				$log2 = "操作成功";
				$gmlog = $log1.$log2;
				$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '$zhanghao', '', '', '', now(), now(),'', '14','0','$gmlog')";
				$dosql->ExecNoneQuery($sql);
				echo '<script type="text/javascript">alert("密保卡已解除绑定");</script>';
			}
			else
			{
				$GLOBALS['db_host'] = $hostGM;
				$GLOBALS['db_user'] = $userGM;
				$GLOBALS['db_name'] = $dbGM;
				$GLOBALS['db_pwd'] = $pwdGM;
				
				$dosql->Close(true);
				$dosql = new MySql(false);
				
				$log2 = "密保卡已解除绑定,web端删除失败";
				$gmlog = $log1.$log2;
				$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '$zhanghao', '', '', '', now(), now(),'', '14','0','$gmlog')";
				$dosql->ExecNoneQuery($sql);
				echo '<script type="text/javascript">alert("密保卡已解除绑定,web端删除失败,请联系技术");</script>';
			}
		}
		else
		{
			$GLOBALS['db_host'] = $hostGM;
			$GLOBALS['db_user'] = $userGM;
			$GLOBALS['db_name'] = $dbGM;
			$GLOBALS['db_pwd'] = $pwdGM;
			
			$dosql->Close(true);
			$dosql = new MySql(false);
			
			$log2 = "密保卡已解除绑定,web端删除失败";
			$gmlog = $log1.$log2;
			$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '$zhanghao', '', '', '', now(), now(),'', '14','0','$gmlog')";
			$dosql->ExecNoneQuery($sql);
			echo '<script type="text/javascript">alert("密保卡已解除绑定,web端删除失败,请联系技术");</script>';
		}
		echo '<script type="text/javascript">window.location.href="mibao.php";</script>';
		exit(-1);
	}
	if($result->ErrorCode!=0)
	{
		$GLOBALS['db_host'] = $hostGM;
		$GLOBALS['db_user'] = $userGM;
		$GLOBALS['db_name'] = $dbGM;
		$GLOBALS['db_pwd'] = $pwdGM;
		
		$dosql->Close(true);
		$dosql = new MySql(false);
		
		
		echo '<script type="text/javascript">alert("失败");</script>';
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		$log2 = "操作失败，密保卡解除绑定失败<hr class=dotline>错误：".$arr['ErrorCode']."，".$arr['ErrorText'];
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '$zhanghao', '', '', '', now(), now(),'', '14','0','$gmlog')";
		$dosql->ExecNoneQuery($sql);
		print_r($arr);
		exit(-1);
	}
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>密保挂失</title>
<link href="style/admin.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="js/serverlist.js"></script>
</head>
<body> 
<div class="gray_header"> <span class="title">密保挂失</span> <span class="reload"><a href="javascript:location.reload();">刷新</a></span> </div>
<form name="form" id="form" method="post" action="mibao.php" >
	<table width="100%" border="0" cellspacing="0" cellpadding="0" class="form_table">
        <tr height="42">
        	<td width="25%" height="35" align="right">帐&nbsp;&nbsp;号：</td>
             <td width="75%" style="font-size:14px; margin-left:0px;"><input type="text" name="acc"  /></td>
        </tr>
	</table>	
	<div class="subbtn_area" style="width:55%">
		<input type="submit" class="blue_submit_btn" value=""  />
		<input type="button" class="blue_back_btn" value="" onclick="history.go(-1)"  />
	</div>
</form>
</body>
</html>