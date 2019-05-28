add_custom_target(compile-translation ALL DEPENDS)

set(LINGUAS ru uk)

set(DOMAIN messages)

set(POT_FILE ${CMAKE_CURRENT_LIST_DIR}/messages.pot)

set(XGETTEXT_KEYWORDS --keyword=pgtx:1c,2 --keyword=npgtxf:1c,2,3)
get_target_property(XGETTEXT_INPUT mazemaze SOURCES)

add_custom_command(TARGET compile-translation
                   COMMAND xgettext ${XGETTEXT_KEYWORDS} -C -o ${POT_FILE} ${XGETTEXT_INPUT}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../)

foreach(LANG ${LINGUAS})
    set(PO_FILE ${CMAKE_CURRENT_LIST_DIR}/${LANG}/LC_MESSAGES/${DOMAIN}.po)
    set(MO_FILE ${CMAKE_CURRENT_LIST_DIR}/${LANG}/LC_MESSAGES/${DOMAIN}.mo)

    add_custom_command(TARGET compile-translation
                       COMMAND msgmerge -q -v ${PO_FILE} ${POT_FILE} -o ${PO_FILE}
                       COMMAND msgfmt -v -c -o ${MO_FILE} ${PO_FILE})
endforeach(LANG)
