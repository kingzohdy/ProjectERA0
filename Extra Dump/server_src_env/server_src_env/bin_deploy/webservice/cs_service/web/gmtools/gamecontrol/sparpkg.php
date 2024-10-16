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
		$arr = $data->ZoneRolePackage->SparPkg;  //晶石
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
<table width="610" height="405" border="1" cellpadding="0" cellspacing="0"  >	
	   <tr align="center" class="mgr_tr" width="93%" height="30">
            <td  align="center" >晶石</td>
            <td  align="center" >晶石ID</td>
            <td  align="center" >等级</td>
            <td  align="center" >经验</td>
        </tr>
        <?php 
			date_default_timezone_set('PRC');
		 	 for($i=0; $i<$arr->Num; $i++ ){
				 //$eq = $arr[$i]->asXML();
				 $aid = $arr->RoleItems[$i]->DefID;
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
            <td  align="center" ><a href="#" ><?php echo $equipName ?></a></td>
            <td  align="center" ><a href="#" ><?php echo $arr->RoleItems[$i]->WID ?></a></td>
            <td  align="center" ><a href="#" ><?php echo $arr->RoleItems[$i]->InstVal1;?></a></td>
            <td  align="center" ><a href="#" ><?php echo $arr->RoleItems[$i]->InstVal2;?></a></td>
        </tr>
        <?php } ?>
	</table>
</body>
</html>
