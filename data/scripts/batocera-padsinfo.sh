#!/bin/sh

SDLINFOS=$(sdl2-jstest --list)

echo "<?xml version=\"1.0\"?>"
echo "<pads>"
for PADBAT in /sys/class/power_supply/*/device/input/input*/event*
do
    if test -e "${PADBAT}"
    then
	    DEVICE_PATH="/dev/"$(grep -E '^DEVNAME=' "${PADBAT}/uevent" | sed -e s+"^DEVNAME="++)
	    SDLPAD=$(echo "${SDLINFOS}" | grep -E "${DEVICE_PATH} " | cut -d ' ' -f 2)
	    if test -n "${SDLPAD}"
	      then
          DEVICE_NAME=$(cat "${PADBAT}/device/name")
          BATTERYPATH=$(echo "${PADBAT}" | sed -e s+"device/input/input[0-9]*/event[0-9]*$"+""+)
          BATTERYINFO=${BATTERYPATH}/uevent
          BATTERY_CAPATICY=$(grep -E "^POWER_SUPPLY_CAPACITY=" "${BATTERYINFO}" | sed -e s+'^POWER_SUPPLY_CAPACITY='++)
          BATTERY_STATUS=$(grep -E "^POWER_SUPPLY_STATUS=" "${BATTERYINFO}" | sed -e s+'^POWER_SUPPLY_STATUS='++)
          echo "  <pad device=\"${DEVICE_PATH}\" name=\"${DEVICE_NAME}\" id=\"${SDLPAD}\" battery=\"${BATTERY_CAPATICY}\" status=\"${BATTERY_STATUS}\" />"
	    fi
    fi
done 2>/dev/null
echo "</pads>"
exit 0
