<?php	require_once(dirname(__FILE__).'/../inc/config.inc.php');
//检测用户是否有权限
if($_SESSION['admin1'] == "")
{
	$_SESSION = array();
	session_destroy();	
	echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
	exit();
}
 ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>角色快照</title>
<link href="style/admin.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="../data/plugin/calendar/calendar1.js"></script>
<script type="text/javascript" src="js/jquery.kangThickBoxV2.js" ></script>
<script type="text/javascript">
	var i = 0;
	var j = 0;
	var t = 0;
	var $tri=0;
	var qu = "";
	var fu ="";
$(document).ready(function(){
  $("#ttt").hide();
});
function xf()
{
	//$GLOBALS['db_name'];
	var keyword = $("#dq").val();
	if(keyword == '' || keyword == '-请选择-')
	{
		$("#qu").html("请选择大区");
		$("#fw").empty();
		return;
	}
	if(keyword != '' && keyword != '-请选择-')
	{
		$("#qu").html("");
		i=1;
	}

	$.ajax({
		url : "select_log.php",
		type:'post',
		dataType:'json',
		data:{"daqu":keyword},
		success:function(msg){
			 $(document).ready(function () {
 				 $("#fw").empty();
 				 $.each(msg, function (index, value) {
 				 	$("#fw")[0].options.add(new Option(value.worldname, value.worldid, false, false));
  				});
  			});
		}
	});
}
function jc()
{
	 var stp = $("#us").val();
	if(stp == '')
	{
		$("#us1").html("请输入角色名");
		//$("#tt1").html("请输入角色名");
		//$("#ttt").show();
		return;
	}
	if(stp != '')
	{
		$("#us1").html(""); 
		j=1;
	}
}
function stime()
{
	 var st = $("#posttime").val();
	if(st == '')
	{
		$("#sta").html("请选择开始时间");
		return;
	}
	if(st != '')
	{
		$("#sta").html(""); 
		t=1;
	}
}
//搜索
function sear()
{
	 if(i ==1 && j == 1&& t==1){
	qu=$("#dq").val();
	fu=$("#fw").val();
	 //选择服务器
	var f = qu+'_'+fu;
	//时间
	var tb=$("#posttime").val();
	var tb1=$("#posttime1").val();
	var fag = $("#jz").val();
	//角色名
	var jsm=$("#us").val();
		
	$.ajax({
		type: 'POST',
		url: 'select_log.php',
		dataType: 'json',
		data: {"LogRoleSnapshot":"","xzdb": f,"time":tb, "time1":tb1, "us":jsm,"fag":fag,"qq":fu},
		success: function(msg){
			if( null ==msg || ""== msg)
			{
				$("#tabr").empty();
				$("#tt1").html("未检索到数据......");
				$("#ttt").show();
				exit();
			}
			if("不存在数据表"==msg)
			{
				$("#tabr").empty();
				$("#tt1").html("不存在数据表");
				$("#ttt").show();
				exit();
			}
			if("结束时间不能低于开始时间"==msg)
			{
				$("#tabr").empty();
				$("#tt1").html("结束时间不能低于开始时间!!!");
				$("#ttt").show();
				exit();
			}
			if("结束时间与开始时间相差不能超过3天"==msg)
			{
				$("#tabr").empty();
				$("#tt1").html("结束时间与开始时间相差不能超过3天");
				$("#ttt").show();
				exit();
			}
			else
			{
				$("#ttt").hide();
				$("#tabr").empty();
				//处理检索到的数据......
				$.each(msg, function (index, value) {
					//alert(value.RoleName);
					var s = '<tr class=mgr_tr align=center ><td height=32 style="border-right:1px solid #F0F0F0" width="7%" >'+value['Uin']+'</td><td style="border-right:1px solid #F0F0F0" width="7%">'+value['LoginName']+'</td><td width="7%" style="border-right:1px solid #F0F0F0" >'+value['RoleName']+'</td><td width="5%" style="border-right:1px solid #F0F0F0" >'+value['Level']+'</td><td width="5%" style="border-right:1px solid #F0F0F0" >'+value['Exp']+'</td><td width="5%" style="border-right:1px solid #F0F0F0" >'+value['Money']+'</td><td width="5%" style="border-right:1px solid #F0F0F0" >'+value['Bull']+'</td><td width="5%" style="border-right:1px solid #F0F0F0" >'+value['StashMoney']+'</td><td width="5%" style="border-right:1px solid #F0F0F0" >'+value['SundriesItemNum']+'</td><td width="5%" style="border-right:1px solid #F0F0F0" >'+value['StuffItemNum']+'</td><td width="5%" style="border-right:1px solid #F0F0F0" >'+value['TaskItemNum']+'</td><td width="5%" style="border-right:1px solid #F0F0F0">'+value['WearItemNum']+'</td><td width="5%" style="border-right:1px solid #F0F0F0" >'+value['SkillNum']+'</td><td width="5%" style="border-right:1px solid #F0F0F0">'+value['Map']+'</td><td width="7%" style="border-right:1px solid #F0F0F0" >'+value['xy']+'</td><td width="7%" style="border-right:1px solid #F0F0F0" >'+value['LogTime']+'</td><td width="10%" ><a style="color:#00F;" href=# onclick="rolexq(\''+value['LogRoleData']+'\')" >[人物]</a><a style="color:#00F;" href=# onclick="roletf(\''+value['LogRoleData']+'\')" >[天赋]</a><a style="color:#00F;" href=# onclick="snapzuoqi(\''+value['LogRoleData']+'\')" >[坐骑]</a><a style="color:#00F;" href=# onclick="equip(\''+value['LogRoleData']+'\',\''+value['RoleName']+'\')" >[装备]</a><a style="color:#00F;" href=# onclick="sparpkg(\''+value['LogRoleData']+'\',\''+value['RoleName']+'\')" >[晶石]</a><a style="color:#00F;" href=# onclick="res(\''+value['LogRoleData']+'\',\''+value['RoleName']+'\')" >[背包仓库宠物]</a><a style="color:#00F;" href=# onclick="skill(\''+value['LogRoleData']+'\')" >[技能]</a>--<a style="color:#00F;" href=# onclick="rolefile(\''+value['LogRoleData']+'\')" >下载XML</a></td></tr>';
					$("#tabr").append(s);
				});
			}
		}
	});
	}
	else{
		alert(msg);
		$("#tabr").empty();
		$("#tt1").html("未检索到数据......");
		$("#ttt").show();
	}
}
//rolefile
function rolexq(hex)
{
	var hex = hex;
	$.ajax({
		//async:false,
		type: 'POST',
		url: 'Snapshot.php',
		dataType: 'html',
		data: {"hex":hex},
		success: function(msg){
				$.kangThickBox({
				type: "iframe",
				title:"角色概要",
				source: 'Snapshot.php?xx=1',
				width: 650,
				height: 400,
				_autoReposi: true
			});
		}
	});
}
function roletf(hex)
{
	var hex = hex;
	$.ajax({
		//async:false,
		type: 'POST',
		url: 'Genius.php',
		dataType: 'html',
		data: {"hex":hex},
		success: function(msg){
				$.kangThickBox({
				type: "iframe",
				title:"天赋信息",
				source: 'Genius.php?xx=1',
				width: 650,
				height: 320,
				_autoReposi: true
			});
		}
	});
}
function rolefile(hex)
{
	var hex = hex;
	$.ajax({
		async:false,
		type: 'POST',
		url: 'rolefile.php',
		dataType: 'html',
		data: {"hex":hex},
		success: function(msg){
			alert("若导入数据报错，请将xml文档转换成ANSI格式");
			window.location = 'rolefile.php?file=1';
		}
	});
}
function res(hex,role)
{
	var role = role;
	var hex = hex;
	$.ajax({
		async:false,
		type: 'POST',
		url: 'snapRes.php',
		dataType: 'html',
		data: {"hex":hex,"role":role,"qu":qu,"fu":fu},
		success: function(msg){
			window.open('snapRes.php?res=1');
		}
	});
}
function skill(hex)
{
	var hex = hex;
	$.ajax({
		//async:false,
		type: 'POST',
		url: 'snapSkill.php',
		dataType: 'html',
		data: {"hex":hex},
		success: function(msg){
				$.kangThickBox({
				type: "iframe",
				title:"技能信息(一个技能关联一个镶嵌的符文ID,0表示无此技能，或未镶嵌符文)",
				source: 'snapSkill.php?skill=1',
				width: 850,
				height: 500,
				_autoReposi: true
			});
		}
	});
}
function snapzuoqi(hex)
{
	var hex = hex;
	$.ajax({
		//async:false,
		type: 'POST',
		url: 'snapzuoqi.php',
		dataType: 'html',
		data: {"hex":hex},
		success: function(msg){
				$.kangThickBox({
				type: "iframe",
				title:"坐骑信息",
				source: 'snapzuoqi.php?zq=1',
				width: 530,
				height: 300,
				_autoReposi: true
			});
		}
	});
}
function equip(hex,role)
{
	var hex = hex;
	var role = role;
	$.ajax({
		//async:false,
		type: 'POST',
		url: 'snapequip.php',
		dataType: 'html',
		data: {"hex":hex,"role":role,"qu":qu,"fu":fu},
		success: function(msg){
				$.kangThickBox({
				type: "iframe",
				title:"装备信息(含时装),可选择左侧复选框发送邮件，寄给玩家[一次最多发送8件]",
				source: 'snapequip.php?equip=1',
				width: 850,
				height: 550,
				_autoReposi: true
			});
		}
	});
}
function sparpkg(hex,role)
{
	var hex = hex;
	var role = role;
	$.ajax({
		//async:false,
		type: 'POST',
		url: 'snapequip.php',
		dataType: 'html',
		data: {"hex":hex,"role":role,"qu":qu,"fu":fu},
		success: function(msg){
				$.kangThickBox({
				type: "iframe",
				title:"晶石信息(身上装备的晶石),可选择左侧复选框发送邮件，寄给玩家",
				source: 'snapsparpkg.php?sparpkg=1',
				width: 650,
				height: 550,
				_autoReposi: true
			});
		}
	});
}
</script>
<style type="text/css">
body
{
	margin:0 auto;
	padding:0 auto;
	background:#FFF;
}
.tcss
{
	border:0px;
	margin-left:70px;
}
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
<div class="mgr_header"> <span class="title">角色快照</span>
    <span class="reload"><a href="javascript:location.reload();">刷新</a></span>
