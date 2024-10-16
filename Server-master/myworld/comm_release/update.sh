#!/bin/sh
cp ~/tsf4g/lib/libtsf4g.a ./lib/libgamecomm.a
cp ~/tsf4g/lib/libtormdbapi.a ./lib/
cp ~/tsf4g/lib/libtormprotocolapi.a ./lib/
cp ~/tsf4g/apps/tdirty/lib/libtdirty.a ./lib/

cp ~/tsf4g/apps/tagent/bin/tagent ./apps/tagent/bin/
cp ~/tsf4g/apps/tcenterd/bin/tcenterd ./apps/tcenterd/bin/

cp ~/tsf4g/apps/configmngsystem/bin/* ./apps/tcm/bin/
cp ~/tsf4g/apps/tconnd/tconndsvr/tconnd ./apps/tcm/bin/tcm_conn
cp ~/tsf4g/apps/tconnd/tconndsvr/tconnd ./apps/tcm/bin/tconnd
cp ~/tsf4g/apps/configmngsystem/lib/libtcmapi.a ./apps/tcm/lib/
cp ~/tsf4g/apps/configmngsystem/lib/mod_procmng.so.1.0.0 ./apps/tcm/lib/
cp ~/tsf4g/apps/configmngsystem/lib/mod_tbusconfig.so.1.0.0 ./apps/tcm/lib/
cp ~/tsf4g/apps/configmngsystem/lib/mod_procmng.so.1.0.0 ~/myworld/cfg/lib/
cp ~/tsf4g/apps/configmngsystem/lib/mod_tbusconfig.so.1.0.0 ~/myworld/cfg/lib/
cp ~/tsf4g/apps/tagent/lib/mod_tdirty.so ~/myworld/cfg/lib/
cp ~/tsf4g/apps/configmngsystem/src/protocol/tcm_proto.tdr ../cfg/tcm/
cp ~/tsf4g/apps/configmngsystem/src/protocol/tcm_proto.tdr ./apps/tcm/conf/


cp ~/tsf4g/apps/orm/tormsvr/tormsvr_static ./apps/torm/bin_static/tormsvr
cp ~/tsf4g/apps/orm/tormsvr/tormsvr ./apps/torm/bin/


cp ~/tsf4g/services/tbus_service/tbusd ./services/tbus_service/
cp ~/tsf4g/services/tlog_service/tlogd ./services/tlog_service/
cp ~/tsf4g/services_src/tlog_service/tconnddef.tdr ./services/tlog_service/
cp ~/tsf4g/services_src/tlog_service/tconnddef.tdr ../cfg/tcm/

cp ~/tsf4g/tools/meta2tab ./tools/
cp ~/tsf4g/tools/tbusmgr ./tools/
cp ~/tsf4g/tools/tbusmgr ./apps/tcm/bin/
cp ~/tsf4g/tools/tbusmgr ./apps/tcenterd/bin/
cp ~/tsf4g/tools/tdr ./tools/
cp ~/tsf4g/tools/tdr2xml ./tools/
cp ~/tsf4g/tools/tmng ./tools/
cp ~/tsf4g/tools/trelaymgr ./tools/

cp ~/tsf4g/apps/ResConvertBasedDR/lib/libresloader.a ~/myworld/src/lib/
cp -R ~/tsf4g/include ./

