project(parley)

kde4_no_enable_final(parley)

macro_optional_find_package(LibXslt)
macro_optional_find_package(LibXml2)

macro_bool_to_01(LIBXSLT_FOUND AND LIBXML2_FOUND HAVE_LIBXSLT)

macro_log_feature(LIBXSLT_FOUND "LibXSLT" "A library to transform XMLfiles into other XML files" "http://xmlsoft.org/XSLT" FALSE "" "Required to build HTML export for Parley.")
macro_log_feature(LIBXML2_FOUND "LibXML2" "Libraries used to develop XML applications" "http://xmlsoft.org" FALSE "" "Required to build HTML export for Parley.")

# at the end, output the configuration
configure_file(
   ${CMAKE_CURRENT_SOURCE_DIR}/config-parley.h.cmake
   ${CMAKE_CURRENT_BINARY_DIR}/config-parley.h
)

add_subdirectory( scripts_test )
add_subdirectory( src )
add_subdirectory( icons )
add_subdirectory( plugins )
add_subdirectory( tipofday )

if(HAVE_LIBXSLT)
    add_subdirectory( xslt )
endif(HAVE_LIBXSLT)

add_subdirectory(plasmoid)


