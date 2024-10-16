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
if(isset($_GET['equip']))
{
	$hex =$_SESSION['hex'];
	$_SESSION['hex'] = "";
	$result = ParseBlob($hex);
	if($result->ErrorCode==0)
	{
		$str = $result->StrXML;
		$data=simplexml_load_string($str);
		//$skillnum = hexdec($data->RoleSkillInfo->SkillNum); 
		$arr = $data->Wear->RoleArms;  //装备
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
	//echo $GLOBALS['db_host'];
	//$GLOBALS['db_host'] = $db_host;
	//$GLOBALS['db_user'] = $db_user;
	//$GLOBALS['db_name'] = $db_name;
	//$GLOBALS['db_pwd'] = $db_pwd;
	
	//$dosql->Close(true);
	//$dosql = new MySql(false);
	$uid = $_SESSION['id']; 
	$sql = "select * from `adminor` where `id` = '$uid' ";
	$dosql->Execute($sql);
	$user = $dosql->GetArray();
	//检测用户是否有权限
	if($_SESSION['admin1'] != "超级游戏管理员")
	{	
		echo '你不能进行此操作！';
		exit();
	}
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
	$log1 = "<b>将装备通过邮件发送给玩家</b><hr class=dotline>大区:&nbsp;".$qu.",&nbsp;&nbsp;服：&nbsp;".$fu."<hr class=dotline>操作者：".$user['username']."&nbsp;&nbsp;（寄送的玩家：".$role."）<hr class=dotline>操作时间为：".$dqtime."<hr class=dotline>邮件标题：".$title."<hr class=dotline>内容：".$nr."<hr class=dotline>发送装备：";
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
			$gmlog = $log1."0件".$log2;
			$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '','$role','$qu','$fu', now(), DATE_ADD(now(), INTERVAL '$kstime' SECOND),'', '10','0','$gmlog')";
			$dosql->ExecNoneQuery($sql);
			echo "发送成功";			
		}
		if($result->ErrorCode!=0)
		{
			$arr['ErrorCode'] = $result->ErrorCode;
			$arr['ErrorText'] = $result->StrError->ErrorText;
			$log2 = "<hr class=dotline>操作失败<hr class=dotline>错误：".$arr['ErrorCode']."，".$arr['ErrorText'];
			$gmlog = $log1."0件".$log2;
			$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '','$role','$qu','$fu', now(), DATE_ADD(now(), INTERVAL '$kstime' SECOND),'', '10','1','$gmlog')";
			$dosql->ExecNoneQuery($sql);
			print_r($equips);
			
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
			$xx .= $equips[$i];
			$zb=simplexml_load_string($equips[$i]);
			$z[$i] = (string)($zb ->WID);
		}
		$MailItems = '<MailItems type="MailItems">';
		$MailItems .= '<ArmNum>'.$nums.'</ArmNum>'.$xx; //传值的时候传10进制？16进制？
		$MailItems .= "</MailItems>";
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
			$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '','$role', '$qu','$fu', now(), DATE_ADD(now(), INTERVAL '$kstime' SECOND),'', '10','0','$gmlog')";
			$dosql->ExecNoneQuery($sql);
			
			echo "发送成功";	
		}
		if($result->ErrorCode!=0)
		{
			$arr['ErrorCode'] = $result->ErrorCode;
			$arr['ErrorText'] = $result->StrError->ErrorText;
			//print_r($MailItems);
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
 <form action="snapequip.php" method="post" name="form" id="form">
<table width="820" height="480" border="1" cellpadding="0" cellspacing="0"  >	
	   <tr align="center" class="mgr_tr" width="93%" height="30">
        	<td >&nbsp;</td>
            <td  align="center" >装备</td>
            <td  align="center" >装备ID</td>
            <td  align="center" >品质</td>
            <td  align="center" >强化(星级：强化百分比)</td>
            <td  align="center" >有效期至</td>
            <td  align="center" >绑定</td>
        </tr>
        <?php 
			date_default_timezone_set('PRC');
		 	 for($i=0; $i<$data->Wear->ArmNum; $i++ ){
				 $eq = $arr[$i]->asXML();
				 $aid = $arr[$i]->DefID;
				 $equipName = $aid; 
				 $sql = "SELECT `Name`,`Important` FROM ItemDef where `ItemID`='$aid'";
				 $dosql->ExecNoneQuery('SET NAMES Default');
				 mysql_select_db('resdb');
				 $rest = mysql_query($sql);
				 $rowf = mysql_fetch_array($rest);
				 if($rowf != "" && $rowf != false)
				 {
				 	$equipName = iconv("gbk","utf8",$rowf['Name']);
				 }
				 $pz = $rowf['Important'];
				 if(4 <= $pz)
				 {
				 	$pz = "粉装";
				 }
				 else if(3 == $pz)
				 {
				 	$pz = "紫装";
				 }
				 else if(2 == $pz)
				 {
				 	$pz = "蓝装";
				 }
				 else if(1 == $pz)
				 {
				 	$pz = "绿装";
				 }
				 else if(0 == $pz)
				{
					$pz = "白装";
				}
				 //http://www.w3school.com.cn/php/php_ref_string.asp 
		?>
        <tr align="center" class="mgr_tr" width="93%">
        	<td align="center" ><input type="checkbox" name="checkid[]"  value='<?php print_r($eq); ?>' ></td>
            <td  align="center" ><a href="#" ><?php echo $equipName ?></a></td>
            <td  align="center" ><a href="#" ><?php echo $arr[$i]->WID ?></a></td>
            <td  align="center" ><a href="#" ><?php echo $pz ?></a></td>
            <td  align="center" ><a href="#" >
            	<?php
            		if(0==hexdec($arr[$i]->StarLv))
					{
						echo "未强化";
					}
					else
					{
						$qh = "(".hexdec($arr[$i]->StarLv)."星：";
						if(1==hexdec($arr[$i]->StarLv))
						{
							$qh .= hexdec($arr[$i]->StarInfo->ValIdx)+1;
						}
						else
						{
							
							for($ij=0;$ij<hexdec($arr[$i]->StarLv);$ij++)
							{	
								$ss = hexdec($arr[$i]->StarInfo[$ij]->ValIdx);
								$zdj = hexdec($arr[$i]->StarLv);
								$zd = "StarInfo".($ss+1)."_Mul";
								$sqls = "SELECT `$zd` FROM `ArmStarDef` where `Star`='$zdj'";
								echo $sqls."<br>";
								$xj = mysql_query($sqls);
								$xjs = mysql_fetch_array($xj);
								$qh .= $xjs[$zd].",";
								
							}
						}						
						$qh .= ")";
						echo $qh;						
					}            			
			 	?>
			</a></td>
            <td  align="center" ><a href="#" ><?php if($arr[$i]->ValidEnd == 0){echo "永久";} else {echo date('Y-m-d',(int)($arr[$i]->ValidEnd)); } ?></a></td>
            <td  align="center" ><a href="#" ><?php  if($arr[$i]->InstFlag & 1 == 1){ echo "是";} else { echo "否";} ?></a></td>
        </tr>
        <?php } ?>
	</table>
    <div  style="width:100%; height:20px; margin-top:10px; text-align:center;">
		标题：<input type="text" name="title">(必填)&nbsp;&nbsp;内容：<input type="text" name="nr">(选填)&nbsp;&nbsp;<input type="submit" value="发送邮件" name="youji" />
	</div>
    </form>
</body>
</html>
