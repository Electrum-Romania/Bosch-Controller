#!/bin/sh

cd ..

cat $(cat options/all_options_list) >options/all_options.inc
cat $(cat options/all_pdata_list) >options/all_pdata.inc
