<?php
$xml = "";
$client = new SoapClient("../cfg/acc_service.wsdl",array('trace' => 1,'exceptions' => true,'cache_wsdl' => WSDL_CACHE_NONE,'features' => SOAP_SINGLE_ELEMENT_ARRAYS,"login" => "soapuser","password"=> "soapuser#1234"));

//挂失密保
function lossmibao($AccName)
{
	$m = "65,41,22,47,33,96,67,45,19,80,75,31,02,35,05,79,60,16,69,56,11,76,34,59,10,94,28,57,17,23,39,87,88,97,38,84,25,72,04,26,71,08,91,58,61,03,15,98,73,99,24,85,36,52,90,70,50,64,14,74,30,44,37,51,40,21,01,06,63,93,95,55,18,07,27,54,49,09,78,13";
	global $client;
	$xml=$client->UpdateAccountPasswdCard(
							array
							(
								'RegionID'=>0,
								'AccName'=>$AccName,
								'BindType'=>0,
								'RowNum'=>0,
								'ColumnNum'=>0,
								'PasswdCard'=>0
							)
					);
	return $xml;
}
?>
