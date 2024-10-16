<?php 
	require_once(dirname(__FILE__).'/../inc/config.inc.php'); 
//检测用户是否有权限
if($_SESSION['admin1'] == "游戏管理员")
{
	$_SESSION = array();
	session_destroy();	
	echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
	exit();
}
if(isset($_GET['AccName']))
{
	$q = $_GET['q'];
	$AccName = $_GET['AccName'];
	$f=-1;
}
if(isset($_GET['RoleName']))
{
	$q = $_GET['q'];
	$f = $_GET['fu'];
	$AccName = $_GET['RoleName'];
}
?>
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/jscript">
function tj()
{
	var jintime=$("#jt").val();
	var daqu = $("#daqu").val();
    var Account = $("#Account").val();
	if(jintime >= 0)
	{
		/*
			1.调用接口修改密码
			2.判断返回值是否修改成功(成功则关闭窗口)
		*/
		$.ajax
		({
			type: 'POST',
			url: 'conduct.php',
			dataType: 'json',
			data: {"jingyan":"","jintime": jintime,"daqu":daqu,"Account":Account}, 
			beforeSend:function(){
				$("#BgDiv").css({ display: "block", height: $(document).height() });
				$('#DialogDiv').show();
			 },
			success: function(array)
			{
				$('#DialogDiv').hide();
				$("#BgDiv").hide();
				if(array == 0)
				{
					alert("成功");
				}
				if(array.ErrorCode)
				{
					alert("ErrorCode: "+array.ErrorCode+"\nErrorText: "+array.ErrorText);
				}
				 javascript:parent.kangThickBoxclose();
			}
		});
		
	}else
	{
		$("#ts").html("时间输入有误");
	}
}
function tj1()
{
	var jintime=$("#jt").val();
	var daqu = $("#daqu").val();
    var RoleName = $("#Account").val();
	var fu = $("#fu").val();
	if(jintime >= 0)
	{
		/*
			1.调用接口修改密码
			2.判断返回值是否修改成功(成功则关闭窗口)
		*/
		$.ajax
		({
			type: 'POST',
			url: 'conduct.php',
			dataType: 'json',
			data: {"jingyanRole":"","jintime": jintime,"daqu":daqu,"fu":fu,"RoleName":RoleName}, 
			beforeSend:function(){
				$("#BgDiv").css({ display: "block", height: $(document).height() });
				$('#DialogDiv').show();
			 },
			success: function(array)
			{
				$('#DialogDiv').hide();
				$("#BgDiv").hide();
				if(array == 0)
				{
					alert("成功");
				}
				if(array.ErrorCode)
				{
					alert("ErrorCode: "+array.ErrorCode+"\nErrorText: "+array.ErrorText);
				}
				 javascript:parent.kangThickBoxclose();
			}
		});
		
	}else
	{
		$("#ts").html("时间输入有误");
	}
}
</script>
<style type="text/css" >
#BgDiv{background-color:#F7F7F7; position:absolute; z-index:99; left:0; top:0; display:none; width:100%; height:100%;opacity:0.5;filter: alpha(opacity=50);-moz-opacity: 0.5;} 
#DialogDiv{position:absolute;width:300px; left:100%; top:-10%; margin-left:-200px; height:50px; z-index:10008; border:0px #ccc solid; padding:1px;}
</style>
<body>
<div id="BgDiv"></div>
<div id="DialogDiv" style="display:none">
	<h2 style="color:#F00">请稍等...</h2>
</div>
<p>请输入禁言时间：(0表示解禁)<br/><span style="color:#F00; font-size:12px; font-weight:bold" id="ts"></span></p>
<p>
  <input type="text" id="jt" name="jt" />秒
  <input type="button" <?php if($f != -1){ echo "onclick=tj1() "; } ?> <?php if($f == -1){ echo "onclick=tj() "; } ?> value="确认" />
  <input type="hidden" value="<?php echo $q; ?>" id="daqu" />
  <input type="hidden" value="<?php echo $AccName; ?>" id="Account" />
  <input type="hidden" value="<?php echo $f; ?>" id="fu" />
</p>
</body>
</html>
