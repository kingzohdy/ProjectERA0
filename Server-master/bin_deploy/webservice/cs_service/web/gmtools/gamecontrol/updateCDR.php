<?php require_once(dirname(__FILE__).'/../inc/config.inc.php');
//检测用户是否有权限
if($_SESSION['admin1'] == "游戏管理员")
{
	$_SESSION = array();
	session_destroy();	
	echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
	exit();
}
require_once('soap.php');
$q="";
$f="";
//获取服务器列表
if($_SESSION['fwq'] == "")
{
	$result123=fwqlist();
	$_SESSION['fwq'] = $result123;
}
if($_SESSION['fwq'] != "")
{
	 $result123 = $_SESSION['fwq'];
}
if($result123->ErrorCode==0 )
{
	$err = 1;
	$str=$result123->StrXML;
	$arr=simplexml_load_string($str);
}
if($result123->ErrorCode!=0)
{
	$err = -1;
	$arr['ErrorCode'] = $result123->ErrorCode;
	$arr['ErrorText'] = $result123->StrError->ErrorText;
}
if(isset($_POST['dqlist']))
{
//require_once('soap.php');
$qu = $_POST['xdq'];
$fu = $_POST['dqlist'];
$roleName = $_POST['role'];
$clan = $_POST['clan'];

	$uid = $_SESSION['id']; 
	$sql = "select * from `adminor` where `id` = '$uid' ";
	$dosql->Execute($sql);
	$user = $dosql->GetArray();
	
	//设置时区
	date_default_timezone_set('PRC');
	//获取当前时间
	$dqtime = date('Y-m-d H:i:s');
	//日志记录
	$log1 = "<b>修改会长</b><hr class=dotline>大区:&nbsp;".$qu.",&nbsp;&nbsp;服：&nbsp;".$fu."<hr class=dotline>操作者：".$user['username']."&nbsp;&nbsp;（公会名：".$clan."）<hr class=dotline>操作时间为：".$dqtime."<hr class=dotline>会长改为：".$roleName;
	
 $result="";
$result = Epresident($qu,$fu,$clan,$roleName);
	if($result->ErrorCode==0 )
	{
		$log2 = "，操作成功";
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '$zhanghao', '', '', '', now(), now(),'', '16','0','$gmlog')";
		$dosql->ExecNoneQuery($sql);
		echo '<script type="text/javascript">alert("成功");</script>';
	}
	if($result->ErrorCode!=0)
	{
		echo '<script type="text/javascript">alert("失败");</script>';
		var_dump($result);
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		$log2 = "，操作失败<hr class=dotline>错误：".$arr['ErrorCode']."，".$arr['ErrorText'];
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '$zhanghao', '', '', '', now(), now(),'', '16','1','$gmlog');";
		//print_r($arr);
		exit(-1);
	}
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>换会长</title>
<link href="style/admin.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" >
var serverlist = -1;//用于保存服务器列表,若值为-1表示未获得列
function selectdq()
{
	var j=$("#xdq").val();
	serverlist=jQuery.parseJSON($("#ff").val());
	q = j;
	$("#dqlist").empty();
	if("-请选择-"== j)
	{
		$("#flist").hide();
	}
	else
	{
		if(1==serverlist.WorldCount && j == parseInt(serverlist.WorldInfos.RegionID))
		{
			$("#dqlist")[0].options.add(new Option(serverlist.WorldInfos.WorldName,serverlist.WorldInfos.WorldID,false, false));
		}
		else
		{
				for(var i = 0; i < serverlist.WorldCount; i++)
				{					 
					 if(j==parseInt(serverlist.WorldInfos[i].RegionID))
					 {
						 $("#dqlist")[0].options.add(new Option(serverlist.WorldInfos[i].WorldName,serverlist.WorldInfos[i].WorldID,false, false));
					 } 	
				}
		}
		$("#flist").show();				
	}
}
</script>
</head>
<body>
<?php
if($err == -1)
{
	echo "获取服务器,服务器错误:<br/>";
	print_r($arr);
	exit();
}
if($err == 1)
{
	$df = json_encode($arr);
	//print_r($arr);
	//echo $arr['data']->RoleDetailInfo->HPCurr;
}
 ?> 
<div class="gray_header"> <span class="title">公会换会长</span> <span class="reload"><a href="javascript:location.reload();">刷新</a></span> </div>
<form name="form" id="form" method="post" action="updateCDR.php" >
	<table width="100%" border="0" cellspacing="0" cellpadding="0" class="form_table">
    <tr>
		<td  width="25%"  height="35" align="right">&nbsp;&nbsp;&nbsp;&nbsp;
        	<select name="xdq" id="xdq" style="float:right;vertical-align:top; margin-top:6px;" onchange="selectdq()" >
                <?php 
					if($arr->AccountCount == 1)
					{
						$qid = $arr->AccountInfos->RegionID;
						$qname = $arr->AccountInfos->RegionName;
						 echo "<option value=$qid  >$qname</option>";
						 //echo "<input type=hidden value=$qname;' name=qn />";
					}
					else
					{
						for($i=0; $i< $arr->AccountCount; $i++)
						{
							$qid = $arr->AccountInfos[$i]->RegionID;
							$qname = $arr->AccountInfos[$i]->RegionName;
							 echo "<option value=$qid  >$qname</option>";
						}
					}
				?>
                <option value='-请选择-' selected='selected' >-请选择-</option>
        	</select>
            <input type="hidden" value='<?php echo $df; ?>' id="ff" />
        </td>
        <td width="75%" style="font-size:14px; margin-left:0px;display:none;" id="flist">
        	<select name="dqlist" id="dqlist" style="float:left;vertical-align:top; margin-top:6px; width:auto" >
        	 </select>
        </td>
     </tr>
        <tr height="42">
        	<td width="20%" height="35" align="right">公会名：</td>
             <td width="80%" style="font-size:14px; margin-left:0px;"><input type="text" name="clan"  /></td>
        </tr>
        <tr height="42">
        	<td width="20%" height="35" align="right">换给玩家名：</td>
             <td width="80%" style="font-size:14px; margin-left:0px;"><input type="text" name="role"  /></td>
        </tr>
	</table>	
	<div class="subbtn_area" style="width:55%">
		<input type="submit" class="blue_submit_btn" value=""  />
		<input type="button" class="blue_back_btn" value="" onclick="history.go(-1)"  />
	</div>
</form>
</body>
</html>