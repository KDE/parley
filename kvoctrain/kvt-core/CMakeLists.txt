

include_directories( ${CMAKE_SOURCE_DIR}/libkdeedu/kdeeducore/ )


########### next target ###############

set(kvoctraincore_LIB_SRCS 
   langset.cpp 
   LineList.cpp 
   UsageManager.cpp 
   QueryManager.cpp )

kde4_add_kcfg_files(kvoctraincore_LIB_SRCS
   ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/prefs.kcfgc
)

kde4_automoc(kvoctraincore ${kvoctraincore_LIB_SRCS})

kde4_add_library(kvoctraincore SHARED ${kvoctraincore_LIB_SRCS})

target_link_libraries(kvoctraincore  ${KDE4_KDECORE_LIBS} kdeeducore)

set_target_properties(kvoctraincore PROPERTIES VERSION 1.0.0 SOVERSION 1 )
install(TARGETS kvoctraincore  DESTINATION ${LIB_INSTALL_DIR} )

