message("") # empty line
message("--   ********* sendalert *********")
message("--   target sendalert")

message("--   include path ${PROJECT_SOURCE_DIR}/common")
include_directories( ${PROJECT_SOURCE_DIR}/common )
link_directories( ${PROJECT_BINARY_DIR}/lib )

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(IMPORTLIB ${BITCOIN_LIBRARIES} ${THIRD_LIBRARIES}
	pthread  db_cxx-4.8 anl miniupnpc
)

file(GLOB TOOL_HEADERS "*.h" )
file(GLOB TOOL_SRC "*.cpp"  "*.cc")
add_executable(sendalert ${TOOL_SRC} ${TOOL_HEADERS})
target_link_libraries(sendalert ${IMPORTLIB} tool_common)

message("--   ********* sendalert end *********")
message("") # empty line 
