#!/bin/sh

PATH=../../../tools:${PATH}
export PATH

tmng --destroy metabase
tmng --destroy mib
