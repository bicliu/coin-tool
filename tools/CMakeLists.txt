message("") # empty line
message("--   ********* Tools *********")
message("--   target Tools")

message("--   include path ${PROJECT_SOURCE_DIR}/common")
include_directories(${PROJECT_SOURCE_DIR}/common)
message("--   include path ${PROJECT_SOURCE_DIR}/wallet")
include_directories(${PROJECT_SOURCE_DIR}/wallet)
link_directories(${PROJECT_BINARY_DIR}/lib)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(IMPORTLIB ${BITCOIN_LIBRARIES} ${THIRD_LIBRARIES}
	pthread  db_cxx-4.8 anl miniupnpc
	tool_common tool_wallet
)

file(GLOB TOOL_HEADERS "*.h" )
file(GLOB TOOL_SRC "*.cpp"  "*.cc")
add_executable(tools ${TOOL_SRC} ${TOOL_HEADERS})
target_link_libraries(tools ${IMPORTLIB})

message("--   ********* Tools end *********")
message("") # empty line 
