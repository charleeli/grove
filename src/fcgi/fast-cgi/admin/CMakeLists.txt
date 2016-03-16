include_directories("${PROJECT_SOURCE_DIR}/base")
include_directories("${PROJECT_SOURCE_DIR}/base/codec")
include_directories("${PROJECT_SOURCE_DIR}/base/libweb")

add_executable(AdminModule main.cc )
target_link_libraries(AdminModule libweb.a libglog.a libredismq.a libhiredis.a)
target_link_libraries(AdminModule libprotobuf.a libfcgi.a libredismq.a libhiredis.a)
target_link_libraries(AdminModule ${ZLIB_LIBRARIES} mysqlclient_r rt pthread)

INSTALL(TARGETS AdminModule	RUNTIME DESTINATION bin)
