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
	$result = RoleInfo($RegionID,$WorldID,$RoleName);
	if($result->ErrorCode==0)
	{
		/*$str=$result->StrXML;
		$data=simplexml_load_string($str);
		$array['data']=$data;
		echo json_encode($array);
		exit(-1);*/
		$str = $result->StrXML;
		$data=simplexml_load_string($str);
		$arr = $data->AttrVal;//->AttrList;
		$arr3 = $data->RoleDetailInfo;
		$arrtf = $data->GeniusInfo;
		
	}
	if($result->ErrorCode!=0)
	{
		$err = 1;
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		/*echo json_encode($arr);
		exit(-1);*/
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
		//print_r($arrtf);
		//echo $arr['data']->RoleDetailInfo->HPCurr;
	}
 $dosql->select_db('resdb');
 mysql_set_charset('latin1');
 ?>
<table width="100%" border="1" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist" >
		<?php for($tf=0;$tf< hexdec($arrtf->Num); $tf++){ ?>
		<tr align="center" class="mgr_tr">
            <td  height="20"  align="left" colspan="4" >
            &nbsp;&nbsp;<?php if(hexdec($arrtf->One[$tf]->Type) == 1){ echo "基础天赋";} if(hexdec($arrtf->One[$tf]->Type) == 2){ echo "狂暴天赋";} else { echo "";}?>
		</tr>
        <?php 
		for($x=0;$x< ceil(hexdec($arrtf->One[$tf]->Num)/4); $x++)
			{ 
		?>
        <tr align="center" class="mgr_tr">
        	<?php 
				for($t=0; $t<4; $t++ ){ 
					$ti = $t+$x*4;
					$AttrID = hexdec($arrtf->One[$tf]->SubOne[$ti]->AttrID);
					$Lvl = hexdec($arrtf->One[$tf]->SubOne[$ti]->Lvl);
					$zy = hexdec($data->Career);
					if($ti>= $arrtf->One[$tf]->Num ){ $AttrID=""; $Lvl=""; }
					$sql = "SELECT `Name` FROM `GeniusAttrDef` where `Career`='$zy' and `AttrID`='$AttrID' and `AttrLvl`='$Lvl' ";
					$dosql->Query($sql);
					$row = $dosql->GetArray();
					$tfName = iconv("gbk","utf8",$row['Name']);
			?>
            	<td  height="32" ><a href="#" ><?php echo $tfName."/".$Lvl."级"; ?></a></td>
            <?php } ?>
        <?php } ?>
        <?php } ?>
</table>    
</body>
</html>
