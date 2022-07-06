
add_definitions(-Wno-error=missing-field-initializers)
add_definitions(-Wno-missing-field-initializers)
SET(FILE_KEY_MANAGEMENT_PLUGIN_SOURCES file_key_management_plugin.cc parser.cc redis.cpp base64.cpp)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/sql)

MYSQL_ADD_PLUGIN(FILE_KEY_MANAGEMENT ${FILE_KEY_MANAGEMENT_PLUGIN_SOURCES}  LINK_LIBRARIES  nvx_socket nvx_socket_ssl shell_cmd)
