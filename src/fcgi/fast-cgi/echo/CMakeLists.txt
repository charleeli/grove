include_directories("${PROJECT_SOURCE_DIR}/base")
include_directories("${PROJECT_SOURCE_DIR}/base/codec")
include_directories("${PROJECT_SOURCE_DIR}/base/libweb")
include_directories("${PROJECT_SOURCE_DIR}/base/pbjson")

set(ECHO_PB_SOURCES 
    ./sdk-cpp/Echo.pb.cc
    ./sdk-cpp/Head.pb.cc
)
add_library(echoPB STATIC ${ECHO_PB_SOURCES})
target_link_libraries(echoPB libprotobuf.a)

add_executable(EchoModule main.cc )
target_link_libraries(EchoModule echoPB)
target_link_libraries(EchoModule libweb.a libglog.a libredismq.a libhiredis.a)
target_link_libraries(EchoModule libprotobuf.a libfcgi.a libredismq.a libhiredis.a)
target_link_libraries(EchoModule ${ZLIB_LIBRARIES} mysqlclient_r rt pthread)
set_target_properties(EchoModule PROPERTIES COMPILE_FLAGS "-std=c++11")

INSTALL(TARGETS EchoModule	RUNTIME DESTINATION bin)
