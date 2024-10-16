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
$q="";
$f="";
//获取服务器列表
if($_SESSION['fwq'] == "")
{
	$result123=fwqlist();
	$_SESSION['fwq'] = $result123;
}
if($_SESSION['fwq'] != "")
{
	 $result123 = $_SESSION['fwq'];
}
if($result123->ErrorCode==0 )
{
	$err = 1;
	$str=$result123->StrXML;
	$arr=simplexml_load_string($str);
}
if($result123->ErrorCode!=0)
{
	$err = -1;
	$arr['ErrorCode'] = $result123->ErrorCode;
	$arr['ErrorText'] = $result123->StrError->ErrorText;
}
//获取角色列表
if(isset($_POST['Uin']))
{
	$q =  $_POST['xdq'];
	$f = $_POST['dqlist'];
	$db1 = $q."_".$f;
	$db = "account_".$q;
	//查找UIN <!--新加
	$acc = $_POST['Uin'];
	$sqluin = "SELECT `uin` FROM Account where `Pass9Account`= '$acc'";
	
	$sql = "select * from `reg_world` where `dbid`='$db1' ";
	$dosql->Execute($sql);
	$rowdb = $dosql->GetArray();
	
	$GLOBALS['db_host'] = $rowdb['dbip'];
	$GLOBALS['db_user'] = $rowdb['dbuser'];
	$GLOBALS['db_name'] = $db;
	$GLOBALS['db_pwd'] = $rowdb['dbpass'];
	
	$dosql->Close(true);
	$dosql = new MySql(false);
	$dateuin = $dosql->GetOne($sqluin);
	$uin = $dateuin['uin'];
	//-->
	$result = RoleList($q,$f,$uin);
	if($result->ErrorCode==0 )
	{
		$err1 = 1;
		$str=$result->StrXML;
		$data=simplexml_load_string($str);
		//$arr1['FindHandleID']=$result->FindHandleID;
		$arr1['data']=$data;
		//$array['data'] = $data;
		//$jsobject =  json_encode($array);
		//exit(-1);
	}
	if($result->ErrorCode!=0)
	{
		$err1 = -1;
		$arr1['ErrorCode'] = $result->ErrorCode;
		$arr1['ErrorText'] = $result->StrError->ErrorText;
		//echo json_encode($arr);
		//exit(-1);
	}	
}
//恢复角色
if(isset($_GET['rwid']))
{
	$rwid = trim($_GET['rwid']);
	$qu = $_GET['qu'];
	$fu = $_GET['fu'];
	/* 
		trim 去除一个字符串两端空格， 
		rtrim 是去除一个字符串右部空格， 
		ltrim 是去除一个字符串左部空格。 
	*/
	/*
		第二种方法：通过正则表达式替换，功能更强 
		php去除字符串首尾空格(包括全角） 
		复制代码 代码如下:
		<? 
		$str="　　　 　脚本之家 www.jb51.net 　　　　"; 
		$str = mb_ereg_replace('^(　| )+', '', $str); 
		$str = mb_ereg_replace('(　| )+$', '', $str); 
		echo mb_ereg_replace('　　', "\n　　", $str); 
		?> 

	*/
	$result = RestoreRole($qu,$fu,$rwid);
	if($result->ErrorCode==0 )
	{
		echo "成功";
	}
	if($result->ErrorCode!=0)
	{
		$arr2['ErrorCode'] = $result->ErrorCode;
		$arr2['ErrorText'] = $result->StrError->ErrorText;
		print_r($arr2);
	}
	exit;	
}	
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>角色列表查询</title>
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
<script type="text/javascript" src="js/jquery.kangThickBoxV2.js" ></script>
<script type="text/javascript" src="js/dateInt.js"></script>
<script type="text/javascript" >
var serverlist = -1;//用于保存服务器列表,若值为-1表示未获得列
var RegArray=new Array(); //查询出来的所有结果.-1表示未查询
var tp = "";//判断用户类型
function selectdq()
{
	var j=$("#xdq").val();
	serverlist=jQuery.parseJSON($("#ff").val());
	q = j;
	$("#dqlist").empty();
	if("-请选择-"== j)
	{
		$("#flist").hide();
	}
	else
	{
		if(1==serverlist.WorldCount && j == parseInt(serverlist.WorldInfos.RegionID))
		{
			$("#dqlist")[0].options.add(new Option(serverlist.WorldInfos.WorldName,serverlist.WorldInfos.WorldID,false, false));
		}
		else
		{
				for(var i = 0; i < serverlist.WorldCount; i++)
				{					 
					 if(j==parseInt(serverlist.WorldInfos[i].RegionID))
					 {
						 $("#dqlist")[0].options.add(new Option(serverlist.WorldInfos[i].WorldName,serverlist.WorldInfos[i].WorldID,false, false));
					 } 	
				}
		}
		$("#flist").show();				
	}
}
//技能获取
function skill(rname,q,f)
{
	var qu=q;
	var fu=f;
	var rolename=rname;
	$.kangThickBox({
		type: "iframe",
		title:"技能列表(符文技能：一个技能关联一个镶嵌的符文ID,0表示无此技能，或未镶嵌符文)",
		source: 'RoleSkill.php?qu='+qu+'&&fu='+fu+'&&rolename='+encodeURI(rolename),
		width: 860,
		height: 500,
		_autoReposi: true
	});
}
function zuoqi(rname,q,f)
{
	var qu=q;
	var fu=f;
	var rolename=rname;
	$.kangThickBox({
		type: "iframe",
		title:"坐骑列表",
		source: 'RoleZuoqi.php?qu='+qu+'&&fu='+fu+'&&rolename='+encodeURI(rolename),
		width: 530,
		height: 330,
		_autoReposi: true
	});
}
//角色信息
function nature(rname,q,f)
{
	//encodeURI();
	var qu=q;
	var fu=f;
	var rolename=rname;
	$.kangThickBox({
		type: "iframe",
		title:"角色信息",
		source: 'RoleNature.php?qu='+qu+'&&fu='+fu+'&&rolename='+encodeURI(rolename),
		width: 680,
		height: 320,
		_autoReposi: true
	});
}
function eqs(rname,q,f)
{
	var qu=q;
	var fu=f;
	var rolename=rname;
	$.kangThickBox({
		type: "iframe",
		title:"角色装备（含时装）",
		source: 'equip.php?&&qu='+qu+'&&fu='+fu+'&&rolename='+encodeURI(rolename),
		width: 850,
		height: 550,
		_autoReposi: true
	});
}
//踢下线
function tiAccount(name,q,f)
{
	var name = name;
	$.ajax
		({
			type: 'POST',
			url: 'conduct.php',
			dataType: 'json',
			data: {"tiAccountName":"","quID": q,"AccountName":name}, 
			beforeSend:function(){
				$("#BgDiv").css({ display: "block", height: $(document).height() });
				$('#DialogDiv').show();
			 },
			success: function(array)
			{
				if(array == 0)
				{
					$('#DialogDiv').hide();
					$("#BgDiv").hide();
					alert("成功");
					window.location.reload(); 
				}
				else
				{
					alert("ErrorCode: "+fg.ErrorCode+"\nErrorText: "+fg.ErrorText);
					window.location.reload(); 
				}
			}
		});
}
function dongjieRole(RoleName,q,f)
{
	var qu=q;
	var fu= f;
	var RoleName = RoleName;
	$.kangThickBox({
		type: "iframe",
		title:"封角色",
		source: 'FreezeAccount.php?FreezeRole=1&&q='+qu+'&&f='+fu+'&&RoleName='+RoleName+'',
		width: 240,
		height: 100,
		_autoReposi: true
	});
}
function jingyanRole(AccName,q,f)
{
	var qu=q;
	var RoleName = AccName;
	var fu = f;
	$.kangThickBox({
		type: "iframe",
		title:"禁言角色",
		source: 'SilenceAccount.php?q='+qu+'&&fu='+fu+'&&RoleName='+RoleName+'',
		width: 240,
		height: 100,
		_autoReposi: true
	});
}
function zuobiao(RoleName,q,f)
{
	var qu=q;
	var fu=f;
	var RoleName = RoleName;
	$.kangThickBox({
		type: "iframe",
		title:"修改角色坐标",
		source: 'UpdateCoord.php?qu='+qu+'&&fu='+fu+'&&RoleName='+encodeURI(RoleName),
		width: 210,
		height: 170,
		_autoReposi: true
	});
}
//查找信息
function mingxi()
{
	q = $("#xdq").val();
	f = $("#dqlist").val();
	var uin = $("#uin").val();
	if(q == "" || f== "" || uin == "")
	{
		alert("参数错误!");
	}
	else{
		$("#formrole").submit();
	}
}
//打印js对象 （用于调试）
function allPrpos ( obj )
{    
    var props = "";
    for ( var p in obj )
    {
        if ( typeof ( obj[p] ) == " function " )
        {
            //obj[p](); 
        } else {
            // p 为属性名称，obj[p]为对应属性的值
            props += p + " => " + obj[p] + "\n\n";
        }
    }
    alert ( props );
}
</script>
</head>
<body>
<?php
if($err == -1)
{
	echo "获取服务器,服务器错误:<br/>";
	print_r($arr);
	exit();
}
if($err == 1)
{
	$df = json_encode($arr);
	//print_r($arr);
	//echo $arr['data']->RoleDetailInfo->HPCurr;
}
if($err1 == -1)
{
	echo "查找数据,服务器错误:<br/>";
	print_r($arr1);
	exit();
}
if($err1 == 1)
{
	$df1 = json_encode($arr1);
	//print_r($arr1);
	//echo $arr['data']->RoleDetailInfo->HPCurr;
}
 ?>
