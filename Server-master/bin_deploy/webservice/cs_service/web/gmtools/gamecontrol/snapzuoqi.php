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
	return 1;
}
if(isset($_GET['zq']))
{
	$hex =$_SESSION['hex'];
	$_SESSION['hex'] = "";
	$result = ParseBlob($hex);
	if($result->ErrorCode==0)
	{
		$str = $result->StrXML;
		$data=simplexml_load_string($str);
		//$skillnum = hexdec($data->RoleSkillInfo->SkillNum); 
		$arr = $data->MiscInfo->RideList;
		
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
<table width="500" height="270" border="1" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist" >
        <?php 
			date_default_timezone_set('PRC');
			if($arr->Num == 0 || $arr->Num == "")
			{
				echo "无坐骑信息";
			}
		 	else{
				 for($i=0; $i<$arr->Num; $i++ ){ 
		?>
        <tr align="center" class="mgr_tr">
            <td  align="center" colspan="4" >坐骑ID：<a href="#"  ><?php echo $arr->RideGrids[$i]->ItemID ?></a></td>
            <td  align="center" >等级：<a href="#"  ><?php echo hexdec($arr->RideGrids[$i]->Level); ?></a></td>
            <td  align="center" >有效期至：<a href="#"  ><?php if($arr->RideGrids[$i]->Endtime == 0){echo "永久";} else {echo date('Y-m-d H:i:s',(int)($arr->RideGrids[$i]->Endtime)); } ?></a></td>
        </tr>
        <?php } 
		}?>
	</table>
</body>
</html>
