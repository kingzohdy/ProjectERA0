#!/bin/bash
export LD_LIBRARY_PATH=/usr/local/mmog/cluster/bin/
nohup ./agent_charge IFM_CHARGE_WYYL_I001 0 1 > log/ifmagent.log &
