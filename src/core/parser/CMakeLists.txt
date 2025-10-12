add_subdirectory(parsed_data)
add_subdirectory(tableref)

set(EXTENSION_SOURCES
    ${EXTENSION_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/duckpgq_parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/path_element.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/path_pattern.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/path_reference.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/property_graph_table.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/subpath_element.cpp
    PARENT_SCOPE)
