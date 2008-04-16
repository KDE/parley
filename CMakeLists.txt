project(parley)

macro_optional_find_package(Plasma)
macro_bool_to_01(PLASMA_FOUND HAVE_PLASMA)
macro_log_feature(PLASMA_FOUND "Plasma" "Plasma is the KDE4 desktop" "http://plasma.kde.org" FALSE "" "Provides a Parley Plasmoid")

add_subdirectory( practice )
add_subdirectory( src )
add_subdirectory( icons )
add_subdirectory( examples )
add_subdirectory( tipofday )

MESSAGE(STATUS "Parley plasmoid disabled until it will port to new API")

if(HAVE_PLASMA)
        #add_subdirectory(plasmoid)
endif(HAVE_PLASMA)


