#!/bin/bash
timeout 5 bluetoothctl -- scan on
bluetoothctl -- scan off
bluetoothctl -- remove 98:D3:51:FE:2B:3F
timeout 5 bluetoothctl -- scan on
bluetoothctl -- scan off
bluetoothctl -- trust 98:D3:51:FE:2B:3F
echo '1234' | bluetoothctl -- pair 98:D3:51:FE:2B:3F
bluetoothctl -- exit
rfcomm release rfcomm0
rfcomm bind rfcomm0 98:D3:51:FE:2B:3F
echo 'y' | echo 'k' | echo $'\ca' #| screen /dev/rfcomm0 9600,cs8,-parenb,-cstopb,-hupcl  #  use ^a, k to exit.