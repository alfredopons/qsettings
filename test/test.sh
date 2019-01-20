#!/bin/bash
#
# Alfredo Pons Menargues - 2019 - apons@linucleus.com
#

FILE_INI="example.conf"

#### Parsing key active #####
ACTIVE=$(qsettings -k active $FILE_INI)
if [[ $? -ne 0 ]];then
  echo "Problems parsing $FILE_INI"
  exit 1
fi

if [[ "$ACTIVE" == "true"  ]]
then
  echo "Active is true"
fi

#### Parsing key device in group nfc #####
NFC=$(qsettings -k nfc/device $FILE_INI)
if [[ $? -ne 0 ]];then
  echo "Problems parsing $FILE_INI"
  exit 1
fi

echo "NFC Device: $NFC"

exit 0