</div>
<div class="mgr_divt" id="ttt">
<span id="tt1" style="color:#F00; font-size:16px; font-weight:bold; margin-left:180px;"></span>
</div>
<div class="mgr_divt">
<table width="100%" height="100%">
	<tr>
		<td>&nbsp;&nbsp;&nbsp;请选择·······大区：<select name="dq" id="dq" onchange="xf()">
        <option value="-请选择-" selected="selected">-请选择-</option>
        <?php
        $dosql->Execute("select distinct regid,regname from `reg_world` order by `regid` ");
        while($row=$dosql->GetArray()){?>
        				<option value="<?php echo $row['regid']; ?>"><?php echo $row['regname']; ?></option>
        <?php } ?> 
                     </select><span style="color:#F00" id="qu"></span>&nbsp;&nbsp;
    		&nbsp;&nbsp;&nbsp;服务器：<select name="fw" id="fw" >
        	 </select>&nbsp;&nbsp;&nbsp;
             <select id="jz">
            	<option value="1" selected="selected">角色</option>
                <option value="2" >帐号</option>
            </select>
            <input type="text" id="us" name="us"  onblur="jc()" />
            <span style="color:#F00" id="us1"></span>&nbsp;&nbsp;&nbsp;
        </td>
     </tr>
</table>
</div>
<div class="mgr_divt" id="sea">
<table width="100%" height="100%">
	<tr class="tcss">
		<td>&nbsp;&nbsp;&nbsp;
          开始时间：<input name="time" type="text" id="posttime" class="input_short" onblur="stime()" />
				<script type="text/javascript">
				date = new Date();
				Calendar.setup({
					inputField     :    "posttime",
					ifFormat       :    "%Y-%m-%d %H:%M:%S",
					showsTime      :    true,
					timeFormat     :    "24"
				});
				</script> <span style="color:#F00" id="sta"></span>
         结束时间：<input name="time1" type="text" id="posttime1" class="input_short" />
				<script type="text/javascript">
				date = new Date();
				Calendar.setup({
					inputField     :    "posttime1",
					ifFormat       :    "%Y-%m-%d %H:%M:%S",
					showsTime      :    true,
					timeFormat     :    "24"
				});
				</script>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<input type="button" id="s1" title="搜索" value="搜索" onclick="sear()" style="width:50px; height:22px;"/> 
                &nbsp;&nbsp;<span  name="ts" id="ts" style="color:#F00"></span>
        </td>
     </tr>
</table></div>
<table width="100%" border="0" cellpadding="0" cellspacing="0" class="mgr_table" id="bd">
		<tr class="thead2" align="center">
				<td width="7%">Uin</td>
				<td width="7%">帐号</td>
				<td width="7%">角色名</td>
                <td width="5%">等级</td>
                <td width="5%">Exp</td>
                <td width="5%">银币</td>
                <td width="5%">绑银</td>
                <td width="5%">仓库银币</td>
                <td width="5%">杂物包数量</td>
                <td width="5%">材料包数量</td>
                <td width="5%">任务包数量</td>
                <td width="5%">身上装备数量(含时装)</td>
                <td width="5%">技能数</td>
                <td width="5%">地图ID</td>
                <td width="7%">坐标（X,Y）</td>
                <td width="7%">时间</td>
                <td width="10%">操作</td>
		</tr>
</table>
<table width="100%" border="0" cellpadding="0" id="tabr" cellspacing="0" class="mgr_table"></table>

 <div class="page_area">
</div>
</body>
</html>