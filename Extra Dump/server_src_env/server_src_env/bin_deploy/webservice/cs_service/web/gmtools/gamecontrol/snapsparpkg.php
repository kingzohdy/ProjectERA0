<?php 
	require_once(dirname(__FILE__).'/../inc/config.inc.php'); 
//检测用户是否有权限
if($_SESSION['admin1'] == "")
{
	$_SESSION = array();
	session_destroy();	
	echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
	exit();
}
require_once('soap.php');
$arr="";
$hex = "";
if(isset($_POST['hex']))
{
	$_SESSION['hex'] = $_POST['hex'];
	$_SESSION['role'] = $_POST['role'];
	$_SESSION['qu'] = $_POST['qu'];
	$_SESSION['fu'] = $_POST['fu'];
	return 1;
}
if(isset($_GET['sparpkg']))
{
	$hex =$_SESSION['hex'];
	$_SESSION['hex'] = "";
	$result = ParseBlob($hex);
	if($result->ErrorCode==0)
	{
		$str = $result->StrXML;
		$data=simplexml_load_string($str);
		//$skillnum = hexdec($data->RoleSkillInfo->SkillNum); 
		$arr = $data->Package->SparPkg->RoleItems;  //装备晶石
		//$arr2 = $data->Wear->RideList; 
		
	}
	if($result->ErrorCode!=0)
	{
		$err = 1;
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
	}
}
if(isset($_POST['youji']))
{
	//检测用户是否有权限
	if($_SESSION['admin1'] != "超级游戏管理员")
	{	
		echo '你不能进行此操作！';
		exit();
	}
	$uid = $_SESSION['id']; 
	$sql = "select * from `adminor` where `id` = '$uid' ";
	$dosql->Execute($sql);
	$user = $dosql->GetArray();
	$equips = "";
	$equips = $_POST['checkid'];
	$title = $_POST['title'];  //标题
	$nr = $_POST['nr'];	//内容
	$role = $_SESSION['role'];
	$qu = $_SESSION['qu'];
	$fu = $_SESSION['fu'];
	$_SESSION['role'] = "";
	$_SESSION['qu'] = "";
	$_SESSION['fu'] = "";
	$nums = count($equips);
	//设置时区
	date_default_timezone_set('PRC');
	//获取当前时间
	$dqtime = date('Y-m-d H:i:s');
	//日志记录
	$log1 = "<b>将晶石通过邮件发送给玩家</b><hr class=dotline>大区:&nbsp;".$qu.",&nbsp;&nbsp;服：&nbsp;".$fu."<hr class=dotline>操作者：".$user['username']."&nbsp;&nbsp;（寄送的玩家：".$role."）<hr class=dotline>操作时间为：".$dqtime."<hr class=dotline>邮件标题：".$title."<hr class=dotline>内容：".$nr."<hr class=dotline>发送晶石：";
	if($nums > 8)
	{
		echo "发送失败，数量不能大于8件";
	}
	else if($nums <= 0)
	{
		$result = SendSysMail($qu,$fu,$role,$title,$nr,'');
		if($result->ErrorCode==0)
		{
			$log2 = "<hr class=dotline>操作成功";
			$gmlog = $log1."0个".$log2;
			$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '','$role','$qu','$fu', now(), DATE_ADD(now(), INTERVAL '$kstime' SECOND),'', '13','0','$gmlog')";
			$dosql->ExecNoneQuery($sql);
			echo "发送成功";
			
		}
		if($result->ErrorCode!=0)
		{
			print_r($equips);
			$arr['ErrorCode'] = $result->ErrorCode;
			$arr['ErrorText'] = $result->StrError->ErrorText;
			$log2 = "<hr class=dotline>操作失败<hr class=dotline>错误：".$arr['ErrorCode']."，".$arr['ErrorText'];
			$gmlog = $log1."0个".$log2;
			$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '','$role','$qu','$fu', now(), DATE_ADD(now(), INTERVAL '$kstime' SECOND),'', '13','1','$gmlog')";
			$dosql->ExecNoneQuery($sql);
			echo "失败....";
			echo "<br/>";
			print_r($arr);
		}
	}
	else
	{
		$xx = "";
		for($i=0;$i<$nums;$i++)
		{
			$zb=simplexml_load_string($equips[$i]);
			$z[$i] = (string)($zb ->WID);
			$equips[$i] = str_replace('RoleItems','RoleItem',$equips[$i]);
			$xx .= '<RoleGrids type="RoleGrid"><Type>GRID_ITEM_ITEM </Type><GridData type="RoleGridData">'.$equips[$i].'</GridData></RoleGrids>';
		}
		$MailItems = '<MailItems type="MailItems"><ItemNum>'.$nums.'</ItemNum>';
		$MailItems .= $xx;
		$MailItems .= "</MailItems>";
		//print_r($MailItems);
		//exit;
		$result = SendSysMail($qu,$fu,$role,$title,$nr,$MailItems);  
		if($result->ErrorCode==0)
		{
			$log2 = "<hr class=dotline>操作成功";
			$gmlog = $log1."唯一ID（ ";
			for($i=0;$i<count($z);$i++)
			{
				$gmlog .= $z[$i].",<br/>";
			}
			$gmlog .= "）".$log2;
			$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '','$role', '$qu','$fu', now(), DATE_ADD(now(), INTERVAL '$kstime' SECOND),'', '13','0','$gmlog')";
			$dosql->ExecNoneQuery($sql);
			echo "发送成功";
			
		}
		if($result->ErrorCode!=0)
		{
			//print_r($MailItems);
			$arr['ErrorCode'] = $result->ErrorCode;
			$arr['ErrorText'] = $result->StrError->ErrorText;
			$log2 = "<hr class=dotline>操作失败<hr class=dotline>错误：".$arr['ErrorCode']."，".$arr['ErrorText'];
			$gmlog = $log1."唯一ID（ ";
			for($i=0;$i<count($z);$i++)
			{
				$gmlog .= $z[$i].",<br/>";
			}
			$gmlog .= "）".$log2;
			$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]','','$role','$qu','$fu', now(), DATE_ADD(now(), INTERVAL '$kstime' SECOND),'', '10','1','$gmlog')";
			$dosql->ExecNoneQuery($sql);
			echo "失败....";
			echo "<br/>";
			print_r($arr);
		}
	
		//print_r($equips);
		
	}
	exit;
}
?>
<link href="style/admin.css" rel="stylesheet" type="text/css" />
<style type="text/css">
 a{ color:#00F;}
 </style>
<script type="text/jscript">
</script>
<body style="width:612px;">
<?php
	if($err == 1)
	{
		echo "服务器错误:<br/>";
		print_r($arr);
		exit();
	}
	else
	{
		//print_r($data);
		//echo $data->RoleSkillInfo->SkillNum;
		//echo hexdec($data->RoleSkillInfo->SkillNum);
		//echo $arr['data']->RoleDetailInfo->HPCurr;
	}
 ?>
 <form action="snapsparpkg.php" method="post" name="form" id="form">
<table width="610" height="400" border="1" cellpadding="0" cellspacing="0"  >	
	   <tr align="center" class="mgr_tr" width="93%" height="30">
        	<td width="5%">&nbsp;</td>
            <td  align="center" >晶石</td>
            <td  align="center" >晶石ID</td>
            <td  align="center" >等级</td>
            <td  align="center" >经验</td>
        </tr>
        <?php 
			date_default_timezone_set('PRC');
		 	 for($i=0; $i<$data->Package->SparPkg->Num; $i++ ){
				 $eq = $arr[$i]->asXML();
				 $aid = $arr[$i]->DefID;
				 $equipName = $aid;
				 $sql = "SELECT `Name` FROM ItemDef where `ItemID`='$aid'";
				 $dosql->ExecNoneQuery('SET NAMES Default');
				 mysql_select_db('resdb');
				 $rest = mysql_query($sql);
				 $rowf = mysql_fetch_array($rest);
				 if($rowf != "" && $rowf != false)
				 {
				 	$equipName = iconv("gbk","utf8",$rowf['Name']);
				 }
				 //http://www.w3school.com.cn/php/php_ref_string.asp 
		?>
        <tr align="center" class="mgr_tr" width="93%">
        	<td width="5%"><input type="checkbox" name="checkid[]"  value='<?php print_r($eq); ?>' ></td>
            <td  align="center" ><a href="#" ><?php echo $equipName ?></a></td>
            <td  align="center" ><a href="#" ><?php echo $arr[$i]->WID ?></a></td>
            <td  align="center" ><a href="#" ><?php echo $arr[$i]->InstVal1; ?></a></td>
             <td  align="center" ><a href="#" ><?php echo $arr[$i]->InstVal3; ?></a></td>
        </tr>
        <?php } ?>
	</table>
    <div  style="width:100%; height:100px; margin-top:10px; text-align:center;">
		标题：<input type="text" name="title">(必填)&nbsp;&nbsp;<input type="submit" value="发送邮件" name="youji" /><p>&nbsp;</p>
        内容：<textarea name="nr" style="height:70px; width:252px;"></textarea>(选填)&nbsp;&nbsp;
	</div>
    </form>
</body>
</html>
