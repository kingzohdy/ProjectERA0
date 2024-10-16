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
//第一次查询角色
if(isset($_GET['role']))
{
	$q =  $_GET['dq'];
	$f = $_GET['fu'];
	$rolename = $_GET['role'];
	$result = selectFirstMail($q,$f,$rolename);
	if($result->ErrorCode==0 )
	{
		$err = 1;
		$str=$result->StrXML;
		$data=simplexml_load_string($str);
		$FindHandleID=$result->FindHandleID;
		//$arr=$data->MailHeads;
		//$array['data'] = $data;
		//$jsobject =  json_encode($array);
		//exit(-1);
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
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="js/jquery.kangThickBoxV2.js" ></script>
<script type="text/javascript" src="js/dateInt.js"></script>
<script type="text/javascript" >
var serverlist = -1;//用于保存服务器列表,若值为-1表示未获得列
var FindId=-1;//当前句柄
var RegArray=new Array(); //查询出来的所有结果.-1表示未查询
var page = ""; //当前页
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
			data: {"mail":"","page": FindId}, 
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
			d +='<tr align=center ><td width="10%" >'+jg.Pass9Account+'</td><td  height=32 width="12%">'+jg.RoleName+'</td><td width="10%" >'+jg.RoleWID+'</td><td width="16%" >'+date('Y-m-d H:i:s',jg.CreateTime)+'</td><td width="8%" >'+parseInt(jg.RegionID)+'</td><td width="8%" >'+jg.ServerID+'</td><td width="9%" >'+jg.OnlineFlag+'</td><td width="9%" style="color:#00F" >'+jg.Uin+'</td><td width="15%" >[<a href=# onclick = tiAccount("'+jg.Pass9Account+'","'+parseInt(jg.RegionID)+'","'+parseInt(jg.ServerID)+'") >踢下线</a>][<a href="#" onclick = jingyan("'+jg.Pass9Account+'","'+parseInt(jg.RegionID)+'","'+parseInt(jg.ServerID)+'") >禁言/解禁</a>][<a href="#" onclick=nature("'+jg.RoleName+'","'+parseInt(jg.RegionID)+'","'+parseInt(jg.ServerID)+'") >角色信息</a>][<a href="#" onclick=skill("'+jg.RoleName+'","'+parseInt(jg.RegionID)+'","'+parseInt(jg.ServerID)+'") >技能信息</a>]][<a href="#" onclick=zuobiao("'+jg.RoleName+'","'+parseInt(jg.RegionID)+'","'+parseInt(jg.ServerID)+'") >修改坐标</a>]';
		if("高级游戏管理员"==tp)
		{
			//[<a href="#" class="box a2" onclick="fck()"  >改安全锁</a>]第二版新加
			d += '[<a href="#" onclick = dongjie("'+jg.Pass9Account+'","'+parseInt(jg.RegionID)+'") >冻结此帐号</a>][<a href="#" onclick=fck("'+jg.Pass9Account+'","'+parseInt(jg.RegionID)+'","'+parseInt(jg.ServerID)+'")  >改安全锁</a>]</td></tr>';
		}
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
//mail详情
function mailx(wid,rname)
{
	var qu=<?php echo $q; ?>;
	var fu=<?php echo $f ?>;
	var rolename=rname;
	$.kangThickBox({
		type: "iframe",
		title:"mail详情",
		source: 'mailxq.php?qu='+qu+'&&fu='+fu+'&&wid='+wid+'&&rolename='+encodeURI(rolename),
		width: 860,
		height: 240,
		_autoReposi: true
	});
}

</script>
</head>
<body>
<?php
if($err == 1)
{
	//$df = json_encode($arr);
	//print_r($FindHandleID);
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
	echo "该用户暂无邮件<br/>";
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
                <td width="15%">邮件ID</td>
        		<td width="15%">收件人</td>
                <td width="15%">发件人</td>
                <td width="15%">发件时间</td>
                <td width="15%">标题</td>
                <td width="15%">操作</td>
		</tr>
 </table>
 <table width="100%" border="0" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist">
<?php 
	date_default_timezone_set('PRC');
	for($i=0; $i< $data->ResNum; $i++ ){
		$pagefag = -1; //标记
		if($data->ResNum == 1)
		{
			$j = $data->MailHeads;
		}
		if($data->ResNum > 1)
		{
			$j = $data->MailHeads[$i];
		}
		
?>
    <tr align=center >
    	<td width="15%" ><?php echo  $j->WID ?></td>
        <td  height=32 width="15%"><?php echo $j->Recv ?></td>
        <td width="15%" ><?php echo $j->Send ?></td>
        <td width="15%" ><?php echo date('Y-m-d H:i:s',(int)($j->Time)); ?></td>
        <td width="15%" ><?php echo $j->Title; ?></td>
        <td width="15%" ><a href="javascript:;" onclick="mailx('<?php echo trim($j->WID) ?>','<?php echo $j->Recv ?>')">详细</a></td>
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
    <input type="hidden" value='<?php echo $FindHandleID; ?>' id="fid" />
	<input type="button" value="上一页" id="up" onclick="pup()" />&nbsp;&nbsp;&nbsp;&nbsp;
    <input type="button" value="下一页" id="next" onclick="pnext()" />
</div>
</form>

<!--/list end-->
</div>
</body>
</html>