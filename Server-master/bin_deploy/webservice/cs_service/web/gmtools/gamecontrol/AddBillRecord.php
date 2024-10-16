<?php	require_once(dirname(__FILE__).'/../inc/config.inc.php');
//检测用户是否有权限
if($_SESSION['admin1'] == "")
{
	$_SESSION = array();
	session_destroy();	
	echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
	exit();
}
$GLOBALS['db_host'] = $db_host;
$GLOBALS['db_user'] = $db_user;
$GLOBALS['db_name']= $db_name;
$GLOBALS['db_pwd'] = $db_pwd;
$dosql = new MySql();
$dosql->Execute("select distinct regid,regname from `reg_world` order by `regid` ");
$k = 0;
while($rowm=$dosql->GetArray())
{
	$row[$k] = $rowm;
	$k++;
}
	$arr['card'] = "";
	$arr['money']="";
	$arr['status']="";
	$arr['qu'] ="";
	$arr['Uin'] ="";
	$arr['fs'] = "";
	$row[]="";
if(isset($_POST['dq']))
{
	$Uin = $_POST['us'];
	$tt = $_POST['time'];
	$GLOBALS['db_host'] = $hostBill;
	$GLOBALS['db_user'] = $userBill;
	$GLOBALS['db_pwd'] = $pwdBill;
	$db = $_POST['dq'];
	$GLOBALS['db_name'] = "bill_".$db;
	
	
	$dosql->Close(true);
	$dosql2 = new MySql(false);
	
	$card = $_POST['dq'];
	$sql1 = "select * from `AddBillRecord` where `Uin`='$Uin' and `Sequence`='$tt' ";
	
	$sqlcard=$dosql2->Execute($sql1);
	$rowdb = $dosql2->GetArray();
	
	$arr['card'] = $rowdb['Sequence'];
	$arr['money'] = $rowdb['Points'];
	$arr['qu'] = $rowdb['ServerID'];
	$arr['status']=$rowdb['LogTime'];
	$arr['Uin'] =$rowdb['Uin'];
	$arr['fs'] = $rowdb['BillType'];
	if($rowdb['BillType'] == '1')
	{
		$arr['fs'] = "卡密充值";
	}
	if($rowdb['BillType'] == '2')
	{
		$arr['fs'] = "支付宝充值";
	}
	if($rowdb['BillType'] == '3')
	{
		$arr['fs'] = "银联充值";
	}
	$link = mysql_connect($db_host,$db_user,$db_pwd) or die("连接失败");
	mysql_select_db($db_name,$link);
	mysql_query("set character set utf8");
	mysql_query("set names utf8");
	
	/*$GLOBALS['db_host'] = $db_host;
	$GLOBALS['db_user'] = $db_user;
	$GLOBALS['db_name']= $db_name;
	$GLOBALS['db_pwd'] = $db_pwd;*/
	//$dosql->Close(true);
	$dosql = new MySql(false);
	$mm = mysql_query("select distinct regid,regname from `reg_world` order by `regid` ");
	$k = 0;
	while($rowm = @mysql_fetch_array($mm))
	{
		$row[$k] = $rowm;
		$k++;
	}
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>查询用户是否充值成功</title>
<link href="style/admin.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="../data/plugin/calendar/calendar1.js"></script>
</head>
<body>
<div class="gray_header"> <span class="title">查询用户是否充值成功</span><span id="ttt" style="margin-left:100px; color:red"></span> <span class="reload"><a href="javascript:location.reload();">刷新</a></span> </div>
<form name="form" id="form" method="post" action="AddBillRecord.php">
<div class="mgr_divt">
<table width="100%" height="100%">
	<tr>
		<td>&nbsp;&nbsp;&nbsp;请选择·······大区：<select name="dq" id="dq" >
        <?php for($i=0;$i<count($row)-1;$i++){?>
        				<option value="<?php echo $row[$i]['regid']; ?>"><?php echo $row[$i]['regname']; ?></option>
        <?php } ?> 
                     </select><span style="color:#F00" id="qu"></span>
        	 </select>&nbsp;&nbsp;&nbsp;帐号Uin：
            <input type="text" id="us" name="us"  />
            <span style="color:#F00" id="us1"></span>&nbsp;&nbsp;&nbsp;
            &nbsp;&nbsp;&nbsp;充值订单号（卡号）：<input type="text" name="time" />&nbsp;&nbsp;
             <input type="submit" value="查询"/>(请输入充值订单号或卡密卡号,可先到充值后台查询)
        </td>
     </tr>
</table>
</div>
</form>
         <div <?php  if($arr['card'] == "" ){ echo "style=display:none";} else {echo "style=display:block;";}  ?>   >
         <table width="100%" border="0" cellspacing="0" cellpadding="0" class="form_table">
		 <tr>
        	<td height="35" align="right">充值订单/卡号&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;：</td>
			<td align="left">&nbsp;<?php echo $arr['card']; ?> </td>
            <td width="46%"></td>
        </tr> 
         <tr>
        	<td height="35" align="right">充值Uin&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;：</td>
			<td align="left">&nbsp;<?php echo $arr['Uin']; ?> </td>
            <td width="46%"></td>
        </tr> 
         <tr>
        	<td height="35"  align="right">充值点数&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;：</td>
			<td align="left">&nbsp;<?php echo $arr['money']; ?> </td>
            <td width="46%"></td>
        </tr>
        <tr>
        	<td height="35"  align="right">充值方式&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;：</td>
			<td align="left">&nbsp;<?php echo $arr['fs']; ?> </td>
            <td width="46%"></td>
        </tr>  
        <tr>
        	<td height="35"  align="right">充值时间&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;：</td>
			<td align="left">&nbsp;<?php echo $arr['status']; ?></td>
            <td width="46%"></td>
        </tr>
        <tr>
			<td height="35"  align="right">使用大区&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;：</td>
            <td align="left">&nbsp;<?php echo $arr['qu']; ?></td>
            <td width="46%"></td>
		</tr>
        </table>
        </div>
</body>
</html>