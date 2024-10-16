/*
MySQL Data Transfer
Source Host: 192.168.0.77
Source Database: gmtools
Target Host: 192.168.0.77
Target Database: gmtools
Date: 2012-12-17 11:46:37
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for gmlog
-- ----------------------------
DROP TABLE IF EXISTS `gmlog`;
CREATE TABLE `gmlog` (
  `LogId` int(11) NOT NULL auto_increment COMMENT 'æ—¥å¿—ID',
  `UserId` int(11) NOT NULL COMMENT '用户ID',
  `GameUserID` int(11) default NULL COMMENT 'Ã¨Â¢Â«Ã¦â€œÂÃ¤Â½Å“Ã§Å¡â€žÃ¨Â´Â¦Ã¦Ë†Â·ID',
  `GameUserRole` varchar(32) character set utf8 collate utf8_icelandic_ci default NULL COMMENT 'è¢«æ“ä½œçš„è´¦æˆ·è§’è‰²å',
  `RegionID` int(11) default NULL COMMENT 'è¢«æ“ä½œçš„è§’è‰²æ‰€åœ¨å¤§åŒº',
  `WorldID` int(11) default NULL COMMENT 'è¢«æ“ä½œçš„è§’è‰²æ‰€åœ¨æœ',
  `time` timestamp NULL default NULL COMMENT 'ÃƒÆ’Ã‚Â¦ÃƒÂ¢Ã¢â€šÂ¬Ã…â€œÃƒâ€šÃ‚ÂÃƒÆ’Ã‚Â¤Ãƒâ€šÃ‚Â½Ãƒâ€¦Ã¢â‚¬Å“ÃƒÆ’Ã‚Â¦ÃƒÂ¢Ã¢â€šÂ¬Ã¢â‚¬ÂÃƒâ€šÃ‚Â¶ÃƒÆ’Ã‚Â©ÃƒÂ¢Ã¢â€šÂ¬Ã¢â‚¬ÂÃƒâ€šÃ‚Â´',
  `StartTime` timestamp NULL default NULL COMMENT 'Ã§â€Å¸Ã¦â€¢Ë†Ã¦â€”Â¶Ã©â€”Â´',
  `ContinueTime` int(11) default NULL COMMENT 'æŒç»­æ—¶é—´',
  `OperateType` varchar(20) default NULL COMMENT 'Ã¦â€œÂÃ¤Â½Å“Ã§Â±Â»Ã¥Å¾â€¹Ã¯Â¼Å¡1.Ã¥â€¦Â¬Ã¥â€˜Å Ã¦Â·Â»Ã¥Å Â Ã¯Â¼Å’2.Ã¥â€¦Â¬Ã¥â€˜Å Ã¥Ë†Â Ã©â„¢Â¤',
  `OperateState` varchar(20) default NULL COMMENT 'ÃƒÂ¦Ã¢â‚¬Å“Ã‚ÂÃƒÂ¤Ã‚Â½Ã…â€œÃƒÂ§Ã…Â Ã‚Â¶ÃƒÂ¦Ã¢â€šÂ¬Ã‚Â,1:ÃƒÂ¨Ã‚Â¡Ã‚Â¨ÃƒÂ§Ã‚Â¤Ã‚ÂºÃƒÂ¥Ã‚Â¤Ã‚Â±ÃƒÂ¨Ã‚Â´Ã‚Â¥ 0ÃƒÂ¨Ã‚Â¡Ã‚Â¨ÃƒÂ§Ã‚Â¤Ã‚ÂºÃƒÂ¦Ã‹â€ Ã‚ÂÃƒÂ¥Ã…Â Ã…Â¸',
  `log` text COMMENT 'å½•è®°æ—¥å¿—',
  PRIMARY KEY  (`LogId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records 
-- ----------------------------
