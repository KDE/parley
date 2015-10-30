project (scripts)

install(FILES myscript.py DESTINATION ${KDE_INSTALL_DATADIR}/parleyscripts/)

set(scripts_SRCS main.cpp scripts.cpp translationscript.cpp)

add_executable(scripts ${scripts_SRCS})

target_link_libraries(scripts
  PRIVATE
  KF5::KrossCore
  KF5::KrossUi
  Qt5::Core
  Qt5::Gui
)
