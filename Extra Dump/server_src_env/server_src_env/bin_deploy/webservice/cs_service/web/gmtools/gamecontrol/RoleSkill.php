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
if(isset($_GET['qu']))
{
	$RegionID = $_GET['qu'];
	$WorldID = $_GET['fu'];
	//urldecode();
	$RoleName = urldecode($_GET['rolename']);
	$result = SkillInfo($RegionID,$WorldID,$RoleName);
	if($result->ErrorCode==0)
	{
		$str = $result->StrXML;
		$data=simplexml_load_string($str);
		$skillnum = hexdec($data->RoleSkillInfo->SkillNum); 
		$arr = $data->RoleSkillInfo->SkillList;
		
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
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/jscript">
</script>
<body>
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
<table width="820px" height="440px;" border="1" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist" >
<?php 
	for($i=0; $i<$skillnum; $i++){
?>
		<tr align="center" class="mgr_tr">
            <td  height="auto" width="auto"  align="left"  style="font-weight:bold" >&nbsp;&nbsp;技能:<?php echo $arr[$i]->SkillID; ?>
            <td  height="auto" >技能等级:<a href="#" ><?php echo hexdec($arr[$i]->SkillLevel); ?></a></td>
        	<!--<td >符文槽是否可用:<a href="#" ><?php if($arr[$i]->RuneSlotEnabled == 0){ echo "否";} else {echo "是";} ?></a></td> -->
            <td  >符文(技能：符文ID):<a href="#" >
							<?php 
									for($j=0;$j<4;$j++){
										echo $arr[$i]->RuneInfo[$j]->SkillID.":".$arr[$i]->RuneInfo[$j]->RuneItemID."<b style='color:#f00'>|</b>";
									}
							 ?>
               				 </a>
            </td>
		</tr>
<?php
	}
?>
</table>   
</body>
</html>
