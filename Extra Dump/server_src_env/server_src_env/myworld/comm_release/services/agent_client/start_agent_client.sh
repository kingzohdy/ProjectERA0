#!/bin/bash
export LD_LIBRARY_PATH=/usr/local/mmog/cluster/bin/
nohup ./agent_client IFM_ACTIVE_SERVICE_WYYL_I001 0 1 > log/ifmagent.log &
