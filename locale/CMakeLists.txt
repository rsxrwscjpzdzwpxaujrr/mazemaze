set(DOMAIN ${PROJECT_NAME})

find_package(Gettext MODULE)
find_program(XGETTEXT_EXECUTABLE xgettext)

if (Gettext_FOUND AND XGETTEXT_EXECUTABLE)
    set(LINGUAS ru uk)

    set(POT_FILE ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}.pot)

    set(XGETTEXT_KEYWORDS --keyword=pgtx:1c,2 --keyword=npgtxf:1c,2,3)
    get_target_property(XGETTEXT_INPUT mazemaze SOURCES)

    add_custom_target(translations ALL DEPENDS)
    add_custom_target(update-translation-templates)

    add_custom_command(TARGET update-translation-templates
                      COMMAND ${XGETTEXT_EXECUTABLE}
                              ${XGETTEXT_KEYWORDS} -C -o ${POT_FILE} ${XGETTEXT_INPUT}
                      WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../)

      foreach(LANG ${LINGUAS})
          set(PO_FILE ${CMAKE_CURRENT_LIST_DIR}/${LANG}/LC_MESSAGES/${DOMAIN}.po)
          set(MO_FILE ${CMAKE_CURRENT_LIST_DIR}/${LANG}/LC_MESSAGES/${DOMAIN}.mo)

          add_custom_command(TARGET update-translation-templates
                             COMMAND ${GETTEXT_MSGMERGE_EXECUTABLE}
                                     -q -v ${PO_FILE} ${POT_FILE} -o ${PO_FILE})

          add_custom_command(TARGET translations
                             COMMAND ${GETTEXT_MSGFMT_EXECUTABLE}
                                     -v -c -o ${MO_FILE} ${PO_FILE})
      endforeach(LANG)
else (Gettext_FOUND AND XGETTEXT_EXECUTABLE)
    message(WARNING "Gettext not found, translations can't be done")
endif (Gettext_FOUND AND XGETTEXT_EXECUTABLE)

install(DIRECTORY .
        DESTINATION ${PROJECT_NAME}/locale
        FILES_MATCHING PATTERN *.mo)
