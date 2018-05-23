message("") # empty line
message("--   ********* cmdtools *********")
message("--   target cmdtools")

message("--   include path ${PROJECT_SOURCE_DIR}/common")
include_directories(${CHAIN_SRC_ROOT}/src ${PROJECT_SOURCE_DIR}/common)
link_directories( ${PROJECT_BINARY_DIR}/lib )

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(IMPORTLIB ${BITCOIN_LIBRARIES} ${THIRD_LIBRARIES}
	pthread  db_cxx-4.8 anl
)

#message("--      ${IMPORTLIB}")

file(GLOB TOOL_HEADERS "*.h" )
file(GLOB TOOL_SRC "*.cpp"  "*.cc")
add_executable(runcmd ${TOOL_SRC} ${TOOL_HEADERS} ${COMMON_SRC} ${COMMON_H})
target_link_libraries(runcmd ${IMPORTLIB})

message("--   ********* cmdtools end *********")
message("") # empty line 
