#!/bin/bash
PATH=../../../tools:${PATH}
export PATH

#meta2tab -B ../cfg/test_ormsvr_database.tdr  --meta_name=RoleDataVinson -o tormtestdb.sql
meta2tab -B ../cfg/tdr_database.tdr  --meta_name=UserInfo -o userdb.sql
