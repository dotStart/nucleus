# - Try to find EasyHook
# Once done this will define
#  EasyHook_FOUND - System has LibXml2
#  EasyHook_INCLUDE_DIRS - The LibXml2 include directories
#  EasyHook_LIBRARIES - The libraries needed to use LibXml2
#  EasyHook_DEFINITIONS - Compiler switches required for using LibXml2

find_path(EasyHook_INCLUDE_DIR easyhook.h
    HINTS ${EasyHook_INCLUDEDIR} ${EasyHook_INCLUDE_DIRS} ${EasyHook_ROOT})

find_library(EasyHook_LIBRARY NAMES easyhook32
    HINTS ${EasyHook_LIBDIR} ${EasyHook_LIBRARY_DIRS} ${EasyHook_ROOT})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set EasyHook_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(EasyHook DEFAULT_MSG
    EasyHook_LIBRARY EasyHook_INCLUDE_DIR)

mark_as_advanced(EasyHook_INCLUDE_DIR EasyHook_LIBRARY)

set(EasyHook_LIBRARIES ${EasyHook_LIBRARY})
set(EasyHook_INCLUDE_DIRS ${EasyHook_INCLUDE_DIR})
set(EasyHook_BINARIES ${EasyHook_INCLUDE_DIRS}/EasyHook32.dll ${EasyHook_INCLUDE_DIRS}/EasyLoad32.dll)
set(EasyHook_LICENSES ${EasyHook_INCLUDE_DIRS}/LICENSE)