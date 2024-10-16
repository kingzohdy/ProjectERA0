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
if(isset($_GET['qu']))
{
	$q =  $_GET['qu'];
	$f = $_GET['fu'];
	$rolename = $_GET['rolename'];
	$result = ItemInfo($q,$f,$rolename);
	if($result->ErrorCode==0)
	{
		$str = $result->StrXML;
		$data=simplexml_load_string($str);
		//$skillnum = hexdec($data->RoleSkillInfo->SkillNum); 
		$arr = $data->ZoneRoleWear;  //装备
		//$arr2 = $data->Wear->RideList; 
		
	}
	if($result->ErrorCode!=0)
	{
		$err = 1;
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
	}
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
		//print_r($arr);
		//echo $data->RoleSkillInfo->SkillNum;
		//echo hexdec($data->RoleSkillInfo->SkillNum);
		//echo $arr['data']->RoleDetailInfo->HPCurr;
	}
 ?>
<table width="810" height="480" border="1" cellpadding="0" cellspacing="0"  >	
	   <tr align="center" class="mgr_tr" width="93%" height="30">
            <td  align="center" >装备</td>
            <td  align="center" >装备ID</td>
            <td  align="center" >品质</td>
            <td  align="center" >强化(星级：强化百分比)</td>
            <td  align="center" >有效期至</td>
            <td  align="center" >绑定</td>
        </tr>
        <?php 
			date_default_timezone_set('PRC');
		 	 for($i=0; $i<hexdec($arr->ArmNum); $i++ ){
				 //$eq = $arr[$i]->asXML();
				 $aid = $arr->RoleArms[$i]->DefID;
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
            <td  align="center" ><a href="#" ><?php echo $equipName ?></a></td>
            <td  align="center" ><a href="#" ><?php echo $arr->RoleArms[$i]->WID ?></a></td>
            <td  align="center" ><a href="#" ><?php echo $pz ?></a></td>
            <td  align="center" ><a href="#" >
            	<?php
            		if(0==hexdec($arr->RoleArms[$i]->StarLv))
					{
						echo "未强化";
					}
					else
					{
						$qh = "(".hexdec($arr->RoleArms[$i]->StarLv)."星：";
						if(1==hexdec($arr->RoleArms[$i]->StarLv))
						{
							$qh .= hexdec($arr->RoleArms[$i]->StarInfo->ValIdx)+1;
						}
						else
						{
							
							for($ij=0;$ij<hexdec($arr->RoleArms[$i]->StarLv);$ij++)
							{
								$ss =  hexdec($arr->RoleArms[$i]->StarInfo[$ij]->ValIdx);
								$zdj = hexdec($arr->RoleArms[$i]->StarLv);
								$zd = "StarInfo".($ss+1)."_Mul";
								$sqls = "SELECT `$zd` FROM `ArmStarDef` where `Star`='$zdj'";
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
            <td  align="center" ><a href="#" ><?php if($arr->RoleArms[$i]->ValidEnd == 0){echo "永久";} else {echo date('Y-m-d',(int)($arr->RoleArms[$i]->ValidEnd)); } ?></a></td>
            <td  align="center" ><a href="#" ><?php if($arr->RoleArms[$i]->InstFlag & 1 == 1){ echo "是";} else { echo "否";} ?></a></td>
        </tr>
        <?php } ?>
	</table>
</body>
</html>
