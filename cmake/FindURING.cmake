find_path(URING_INCLUDE_DIRS NAMES liburing.h)
find_library(URING_LIBRARIES NAMES uring)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    URING DEFAULT_MSG
    URING_LIBRARIES URING_INCLUDE_DIRS)

mark_as_advanced(URING_INCLUDE_DIRS URING_LIBRARIES)
