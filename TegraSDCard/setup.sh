#!/bin/sh -e
#brightness 30
#contras 10
#saturation 200
#White-balance auto 0
#white balance temp 6200
#sharpbess 50
#backlight 0
#v4l2-ctl --set-ctrl=<ctrl>=<val>[,<ctrl>=<val>...]
# brightness (int)    : min=30 max=255 step=1 default=133 value=30
#                       contrast (int)    : min=0 max=10 step=1 default=5 value=10
#                     saturation (int)    : min=0 max=200 step=1 default=83 value=146
# white_balance_temperature_auto (bool)   : default=1 value=1
#           power_line_frequency (menu)   : min=0 max=2 default=2 value=2
#      white_balance_temperature (int)    : min=2800 max=10000 step=1 default=4500 value=6200 flags=inactive
#                      sharpness (int)    : min=0 max=50 step=1 default=25 value=2
#         backlight_compensation (int)    : min=0 max=10 step=1 default=0 value=0
#                  exposure_auto (menu)   : min=0 max=3 default=1 value=3
#              exposure_absolute (int)    : min=5 max=20000 step=1 default=156 value=156 flags=inactive
#                   pan_absolute (int)    : min=-201600 max=201600 step=3600 default=0 value=0
#                  tilt_absolute (int)    : min=-201600 max=201600 step=3600 default=0 value=0
#                  zoom_absolute (int)    : min=0 max=10 step=1 default=0 value=0
v4l2-ctl --set-ctrl=exposure_auto=1,contrast=10,saturation=200,white_balance_temperature_auto=0,white_balance_temperature=6200,sharpness=50,backlight_compensation=0,exposure_absolute=9
v4l2-ctl --set-fmt-video=pixelformat=1
#Enables All Cores TAKES more power
echo 0 > /sys/devices/system/cpu/cpuquiet/tegra_cpuquiet/enable
echo 1 > /sys/devices/system/cpu/cpu0/online
echo 1 > /sys/devices/system/cpu/cpu1/online
echo 1 > /sys/devices/system/cpu/cpu2/online
echo 1 > /sys/devices/system/cpu/cpu3/online
echo performance > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor

