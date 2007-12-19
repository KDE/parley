include_directories( ${CMAKE_SOURCE_DIR}/libkdeedu/keduvocdocument/
)

set(parleypractice_SRCS
    main.cpp
    parleypracticemainwindow.cpp
)

kde4_add_executable(parleypractice ${parleypractice_SRCS})

target_link_libraries(
    parleypractice
    keduvocdocument
#     ${KDE4_KNEWSTUFF2_LIBS}
    ${KDE4_PHONON_LIBS}
    ${KDE4_KDEUI_LIBS}
)

install(TARGETS parleypractice  DESTINATION ${BIN_INSTALL_DIR} )
install( FILES images/card.svg  DESTINATION  ${DATA_INSTALL_DIR}/parley/images/ )

