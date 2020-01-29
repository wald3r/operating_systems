#!/bin/bash
#Backup script
###########
#
# cp <- copy 
#  . <- current directory
#  $1 <- given Argument(path)
#  -R <- recursive 
#  -u <- update
#
############


cp -R -u . $1

exit 




