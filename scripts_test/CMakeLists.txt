project (scripts)
 
#find_package(KDE4 REQUIRED)
#include_directories( ${KDE4_INCLUDES} )

#install(FILES myscript.py DESTINATION ${DATA_INSTALL_DIR}/parleyscripts/)
 
set(scripts_SRCS main.cpp scripts.cpp translationscript.cpp)
 
kde4_add_executable(scripts ${scripts_SRCS})

target_link_libraries(scripts ${KDE4_KROSSCORE_LIBS} ${KDE4_KROSSUI_LIBS} ${QT_QTCORE_LIBRARY})
