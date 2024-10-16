<?php	require(dirname(__FILE__).'/../inc/config.inc.php'); ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>后台首页</title>
<link href="style/admin.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript">
$(function(){
	$(".notewarn").fadeIn();
	$(".notewarn .close a").click(function(){
		$(".notewarn").fadeOut();
	});
});
</script>
</head>
<body>
<div class="home_header">
	<div class="refurbish"><span class="title">官方公告</span><span class="reload"><a href="javascript:location.reload();">刷新</a></span></div>
	<div class="home_info">
	</div>
</div>
<div class="main_area">
	<div class="main_l">
		<div class="main_l_left">
			<div class="weladmin"> <span>Hi,</span> <strong><?php echo $_SESSION['admin1']; ?></strong></div>
			<div class="logininfo">您登录的时间是：<span><?php date_default_timezone_set('PRC'); echo date('Y-m-d H:i:s'); ?></span>	
				 <span><a href="# "  class="uppwd"></a></span></div>
			<div class="cl"></div>
			<div class="siteinfo">
				<h2 class="title">系统信息</h2>
				<?php
				function ShowResult($revalue)
				{
					if($revalue == 1) return '<span class="ture">支持</span>';
					else return '<span class="flase">不支持</span>';
				}
				?> 
				<table width="100%" border="0" cellspacing="0" cellpadding="0" class="home_table">
					<tr>
						<td height="28" colspan="2">软件版本号： 1.0.0&nbsp;&nbsp;</td>
					</tr>
					<tr>
						<td width="50%" height="28">服务器名称： <span title="<?php echo $_SERVER["SERVER_NAME"]; ?>"><?php echo ReStrLen($_SERVER["SERVER_NAME"],7); ?></span></td>
						<td width="50%">操作系统： <?php echo PHP_OS; ?></td>
					</tr>
					<tr>
						<td height="28">服务器版本： <span title="<?php echo $_SERVER["SERVER_SOFTWARE"]; ?>"><?php echo ReStrLen($_SERVER["SERVER_SOFTWARE"],7) ?></span></td>
						<td>GDLibrary： <?php echo ShowResult(function_exists("imageline")); ?></td>
					</tr>
					<tr>
						<td height="28">PHP版本号： <?php echo PHP_VERSION?></td>
						<td>ZEND支持： <?php echo ShowResult(function_exists("zend_version")); ?></td>
					</tr>
					<tr>
						<td height="28" colspan="2">MySQL版本： <?php echo $dosql->GetVersion(); ?></td>
					</tr>
					<tr class="nb">
						<td height="28" colspan="2">支持上传的最大文件：<?php echo ini_get('upload_max_filesize'); ?></td>
					</tr>
				</table>
			</div>
		</div>
		<div class="main_l_right">
			<h2 class="lnktitle"></h2>
			<div class="lnkarea">
				
			</div>
			<div class="countinfo">
				<h2 class="title">信息统计</h2>
				<table width="100%" border="0" cellspacing="0" cellpadding="0" class="home_table">
					<tr>
						<td width="80" height="28">网站栏目数：</td>
						<td class="num"><?php echo '1' ?></td>
					</tr>
					<tr>
						<td height="28">列表信息数：</td>
						<td class="num"><?php echo '11' ?></td>
					</tr>
					<tr>
						<td height="28">图片信息数：</td>
						<td class="num"><?php echo '22'; ?></td>
					</tr>
					<tr>
						<td height="28">商品信息数：</td>
						<td class="num"><?php echo '33'; ?></td>
					</tr>
					<tr>
						<td height="28">商品订单数：</td>
						<td class="num"><?php echo '44'; ?></td>
					</tr>
					<tr class="nb">
						<td height="28">注册会员数：</td>
						<td class="num"><?php echo '55'; ?></td>
					</tr>
				</table>
			</div>
		</div>
	</div>
	<div class="main_r">
		<div class="main_r_dev">
			<div class="title" style="border-top-color:<?php $border_color = array('#fcdf1f','#85cb20','#01b8f4','#f79d00');echo $border_color[mt_rand(0,3)]; ?>">开发团队</div>
			<ul class="cont">
				<li>程序开发及交互设计：<i>Feng</i>, <i>Adu</i></li>
				<li>UI界面设计：<i>守望者</i>　官方论坛管理：<i>Adu</i></li>
				<li>鸣谢：<i>兔饭饭</i>, <i>螺丝钉</i>, <i>薛</i>, <i>风青云</i>, <i>上杉达也</i>, <i>Xyao</i>, <i>Lucifer</i>, <i>leaves</i>, <i>Long</i></li>
				<li class="btn"><a href="#" class="devhelp">开发帮助</a><a href="#" class="fbmsg">给我们留言</a></li>
			</ul>
		</div>
	</div>
	<div class="cl"></div>
</div>
<div class="notewarn"> <span class="close"><a href="javascript:;"></a></span>
	<div>显示分辨率 1360*768 显示效果最佳，建议使用<strong style="color:#00F">谷歌浏览器</strong>；欢迎使用！</div>
</div>
</body>
</html>