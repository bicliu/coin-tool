message("") # empty line
message("------------------------------- sendalert -------------------------------")
message("-- target sendalert")

message("-- include path ${PROJECT_SOURCE_DIR}/common")
include_directories( ${PROJECT_SOURCE_DIR}/common )
link_directories( ${PROJECT_BINARY_DIR}/lib )

#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(IMPORTLIB ${BITCOIN_LIBRARIES} ${THIRD_LIBRARIES} boost_system boost_filesystem boost_program_options boost_thread boost_log_setup boost_serialization
	pthread  db_cxx-4.8 #"${CHAIN_SRC_ROOT}/src/libbitcoin_server_a-alert.o" "${CHAIN_SRC_ROOT}/src/libbitcoin_server_a-net.o"
	m
)
foreach(loop_var IN LISTS ${IMPORTLIB}) 
message("--     ${loop_var}")
endforeach(loop_var)
message("--    ${IMPORTLIB}")

file(GLOB TOOL_HEADERS "*.h" )
file(GLOB TOOL_SRC "*.cpp"  "*.cc")
add_executable(sendalert ${TOOL_SRC} ${TOOL_HEADERS})
target_link_libraries(sendalert ${IMPORTLIB} tool_common)

message("-- sendalert end")
message("") # empty line 
