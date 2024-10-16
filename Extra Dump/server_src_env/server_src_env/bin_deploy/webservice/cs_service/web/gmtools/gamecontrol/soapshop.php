<?php
$xml = "";
$client = new SoapClient("../cfg/cs_service.wsdl",array('trace' => 1,'exceptions' => true,'cache_wsdl' => WSDL_CACHE_NONE,'features' => SOAP_SINGLE_ELEMENT_ARRAYS,"login" => "soapuser","password"=> "soapuser#1234"));
//金银币交易
function ShopCgi($RegionID,$WorldID,$Type,$Num,$Price,$ExchgNum,$StartTime,$EndTime,$InterVal,$TradeID,$TradeFlag,$ReMoveType)
{
	global $client;
	$xml=$client->ShopCgi(
							   array
									(										
										'RegionID'=>$RegionID,
										'WorldID'=>$WorldID,
										'Type'=>$Type,
										'Num'=>$Num,
										'Price'=>$Price,
										'ExchgNum'=>$ExchgNum,
										'StartTime'=>$StartTime,
										'EndTime'=>$EndTime,
										'InterVal'=>$InterVal,
										'TradeID'=>$TradeID,
										'TradeFlag'=>$TradeFlag,
										'ReMoveType'=>$ReMoveType,
										'TimeOut'=>3000
									)
						);
	return $xml;
}
?>