<div id="BgDiv"></div>
<div id="DialogDiv" style="display:none">
	<h2 style="color:#F00">请稍等...</h2>
</div>
</div>
<form action="rolelists.php" method="post" id="formrole">
<div class="mgr_divt">
<table width="100%" height="100%">
	<tr>
		<td >
        	<select name="xdq" id="xdq" style="float:right;vertical-align:top; margin-top:6px;" onchange="selectdq()" >
                <?php 
					if($arr->AccountCount == 1)
					{
						$qid = $arr->AccountInfos->RegionID;
						$qname = $arr->AccountInfos->RegionName;
						 echo "<option value=$qid  >$qname</option>";
						 //echo "<input type=hidden value=$qname;' name=qn />";
					}
					else
					{
						for($i=0; $i< $arr->AccountCount; $i++)
						{
							$qid = $arr->AccountInfos[$i]->RegionID;
							$qname = $arr->AccountInfos[$i]->RegionName;
							 echo "<option value=$qid  >$qname</option>";
						}
					}
				?>
                <option value='-请选择-' selected='selected' >-请选择-</option>
        	</select>
            <input type="hidden" value='<?php echo $df; ?>' id="ff" />
        </td>
        <td width="31px" style="display:none;" id="flist">
        	<select name="dqlist" id="dqlist" style="float:left;vertical-align:top; margin-top:6px; width:auto" >
        	 </select>
        </td>
         <td style=" padding:0px; margin:0px;vertical-align:top; ">
				<div id="search" class="search" style="float:left;vertical-align:middle;"> <span class="s">
				<input style="float:left;vertical-align:middle;color:#999;" name="Uin" id="Uin" type="text" value="输入账号进行搜索" onclick="javascript:this.value=''" title="输入账号进行搜索"/>
				</span> <span class="b"><a href="javascript:;" onclick="mingxi()"><img src="images/search_btn.png" title="搜索" /></a></span></div>
                <span style="float:left;vertical-align:top;color:#F00;margin-top:8px;" >请输入精确账号</span>
            <span style="color:#F00" id="us1"></span>
        </td>
     </tr>
