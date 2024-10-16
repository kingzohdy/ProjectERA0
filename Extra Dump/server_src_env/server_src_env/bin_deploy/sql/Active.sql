CREATE TABLE IF NOT EXISTS Active (
 ActiveCode VARCHAR(255)    ,
 IsActive INT    ,
 Pass9Account VARCHAR(51)    ,
UNIQUE (ActiveCode))  ENGINE=InnoDB DEFAULT CHARSET=latin1;
create index Active_Idx_0 on Active ( Pass9Account);
