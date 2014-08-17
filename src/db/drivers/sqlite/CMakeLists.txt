

include_directories( ${SQLITE_INCLUDE_DIR} ${KDE4_INCLUDES} )

ADD_DEFINITIONS( -D__KEXIDB__= )

########### next target ###############

set(parleydb_sqlite3driver_PART_SRCS 
   sqliteconnection.cpp 
   sqlitedriver.cpp 
   sqlitecursor.cpp 
   sqlitekeywords.cpp 
   sqlitepreparedstatement.cpp
   sqliteadmin.cpp
   sqlitevacuum.cpp
   sqlitealter.cpp
)

kde4_add_plugin(parleydb_sqlite3driver ${parleydb_sqlite3driver_PART_SRCS})

target_link_libraries(parleydb_sqlite3driver ${SQLITE_LIBRARIES} parleycalligradb)

install(TARGETS parleydb_sqlite3driver  DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install( FILES  parleydb_sqlite3driver.desktop  DESTINATION ${SERVICES_INSTALL_DIR}/parley)

add_subdirectory(icu)
