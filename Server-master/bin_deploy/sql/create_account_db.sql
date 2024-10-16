CREATE TABLE IF NOT EXISTS Account (
 Uin INT  NOT NULL AUTO_INCREMENT ,
 UinFlag INT    ,
 Pass9Account VARCHAR(51)    ,
 PasswdHash BLOB    ,
 BirthDay VARCHAR(9)    ,
 Gender TINYINT    ,
 RegisterTs VARCHAR(15)    ,
 SilenceEndTime INT    ,
 SilenceStartTime INT    ,
 FreezeReason VARCHAR(32)    ,
 FreezeStartTime INT    ,
 FreezeEndTime INT    ,
 LastLoginTime INT    ,
 LastLoginIP INT UNSIGNED    ,
 AccountCreateTime INT    ,
 LastLoginWorldID INT    ,
 SafePwd BLOB    ,
 QuickStatus INT    ,
 ActiveKeyStatus INT    ,
 LastLogout INT    ,
 FirstLoginCltType INT    ,
 TuiGuangNum INT    ,
 TopTuiGuangNum INT    ,
 PasswdCard BLOB    ,
 KEY(Uin),
PRIMARY KEY(Uin),
UNIQUE (Pass9Account))  ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1000;
create index Account_Idx_0 on Account ( Uin);
create index Account_Idx_1 on Account ( Pass9Account);
create index Account_Idx_2 on Account ( LastLoginTime);
create index Account_Idx_3 on Account ( LastLogout);
ALTER TABLE Account MODIFY COLUMN Pass9Account varchar(51) character set latin1 collate latin1_bin default NULL;
