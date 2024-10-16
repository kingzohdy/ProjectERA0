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
if(isset($_GET['qu']))
{
	$qq = $_GET['qu'];
	$ff = $_GET['fu'];
	//urldecode();
	$RoleName = urldecode($_GET['RoleName']);
}
?>
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/jscript">
function tj()
{
	var us=$("#uname").val();
	var daqu = $("#qu").val();
	var fu = $("#fu").val();
	var mapid = $("#mapid").val();
	var mapx = $("#mapx").val();
	var mapy = $("#mapy").val();
    var Account = $("#Account").val();
		/*
			1.调用接口修改密码
			2.判断返回值是否修改成功(成功则关闭窗口)
		*/
		$.ajax
		({
			type: 'POST',
			url: 'conduct.php',
			dataType: 'json',
			data: {"gzb":"","role":us,"fu": fu,"daqu":daqu,"mapid":mapid,"mapx":mapx,"mapy":mapy}, 
			beforeSend:function(){
				$("#BgDiv").css({ display: "block", height: $(document).height() });
				$('#DialogDiv').show();
			 },
			success: function(array)
			{
				javascript:parent.kangThickBoxclose();
				if(array == 0)
				{
					alert("成功");
				}
				if(array.ErrorCode)
				{
					alert("ErrorCode: "+array.ErrorCode+"\nErrorText: "+array.ErrorText);
				}
			}
		});
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
<form action="#" method="post" id="form">
<p>
	地图ID： <input type="text" id="mapid" style="width:100px" />
</p>
<p>
  X 坐 标：<input type="text" id="mapx" style="width:100px" />
</p>
<p>
  Y 坐 标：<input type="text" id="mapy" style="width:100px" />
</p>
<input type="hidden" id="qu" value="<?php echo $qq; ?>" >
<input type="hidden" id="fu" value="<?php echo $ff; ?>" >
<input type="hidden" id="uname" value="<?php echo $RoleName; ?>" >
<p>
  	<input style="margin-left:75px;" type="button"  onclick="tj()" value="确认" />
</p>
</form>
</body>
</html>
