cmd_/home/pi/BackApp/LightRimocon/module/modules.order := {   echo /home/pi/BackApp/LightRimocon/module/LightRimocon.ko; :; } | awk '!x[$$0]++' - > /home/pi/BackApp/LightRimocon/module/modules.order
