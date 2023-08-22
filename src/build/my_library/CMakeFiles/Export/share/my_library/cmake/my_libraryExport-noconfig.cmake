#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "my_library::my_library" for configuration ""
set_property(TARGET my_library::my_library APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(my_library::my_library PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libmy_library.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS my_library::my_library )
list(APPEND _IMPORT_CHECK_FILES_FOR_my_library::my_library "${_IMPORT_PREFIX}/lib/libmy_library.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
