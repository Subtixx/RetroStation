#!/bin/bash

do_usage() {
	echo "$1 list"
	echo "$1 get"
	echo "$1 set {auto|<device>}"
	echo "$1 list-profiles"
	echo "$1 get-profile"
	echo "$1 set-profile {auto|<profile>}"
	echo "$1 getSystemVolume"
	echo "$1 setSystemVolume {mute|unmute|mute-toggle|[+-]<0...100>}"
	echo "$1 test"
}

# Export XDG for PW
export XDG_RUNTIME_DIR=/var/run

# Set variables
MAX_VOLUME="$(batocera-settings-get audio.volume.boost)" || MAX_VOLUME=100
DEFAULT_SINK=$(pactl info | grep -E '^Default Sink: ' | sed -e s+"^Default Sink: "+""+ | head -1)
DETECT_VOLUME=$(pactl list sinks-raw | grep -E '^[^ ]* name='"${DEFAULT_SINK}"' ' | sed -e s+"^.* volume=\([0-9]*\) .*$"+'\1'+)

ACTION="$1"

case "${ACTION}" in
	list)
	    echo "auto auto"
	    pactl list sinks-raw | sed -e s+"^sink=[0-9]* name=\([^ ]*\) .* description=\(.*\)$"+"\1 \2"+
	;;

	get)
	    batocera-settings-get audio.device
	;;

	set)
		MODE="$2"
		case "${MODE}" in
			auto)
			    ARCH=$(cat /usr/share/batocera/batocera.arch)
			    case "${ARCH}" in
				"rk3399|rk3288") # defaults to HDMI
				    MODE=alsa_output.platform-hdmi-sound.stereo-fallback
				    pactl set-default-sink "${MODE}" || exit 1
				    ;;
				*)
				    # do nothing!
			    esac
			;;
			*)
			    pactl set-default-sink "${MODE}" || exit 1
		esac
		;;

	list-profiles)
	    echo "auto auto"
	    pactl list cards-profiles-raw | grep "available=1" | sed -e s+"^card=[0-9]* name=\([^ ]*\) profile=\([^ ]*\) available=[01] active=[01] description=\(.*\)$"+"\2@\1 \3"+
	    ;;

	get-profile)
	    /usr/bin/batocera-settings-get audio.profile
	    ;;

	set-profile)
	    MODE="$2"
	    if test "${MODE}" != "auto"
	    then
		PROFILE_NAME=$(echo "${MODE}" | cut -d @ -f 1)
		CARD_NAME=$(echo "${MODE}" | cut -d @ -f 2)
		pactl set-card-profile "${CARD_NAME}" "${PROFILE_NAME}" || exit 1
	    fi
	    ;;

	getSystemVolume)
	    echo "${DETECT_VOLUME}"
	    ;;

	setSystemVolume)
	    VOLUME="$2"
	    case "${VOLUME}" in
		"mute")
		    pactl set-sink-mute @DEFAULT_SINK@ 1 || exit 1
		    ;;
		"unmute")
		    pactl set-sink-mute @DEFAULT_SINK@ 0 || exit 1
		    ;;
		"mute-toggle")
		    pactl set-sink-mute @DEFAULT_SINK@ toggle || exit 1
		    ;;
		*)
		    if echo "${VOLUME}" | grep -qE '^[0-9]+$'
		    then
		      if test "${VOLUME}" -ge 0 -a "${VOLUME}" -le "${MAX_VOLUME}"
		      then
		        pactl set-sink-volume @DEFAULT_SINK@ "${VOLUME}%" || exit 1
		        exit 0
		      fi
		    else
		      if echo "${VOLUME}" | grep -qE '^[+-][0-9]+$'
		      then
		        let NEWVOLUME=${DETECT_VOLUME}${VOLUME}
		        test "${NEWVOLUME}" -lt 0 && NEWVOLUME=0
		        test "${NEWVOLUME}" -gt "${MAX_VOLUME}" && NEWVOLUME=${MAX_VOLUME}
		        pactl set-sink-volume @DEFAULT_SINK@ "${NEWVOLUME}%" || exit 1
		        exit 0
		      fi
		      echo "invalid volume value" >&2
		      exit 1
		    fi
	    esac
	    ;;

	test)
		aplay "/usr/share/sounds/Mallet.wav" || exit 1
	;;
	*)
		do_usage $(basename $0)
esac
