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
//第一次查询角色
if(isset($_POST['role']))
{
	$q =  $_POST['xdq'];
	$f = $_POST['dqlist'];
	$rolename = $_POST['role'];
	$cfs = $_POST['cxfs'];
	if($cfs == 2)
	{
		$rolename = "%".$rolename."%";
	}
	$result = SelctRole($q,$f,$rolename);
	if($result->ErrorCode==0 )
	{
		$err1 = 1;
		$str=$result->StrXML;
		$data=simplexml_load_string($str);
		$arr1['FindHandleID']=$result->FindHandleID;
		$arr1['data']=$data;
		$array['data'] = $data;
		$jsobject =  json_encode($array);
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
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>角色查找</title>
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
#DialogDivIDC{position:absolute;width:240px;height:120px; left:35%; top:20%;z-index:100000002;overflow:hidden;box-shadow:0 10px 8px #6c6c6c; background-color:#fff;}
</style>
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="js/jquery.kangThickBoxV2.js" ></script>
<script type="text/javascript" src="js/dateInt.js"></script>
<script type="text/javascript" >
var serverlist = -1;//用于保存服务器列表,若值为-1表示未获得列
var FindId=-1;//当前句柄
var RegArray=new Array(); //查询出来的所有结果.-1表示未查询
var page = ""; //当前页
var pageSum = 0; //最后一页的页数
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
//查找信息
function mingxi()
{
	q = $("#xdq").val();
	f = $("#dqlist").val();
	var rolename = $("#role").val();
	if(q == "" || f== "" || rolename == "")
	{
		alert("参数错误!");
	}
	else{
		$("#formrole").submit();
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
function jingshi(rname,q,f)
{
	var qu=q;
	var fu=f;
	var rolename=rname;
	$.kangThickBox({
		type: "iframe",
		title:"装备晶石",
		source: 'sparpkg.php?&&qu='+qu+'&&fu='+fu+'&&rolename='+encodeURI(rolename),
		width: 650,
		height: 450,
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
//角色天赋
function tianfu(rname,q,f)
{
	//encodeURI();
	var qu=q;
	var fu=f;
	var rolename=rname;
	$.kangThickBox({
		type: "iframe",
		title:"天赋信息",
		source: 'tianfu.php?qu='+qu+'&&fu='+fu+'&&rolename='+encodeURI(rolename),
		width: 680,
		height: 320,
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
				if(array.ErrorCode)
				{
					alert("ErrorCode: "+array.ErrorCode+"\nErrorText: "+array.ErrorText);
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
function dongjie(AccName,q)
{
	var qu=q;
	var AccName = AccName;
	$.kangThickBox({
		type: "iframe",
		title:"冻结账号",
		source: 'FreezeAccount.php?FreezeAccount=0&&q='+qu+'&&AccName='+AccName+'',
		width: 300,
		height: 260,
		_autoReposi: true
	});
}
function fck(AccName,q,f)
{
	var qu=q;
	var AccName = AccName;
	$.kangThickBox({
		type: "iframe",
		title:"修改安全锁密码",
		source: 'ContainerLock.php?q='+qu+'&&AccName='+AccName+'',
		width: 240,
		height: 100,
		_autoReposi: true
	});
}
function jingyan(AccName,q)
{
	var qu=q;
	var AccName = AccName;
	$.kangThickBox({
		type: "iframe",
		title:"禁言账号",
		source: 'SilenceAccount.php?q='+qu+'&&AccName='+AccName+'',
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
//上一页
function pup()
{
	if(page == "" || page == 1)
	{
		alert("已在第一页");
	}
	if(page > 1)
	{
		var msg = RegArray[page-2];
		$("#ajaxlist").empty();
		shuchu(msg);
		page-=1;//当前页
	}
}
function pnext()
{
	var d = '';
	var msg = '';
	var jg='';
	if(page == "")
	{
		page = $("#pg").val();
		if(page == "")
		{
			alert("请输入角色查询");
			exit;
		}
		if(page != "")
		{
			page = 1;
			//var jj = jQuery.parseJSON($("#xinxi").val());
			//alert(jj);
			FindId = $("#fid").val();
			RegArray[0] = jQuery.parseJSON($("#xinxi").val());
		}
	}
	if(page == RegArray.length)
	{
		$.ajax
		({
			type: 'POST',
			url: 'conduct.php',
			dataType: 'json',
			data: {"roleselect2np":"","page": FindId}, 
			beforeSend:function(){
				$("#BgDiv").css({ display: "block", height: $(document).height() });
				$('#DialogDiv').show();
			 },
			success: function(array)
			{
				$('#DialogDiv').hide();
				$("#BgDiv").hide();
				if(array.ErrorCode == -6)
				{
					alert("没有了......");
					exit;
				}
				if(array.ErrorCode == -4)
				{
					alert("参数传递错误");
					exit;
				}
				if(array.ErrorCode)
				{
					alert("ErrorCode: "+array.ErrorCode+"\nErrorText: "+array.ErrorText);
					exit;
				}
				RegArray[page] = array;
				msg = RegArray[page];
				page+=1;//当前页
				$('#DialogDiv').hide();
				$("#BgDiv").hide();
				$("#ajaxlist").empty();	
				shuchu(msg);
			},
			error:function(array)
			{
				alert(array+'\n发送数据出错，请重新发送');
				$('#DialogDiv').hide();
				$("#BgDiv").hide();
				exit;
			}
		});
	}
	if(page < RegArray.length)
	{
		msg = RegArray[page];
		//alert(RegArray[page]);
		page+=1;//当前页
		$("#ajaxlist").empty();	
		shuchu(msg);
	}
}//allPrpos(msg);
function shuchu(array){	
		//alert(msg.RoleArray[0].Pass9Account);
		//allPrpos(msg);
		msg = array.data;
		var jg;	
		tp = $("#tp").val();
		var d='';
	for(var i = 0; i < msg.ResNum; i++)
	{
		if(msg.ResNum == 1)
		{
			jg = msg.RoleArray;
		}
		if(msg.ResNum > 1)
		{
			jg = msg.RoleArray[i];
		}
		if(jg.OnlineFlag ==0 ){jg.OnlineFlag = "不在线";}
		else{jg.OnlineFlag = "在线";}
			d +='<tr align=center ><td width="10%" >'+jg.Pass9Account+'</td><td  height=32 width="12%">'+jg.RoleName+'</td><td width="10%" >'+jg.RoleWID+'</td><td width="16%" >'+date('Y-m-d H:i:s',jg.CreateTime)+'</td><td width="8%" >'+parseInt(jg.RegionID)+'</td><td width="8%" >'+jg.ServerID+'</td><td width="9%" >'+jg.OnlineFlag+'</td><td width="9%" style="color:#00F" >'+jg.Uin+'</td><td width="15%" >[<a href=# onclick = tiAccount("'+jg.Pass9Account+'","'+parseInt(jg.RegionID)+'","'+parseInt(jg.ServerID)+'") >踢下线</a>][<a href="#" onclick=eqs("'+jg.RoleName+'","'+parseInt(jg.RegionID)+'","'+parseInt(jg.ServerID)+'") >装备</a>][<a href="#" onclick=jingshi("'+jg.RoleName+'","'+parseInt(jg.RegionID)+'","'+parseInt(jg.ServerID)+'") >晶石</a>][<a href="#" onclick=nature("'+jg.RoleName+'","'+parseInt(jg.RegionID)+'","'+parseInt(jg.ServerID)+'") >角色信息</a>][<a href="#" onclick=tianfu("'+jg.RoleName+'","'+parseInt(jg.RegionID)+'","'+parseInt(jg.ServerID)+'") >天赋</a>][<a href="#" onclick=skill("'+jg.RoleName+'","'+parseInt(jg.RegionID)+'","'+parseInt(jg.ServerID)+'") >技能信息</a>][<a href="#" onclick=zuoqi("'+jg.RoleName+'","'+parseInt(jg.RegionID)+'","'+parseInt(jg.ServerID)+'") >坐骑</a>][<a href="#" onclick=zuobiao("'+jg.RoleName+'","'+parseInt(jg.RegionID)+'","'+parseInt(jg.ServerID)+'") >修改坐标</a>][<a target="_blank" href="email.php?role='+jg.RoleName+'&&dq='+parseInt(jg.RegionID)+'&&fu='+parseInt(jg.ServerID)+'" >邮件查看</a>][<a target="_blank" href="res.php?role='+jg.RoleName+'&&dq='+parseInt(jg.RegionID)+'&&fu='+parseInt(jg.ServerID)+'") >物品</a>]';
		if("高级游戏管理员"==tp)
		{
			//[<a href="#" class="box a2" onclick="fck()"  >改安全锁</a>]第二版新加
			d += '[<a href="#" onclick = jingyanRole("'+jg.RoleName+'","'+parseInt(jg.RegionID)+'","'+parseInt(jg.ServerID)+'") >禁言/解禁(角色)</a>][<a href="#" onclick = jingyan("'+jg.Pass9Account+'","'+parseInt(jg.RegionID)+'") >禁言/解禁(账号)</a>][<a href="#" onclick = dongjie("'+jg.Pass9Account+'","'+parseInt(jg.RegionID)+'") >冻结此帐号</a>][<a href="#" onclick = sfyz("'+jg.Pass9Account+'") >修改安全邮箱</a>]';
		}
		if("超级游戏管理员"==tp)
		{
			d += '[<a href="#" onclick=fck("'+jg.Pass9Account+'","'+parseInt(jg.RegionID)+'","'+parseInt(jg.ServerID)+'")  >改安全锁</a>]';
		}
		d += '</td></tr>';
	}
	$("#ajaxlist").append(d);
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
//修改安全邮箱
function sfyz(acc)
{
	acct = acc;
	$("#BgDiv").css({ display: "block", height: $(document).height() });
	$("#DialogDivIDC").show();
}
function upel()
{
	var sfz = $("#sfz").val();
	if(sfz.length != 18 && sfz.length != 15)
	{
		$("#DialogDivIDC").hide();
		$("#BgDiv").hide();
		exit;
	}
	else
	{
		$.ajax
		({
			url: 'safetyEmail.php',
			type: 'post',
			dataType: 'json',
			data: {"idc":sfz,"acc":acct}, 
			success: function(msg)
			{
				$("#DialogDivIDC").hide();
				$("#BgDiv").hide();
				if(msg > 2)
				{
					alert("验证失败");
					exit;
				}
				else if(msg <= 2)
				{
					if(msg>0)
					{
						alert("错 "+msg+" 位身份证号码,请谨慎修改");
					}
					upmail(acct,23);
					exit;
				}
			}
		});
	}
}
function upmail(AccName,bj)
{
	var biaoji=bj;
	var AccName = AccName;
	if(biaoji == 23){
		$.kangThickBox({
			type: "iframe",
			title:"修改安全邮箱",
			source: 'safetyEmail.php?AccName='+AccName+'',
			width: 240,
			height: 100,
			_autoReposi: true
		});
	}
	else
	{
		alert("非法访问");
	}
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
	//print_r($arr);
	//echo $arr['data']->RoleDetailInfo->HPCurr;
}
 ?>
<div id="BgDiv"></div>
<div id="DialogDiv" style="display:none">
	<h2 style="color:#F00">请稍等...</h2>
</div>
</div>
<div id="DialogDivIDC" style="display:none">
	<table width="100%" height="100%">
    <tr align="center"><td  style="color:#F00; font-size:16px;">请输入该账户的身份证：</td></tr>
    <tr align="center"><td><input type="text" id="sfz" style="width:70%"/></td></tr>
    <tr align="center"><td><input type="button" value="确定" style="width:40px; height:25px;" onclick="upel()" /></td></tr></table>
</div>
<form action="SelectRole.php" method="post" id="formrole">
<div class="mgr_divt">
<table width="100%" height="100%">
	<tr>
		<td >
        	<select name="xdq" id="xdq" style="float:right;vertical-align:top; margin-top:6px;" onchange="selectdq()" >
                <?php 
					if($arr->AccountCount == 1)
					{
						$q = $arr->AccountInfos->RegionID;
						$f = $arr->AccountInfos->RegionName;
						 echo "<option value=$q  >$f</option>";
					}
					else
					{
						for($i=0; $i< $arr->AccountCount; $i++)
						{
							$q = $arr->AccountInfos[$i]->RegionID;
							$f = $arr->AccountInfos[$i]->RegionName;
							 echo "<option value=$q  >$f</option>";
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
				<input style="float:left;vertical-align:middle;" name="role"  id="role" type="text" title="输入角色进行搜索"/>
				</span> <span class="b"><a href="javascript:;" onclick="mingxi()"><img src="images/search_btn.png" title="搜索" /></a></span></div>
                <span style="float:left;vertical-align:top; margin-top:6px;" ><input type="radio" value="1" name="cxfs" />&nbsp;&nbsp;精确查&nbsp;&nbsp;<input type="radio" value="2" name="cxfs" checked="checked"/>&nbsp;&nbsp;模糊查</span>
            <span style="color:#F00" id="us1"></span>
        </td>
     </tr>
</table>
</div>
<div id="list_area">

<!--/header end-->
<table width="100%" border="0" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist1">
		<tr class="thead2" align="center">
                <td width="10%">账号</td>
        		<td width="12%">角色名</td>
                <td width="10%">角色ID</td>
                <td width="16%">角色创建时间</td>
                <td width="8%">大区</td>
                <td width="8%">服ID</td>
               	<td width="9%">是否在线</td>
                <td width="9%">Uin</td>
                <td width="15%">操作</td>
		</tr>
 </table>
 <table width="100%" border="0" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist">
<?php 
	$msg  = $arr1['data'];
	date_default_timezone_set('PRC');
	for($i=0; $i< $msg->ResNum; $i++ ){
		$pagefag = -1; //标记
		if($msg->ResNum == 1)
		{
			$j = $msg->RoleArray;
		}
		if($msg->ResNum > 1)
		{
			$j = $msg->RoleArray[$i];
		}
		if($j->OnlineFlag ==0 ){$j->OnlineFlag = "不在线";}
		else{$j->OnlineFlag = "在线";}
		
?>
    <tr align=center >
    	<td width="10%" ><?php echo  $j->Pass9Account ?></td>
        <td  height=32 width="12%"><?php echo $j->RoleName ?></td>
        <td width="10%" ><?php echo $j->RoleWID ?></td>
        <td width="16%" ><?php echo date('Y-m-d H:i:s',(int)($j->CreateTime)); ?></td>
        <td width="8%" ><?php echo hexdec($j->RegionID) ?></td>
        <td width="8%" ><?php echo $j->ServerID ?></td>
        <td width="9%" ><?php  echo $j->OnlineFlag ?></td>
        <td width="9%" style="color:#00F" ><?php echo $j->Uin ?></td>
        <td width="15%" >
        	[<a href=# onclick = tiAccount('<?php echo $j->Pass9Account;?>','<?php echo hexdec($j->RegionID);?>','<?php echo (int)($j->ServerID);?>') >踢下线</a>]
        	
            [<a href="#" onclick=eqs('<?php echo $j->RoleName;?>','<?php echo hexdec($j->RegionID);?>','<?php echo (int)($j->ServerID);?>') >装备</a>]
            [<a href="#" onclick=jingshi('<?php echo $j->RoleName;?>','<?php echo hexdec($j->RegionID);?>','<?php echo (int)($j->ServerID);?>') >晶石</a>]
            [<a href="#" onclick=nature('<?php echo $j->RoleName;?>','<?php echo hexdec($j->RegionID);?>','<?php echo (int)($j->ServerID);?>') >角色信息</a>]
           
            [<a href="#" onclick=tianfu('<?php echo $j->RoleName;?>','<?php echo hexdec($j->RegionID);?>','<?php echo (int)($j->ServerID);?>') >天赋</a>]
            [<a href="#" onclick=skill('<?php echo $j->RoleName;?>','<?php echo hexdec($j->RegionID);?>','<?php echo (int)($j->ServerID);?>') >技能信息</a>]
            [<a href="#" onclick=zuoqi('<?php echo $j->RoleName;?>','<?php echo hexdec($j->RegionID);?>','<?php echo (int)($j->ServerID);?>') >坐骑信息</a>]
            [<a href="#" onclick=zuobiao('<?php echo $j->RoleName;?>','<?php echo hexdec($j->RegionID);?>','<?php echo (int)($j->ServerID);?>') >修改坐标</a>]
            [<a target="_blank" href="email.php?role=<?php echo $j->RoleName;?>&&dq=<?php echo hexdec($j->RegionID);?>&&fu=<?php echo (int)($j->ServerID);?>" >邮件查看</a>]
            [<a target="_blank" href="res.php?role=<?php echo $j->RoleName;?>&&dq=<?php echo hexdec($j->RegionID);?>&&fu=<?php echo (int)($j->ServerID);?>" >物品信息</a>]
            <?php if($_SESSION['admin1'] == "游戏管理员"){ echo "<!--"; } ?>
            [<a href="#" onclick = jingyanRole('<?php echo $j->RoleName;?>','<?php echo hexdec($j->RegionID);?>','<?php echo (int)($j->ServerID);?>') >禁言/解禁(角色)</a>]
            [<a href="#" onclick = jingyan('<?php echo $j->Pass9Account;?>','<?php echo hexdec($j->RegionID);?>') >禁言/解禁(账号)</a>]
            [<a href="#" onclick = dongjie('<?php echo $j->Pass9Account;?>','<?php echo hexdec($j->RegionID);?>') >冻结此帐号</a>]
              [<a href="#" onclick = dongjieRole('<?php echo $j->RoleName;?>','<?php echo hexdec($j->RegionID);?>','<?php echo (int)($j->ServerID);?>') >冻结此角色</a>]
              [<a href="#" onclick = sfyz('<?php echo $j->Pass9Account;?>') >修改安全邮箱</a>]            
            <? if($_SESSION['admin1'] == "游戏管理员"){ echo "-->"; } ?>
            <?php if($_SESSION['admin1'] != "超级游戏管理员"){ echo "<!--"; } ?>
            [<a href="#" onclick=fck('<?php echo $j->Pass9Account;?>')  >改安全锁</a>]
             <? if($_SESSION['admin1'] != "超级游戏管理员"){ echo "-->"; } ?>
       </td>
    </tr>
<?php
	}
?>                               
</table>
<div class="mgr_divb"> 
</div>
<div class="page_area" style="margin-top:10px; text-align:center">
    <input type="hidden" value='<?php echo $_SESSION['admin1']; ?>' id="tp" />
    <input type="hidden" value='<?php echo $jsobject; ?>' id="xinxi" />
    <input type="hidden" value='<?php echo $pagefag; ?>' id="pg" />
    <input type="hidden" value='<?php echo $arr1['FindHandleID']; ?>' id="fid" />
	<input type="button" value="上一页" id="up" onclick="pup()" />&nbsp;&nbsp;&nbsp;&nbsp;
    <input type="button" value="下一页" id="next" onclick="pnext()" />
</div>
</form>

<!--/list end-->
</div>
</body>
</html>