include_directories( ${PROJECT_SOURCE_DIR}/common )
link_directories( ${PROJECT_BINARY_DIR}/lib )

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(IMPORTLIB ${BITCOIN_LIBRARIES} ${THIRD_LIBRARIES})

file(GLOB TOOL_HEADERS "*.h" )
file(GLOB TOOL_SRC "*.cpp"  "*.cc")
add_executable(sendalert ${TOOL_SRC} ${TOOL_HEADERS})
target_link_libraries(sendalert ${IMPORTLIB})