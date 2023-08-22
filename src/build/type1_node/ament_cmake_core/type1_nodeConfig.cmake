# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_type1_node_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED type1_node_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(type1_node_FOUND FALSE)
  elseif(NOT type1_node_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(type1_node_FOUND FALSE)
  endif()
  return()
endif()
set(_type1_node_CONFIG_INCLUDED TRUE)

# output package information
if(NOT type1_node_FIND_QUIETLY)
  message(STATUS "Found type1_node: 0.0.0 (${type1_node_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'type1_node' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${type1_node_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(type1_node_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${type1_node_DIR}/${_extra}")
endforeach()
