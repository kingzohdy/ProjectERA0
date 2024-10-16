<?php	require_once(dirname(__FILE__).'/../inc/config.inc.php');
//检测用户是否有权限
if($_SESSION['type'] != "5")
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
<title>金银币交易工具</title>
<link href="style/admin.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="js/jquery.kangThickBoxV2.js" ></script>
<script type="text/javascript" src="../data/plugin/calendar/calendar1.js"></script>
<script type="text/javascript" src="js/dateInt.js"></script>
<script type="text/javascript">
	var i = 0;
	var j = 0;
	var t = 0;
	var k = 0;
	var z = 0;
	var jzd = 0;
	var jzg = 0;
	var yzd = 0;
	var yzg = 0;
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
		i=0;
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
				tradeing();
				sysear();
				sbuylog();
  			});
		}
	});
}
function fgb()
{
	var q = document.getElementById("dq").options[window.document.getElementById("dq").selectedIndex].text;
	var f = document.getElementById("fw").options[window.document.getElementById("fw").selectedIndex].text;
	$("#qname").html(q);
	$("#fname").html(f);
	$("#qname2").html(q);
	$("#fname2").html(f);
	$("#qname3").html(q);
	$("#fname3").html(f);
	tradeing();
	sysear();
}
function bstype()
{
	var ss = $("#fag").val();
	if("b" == ss)
	{
		$("#bstp").html("购买");
	}
	if("s" == ss)
	{
		$("#bstp").html("出售");
	}
	zui();
}
function jytype()
{
	var num = $("#ms").val();
	if(num=="j1" || num=="j2" || num=="j3" || num=="j4" || num=="j5")
	{
		$("#jytps").html("银");
		$("#shijia").html("银币最低价：");
	}
	if(num=="y1" || num=="y2" || num=="y3" || num=="y4" || num=="y5")
	{
		$("#jytps").html("金");
		$("#shijia").html("金币最低价：");
	}
	zui();
}
function zui()
{
	var num = $("#ms").val();
	var ss = $("#fag").val();
	if(num=="j1" || num=="j2" || num=="j3" || num=="j4" || num=="j5")
	{	
		if("b" == ss)
		{
			$("#shijia").html("金币最低价：1金="+yzd+"银");
		}
		if("s" == ss)
		{
			$("#shijia").html("金币最高价：1金="+yzg+"银");
		}
	}
	if(num=="y1" || num=="y2" || num=="y3" || num=="y4" || num=="y5")
	{
		if("b" == ss)
		{
			$("#shijia").html("银币最高价："+jzg+"银=1金");
		}
		if("s" == ss)
		{
			$("#shijia").html("银币最低价："+jzd+"银=1金");
		}
	}
}
function jc()
{
	 var stp = $("#us").val();
	if(isNaN(stp)|| stp == "" || stp < 1)
	{
		$("#us1").html("价格不能为空或小于1");
		j=0;
		return;
	}
	else
	{
		$("#us1").html(""); 
		j=1;
	}
}
function ds()
{
	 var stp = $("#tnums").val();
	if(isNaN(stp)|| stp == "" || stp < 1 || stp >100)
	{
		$("#us1").html("执行的单数不能小于1或大于100");
		k=0;
		return;
	}
	else
	{
		$("#us1").html(""); 
		k=1;
	}
}
function jg()
{
	 var stp = $("#jgtime").val();
	if(isNaN(stp)|| stp == "" || stp < 1)
	{
		$("#us1").html("间隔秒数不能小于1");
		z=0;
		return;
	}
	else
	{
		$("#us1").html(""); 
		z=1;
	}
}
//搜索当前交易信息
function tradeing()
{
	if(i==0)
	{
		alert("请选择服务器");
		return;
	}
	var q = document.getElementById("dq").options[window.document.getElementById("dq").selectedIndex].text;
	var f = document.getElementById("fw").options[window.document.getElementById("fw").selectedIndex].text;
	$("#qname").html(q);
	$("#fname").html(f);
	$("#qname2").html(q);
	$("#fname2").html(f);
	$("#qname3").html(q);
	$("#fname3").html(f);
	var dq = $("#dq").val();
	var fu = $("#fw").val();
	var money = 0;
	var gold = 0;
	$.ajax({
		type:'POST',
		url:'tradelog.php',
		dataType:'json',
		data:{"tradeing":"","rid":dq,"wid":fu},
		success:function(data){
			$("#money").empty();
			$("#gold").empty();
			$("#glow").empty();
			$("#gtall").empty();
			$("#mlow").empty();
			$("#mtall").empty();
			var zd = "最低时价：";
			if(data.count2!= 0)
			{
				var mlow = (parseInt(data.buy[0]['Moeny'])/parseInt(data.buy[0]['Gold'])).toFixed(2);
				var mtall = (parseInt(data.buy[0]['Moeny'])/parseInt(data.buy[0]['Gold'])).toFixed(2);
				for (var b in data.buy)
				{
					money += parseInt(data.buy[b]['Moeny']);
					if( mlow < (parseInt(data.buy[b]['Moeny'])/parseInt(data.buy[b]['Gold'])).toFixed(2))
					{
						mlow = (parseInt(data.buy[b]['Moeny'])/parseInt(data.buy[b]['Gold'])).toFixed(2);
					}
					if( mtall > (parseInt(data.buy[b]['Moeny'])/parseInt(data.buy[b]['Gold'])).toFixed(2))
					{
						mtall = (parseInt(data.buy[b]['Moeny'])/parseInt(data.buy[b]['Gold'])).toFixed(2);
					}
				}
				jzd = mlow;
				jzg = mtall;
				$("#money").html(money);
				$("#mlow").html("1金="+mlow+"银");
				$("#mtall").html("1金="+mtall+"银");
				
			}
			else if(data.count2 == 0)
			{
				jzd = "";
				jzg = "";
				$("#money").html("");
				$("#mlow").html("");
				$("#mtall").html("");
			}
			if(data.count != 0)
			{
				var glow =(parseInt(data.sell[0]['Moeny'])/parseInt(data.sell[0]['Gold'])).toFixed(2);
				var gtall = (parseInt(data.sell[0]['Moeny'])/parseInt(data.sell[0]['Gold'])).toFixed(2);
				for (var c in data.sell)
				{
					gold += parseInt(data.sell[c]['Gold']);
					if( glow > (parseInt(data.sell[c]['Moeny'])/parseInt(data.sell[c]['Gold'])).toFixed(2) )
					{
						glow = (parseInt(data.sell[c]['Moeny'])/parseInt(data.sell[c]['Gold'])).toFixed(2);
					}
					if( gtall < (parseInt(data.sell[c]['Moeny'])/parseInt(data.sell[c]['Gold'])).toFixed(2) )
					{
						gtall = (parseInt(data.sell[c]['Moeny'])/parseInt(data.sell[c]['Gold'])).toFixed(2);
					}
				}
				yzd = glow;
				yzg = gtall;
				$("#gold").html(gold);
				$("#gtall").html(gtall+"银=1金");
				$("#glow").html(glow+"银=1金");
				zui();
			}
			else if(data.count == 0)
			{
				yzd = "";
				yzg = "";
				$("#gold").html("");
				$("#glow").html("");
				$("#gtall").html("");
				zui();
			}								
		}
	});
	
}
//交易详情
function tradex(tp)
{
	if(i==0)
	{
		alert("请选择服务器");
		return;
	}
	var qu = $("#dq").val();
	var fu = $("#fw").val();
	
	$.kangThickBox({
		type: "iframe",
		title:"",
		source: 'tradex.php?qu='+qu+'&&fu='+fu+'&&tp='+tp,
		width: 750,
		height: 500,
		_autoReposi: true
	});
}
//执行单数
function execute()
{
	ds();
	if(i ==1 && j == 1 && k==1 && z==1){
		
		if(!confirm("确认执行?"))
	   {
		   return;
	   }
		
		var qu=$("#dq").val();
		var fu=$("#fw").val();
		var type=$("#fag").val();
		var num = $("#ms").val();
		var price = $("#us").val();
		var exchgnum = $("#tnums").val();
		var stat=$("#posttime").val();
		var endt=$("#posttime1").val();
		var interval = $("#jgtime").val();
		
		if(num=="j1" || num=="j2" || num=="j3" || num=="j4" || num=="j5")
		{
			if(type == "b")
			{
				type = 1;
			}
			else if(type == "s")
			{
				type = 0;
			}
		}
		if(num=="y1" || num=="y2" || num=="y3" || num=="y4" || num=="y5")
		{
			if(type == "b")
			{
				type = 3;
			}
			else if(type == "s")
			{
				type = 2;
			}
		}
		if(num == "j1")
		{
			num = 10;
		}
		else if(num == "j2")
		{
			num = 50;
		}
		else if(num == "j3")
		{
			num = 100;
		}
		else if(num == "j4")
		{
			num = 500;
		}
		else if(num == "j5")
		{
			num = 1000;
		}
		else if(num == "y1")
		{
			num = 1000;
		}
		else if(num == "y2")
		{
			num = 5000;
		}
		else if(num == "y3")
		{
			num = 10000;
		}
		else if(num == "y4")
		{
			num = 50000;
		}
		else if(num == "y5")
		{
			num = 100000;
		}
			
		$.ajax({
			type: 'POST',
			url: 'tradelog.php',
			dataType: 'json',
			data: {"shopcgi":"","qu": qu,"fu":fu,"type":type,"num":num,"price":price,"exchgnum":exchgnum,"stime":stat, "etime":endt,"interval":interval},
			success: function(msg){
				$("#ttt").hide();
				$("#bd").empty();
				if(msg.Succ == 0)
				{
					alert("成功");
					sysear();
					return;
				}
				else if(msg.Succ == -1)
				{
					alert("操作的价格不对");
				}
				else if(msg.Succ == -2)
				{
					alert("购买或者出售操作已经有5次了");
				}
				else if(msg.Succ == -3)
				{
					alert("对应的价格正在操作,若希望重新执行操作，请选择下方同类价格区间点击撤销");
				}
				else if(msg.Succ == -4)
				{
					alert("时间不对");
				}
				else if(msg.Succ == -5)
				{
					alert("找不到要操作的单");
				}
				else if(msg.Succ == -6)
				{
					alert("购买某条玩家银币单时，系统交易进来的金币比将要交易出去的金币少");
				}
				else if(msg.Succ == -7)
				{
					alert("就是执行总单数超过100");
				}
				else if(msg.Succ == -99)
				{
					alert("系统忙");
				}
				else if(msg.ErrorCode == -1)
				{
					alert("系统错误");
				}
				else if(msg.ErrorCode == -2)
				{
					alert("网络错误");
				}
				else if(msg.ErrorCode == -3)
				{
					alert("TDR错误");
				}
				else if(msg.ErrorCode == -4)
				{
					alert("参数错误");
				}
				else if(msg.ErrorCode == -10)
				{
					alert("超时");
				}
				sysear();
				sbuylog();
				return;
			}
		});
	}
	else
	{
		alert("信息输入不完整");
	}
}
function sysear()
{
	if(i==0)
	{
		alert("请选择服务器");
		return;
	}
	var q = document.getElementById("dq").options[window.document.getElementById("dq").selectedIndex].text;
	var f = document.getElementById("fw").options[window.document.getElementById("fw").selectedIndex].text;
	$("#qname").html(q);
	$("#fname").html(f);
	$("#qname2").html(q);
	$("#fname2").html(f);
	$("#qname3").html(q);
	$("#fname3").html(f);
	var qu = $("#dq").val();
	var fu = $("#fw").val();
	var st = $("#posttime2").val();
	var et = $("#posttime3").val();
	$.ajax({
			type: 'POST',
			url: 'tradelog.php',
			dataType: 'json',
			data: {"sear":"","qu": qu,"fu":fu,"stime":st, "etime":et},
			success: function(msg)
			{
				$("#ttt").hide();
				$("#bd").empty();
				$("#zds").empty();
				$("#shsgold").empty();
				$("#shsmoney").empty();
				$("#sxhgold").empty();
				$("#sxhmoney").empty();
				$("#kstime").empty();
				var datas="<tr class='thead2' align='center'><td>操作类型</td><td>开始时间</td><td>执行次数</td><td>剩余次数</td><td>每单执行</td><td>执行价格</td><td>间隔时间(秒)</td><td>结束时间</td><td>操作</td></tr>";
				$("#bd").append(datas);
				if(msg.ErrorCode == -4)
				{
					$("#nmsts").html("暂无系统操作信息");
					return;
				}
				var gmdata = msg.SysShopInfo;
				$("#zds").html(gmdata.ExchgInfo.ExchgNum);
				$("#shsgold").html(parseInt(gmdata.ExchgInfo.Count));
				$("#shsmoney").html(gmdata.ExchgInfo.MoneyCount);
				$("#sxhgold").html(gmdata.ExchgInfo.OutCount);
				$("#sxhmoney").html(gmdata.ExchgInfo.OutMoneyCount);
				$("#kstime").html(date('Y-m-d H:i:s',gmdata.ExchgInfo.Time));
				var nm = parseInt(gmdata.iBuyGold)+parseInt(gmdata.iBuyMoney)+parseInt(gmdata.iSellGold)+parseInt(gmdata.iSellMoney);
				
				if(nm <= 0)
				{
					$("#nmsts").html("暂无系统操作信息");
					return;
				}
				var ds = "";
				$("#nmsts").html("");
	
				if(1 == gmdata.iBuyGold)
				{
					ds = gmdata.BuyGold;
					datas += "<tr height='27' align='center'><td>购买金币</td><td>"+date('Y-m-d H:i:s',ds.StartTime)+"</td><td>"+ds.ExchgNum+"</td><td>"+ds.RemNum+"</td><td>购买："+ds.Num+"金</td><td>"+ds.Price+"银</td><td>"+ds.InterVal+"</td><td>"+date('Y-m-d H:i:s',ds.EndTime)+"</td><td><input type='button' value='撤销' onclick='che("+ds.Num+",1)'  /></td></tr>";						
				}
				else if(1 < gmdata.iBuyGold)
				{					
					for(var s=0;s<gmdata.iBuyGold; s++)
					{
						ds = gmdata.BuyGold[s];
						datas += "<tr height='27' align='center'><td>购买金币</td><td>"+date('Y-m-d H:i:s',ds.StartTime)+"</td><td>"+ds.ExchgNum+"</td><td>"+ds.RemNum+"</td><td>购买："+ds.Num+"金</td><td>"+ds.Price+"银</td><td>"+ds.InterVal+"</td><td>"+date('Y-m-d H:i:s',ds.EndTime)+"</td><td><input type='button' value='撤销' onclick='che("+ds.Num+",1)' /></td></tr>";
					}
				}
				if(1 == gmdata.iBuyMoney)
				{
					ds = gmdata.BuyMoney;
					datas += "<tr height='27' align='center'><td>购买银币</td><td>"+date('Y-m-d H:i:s',ds.StartTime)+"</td><td>"+ds.ExchgNum+"</td><td>"+ds.RemNum+"</td><td>购买："+ds.Num+"银</td><td>"+ds.Price+"金</td><td>"+ds.InterVal+"</td><td>"+date('Y-m-d H:i:s',ds.EndTime)+"</td><td><input type='button' value='撤销' onclick='che("+ds.Num+",3)' /></td></tr>";						
				}
				else if(1 < gmdata.iBuyMoney)
				{					
					for(var s=0;s<gmdata.iBuyMoney; s++)
					{
						ds = gmdata.BuyMoney[s];
						datas += "<tr height='27' align='center'><td>购买银币</td><td>"+date('Y-m-d H:i:s',ds.StartTime)+"</td><td>"+ds.ExchgNum+"</td><td>"+ds.RemNum+"</td><td>购买："+ds.Num+"银</td><td>"+ds.Price+"金</td><td>"+ds.InterVal+"</td><td>"+date('Y-m-d H:i:s',ds.EndTime)+"</td><td><input type='button' value='撤销' onclick='che("+ds.Num+",3)' /></td></tr>";
					}
				}
				if(1 == gmdata.iSellGold)
				{
					ds = gmdata.SellGold;
					datas += "<tr height='27' align='center'><td>出售金币</td><td>"+date('Y-m-d H:i:s',ds.StartTime)+"</td><td>"+ds.ExchgNum+"</td><td>"+ds.RemNum+"</td><td>出售："+ds.Num+"金</td><td>"+ds.Price+"银</td><td>"+ds.InterVal+"</td><td>"+date('Y-m-d H:i:s',ds.EndTime)+"</td><td><input type='button' value='撤销' onclick='che("+ds.Num+",0)' /></td></tr>";						
				}
				else if(1 < gmdata.iSellGold)
				{					
					for(var s=0;s<gmdata.iSellGold; s++)
					{
						ds = gmdata.SellGold[s];
						datas += "<tr height='27' align='center'><td>出售金币</td><td>"+date('Y-m-d H:i:s',ds.StartTime)+"</td><td>"+ds.ExchgNum+"</td><td>"+ds.RemNum+"</td><td>出售："+ds.Num+"金</td><td>"+ds.Price+"银</td><td>"+ds.InterVal+"</td><td>"+date('Y-m-d H:i:s',ds.EndTime)+"</td><td><input type='button' value='撤销' onclick='che("+ds.Num+",0)' /></td></tr>";
					}
				}
				if(1 == gmdata.iSellMoney)
				{
					ds = gmdata.SellMoney;
					datas += "<tr height='27' align='center'><td>出售银币</td><td>"+date('Y-m-d H:i:s',ds.StartTime)+"</td><td>"+ds.ExchgNum+"</td><td>"+ds.RemNum+"</td><td>出售："+ds.Num+"银</td><td>"+ds.Price+"金</td><td>"+ds.InterVal+"</td><td>"+date('Y-m-d H:i:s',ds.EndTime)+"</td><td><input type='button' value='撤销' onclick='che("+ds.Num+",2)' /></td></tr>";						
				}
				else if(1 < gmdata.iSellMoney)
				{					
					for(var s=0;s<gmdata.iSellMoney; s++)
					{
						ds = gmdata.SellMoney[s];
						datas += "<tr height='27' align='center'><td>出售银币</td><td>"+date('Y-m-d H:i:s',ds.StartTime)+"</td><td>"+ds.ExchgNum+"</td><td>"+ds.RemNum+"</td><td>出售："+ds.Num+"银</td><td>"+ds.Price+"金</td><td>"+ds.InterVal+"</td><td>"+date('Y-m-d H:i:s',ds.EndTime)+"</td><td><input type='button' value='撤销' onclick='che("+ds.Num+",2)' /></td></tr>";
					}
				}
				$("#bd").empty();
				$("#bd").append(datas);
			}
	});
	sbuylog();
}
function che(num,tp)
{
	if(i==0)
	{
		alert("请选择服务器");
		return;
	}
	var qu = $("#dq").val();
	var fu = $("#fw").val();
	if(!confirm("确认撤销?"))
   {
	   return;
   }
	$.ajax({
			type: 'POST',
			url: 'tradelog.php',
			dataType: 'json',
			data: {"chexiao":"","num": num,"tp":tp,"qu": qu,"fu":fu,},
			success: function(msg)
			{
				if(msg.Succ == 0)
				{
					alert("成功");
					sysear();
					return;
				}
				else if(msg.Succ == -1)
				{
					alert("操作的价格不对");
				}
				else if(msg.Succ == -2)
				{
					alert("购买或者出售操作已经有5次了");
				}
				else if(msg.Succ == -3)
				{
					alert("对应的价格正在操作,若希望重新执行操作，请选择下方同类价格区间点击撤销");
				}
				else if(msg.Succ == -4)
				{
					alert("时间不对");
				}
				else if(msg.Succ == -5)
				{
					alert("找不到要操作的单");
				}
				else if(msg.Succ == -6)
				{
					alert("购买某条玩家银币单时，系统交易进来的金币比将要交易出去的金币少");
				}
				else if(msg.Succ == -7)
				{
					alert("就是执行总单数超过100");
				}
				else if(msg.Succ == -99)
				{
					alert("系统忙");
				}
				else if(msg.ErrorCode == -1)
				{
					alert("系统错误");
				}
				else if(msg.ErrorCode == -2)
				{
					alert("网络错误");
				}
				else if(msg.ErrorCode == -3)
				{
					alert("TDR错误");
				}
				else if(msg.ErrorCode == -4)
				{
					alert("参数错误");
				}
				else if(msg.ErrorCode == -10)
				{
					alert("超时");
				}
				sysear();
				return;
			}
	});
}
function daochu()
{
	if(i==0)
	{
		alert("请选择服务器");
		return;
	}
	var qu = $("#dq").val();
	var fu = $("#fw").val();
	window.location.href="tradeGMout.php?leadout=''&qu="+qu+"&fu="+fu;
}
function sbuylog()
{
	if(i==0)
	{
		$("#ts2").html("请选择服务器");
		return;
	}
	var qu = $("#dq").val();
	var fu = $("#fw").val();
	var t1 = $("#posttime").val();
	var t2 = $("#posttime1").val();
	if("" == t1)
	{
		$("#ts2").html("未选择开始时间");
		return;
	}
	else
	{
		$("#ts2").html("");
		$.ajax({
			type:'POST',
			url:'tradelog.php',
			dataType:'json',
			data:{"sbuylog":"","rid":qu,"wid":fu,"t1":t1,"t2":t2},
			success:function(data)
			{
				if(data.error == 0)
				{
					$("#rbuyg1").html(data.rg10);$("#rbuyg5").html(data.rg50);
					$("#rbuyg10").html(data.rg100);$("#rbuyg50").html(data.rg500);
					$("#rbuyg100").html(data.rg1000);
					$("#rbuym1").html(data.rm10);$("#rbuym5").html(data.rm50);
					$("#rbuym10").html(data.rm100);$("#rbuym50").html(data.rm500);
					$("#rbuym100").html(data.rm1000);
					$("#sbuyg1").html(data.sg10);$("#sbuyg5").html(data.sg50);
					$("#sbuyg10").html(data.sg100);$("#sbuyg50").html(data.sg500);
					$("#sbuyg100").html(data.sg1000);
					$("#sbuym1").html(data.sm10);$("#sbuym5").html(data.sm50);
					$("#sbuym10").html(data.sm100);$("#sbuym50").html(data.sm500);
					$("#sbuym100").html(data.sm1000);					
				}
				else
				{
					$("#ts2").html(data.error);
					$("#rbuyg1").html("");$("#rbuyg5").html("");
					$("#rbuyg10").html("");$("#rbuyg50").html("");
					$("#rbuyg100").html("");
					$("#rbuym1").html("");$("#rbuym5").html("");
					$("#rbuym10").html("");$("#rbuym50").html("");
					$("#rbuym100").html("");
					$("#sbuyg1").html("");$("#sbuyg5").html("");
					$("#sbuyg10").html("");$("#sbuyg50").html("");
					$("#sbuyg100").html("");
					$("#sbuym1").html("");$("#sbuym5").html("");
					$("#sbuym10").html("");$("#sbuym50").html("");
					$("#sbuym100").html("");
				}
				
			}
		});
	}
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
hr{
	border:none;
	border-top:1px solid #333;
	height:0;
}
p{
	line-height:45px;
}
.thickdiv{position:fixed;top:0;left:0;z-index:10000001;width:100%;height:100%;background:#F7F7F7;border:0;filter:alpha(opacity=70);opacity:0.70;}
.thickbox{position:absolute;z-index:100000002;overflow:hidden;box-shadow:0 10px 8px #6c6c6c; background-color:#fff;}
.thicktitle{height:30px;padding:0 15px; margin-top:8px; text-align:left;line-height:27px;font-family:arial,"宋体";font-size:14px;font-weight:bold;color:#F00;}

.thickclose:link,.thickclose:visited{display:block;position:absolute;z-index:100000;top:10px;right:12px;overflow:hidden;width:21px;height:21px;background:url(images/close_ico_dis.gif) no-repeat 0 3px;font-size:0;line-height:100px;}
.thickclose:hover{ background-position:left -18px;}
.thickcon{overflow:auto;padding:7px;text-align:center; vertical-align:top;}
</style>
</head>

<body>
<div class="mgr_header"> <span class="title">金银币交易工具</span>
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
    		&nbsp;&nbsp;&nbsp;服务器：<select name="fw" id="fw" onchange="fgb()" >
        	 </select>&nbsp;&nbsp;&nbsp;
			每单&nbsp;<select id="fag" onchange="bstype()">
            	<option value="b" selected="selected">购买</option>
                <option value="s">出售</option>
            </select>&nbsp;
            <select id="ms" onchange="jytype()" >
            	<option value="j1" selected="selected">10金</option>
                <option value="j2">50金</option>
                <option value="j3" >100金</option>
                <option value="j4">500金</option>
                <option value="j5">1000金</option>
                <option value="y1">1000银</option>
                <option value="y2">5000银</option>
                <option value="y3">1万银</option>
                <option value="y4">5万银</option>
                <option value="y5">10万银</option>
            </select>&nbsp;&nbsp;
            <span id="bstp" >购买</span>价格：<input type="text" id="us" name="us"  onblur="jc()" />&nbsp;<span id="jytps" >银</span>&nbsp;            
            &nbsp;&nbsp;执行总单数：<input type="text" id="tnums" onblur="ds()" style="width:50px;" />&nbsp;(最大100单)&nbsp;
            <span id="shijia" style="color:red"></span>
        </td>
     </tr>
</table>
</div>
<div class="mgr_divt" id="sea">
<table width="100%" height="100%">
	<tr class="tcss">
		<td>&nbsp;&nbsp;&nbsp;
          开始时间：<input name="time" type="text" id="posttime" class="input_short"/>
				<script type="text/javascript">
				date2 = new Date();
				Calendar.setup({
					inputField     :    "posttime",
					ifFormat       :    "%Y-%m-%d %H:%M:%S",
					showsTime      :    true,
					timeFormat     :    "24"
				});
				</script>
         结束时间：<input name="time1" type="text" id="posttime1" class="input_short" onblur="sbuylog()" />
				<script type="text/javascript">
				date3 = new Date();
				Calendar.setup({
					inputField     :    "posttime1",
					ifFormat       :    "%Y-%m-%d %H:%M:%S",
					showsTime      :    true,
					timeFormat     :    "24"
				});
				</script>（开始和结束时间不能为空）
                执行间隔时间：<input type="text" style="width:50px;" id="jgtime" onblur="jg()" />&nbsp;(秒)&nbsp;&nbsp;&nbsp;&nbsp;
				<input type="button" id="s1" title="执行" value="执行" onclick="execute()" style="width:50px; height:22px;"/>
                &nbsp;&nbsp;<span style="color:#F00" id="us1"></span>&nbsp;&nbsp;&nbsp;
        </td>
     </tr>
</table></div>
<div style="width:100%; height:auto" > 
<div style="float:left; width:70%; height:auto;">
<div class="mgr_divt" style="width:100%;">
<center>
	<b style="line-height:32px; font-size:15px;">系统交易详情&nbsp;</b>（<span id="qname3"></span>&nbsp;<span id="fname3"></span>）
    <input type="button" value="刷新" style="height:28px; width:40px;" onclick="sysear()" />&nbsp;&nbsp;&nbsp;
    <input type="button" style="height:28px;" onclick="daochu()" value="导出明细" />&nbsp;<span id="nmsts" style="color:red">&nbsp;</span>
</center>
</div>
<table width="100%" border="0" cellpadding="0" cellspacing="0" class="mgr_table" id="bd">
<tr class='thead2' align='center'>
<td>操作类型</td>
<td>开始时间</td>
<td>执行次数</td>
<td>剩余次数</td>
<td>每单执行</td>
<td>执行价格</td>
<td>间隔时间(秒)</td>
<td>结束时间</td>
<td>操作</td></tr>
</table>
<div style="margin-top:10px; height:auto; width:100%; background-color:#F0F0F0;" align="center" >
<div style="height:10px; width:100%;" ></div>
<div style=" margin-bottom:20px;height:10px; width:100%; color:#00F" >系统交易单数记录（选择上方时间点击刷新查看交易记录，只输入开始时间表示当天交易信息）&nbsp;&nbsp;<span id="ts2" style="color:red"></span></div>
    <ul style="height:36px;">
        <li style="float:left; width:20%">用户买系统10金：<span style="color:red" id="rbuyg1"></span>&nbsp;单</li>
        <li style="float:left; width:20%">用户买系统50金：<span style="color:red" id="rbuyg5"></span>&nbsp;单</li>
        <li style="float:left; width:20%">用户买系统100金：<span style="color:red" id="rbuyg10"></span>&nbsp;单</li>
        <li style="float:left; width:20%">用户买系统500金：<span style="color:red" id="rbuyg50"></span>&nbsp;单</li>
        <li style="float:left; width:20%">用户买系统1000金：<span style="color:red" id="rbuyg100"></span>&nbsp;单</li>
    </ul>
    <ul style="height:36px;">
        <li style="float:left; width:20%">用户买系统1千银：<span style="color:red" id="rbuym1"></span>&nbsp;单</li>
        <li style="float:left; width:20%">用户买系统5千银：<span style="color:red" id="rbuym5"></span>&nbsp;单</li>
        <li style="float:left; width:20%">用户买系统1万银：<span style="color:red" id="rbuym10"></span>&nbsp;单</li>
        <li style="float:left; width:20%">用户买系统5万银：<span style="color:red" id="rbuym50"></span>&nbsp;单</li>
        <li style="float:left; width:20%">用户买系统10万银：<span style="color:red" id="rbuym100"></span>&nbsp;单</li>
    </ul>
    <hr style="border-top:1px dashed #1E90FF; width:98%"/><div style="height:10px; width:100%;" ></div>
    <ul style="height:36px;">
    <li style="float:left; width:20%">系统买用户10金：<span style="color:red" id="sbuyg1"></span>&nbsp;单</li>
    <li style="float:left; width:20%">系统买用户50金：<span style="color:red" id="sbuyg5"></span>&nbsp;单</li>
    <li style="float:left; width:20%">系统买用户100金：<span style="color:red" id="sbuyg10"></span>&nbsp;单</li>
    <li style="float:left; width:20%">系统买用户500金：<span style="color:red" id="sbuyg50"></span>&nbsp;单</li>
    <li style="float:left; width:20%">系统买用户1000金：<span style="color:red" id="sbuyg100"></span>&nbsp;单</li>
    </ul>
    <ul style="height:36px;">
    <li style="float:left; width:20%">系统买用户1千银：<span style="color:red" id="sbuym1"></span>&nbsp;单</li>
    <li style="float:left; width:20%">系统买用户5千银：<span style="color:red" id="sbuym5"></span>&nbsp;单</li>
    <li style="float:left; width:20%">系统买用户1万银：<span style="color:red" id="sbuym10"></span>&nbsp;单</li>
    <li style="float:left; width:20%">系统买用户5万银：<span style="color:red" id="sbuym50"></span>&nbsp;单</li>
    <li style="float:left; width:20%">系统买用户10万银：<span style="color:red" id="sbuym100"></span>&nbsp;单</li>
    </ul>
</div>
</div>
<div style=" width:30%; float:right; background-color:#F0F0F0; ">
<center><b style="line-height:24px; font-size:15px;">系统交易信息显示</b>（<span id="qname"></span>&nbsp;<span id="fname"></span>）</center>
<p style="font-size:14px;">&nbsp;&nbsp;系统交易总单数：<span id="zds"></span>&nbsp;</p>
<p style="font-size:14px;">&nbsp;&nbsp;系统回收金币总量：<span id="shsgold"></span>&nbsp;(金)</p>
<p style="font-size:14px;">&nbsp;&nbsp;回收金币消耗银币：<span id="sxhmoney"></span>&nbsp;(银)</p>
<hr style="border-top:1px dashed #1E90FF; width:97%"/>
<p style="font-size:14px;">&nbsp;&nbsp;系统回收银币总量：<span id="shsmoney"></span>&nbsp;(银)</p>
<p style="font-size:14px;">&nbsp;&nbsp;回收银币消耗金币：<span id="sxhgold"></span>&nbsp;(金)</p>
<p style="font-size:14px;">&nbsp;&nbsp;开始时间：<span id="kstime"></span>&nbsp;</p>
<center>
<input type="button" value="刷新" onclick="sysear()" style="width:60px; height:30px;" /> &nbsp;&nbsp;&nbsp;&nbsp;
</center>
<hr />
<center><b style="line-height:24px; font-size:15px;">当前交易显示信息</b>（<span id="qname2"></span>&nbsp;<span id="fname2"></span>）</center>
<p style="font-size:14px;">&nbsp;&nbsp;金币可交易总量：<span id="gold"></span>&nbsp;(金)</p>
<p style="font-size:14px;">&nbsp;&nbsp;银币可交易总量：<span id="money"></span>&nbsp;(银)</p>
<p ><table width="100%"><tr style="font-size:14px;"><td>&nbsp;&nbsp;金币最低：<span id="glow"></span></td><td>最高：<span id="gtall"></span></td></tr></table></p>
<p style="height:15px; width:100%">&nbsp;</p>
<p ><table width="100%"><tr style="font-size:14px;"><td>&nbsp;&nbsp;银币最低：<span id="mlow"></span></td><td>最高：<span id="mtall"></span></td></tr></table></p>
<p style="height:20px; width:100%">&nbsp;</p>
<center><input type="button" value="刷新" onclick="tradeing()" style="width:50px; height:30px;" />&nbsp;&nbsp;<input type="button" value="金币出售(详)" onclick="tradex('j')" style="width:80px; height:30px;" />&nbsp;&nbsp;<input type="button" value="银币出售(详)" onclick="tradex('y')" style="width:80px; height:30px;" /></center>
<p style="height:10px; width:100%">&nbsp;</p>
</div>
</div>
 <div class="page_area">
</div>
</body>
</html>