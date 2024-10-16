<?php require_once(dirname(__FILE__).'/../inc/config.inc.php');

if($_SESSION['id'] != "11")
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
<title>baobao</title>
<link href="style/admin.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="js/listajax.js"></script>
<script type="text/javascript" src="js/mgr.func.js"></script>
</head>
<?php 
if(isset($_POST['account']))
{
	$dq = $_POST['dq'];
	$account = iconv("utf8","gbk",$_POST['account']);
	$itemid = $_POST['itemid'];
	$itemname = iconv("utf8","gbk",$_POST['itemname']);
	$nums = $_POST['nums'];
	$miaoshu = iconv("utf8","gbk",$_POST['miaoshu']);
	if($account == "")
	{
		echo "<script type=text/javascript>alert('账号输入错误')</script>";
		echo "<meta http-equiv='refresh' content='no-cache' content='0; url=baobao.php' > ";
		exit;
	}
	if($itemid == "" || $itemid < 0)
	{
		echo "<script type=text/javascript>alert('物品ID输入错误')</script>";
		echo "<meta http-equiv='refresh' content='no-cache' content='0; url=baobao.php' > ";
		exit;
	}
	if($nums == "" || $nums < 0)
	{
		echo "<script type=text/javascript>alert('数量输入错误')</script>";
		echo "<meta http-equiv='refresh' content='no-cache' content='0; url=baobao.php' > ";
		exit;
	}
	
	date_default_timezone_set('PRC');
	$dqtime = date('Y-m-d H:i:s');
	
	$sqlrw = "select * from `reg_world2` where `regid`='$dq' ";

	$resdb = "account_".$dq;
	$dosql->Execute($sqlrw);
	$fname = $dosql->GetArray();

	$GLOBALS['db_host'] = $fname['dbip'];
	$GLOBALS['db_user'] = $fname['dbuser'];
	$GLOBALS['db_name'] = $resdb;
	$GLOBALS['db_pwd'] = $fname['dbpass'];
	$dosql->Close(true);
	$dosql = new MySql(false);
	
	$sqluin = "SELECT `Uin` FROM Account where `Pass9Account`='$account'";
	
	$uins = mysql_fetch_row(mysql_query($sqluin));
	$uin = $uins[0];

	$Baodb = "bill_".$dq;

	$dosql->select_db($Baodb);
	
	$sql = "insert into AccGift (ID,Uin,RoleID,Name,GiftItems1_ItemID,GiftItems1_ItemNum,GiftDesc, SetGiftTime) SELECT ifnull(max(LAST_INSERT_ID(ID)+1),1), '$uin',0, '$itemname', '$itemid', '$nums', '$itemname', cast(NOW() as datetime) from AccGift";
	$dosql->ExecNoneQuery('set character_set_client = latin1');
	
	$flag = $dosql->ExecNoneQuery($sql);
	
	$GLOBALS['db_host'] = $hostGM;
	$GLOBALS['db_user'] = $userGM;
	$GLOBALS['db_name'] = $dbGM;
	$GLOBALS['db_pwd'] = $pwdGM ;
	$dosql->Close(true);
	$dosql = new MySql(false);

	if($flag == 1)
	{
		$baobaolog = "INSERT INTO `baobaolog` VALUES ('','$_SESSION[id]','$_POST[miaoshu]','$_POST[account]','$uin','$itemid','$nums','$dq','$fname[regname]','$dqtime','1','无')";
		$flag2 = $dosql->ExecNoneQuery($baobaolog);
		if(1 != $flag2)
		{
			echo "<script type=text/javascript>alert('游戏插入成功，日志记录失败')</script>";
			exit;
echo "<meta http-equiv='refresh' content='no-cache' content='0; url=baobao.php' > ";
		}
		echo "<script type=text/javascript>alert('成功')</script>";
		echo "<meta http-equiv='refresh' content='no-cache' content='0; url=baobao.php' > ";
	}
	else
	{
		$baobaolog = "INSERT INTO `baobaolog` VALUES ('','$_SESSION[id]','$_POST[miaoshu]','$_POST[account]','$uin','$itemid','$nums','$dq','$fname[regname]','$dqtime','0','sql执行失败')";
		$flag2 = $dosql->ExecNoneQuery($baobaolog);
		if(1 != $flag2)
		{
			echo "<script type=text/javascript>alert('游戏插入失败，日志记录失败')</script>";
			exit;
			echo "<meta http-equiv='refresh' content='no-cache' content='0; url=baobao.php' > ";
		}
		echo "<script type=text/javascript>alert('失败')</script>";
		echo "<meta http-equiv='refresh' content='no-cache' content='0; url=baobao.php' > ";
	}
	
}
?>
<body onload="GetList('baobao','<?php echo ($tid = isset($tid) ? $tid : ''); ?>','')">
<div class="mgr_header"> <span class="title">baobao专用</span>
    <div class="alltype" onmouseover="ShowAllType();" onmouseout="HideAllType();">
    </div>
    <span class="reload"><a href="javascript:location.reload();">刷新</a></span>
</div>
<!--/header end-->
<form name="form" id="form" method="post">
    <div id="list_area">
    	<div class="loading"><img src="../templates/images/loading.gif" />loading...</div>
    </div>
</form>
<!--/list end-->
</body>
</html>