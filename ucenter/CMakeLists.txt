message("") # empty line
message("--   ********* ucenter *********")
message("--   target ucenter")

if(NOT MYSQL)
	message(FATAL_ERROR "MYSQL not defined! (-DMYSQL=1)")
endif()

message("--   include path ${PROJECT_SOURCE_DIR}/common")
include_directories( ${PROJECT_SOURCE_DIR}/common ${MYSQL_INCLUDE})
link_directories( ${PROJECT_BINARY_DIR}/lib )

#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(IMPORTLIB ${BITCOIN_LIBRARIES} ${THIRD_LIBRARIES} ${MYSQL_LIB}
	pthread  db_cxx-4.8 anl miniupnpc
)

file(GLOB TOOL_HEADERS "*.h" )
file(GLOB TOOL_SRC "*.cpp"  "*.cc")
add_executable(ucenter ${TOOL_SRC} ${TOOL_HEADERS})
target_link_libraries(ucenter ${IMPORTLIB} tool_common)

message("--   ********* ucenter end *********")
message("") # empty line 
