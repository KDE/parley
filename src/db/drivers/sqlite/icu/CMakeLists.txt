add_library(parleydb_sqlite3_icu SHARED icu.c)

include_directories(${ICU_INCLUDE_DIRS})

target_link_libraries(parleydb_sqlite3_icu
                      ${SQLITE_LIBRARIES}
                      ${ICU_LIBRARIES}
                      ${ICU_I18N_LIBRARIES}
)
set_target_properties(parleydb_sqlite3_icu PROPERTIES PREFIX "" DEFINE_SYMBOL libsqlite3_EXPORTS)

install(TARGETS parleydb_sqlite3_icu DESTINATION ${PLUGIN_INSTALL_DIR})
