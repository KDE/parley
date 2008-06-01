project(parleypractice)

#include_directories( ../../libkdeedu/keduvocdocument/)

include_directories( ${CMAKE_SOURCE_DIR}/libkdeedu/keduvocdocument/)


find_package(KDE4 REQUIRED)

include_directories(${KDE4_INCLUDES})

#set(defaulttheme_SRCS
#    defaulttheme/vocabularycard.cpp
#)

set(parleypractice_SRCS
 #   ${defaulttheme_SRCS}
    main.cpp
    parleypracticemainwindow.cpp
    input.cpp
    prompt.cpp
    statistics.cpp
    stdbuttons.cpp
    hint.cpp
    answervalidator.cpp
    testentrymanager.cpp
    practiceprefs.cpp
    entryfilter.cpp
)

kde4_add_ui_files(parleypractice_SRCS entryfilter.ui )

kde4_add_kcfg_files(parleypractice_SRCS practiceprefs.kcfgc)

kde4_add_executable(parleypractice ${parleypractice_SRCS})

target_link_libraries(
    parleypractice
    keduvocdocument
#     ${KDE4_KNEWSTUFF2_LIBS}
    ${KDE4_PHONON_LIBS}
    ${KDE4_KDEUI_LIBS}
)

install(TARGETS parleypractice  DESTINATION ${BIN_INSTALL_DIR} )
install(FILES parleypracticeui.rc
        DESTINATION  ${DATA_INSTALL_DIR}/parleypractice/)
install( FILES defaulttheme/card.svg
               defaulttheme/layout.svg
    DESTINATION  ${DATA_INSTALL_DIR}/parley/defaulttheme/ )

