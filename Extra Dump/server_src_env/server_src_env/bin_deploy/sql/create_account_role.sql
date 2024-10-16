CREATE TABLE IF NOT EXISTS AccountRole (
 RoleWID BIGINT UNSIGNED  NOT NULL  ,
 Uin INT    ,
 Pass9Account VARCHAR(51)    ,
 RoleName VARCHAR(32)    ,
 DelTime INT    ,
 RegionID INT    ,
 ServerID INT    ,
 CreateTime INT    ,
 OnlineFlag INT    ,
 MacAddr VARCHAR(20)    ,
 Level INT    ,
 LastLogout INT    ,
 LastLoginIP INT UNSIGNED    ,
 FirstLoginTime INT    ,
 SecondLoginTime INT    ,
PRIMARY KEY(RoleWID))  ENGINE=InnoDB DEFAULT CHARSET=latin1;
create index AccountRole_Idx_0 on AccountRole ( RoleName);
create index AccountRole_Idx_1 on AccountRole ( Uin);
create index AccountRole_Idx_2 on AccountRole ( Pass9Account);
ALTER TABLE AccountRole MODIFY COLUMN Pass9Account varchar(51) character set latin1 collate latin1_bin default NULL;
ALTER TABLE AccountRole MODIFY COLUMN RoleName varchar(32) character set latin1 collate latin1_bin default NULL;
