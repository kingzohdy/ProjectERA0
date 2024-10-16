<?php	if(!defined('IN_PHPMYWIND')) exit('Request Error!');

/*
**************************
(C)2010-2012 phpMyWind.com
update: 2012-1-11 9:47:47
person: Feng
**************************
*/


/*
 * 函数说明：截取指定长度的字符串
 *         utf-8专用 汉字和大写字母长度算1，其它字符长度算0.5
 *
 * @param  string  $str  原字符串
 * @param  int     $len  截取长度
 * @param  string  $etc  省略字符...
 * @return string        截取后的字符串
 */
function ReStrLen($str, $len=10, $etc='...')
{
	$restr = '';
	$i = 0;
	$n = 0.0;

	//字符串的字节数
	$strlen = strlen($str);
	while(($n < $len) and ($i < $strlen))
	{
	   $temp_str = substr($str, $i, 1);

	   //得到字符串中第$i位字符的ASCII码
	   $ascnum = ord($temp_str);

	   //如果ASCII位高与252
	   if($ascnum >= 252) 
	   {
		    //根据UTF-8编码规范，将6个连续的字符计为单个字符
			$restr = $restr.substr($str, $i, 6); 
			//实际Byte计为6
			$i = $i + 6; 
			//字串长度计1
			$n++; 
	   }
	   elseif($ascnum >= 248)
	   {
			$restr = $restr.substr($str, $i, 5);
			$i = $i + 5;
			$n++;
	   }
	   elseif($ascnum >= 240)
	   {
			$restr = $restr.substr($str, $i, 4);
			$i = $i + 4;
			$n++;
	   }
	   elseif($ascnum >= 224)
	   {
			$restr = $restr.substr($str, $i, 3);
			$i = $i + 3 ;
			$n++;
	   }
	   elseif ($ascnum >= 192)
	   {
			$restr = $restr.substr($str, $i, 2);
			$i = $i + 2;
			$n++;
	   }

	   //如果是大写字母 I除外
	   elseif($ascnum>=65 and $ascnum<=90 and $ascnum!=73)
	   {
			$restr = $restr.substr($str, $i, 1);
			//实际的Byte数仍计1个
			$i = $i + 1; 
			//但考虑整体美观，大写字母计成一个高位字符
			$n++; 
	   }

	   //%,&,@,m,w 字符按1个字符宽
	   elseif(!(array_search($ascnum, array(37, 38, 64, 109 ,119)) === FALSE))
	   {
			$restr = $restr.substr($str, $i, 1);
			//实际的Byte数仍计1个
			$i = $i + 1;
			//但考虑整体美观，这些字条计成一个高位字符
			$n++; 
	   }

	   //其他情况下，包括小写字母和半角标点符号
	   else
	   {
			$restr = $restr.substr($str, $i, 1);
			//实际的Byte数计1个
			$i = $i + 1; 
			//其余的小写字母和半角标点等与半个高位字符宽
			$n = $n + 0.5; 
	   }
	}

	//超过长度时在尾处加上省略号
	if($i < $strlen)
	{
	   $restr = $restr.$etc;
	}

	return $restr;
}


//获得当前的页面文件的url
function GetCurUrl()
{
	if(!empty($_SERVER["REQUEST_URI"]))
	{
		$nowurls = explode("?",$_SERVER["REQUEST_URI"]);
		$nowurl = $nowurls[0];
	}
	else
	{
		$nowurl = $_SERVER["PHP_SELF"];
	}

	return $nowurl;
}


//获取IP
function GetIP()
{
	static $ip = NULL;
    if($ip !== NULL) return $ip;

    if(isset($_SERVER['HTTP_X_FORWARDED_FOR']))
	{
        $arr = explode(',', $_SERVER['HTTP_X_FORWARDED_FOR']);
        $pos =  array_search('unknown',$arr);
        if(false !== $pos) unset($arr[$pos]);
        $ip   =  trim($arr[0]);
    }
	else if(isset($_SERVER['HTTP_CLIENT_IP']))
	{
        $ip = $_SERVER['HTTP_CLIENT_IP'];
    }
	else if(isset($_SERVER['REMOTE_ADDR']))
	{
        $ip = $_SERVER['REMOTE_ADDR'];
    }

    //IP地址合法验证
    $ip = (false !== ip2long($ip)) ? $ip : '0.0.0.0';
    return $ip;
}


//查看数据大小
function GetRealSize($size)
{
	$kb = 1024;          // Kilobyte
	$mb = 1024 * $kb;    // Megabyte
	$gb = 1024 * $mb;    // Gigabyte
	$tb = 1024 * $gb;    // Terabyte

	if($size < $kb)
	{
		return $size.'B';
	}
	else if($size < $mb)
	{
		return round($size/$kb,2).'KB';
	}
	else if($size < $gb)
	{
		return round($size/$mb,2).'MB';
	}
	else if($size < $tb)
	{
		return round($size/$gb,2).'GB';
	}
	else
	{
		return round($size/$tb,2).'TB';
	}
}


