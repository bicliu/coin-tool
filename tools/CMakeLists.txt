message("") # empty line
message("--   ********* Tools *********")
message("--   target Tools")

include_directories(${PROJECT_SOURCE_DIR}/common ${PROJECT_SOURCE_DIR}/wallet)
link_directories(${PROJECT_BINARY_DIR}/lib)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(IMPORTLIB ${BITCOIN_LIBRARIES} ${THIRD_LIBRARIES}
	pthread  db_cxx-4.8 anl miniupnpc
)

file(GLOB TOOL_HEADERS "*.h" )
file(GLOB TOOL_SRC "*.cpp"  "*.cc")
add_executable(tools ${TOOL_SRC} ${TOOL_HEADERS})
target_link_libraries(tools ${IMPORTLIB} tool_common tool_wallet)

message("--   ********* Tools end *********")
message("") # empty line 
