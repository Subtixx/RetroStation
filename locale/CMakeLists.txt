add_custom_target (i18n ALL COMMENT "Building i18n messages.")
file (GLOB es_PO_FILES ${dir}/locale/lang/*)
set(es_POT ${dir}/locale/emulationstation2.pot)
# update the pot
MESSAGE("Building the pot file")
add_custom_command (TARGET i18n COMMAND find "${dir}/es-app" "${dir}/es-core" -name "*.cpp" -o -name "*.h" | ${XGETTEXT_EXECUTABLE} --add-comments=TRANSLATION -f - -o ${es_POT} --no-location --keyword=_)
add_custom_command (TARGET i18n COMMAND sed -i '/^\"POT-Creation-Date:/d' ${es_POT})

foreach (es_PO_INPUT ${es_PO_FILES})
        get_filename_component (es_PO_LANG ${es_PO_INPUT} NAME_WE)
	MESSAGE("LANG = ${es_PO_LANG}")
	set (es_MO_OUTPUT ${es_PO_INPUT}/LC_MESSAGES/emulationstation2.mo)
	# update the po from the pot
	add_custom_command (TARGET i18n COMMAND ${MSGMERGE_EXECUTABLE} -U --no-fuzzy-matching ${es_PO_INPUT}/LC_MESSAGES/emulationstation2.po ${es_POT})
	# compile the po to mo
        add_custom_command (TARGET i18n COMMAND basename ${es_PO_INPUT} && ${MSGFMT_EXECUTABLE} -o ${es_MO_OUTPUT} ${es_PO_INPUT}/LC_MESSAGES/emulationstation2.po)
        install (FILES ${es_MO_OUTPUT} DESTINATION share/locale/${es_PO_LANG}/LC_MESSAGES RENAME emulationstation2.mo)
endforeach (es_PO_INPUT ${es_PO_FILES})

add_custom_command (TARGET i18n COMMAND echo)
foreach (es_PO_INPUT ${es_PO_FILES})
  add_custom_command (TARGET i18n COMMAND printf "%-5s " `basename ${es_PO_INPUT}` && LANG=C ${MSGFMT_EXECUTABLE} -o /dev/null ${es_PO_INPUT}/LC_MESSAGES/emulationstation2.po --statistics)
endforeach (es_PO_INPUT ${es_PO_FILES})
add_custom_command (TARGET i18n COMMAND echo)