//获取文件夹大小
function GetDirSize($dir)
{
	$handle = opendir($dir);
	$fsize  = '';

	while(($fname = readdir($handle)) !== false)
	{
		if($fname != '.' && $fname != '..')
		{
			if(is_dir("$dir/$fname"))
			{
				$fsize += GetDirSize("$dir/$fname");
			}
			else
			{
				$fsize += filesize("$dir/$fname");
			}
		}
	}

	closedir($handle);
	if(empty($fsize)) $fsize = 0;

	return $fsize;
}


//返回格林威治标准时间
function MyDate($format='Y-m-d H:i:s', $timest=0)
{
	//global $cfg_timezone;
	$addtime = 8 * 3600;
	if(empty($format))
	{
		$format = 'Y-m-d H:i:s';
	}
	return gmdate($format, $timest+$addtime);
}


//返回格式化(Y-m-d H:i:s)的时间
function GetDateTime($mktime)
{
	return MyDate('Y-m-d H:i:s',$mktime);
}


//返回格式化(Y-m-d)的日期
function GetDateMk($mktime)
{
	return MyDate('Y-m-d', $mktime);
}


//从普通时间转换为Linux时间截
function GetMkTime($dtime)
{
	if(!preg_match("/[^0-9]/", $dtime))
	{
		return $dtime;
	}
	$dtime = trim($dtime);
	$dt = array(1970, 1, 1, 0, 0, 0);
	$dtime = preg_replace("/[\r\n\t]|日|秒/", " ", $dtime);
	$dtime = str_replace("年", "-", $dtime);
	$dtime = str_replace("月", "-", $dtime);
	$dtime = str_replace("时", ":", $dtime);
	$dtime = str_replace("分", ":", $dtime);
	$dtime = trim(preg_replace("/[ ]{1,}/", " ", $dtime));
	$ds = explode(" ", $dtime);
	$ymd = explode("-", $ds[0]);
	if(!isset($ymd[1])) $ymd = explode(".", $ds[0]);
	if(isset($ymd[0])) $dt[0] = $ymd[0];
	if(isset($ymd[1])) $dt[1] = $ymd[1];
	if(isset($ymd[2])) $dt[2] = $ymd[2];
	if(strlen($dt[0])==2) $dt[0] = '20'.$dt[0];
	if(isset($ds[1]))
	{
		$hms = explode(":", $ds[1]);
		if(isset($hms[0])) $dt[3] = $hms[0];
		if(isset($hms[1])) $dt[4] = $hms[1];
		if(isset($hms[2])) $dt[5] = $hms[2];
	}
	foreach($dt as $k=>$v)
	{
		$v = preg_replace("/^0{1,}/", '', trim($v));
		if($v == '')
		{
			$dt[$k] = 0;
		}
	}
	
	$mt = mktime($dt[3], $dt[4], $dt[5], $dt[1], $dt[2], $dt[0]);
	if(!empty($mt)) return $mt;
	else return time();
}


//显示信息
function ShowMsg($msg='', $gourl='-1')
{
	if($gourl != '-1')
	{
		echo '<script>alert("'.$msg.'");location.href="'.$gourl.'";</script>';
	}
	else
	{
		echo '<script>alert("'.$msg.'");history.go(-1);</script>';
	}
}


//读取文件内容
function Readf($file)
{
	if(file_exists($file) && is_readable($file))
	{
		if(function_exists('file_get_contents'))
		{
			$str = file_get_contents($file);
		}
		else
		{
			$str = '';

			$fp = fopen($file, 'r');
			while(!feof($fp))
			{
				$getstr .= fgets($fp, 1024);
			}
			fclose($fp);
		}
		return $str;
	}
	else
	{
		return FALSE;
	}
}


//写入文件内容
function Writef($file,$str,$mode='w')
{
	if(file_exists($file) && is_writable($file))
	{
		$fp = fopen($file, $mode);
		flock($fp, 3);
		fwrite($fp, $str);
		fclose($fp);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


//查看url中是否包含http
function IsHttpUrl($url)
{
	if(!preg_match("/^(http|ftp):/", $url))
	{
		$url = 'http://'.$url;
	}

	return $url;
}


//执行时间函数
function ExecTime()
{
	$time = explode(" ", microtime());
	$usec = (double)$time[0];
	$sec = (double)$time[1];
	return $sec + $usec;
}


//清除HTML
function ClearHtml($str)
{
	$str = strip_tags($str);

	//首先去掉头尾空格
	$str = trim($str);

	//接着去掉两个空格以上的
	$str = preg_replace('/\s(?=\s)/', '', $str);

	//最后将非空格替换为一个空格
	$str = preg_replace('/[\n\r\t]/', ' ', $str);

	return $str;
}


//获取随机字符串
function GetRandStr($length=6)
{
    //'!@#$%^&*()-_ []{}<>~`+=,.;:/?|';
	$chars = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789';
    $random_str = '';

    for($i=0; $i<$length; $i++)
    {
        //这里提供两种字符获取方式
        //第一种是使用 substr 截取$chars中的任意一位字符；
        //第二种是取字符数组 $chars 的任意元素
        //$password .= substr($chars, mt_rand(0, strlen($chars) - 1), 1);
        $random_str .= $chars[mt_rand(0, strlen($chars) - 1)];
    }

    return $random_str;
}


//引用函数
function Inc($file)
{
	require_once($file);
}
?>