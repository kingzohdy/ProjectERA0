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
		//print_r($data);
		//echo $arr['data']->RoleDetailInfo->HPCurr;
	}
 ?>
<table width="100%" border="1" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist" >
		<tr align="center" class="mgr_tr">
            <td  height="20"  align="left" colspan="4" >
            &nbsp;&nbsp;基本属性（职业：<?php 
										if(hexdec($data->Career) == 1){ $zy = "战士" ;}
										if(hexdec($data->Career) == 2){ $zy = "法师" ;}
										if(hexdec($data->Career) == 3){ $zy = "枪手" ;}
										if(hexdec($data->Career) == 4){ $zy = "祭祀" ;}
										if(hexdec($data->Career) == 17){ $zy = "守护者" ;}
										if(hexdec($data->Career) == 33){ $zy = "征服者" ;}
										if(hexdec($data->Career) == 18){ $zy = "聚能者" ;}
										if(hexdec($data->Career) == 34){ $zy = "炼魂师" ;}
										if(hexdec($data->Career) == 19){ $zy = "猎杀者" ;}
										if(hexdec($data->Career) == 35){ $zy = "毁灭者" ;}
										if(hexdec($data->Career) == 20){ $zy = "圣佑者" ;}
										if(hexdec($data->Career) == 36){ $zy = "驭雷师" ;}
										 echo $zy; 
									?>
           &nbsp;&nbsp;性别：<?php 
		   						if(hexdec($data->Gender) == 2){ $sex = "女" ;}
								if(hexdec($data->Gender) == 1){ $sex = "男" ;}
								 echo $sex; 
							?>
           &nbsp;&nbsp;等级：<?php echo hexdec($data->Level); ?>）
		</tr>
        <tr align="center" class="mgr_tr">
            <td  height="20" >生命值：<a href="#" ><?php echo $arr->AttrList[2]->Value->HP."/".$arr->AttrList[1]->Value->HPMax; ?></a></td>
            <td >魔法值：<a href="#"><?php echo $arr->AttrList[4]->Value->MP."/".$arr->AttrList[3]->Value->MPMax; ?></a></td>
            <td >力量：<a href="#" ><?php echo $arr->AttrList[5]->Value->Liliang; ?></a></td>
            <td >智力：<a href="#" ><?php echo $arr->AttrList[6]->Value->Zhili; ?></a></td>
		</tr>
        <tr align="center" class="mgr_tr">
            <td height="20" >敏捷：<a href="#" ><?php echo $arr->AttrList[10]->Value->MinJie; ?></a></td>
            <td >体质：<a href="#" ><?php echo $arr->AttrList[8]->Value->Tili; ?></a></td>
            <td >精神：<a href="#" ><?php echo $arr->AttrList[9]->Value->Jingshen; ?></a></td>
           	<td ></td>
		</tr>
        <tr align="center" class="mgr_tr">
            <td  height="20"  align="left" colspan="4" >&nbsp;&nbsp;高级属性(攻击)
		</tr>
        <tr align="center" class="mgr_tr">
            <td  height="20" >物理攻击：<a href="#" ><?php echo $arr->AttrList[24]->Value->PhsicAtk.' -' .$arr->AttrList[25]->Value->PhsicMaxAtk; ?></a></td>
            <td  height="20" >魔法攻击：<a href="#" ><?php echo $arr->AttrList[26]->Value->IceAtk.' -' .$arr->AttrList[27]->Value->MagicMaxAtk; ?></a></td>
            <td >命中等级：<a href="#"><?php echo $arr->AttrList[35]->Value->Hit; ?></a></td>
            <td ></td>
		</tr>
        <tr align="center" class="mgr_tr">
            <td  height="20"  align="left" colspan="4" >&nbsp;&nbsp;高级属性(防御)
		</tr>
        <tr align="center" class="mgr_tr">
            <td height="20" >物理防御：<a href="#" ><?php echo $arr->AttrList[28]->Value->PhsicDef; ?></a></td>
            <td >魔法防御：<a href="#" ><?php echo $arr->AttrList[29]->Value->IceDef; ?></a></td>
            <td >闪避等级：<a href="#" ><?php echo $arr->AttrList[37]->Value->Flee; ?></a></td>
            <td >暴击抵抗：<a href="#" ><?php echo $arr->AttrList[54]->Value->BaseAttrDefHeavy; ?></a></td>
		</tr>
        <tr align="center" class="mgr_tr">
            <td  height="20" >物理伤害减免：<a href="#" ><?php echo $arr->AttrList[30]->Value->DefPhsicHurt; ?></a></td>
            <td >魔法伤害减免：<a href="#"><?php echo $arr->AttrList[31]->Value->DefIceHurt; ?></a></td>
            <td></td><td></td>
		</tr>
        <tr align="center" class="mgr_tr">
            <td  height="20"  align="left" colspan="4" >&nbsp;&nbsp;特殊属性(辅助属性)+携带金钱
		</tr>
        <tr align="center" class="mgr_tr">
            <td height="20" >反伤等级：<a href="#" ><?php echo $arr->AttrList[56]->Value->ReflectionHP; ?></a></td>
            <td >抵抗等级：<a href="#" ><?php echo $arr->AttrList[58]->Value->DefHP; ?></a></td>
            <td >破防等级：<a href="#" ><?php echo $arr->AttrList[57]->Value->IgnoreDef; ?></a></td>
            <td >金币：<a href="#" ><?php echo $arr3->GodCoin; ?></a></td>
		</tr>
        <tr align="center" class="mgr_tr">
            <td height="20" >背包绑银：<a href="#" ><?php echo $arr3->Bull; ?></td>
            <td >金卷：<a href="#" ><?php echo $arr3->BindGodCoin; ?></a></td>
            <td >仓库存款：<a href="#" ><?php echo $arr3->StashMoney; ?></a></td>
            <td >背包银币：<a href="#" ><?php echo $arr3->Money; ?></a></td>
		</tr>
       
</table>   
</body>
</html>
