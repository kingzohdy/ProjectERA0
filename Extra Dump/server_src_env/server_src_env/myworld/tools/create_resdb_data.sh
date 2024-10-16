#!/bin/sh
mysqldump --add-drop-database --add-drop-table --default-character-set=latin1 -ummog -pmmog  resdb > resdb_data.sql
