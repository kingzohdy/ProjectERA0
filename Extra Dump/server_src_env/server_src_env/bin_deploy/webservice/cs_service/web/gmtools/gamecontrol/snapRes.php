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
if(isset($_POST['hex']))
{
	$_SESSION['hex'] = $_POST['hex'];
	$_SESSION['role'] = $_POST['role'];
	$_SESSION['qu'] = $_POST['qu'];
	$_SESSION['fu'] = $_POST['fu'];
	return;
}
if(isset($_GET['res']))
{
	$hex =$_SESSION['hex'];
	$_SESSION['hex'] = "";
	$result = ParseBlob($hex);
	if($result->ErrorCode==0)
	{
		$str = $result->StrXML;
		$data=simplexml_load_string($str);
		//$skillnum = hexdec($data->RoleSkillInfo->SkillNum); 
		$arr = $data->Package;  //背包信息
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
	$a = "";
	$b = "";
	$res = "";
	$uid = $_SESSION['id']; 
	$sql = "select * from `adminor` where `id` = '$uid' ";
	$dosql->Execute($sql);
	$user = $dosql->GetArray();
	//$res = unserialize($_POST['checkid']);
	$res = $_POST['checkid'];	
	$tp = $_POST['tp']; //0.背包，1.仓库
	if(2 == $tp)
	{
		$sendtp = "宠物";
	}
	else if(2 != $tp)
	{
		$sendtp = "道具通过邮件";
	}
	$title = $_POST['title'];  //标题
	$nr = $_POST['nr'];	//内容
	$nums = count($res);
	//$rolename = $_POST['rolename'];
	$role = $_SESSION['role'];
	$qu = $_SESSION['qu'];
	$fu = $_SESSION['fu'];	
	$_SESSION['role'] = "";
	$_SESSION['qu'] = "";
	$_SESSION['fu'] = "";
	//设置时区
	date_default_timezone_set('PRC');
	//获取当前时间
	$dqtime = date('Y-m-d H:i:s');
	//日志记录
	$log1 = "<b>将".$sendtp."发送给玩家</b><hr class=dotline>大区:&nbsp;".$qu.",&nbsp;&nbsp;服：&nbsp;".$fu."<hr class=dotline>操作者：".$user['username']."&nbsp;&nbsp;（寄送的玩家：".$role."）<hr class=dotline>操作时间为：".$dqtime."<hr class=dotline>邮件标题：".$title."<hr class=dotline>内容：".$nr."<hr class=dotline>发送道具：";
	if($nums > 8 || $nums <=0)
	{
		echo "发送失败，数量不能大于8件 或 小于1件";
	}
	else
	{
		if( 2 != $tp)
		{
			for($i=0;$i<$nums;$i++)
			{
				$b = unserialize(base64_decode($res[$i]));
				$a .= $b[$tp];
				$zb=simplexml_load_string($b[$tp]);
				$z[$i] = (string)($zb ->GridData->RoleItem->WID);
			}
			if($a[$nums-1] == "")
			{
				echo "失败，不能勾选空值";
				exit;
			}
		}
		$MailItems = '<MailItems type="MailItems">';
		$MailItems .= '<ItemNum>'.$nums.'</ItemNum>'.$a; //传值的时候传10进制？16进制？
		$MailItems .= "</MailItems>";
		if(2 != $tp)
		{
			$result = SendSysMail($qu,$fu,$role,$title,$nr,$MailItems); 
		}
		if(2 == $tp)
		{
			if($nums > 1)
			{
				echo "发送失败，数量不能大于1";
				exit;
			}			
			$b = unserialize(base64_decode($res[0]));
			$zb=simplexml_load_string($b[2]);
			$z[0] = (string)($zb ->WID)."宠物名：".(string)($zb ->Name);
			$sendcw .= $b[2];
			$RolePetXml = str_replace('RolePets','RolePet',$sendcw);
			$result = SetPetInfo($qu,$fu,$role,$RolePetXml,2); 
		}
		 
		if($result->ErrorCode==0)
		{
			$log2 = "<hr class=dotline>操作成功";
			$gmlog = $log1."唯一ID（ ";
			for($i=0;$i<count($z);$i++)
			{
				$gmlog .= $z[$i].",<br/>";
			}
			$gmlog .= "）".$log2;
			$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '','$role', '$qu','$fu', now(), DATE_ADD(now(), INTERVAL '$kstime' SECOND),'', '12','0','$gmlog')";
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
			$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]','','$role','$qu','$fu', now(), DATE_ADD(now(), INTERVAL '$kstime' SECOND),'', '12','1','$gmlog')";
			$dosql->ExecNoneQuery($sql);
			echo "失败....";
			echo "<br/>";
			if($arr['ErrorCode'] == 1)
			{
				echo "玩家存在此宠物，错误码：".$arr['ErrorText'];
			}
			if($arr['ErrorCode'] == 5)
			{
				echo "角色在线，错误码：".$arr['ErrorText'];
			}
			if($arr['ErrorCode'] == 10)
			{
				echo "超时，请重发，错误码：".$arr['ErrorText'];
			}
			else{
				print_r($arr);
			}
		}
	}
	exit;
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>列表信息管理</title>
<link href="style/admin.css" rel="stylesheet" type="text/css" />
<style type="text/css">
 a{ color:#00F;}
.thickdiv{position:fixed;top:0;left:0;z-index:10000001;width:100%;height:100%;background:#F7F7F7;border:0;filter:alpha(opacity=70);opacity:0.70;}
.thickbox{position:absolute;z-index:100000002;overflow:hidden;box-shadow:0 10px 8px #6c6c6c; background-color:#fff;}
.thicktitle{height:30px;padding:0 15px; margin-top:8px; text-align:left;line-height:27px;font-family:arial,"宋体";font-size:14px;font-weight:bold;color:#F00;}

.thickclose:link,.thickclose:visited{display:block;position:absolute;z-index:100000;top:10px;right:12px;overflow:hidden;width:21px;height:21px;background:url(images/close_ico_dis.gif) no-repeat 0 3px;font-size:0;line-height:100px;}
.thickclose:hover{ background-position:left -18px;}
.thickcon{overflow:auto;padding:7px;text-align:center; vertical-align:top;}

#BgDiv{background-color:#F7F7F7; position:absolute; z-index:99; left:0; top:0; display:none; width:100%; height:100%;opacity:0.5;filter: alpha(opacity=50);-moz-opacity: 0.5;} 
#DialogDiv{position:absolute;width:300px; left:55%; top:20%; margin-left:-200px; height:50px; z-index:100; border:0px #ccc solid; padding:1px;}
</style>
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript">
function sendcg()
{
	var sendtp = $("#tp").val();
	if(2 == sendtp)
	{
		$("#fcw").hide();
		$("#rname").show();
		$("#sd").val("发送");
	}
	else
	{
		$("#rname").hide();
		$("#fcw").show();
		$("#sd").val("发送邮件");
	}
}
</script>
</head>
<body>
<?php
if($err == 1)
{
	//$df = json_encode($arr);
	//print_r($arr);
	//echo "1111111111";
	//exit;
	//echo $arr['data']->RoleDetailInfo->HPCurr;
}
if($err == -1)
{
	echo "查找数据,服务器错误:<br/>";
	//print_r($arr1);
	exit();
}
if($err == -2)
{
	echo "未找到指定信息:<br/>";
	exit();
}
 ?>
<div id="BgDiv"></div>
<div id="DialogDiv" style="display:none">
	<h2 style="color:#F00">请稍等...</h2>
</div>
</div>
<form action="snapRes.php" method="post" id="formrole">
<div class="mgr_divt">
</div>
<div id="list_area">

<!--/header end-->
<table width="100%" border="0" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist">
		<tr class="thead2" align="center">
        		<td >
                	<select name="tp" id="tp" onchange="sendcg()">
                    	<option value="0" selected="selected">背包</option>
                        <option value="1">仓库</option>
                        <option value="2">宠物</option>
                    </select>
                 </td>
       		 	<td >背包索引</td>
        		<td >背包物品</td>
                <td >数量</td>
                <td >唯一ID</td>
                <td >到期时间</td>
                <td >绑定</td>
                <td >宠物</td>
                <td >宠物等级</td>
                <td >宠物品质</td>
                <td >宠物唯一ID</td>
                <td >仓库索引</td>
        		<td >仓库物品</td>
                <td >数量</td>
                <td >唯一ID</td>
                <td >到期时间</td>
                <td >绑定</td>
               <!--  <td width="9%">操作</td> -->
		</tr>
 <input type="hidden" value="<?php ?>" id="wp" />
<?php 
	date_default_timezone_set('PRC');
	$dosql->select_db('resdb');
	$m = mysql_set_charset('latin1');
	$wids="";
	$x = (int)($arr->SundriesPackage->Num); //人物背包数量
	$y = (int)($arr->StashPackage->Num);  //仓库背包数量
	$z = hexdec($arr->PetPackage->Num);  //宠物背包
	if($x >= $y && $x >= $z)
	{
		$m = (int)($arr->SundriesPackage->Num);
	}
	else if($y >= $x && $y >= $z)
	{
		$m = (int)($arr->StashPackage->Num);
	}
	else if($z >= $x && $z >= $y)
	{
		$m = (int)($arr->PetPackage->Num);
	}
	//echo $arr->ZoneRolePackage->SundriesPackage->RoleGrids[2]->GridData->RoleArm->DefID;
	$ii = 0;
	$iii = 0;
	$iiii = 0;
	for($i=0; $i< $m; $i++ ){
		$pagefag = -1; //标记attributes
		if($arr->SundriesPackage->Num == 1)
		{
			if( $arr->SundriesPackage->RoleGrids->GridData->RoleArm != "")
			{
				$jb = $arr->SundriesPackage->RoleGrids->GridData->RoleArm; //人物背包
				$wids[0] = $arr->SundriesPackage->RoleGrids->asXML();
			}
			if( $arr->SundriesPackage->RoleGrids->GridData->RoleArm == "")
			{
				$jb = $arr->SundriesPackage->RoleGrids->GridData->RoleItem; //人物背包
				$wids[0] = $arr->SundriesPackage->RoleGrids->asXML();
			}
		}
		if($arr->StashPackage->Num == 1)
		{
			//$jc = $arr->ZoneRolePackage->StashPackage->RoleGrids->GridData->RoleItem;   //仓库包
			if($arr->StashPackage->RoleGrids->GridData->RoleArm != "")
			{
				$jc = $arr->StashPackage->RoleGrids->GridData->RoleArm; //仓库背包
				$wids[1] = $arr->StashPackage->RoleGrids->asXML();
			}
			if($arr->StashPackage->RoleGrids->GridData->RoleArm == "")
			{
				$jc = $arr->StashPackage->RoleGrids->GridData->RoleItem; //仓库背包
				$wids[1] = $arr->StashPackage->RoleGrids->asXML();
			}
		}
		if(hexdec($arr->PetPackage->Num) == 1)
		{
			//$jc = $arr->ZoneRolePackage->StashPackage->RoleGrids->GridData->RoleItem;   //宠物包
			if($arr->PetPackage->RolePets != "")
			{
				$jd = $arr->PetPackage->RolePets; //宠物包
				$wids[2] = $arr->PetPackage->RolePets->asXML();
			}
			if($arr->PetPackage->RolePets == "")
			{
				$jd = $arr->PetPackage->RolePets; //宠物包
				$wids[2] = $arr->PetPackage->RolePets->asXML();
			}
		}
		if($arr->SundriesPackage->Num > 1)
		{
			if($ii<$x)
			{
				if($arr->SundriesPackage->RoleGrids[$ii]->GridData->RoleArm != "")
				{
					$jb = $arr->SundriesPackage->RoleGrids[$ii]->GridData->RoleArm; //人物背包
					$wids[0] = $arr->SundriesPackage->RoleGrids[$ii]->asXML();
					
				}
				if($arr->SundriesPackage->RoleGrids[$ii]->GridData->RoleArm == "")
				{
					$jb = $arr->SundriesPackage->RoleGrids[$ii]->GridData->RoleItem; //人物背包
					$wids[0] = $arr->SundriesPackage->RoleGrids[$ii]->asXML();
				}
			}
			if($ii >= $x)
			{
				$jb = "";
			}
			$ii++;
		}
		if($arr->StashPackage->Num > 1)
		{
			if($iii < $y)
			{
				if($arr->StashPackage->RoleGrids[$iii]->GridData->RoleArm != "")
				{
					$jc = $arr->StashPackage->RoleGrids[$iii]->GridData->RoleArm; //仓库背包
					$wids[1] = $arr->StashPackage->RoleGrids[$iii]->asXML();
				}
				if($arr->StashPackage->RoleGrids[$iii]->GridData->RoleArm == "")
				{
					$jc = $arr->StashPackage->RoleGrids[$iii]->GridData->RoleItem; //仓库背包
					$wids[1] = $arr->StashPackage->RoleGrids[$iii]->asXML();
				}
			}
			if($iii >= $y)
			{
				$jc = "";
			}
			$iii++;
		}
		if(hexdec($arr->PetPackage->Num) > 1)
		{
			if($iiii < $z)
			{
				if($arr->PetPackage->RolePets[$iiii]!= "")
				{
					$jd = $arr->PetPackage->RolePets[$iiii]; //宠物背包
					$wids[2] = $arr->PetPackage->RolePets[$iiii]->asXML();
				}
				if($arr->PetPackage->RolePets[$iiii] == "")
				{
					$jd = $arr->PetPackage->RolePets[$iiii]; //宠物背包
					$wids[2] = $arr->PetPackage->RolePets[$iiii]->asXML();
				}
			}
			if($iiii >= $z)
			{
				$jd = "";
			}
			$iiii++;
		}
?>
    <tr align=center >
    	<!-- 记录一行中的背包，宠物，仓库物品唯一ID -->
    	<?php
			$ws = serialize($wids);
		?>
    	<td style="border-right:1px solid #F0F0F0"><input type="checkbox" name="checkid[]"  value='<?php echo base64_encode($ws); ?>' ></td>
    	<td  height=32 width="4%" style="border-right:1px solid #F0F0F0" >
			<?php
				if(1 == $x && 1 <= $i)
				{
					$jb="";
				}
				 echo $jb->GridIdx;
            ?>
        </td>
        <td style="border-right:1px solid #F0F0F0">
			<?php
				$defid = $jb->DefID;
				$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$defid'";
				$dosql->Query($sql);
				$row = $dosql->GetArray();
				$defName = iconv("gbk","utf8",$row['Name']);
				echo $defName; 
			?>
        </td>
        <td style="border-right:1px solid #F0F0F0" ><?php echo  $jb->Num; ?></td>
        <td style="border-right:1px solid #F0F0F0" ><?php echo  $jb->WID; ?></td>
        <td style="border-right:1px solid #F0F0F0" ><?php if($jb == ""){ echo ""; } else if($jb->ValidEnd == 0){ echo "无" ;} else if($jb->ValidEnd != 0){echo date('Y-m-d H:i:s',(int)($jb->ValidEnd)); }?></td>
        <td style="border-right:1px solid #F0F0F0" ><?php if($jb == ""){ echo ""; } else if($jb->InstFlag & 1 == 1){ echo "是"; }else{ echo "否";} ?></td>
        <td style="border-right:1px solid #F0F0F0" >
        <?php
				if(1 == $z && 1 <= $i)
				{
					$jd="";
				}
				$defid = $jd->PetID;
				$sql = "SELECT `Name` FROM PetDef where `PetID`='$defid'";
				$dosql->Query($sql);
				$row = $dosql->GetArray();
				$ptName = iconv("gbk","utf8",$row['Name']);
				echo $ptName; 
		?>
        </td>
        <td style="border-right:1px solid #F0F0F0" ><?php if($jd->Level != "") echo hexdec($jd->Level); ?></td>
        <td style="border-right:1px solid #F0F0F0" >
		<?php 
				$pz = "";
				$f = (string)$jd->PetGenius;
				if($f == "PET_GENIUS_A ")
				{
					 $pz = "杰出";
				} 
				if($f == "PET_GENIUS_B ")
				{
					 $pz = "优秀";
				} 
				if($f == "PET_GENIUS_C ")
				{ 
					$pz = "良好";
				} 
				if($f == "PET_GENIUS_D ")
				{ 
					$pz = "普通";
				} 
				if($f == "PET_GENIUS_S ")
				{ 
					$pz = "完美";
				} 
				echo $pz; 
		?></td>
        <td style="border-right:1px solid #F0F0F0" >
			<?php 
				if(1 == $y && 1 <= $i)
				{
					$jc="";
				} echo  $jd->WID;
			?>
        </td>
        <td style="border-right:1px solid #F0F0F0" ><?php echo  $jc->GridIdx; ?></td>
        <td style="border-right:1px solid #F0F0F0">
			<?php
				$ck = $jc->DefID;  
				$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$ck'";
				$dosql->Query($sql);
				$row1 = $dosql->GetArray();
				$defNameK = iconv("gbk","utf8",$row1['Name']);
				echo $defNameK;
			?></td>
       <td style="border-right:1px solid #F0F0F0" ><?php echo  $jc->Num; ?></td>
       <td style="border-right:1px solid #F0F0F0" ><?php echo  $jc->WID; ?></td>
       <td style="border-right:1px solid #F0F0F0" ><?php if($jc == ""){ echo ""; } else if($jc->ValidEnd == 0){ echo "无" ;} else if($jc->ValidEnd != 0){echo date('Y-m-d H:i:s',(int)($jc->ValidEnd)); } ?></td>
       <td style="border-right:1px solid #F0F0F0" ><?php if($jc == ""){ echo ""; } else if($jc->InstFlag & 1 == 1){ echo "是"; }else{ echo "否";} ?></td>
    </tr>
<?php
	}
?>                               
</table>
<div class="mgr_divb" style="font-size:14px; text-align:center;">
选择右侧选框，你可以点击下方发送邮件按钮，发送给玩家，注意：一次最多发送8件物品(你可通过“追踪道具”菜单查找，此物品的操作【丢弃，交易，分解...】情况)
</div>
<div  style="height:20px; margin-top:10px; text-align:center;">
	<!-- 玩家名字：<input type="text" name="rolename"  value="">(非必填，若玩家名与快照名不一致，需填改后的名字) -->
    <span id="fcw">
    	标题：<input type="text" name="title" id="title" value="">(必填)&nbsp;&nbsp;
        内容：<input type="text" name="nr" value="" >(选填)&nbsp;&nbsp;
    </span>
    <span id="rname" style="display:none">&nbsp;发送宠物时，需保证玩家不在线，玩家的宠物栏未满，发送的宠物玩家不存在，且一次最多发送一条数据&nbsp;&nbsp;</span>
    <input type="submit" value="发送邮件" name="youji" id="sd" />
</div>

</form>

<!--/list end-->
</body>
</html>