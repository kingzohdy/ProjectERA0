<?php
/* vim: set expandtab sw=4 ts=4 sts=4: */
/**
 *
 * @version $Id$
 * @package phpMyAdmin
 */

/**
 *
 */
//require_once './libraries/common.inc.php';


global $client;

$client = new SoapClient("./cfg/cs_service.wsdl", 
							array( 
							// Stuff for development. 
							'trace' => 1, 
							'exceptions' => true, 
							'cache_wsdl' => WSDL_CACHE_NONE, 
							'features' => SOAP_SINGLE_ELEMENT_ARRAYS, 
							'encoding' =>'utf8',
							'login' => 'soapuser',
							'password' => 'soapuser#1234'
							)
						);
global $soap_serverlist;
global $soap_serverlistKey ;
$soap_serverlist = null;

$soap_serverlistKey = 'soap_serverlist';

if (isset($_SESSION[$soap_serverlistKey]))
{	
	$soap_serverlist = $_SESSION[$soap_serverlistKey];
//	var_dump($soap_serverlist);
}
else
{
	$ret=$client->GetRegionWorldInfo(array('TimeOut'=>0));
	
	if ($ret->ErrorCode == 0)
	{
//		$str = $ret->StrXML;
//		$ret->StrXML = simplexml_load_string($str);
//		$ret->StrXML = utf8_decode($str);
//		$ret->StrXML = iconv("UTF-8", "gbk", $str);

//		var_dump($ret->StrXML);
//		var_dump($str);
	}
//	$hex = serialize($ret);
//	var_dump($hex);
//	$hex = base64_encode($hex);
//	var_dump($hex);
//	$soap_serverlist = $hex;
	$soap_serverlist = $ret;
//	var_dump($soap_serverlist);
//	var_dump($soap_serverlistKey);

}
//
//$RegionID = 0;
//$WorldID = 0;
//$TimeOut = 30000;
//if (isset($_POST['region_id']))
//{
//	$RegionID = $_POST['region_id'];
//}
//
//if (isset($_POST['world_id']))
//{
//	$WorldID = $_POST['world_id'];
//}
//
////	var_dump($RegionID);
////	var_dump($WorldID);
////	var_dump($_POST);
////	exit;
//
//$xml=$client->ReloadServer(array('TimeOut'=>$TimeOut,'RegionID'=>$RegionID,'WorldID'=>$WorldID));
//$ret = $xml->ErrorCode;
//if ($ret == 0)
//{
//	$ret = 1;
//}
//else
//{
//	$GLOBALS['soap_err_str'] = $xml->StrError->ErrorText;
////		var_dump($GLOBALS['soap_err_str']);
////		var_dump($xml);
//}
//var_dump(soap_getDBInfo('world_3_10'));
//var_dump(soap_getDBInfo('wor_10'));
//var_dump(soap_getDBInfo());
function soap_getDBInfo($db='')
{
	if ($db == '')
	{
		$db = "world_" . $GLOBALS['RegionID'] ."_" . $GLOBALS['WorldID'];
	}
	$ret = preg_match('/world_/',$db);
	$billRet = preg_match('/bill_/',$db);

	if ($ret < 1 and $billRet < 1)
	{
		return '';
	}

	$a = explode('_',$db);
//	var_dump($a);
	$n = count($a);
//	var_dump($n);
	$RegionID = 0;
	$WorldID = 0;
	
	if ($n == 2)
	{
		if ($ret > 0)
		{
			$WorldID = $a[1];
		}
		else
		{
			$RegionID = $a[1];
		}
//		var_dump($a[1]);
	}
	elseif($n == 3)
	{
		$WorldID = $a[2];
		$RegionID = $a[1];
	}

	global $soap_serverlist;
//	var_dump($soap_serverlist);
	$regionName = '';
	$worldName = '';

	if (isset($soap_serverlist) && $soap_serverlist->ErrorCode == 0)
	{
		

		$str = $soap_serverlist->StrXML;
//		var_dump($str);
		$xml = simplexml_load_string($str);
//		var_dump($xml);
		$regionNum = (int)$xml->AccountCount;
//		var_dump($_SESSION);
//		var_dump($regionNum );
		if ($regionNum == 1)
		{
			$info = $xml->AccountInfos;
			$id = (int)$info->RegionID;
			if ($id == $RegionID)
			{
				$regionName = iconv("UTF-8", "gbk", $info->RegionName);;
			}
		}
		else
		{
			for ($i = 0; $i < $regionNum; $i++)
			{
				$info = $xml->AccountInfos[$i];
				$id = (int)$info->RegionID;
//				var_dump($id );
				if ($id == $RegionID)
				{
					$regionName = iconv("UTF-8", "gbk",$info->RegionName);
					break;
				}
			}
		}

		$worldNum = (int)$xml->WorldCount;
//		var_dump($worldNum );
		if ($worldNum == 1)
		{
			$info = $xml->WorldInfos;
			$id = (int)$info->RegionID;
			$wid = (int)$info->WorldID;
//			var_dump($id );
//			var_dump($wid );
			if ($id == $RegionID and $wid == $WorldID)
			{
				$worldName = iconv("UTF-8", "gbk",$info->WorldName);
			}
		}
		else
		{
			for ($i = 0; $i < $worldNum; $i++)
			{
				$info = $xml->WorldInfos[$i];
				$id = (int)$info->RegionID;
				$wid = (int)$info->WorldID;
//				var_dump($id );
//				var_dump($wid );
				if ($id == $RegionID and $wid == $WorldID)
				{
					$worldName = iconv("UTF-8", "gbk",$info->WorldName);
					break;
				}
			}
		}

		
	}

	return "$regionName $worldName";
}
?>
