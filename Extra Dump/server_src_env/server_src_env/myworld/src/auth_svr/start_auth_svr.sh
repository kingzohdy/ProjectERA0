#!/bin/sh
ipcrm -M 268453377
./auth_svr --id=0.0.70.1 --business-id=66 --bus-key=7381 --conf-file=auth_svr.xml --log-level=600 --log-file=auth_svr --daemon start

