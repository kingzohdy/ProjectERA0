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
//获取物品信息
if(isset($_GET['role']))
{
	$q =  $_GET['dq'];
	$f = $_GET['fu'];
	$rolename = $_GET['role'];
	$result = ItemInfo($q,$f,$rolename);
	if($result->ErrorCode==0 )
	{
		$err = 1;
		$str=$result->StrXML;
		$data=simplexml_load_string($str);
	}
	if($result->ErrorCode!=0 && $result->ErrorCode!=-6)
	{
		$err = -1;
		$arr1['ErrorCode'] = $result->ErrorCode;
		$arr1['ErrorText'] = $result->StrError->ErrorText;
	}
	if($result->ErrorCode==-6)
	{
		$err = -2;
	}	
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
</head>
<body>
<?php
if($err == 1)
{
	//$df = json_encode($arr);
	//print_r($data);
	//echo $arr['data']->RoleDetailInfo->HPCurr;
}
if($err == -1)
{
	echo "查找数据,服务器错误:<br/>";
	print_r($arr1);
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
<form action="SelectRole.php" method="post" id="formrole">
<div class="mgr_divt">
</div>
<div id="list_area">

<!--/header end-->
<table width="100%" border="0" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist1">
		<tr class="thead2" align="center">
                <td width="9%">背包格索引</td>
        		<td width="9%">背包物品</td>
                <td width="9%">数量</td>
                <td width="9%">到期时间</td>
                <td width="9%">仓库格索引</td>
        		<td width="9%">仓库物品</td>
                <td width="9%">数量</td>
                <td width="9%">到期时间</td>
               <!--  <td width="9%">操作</td> -->
		</tr>
 </table>
 <input type="hidden" value="<?php ?>" id="wp" />
 <table width="100%" border="0" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist">
<?php 
	date_default_timezone_set('PRC');
	$dosql->select_db('resdb');
	$m = mysql_set_charset('latin1');
	$x = (int)($data->ZoneRolePackage->SundriesPackage->Num); //人物背包数量
	$y = (int)($data->ZoneRolePackage->StashPackage->Num);  //仓库背包数量
	if((int)($data->ZoneRolePackage->SundriesPackage->Num) > (int)($data->ZoneRolePackage->StashPackage->Num))
	{
		$m = (int)($data->ZoneRolePackage->SundriesPackage->Num);
	}
	else if((int)($data->ZoneRolePackage->SundriesPackage->Num) < (int)($data->ZoneRolePackage->StashPackage->Num))
	{
		$m = (int)($data->ZoneRolePackage->StashPackage->Num);
	}
	//echo $data->ZoneRolePackage->SundriesPackage->RoleGrids[2]->GridData->RoleArm->DefID;
	$ii = 0;
	$iii = 0;
	for($i=0; $i< $m; $i++ ){
		$pagefag = -1; //标记attributes
		if($data->ZoneRolePackage->SundriesPackage->Num == 1)
		{
			if( $data->ZoneRolePackage->SundriesPackage->RoleGrids->GridData->RoleArm != "")
			{
				$jb = $data->ZoneRolePackage->SundriesPackage->RoleGrids->GridData->RoleArm; //人物背包
			}
			if( $data->ZoneRolePackage->SundriesPackage->RoleGrids->GridData->RoleArm == "")
			{
				$jb = $data->ZoneRolePackage->SundriesPackage->RoleGrids->GridData->RoleItem; //人物背包
			}
		}
		if($data->ZoneRolePackage->StashPackage->Num == 1)
		{
			//$jc = $data->ZoneRolePackage->StashPackage->RoleGrids->GridData->RoleItem;   //仓库包
			if($data->ZoneRolePackage->StashPackage->RoleGrids->GridData->RoleArm != "")
			{
				$jc = $data->ZoneRolePackage->StashPackage->RoleGrids->GridData->RoleArm; //仓库背包
			}
			if($data->ZoneRolePackage->StashPackage->RoleGrids->GridData->RoleArm == "")
			{
				$jc = $data->ZoneRolePackage->StashPackage->RoleGrids->GridData->RoleItem; //仓库背包
			}
		}
		if($data->ZoneRolePackage->SundriesPackage->Num > 1)
		{
			if($ii<$x)
			{
				if($data->ZoneRolePackage->SundriesPackage->RoleGrids[$ii]->GridData->RoleArm != "")
				{
					$jb = $data->ZoneRolePackage->SundriesPackage->RoleGrids[$ii]->GridData->RoleArm; //人物背包
				}
				if($data->ZoneRolePackage->SundriesPackage->RoleGrids[$ii]->GridData->RoleArm == "")
				{
					$jb = $data->ZoneRolePackage->SundriesPackage->RoleGrids[$ii]->GridData->RoleItem; //人物背包
				}
			}
			if($ii >= $x)
			{
				$jb = "";
			}
			$ii++;
		}
		if($data->ZoneRolePackage->StashPackage->Num > 1)
		{
			if($iii < $y)
			{
				if($data->ZoneRolePackage->StashPackage->RoleGrids[$iii]->GridData->RoleArm != "")
				{
					$jc = $data->ZoneRolePackage->StashPackage->RoleGrids[$iii]->GridData->RoleArm; //仓库背包
				}
				if($data->ZoneRolePackage->StashPackage->RoleGrids[$iii]->GridData->RoleArm == "")
				{
					$jc = $data->ZoneRolePackage->StashPackage->RoleGrids[$iii]->GridData->RoleItem; //仓库背包
				}
			}
			if($iii >= $y)
			{
				$jc = "";
			}
			$iii++;
		}
?>
    <tr align=center >
    	<td  height=32 width="9%" ><?php echo $jb->GridIdx; ?></td>
        <td width="9%">
			<?php
				$defid = $jb->DefID;
				$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$defid'";
				$dosql->Query($sql);
				$row = $dosql->GetArray();
				$defName = iconv("gbk","utf8",$row['Name']);
				echo $defName; 
			?>
        </td>
        <td width="9%" ><?php echo  $jb->Num; ?></td>
        <td width="9%" ><?php if($jb == ""){ echo ""; } else if($jb->ValidEnd == 0){ echo "无" ;} else if($jb->ValidEnd != 0){echo date('Y-m-d H:i:s',(int)($jb->ValidEnd)); }?></td>
        <td width="9%" ><?php echo  $jc->GridIdx; ?></td>
        <td width="9%">
			<?php
				$ck = $jc->DefID;  
				$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$ck'";
				$dosql->Query($sql);
				$row1 = $dosql->GetArray();
				$defNameK = iconv("gbk","utf8",$row1['Name']);
				echo $defNameK;
			?></td>
        <td width="9%" ><?php echo $jc->Num; ?></td>
        <td width="9%" ><?php if($jc == ""){ echo ""; } else if($jc->ValidEnd == 0){ echo "无" ;} else if($jc->ValidEnd != 0){echo date('Y-m-d H:i:s',(int)($jc->ValidEnd)); } ?></td>
    </tr>
<?php
	}
?>                               
</table>
<div class="mgr_divb"> 
</div>
</form>

<!--/list end-->
</div>
</body>
</html>