cmd_/home/ubuntu/OS-Project/device_driver/batt_to_bright/modules.order := {   echo /home/ubuntu/OS-Project/device_driver/batt_to_bright/backlight_control.ko; :; } | awk '!x[$$0]++' - > /home/ubuntu/OS-Project/device_driver/batt_to_bright/modules.order