</table>
</div>
<div id="list_area">

<!--/header end-->

<table width="100%" border="0" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist">
		<tr class="thead2" align="center">
                <td >角色ID</td>
        		<td >角色名</td>
                <td >大区</td>
                <td >服</td>
                <td >是否删除</td>
                <td >操作</td>
		</tr>
<?php 
	$msg  = $arr1['data'];
	date_default_timezone_set('PRC');
	for($i=0; $i< $msg->RoleNum; $i++ ){
		$pagefag = -1; //标记
		if($msg->RoleNum == 1)
		{
			$j = $msg->RoleList;
		}
		if($msg->RoleNum > 1)
		{
			$j = $msg->RoleList[$i];
		}
		
?>
    <tr align=center >
    	<td ><?php echo  $j->RoleID; ?></td>
        <td height=32 ><?php echo $j->RoleName ?></td>
        <td ><?php echo $q; ?></td>
        <td ><?php echo $f; ?></td>
        <td ><?php if(hexdec($j->IsDelete) == 0){echo "否";} 
					if(hexdec($j->IsDelete) == 1){ ?>是&nbsp;(
                        <a href='rolelists.php<?php echo "?rwid=".trim($j->RoleID)."&qu=".$q."&fu=".$f; ?>' >还原</a>)
        			<?php } ?>
        </td>
        <td >
        	
             [<a href="#" onclick=eqs('<?php echo $j->RoleName;?>','<?php echo (int)$q; ?>','<?php echo (int)$f; ?>') >装备</a>]
            [<a href="#" onclick=nature('<?php echo $j->RoleName;?>','<?php echo (int)$q;?>','<?php echo (int)$f;?>') >角色信息</a>]
            [<a href="#" onclick=skill('<?php echo $j->RoleName;?>','<?php echo (int)$q;?>','<?php echo (int)$f;?>') >技能信息</a>]
            [<a href="#" onclick=zuoqi('<?php echo $j->RoleName;?>','<?php echo (int)$q; ?>','<?php echo (int)$f;?>') >坐骑信息</a>]
            [<a href="#" onclick=zuobiao('<?php echo $j->RoleName;?>','<?php echo (int)$q;?>','<?php echo (int)$f;?>') >修改坐标</a>]
            [<a href="email.php?role=<?php echo $j->RoleName;?>&&dq=<?php echo (int)$q;?>&&fu=<?php echo (int)$f;?>" >邮件查看</a>]
            [<a href="res.php?role=<?php echo $j->RoleName;?>&&dq=<?php echo $q;?>&&fu=<?php echo $f;?>" >物品信息</a>]
            <?php if($_SESSION['admin1'] != "游戏管理员"){ echo "<!--"; } ?>
              [<a href="#" onclick = dongjieRole('<?php echo $j->RoleName;?>','<?php echo (int)$q;?>','<?php echo (int)$f; ?>') >冻结此角色</a>][<a href="#" onclick = jingyanRole('<?php echo $j->RoleName;?>','<?php echo (int)$q;?>','<?php echo (int)$f; ?>') >禁言/解禁(角色)</a>]
            <? if($_SESSION['admin1'] != "游戏管理员"){ echo "-->"; } ?>
       </td>
    </tr>
<?php
	}
?>                               
</table>
</form>
<div class="mgr_divb"> 
</div>
</form>

<!--/list end-->
</div>
</body>
</html>