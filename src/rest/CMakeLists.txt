cmake_minimum_required(VERSION 2.8.3)
project(RSI_PA2_REST)

SET(MHD_HINTS ${CMAKE_SOURCE_DIR}/../contribs)
SET(CURL_HINTS ${CMAKE_SOURCE_DIR}/../contribs) 
find_package(Libmicrohttpd REQUIRED)

find_package(CURL)
if(NOT ${CURL_FOUND})
SET(CURL_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/../contribs/")
SET(CURL_LIBRARIES "${CMAKE_SOURCE_DIR}/../contribs/lib/libcurl.so")
MESSAGE("-----------> ${CURL_LIBRARIES}")
endif()


include_directories( 
	${MHD_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}
	${CURL_INCLUDE_DIRS}

	
)

#add the source files to the client executable
#add_executable(REST_CLIENT ${SRC_CLIENT_FILES} )



#Add application files and gsoap dependencies
SET(SRC_SERVER_FILES
	rest_server.c
)


SET(SRC_CLIENT_FILES
	rest_client.c
	)
#add the source files to the client executable
add_executable(REST_SERVER ${SRC_SERVER_FILES} )

#extra libraries for the executable
target_link_libraries(REST_SERVER ${MHD_LIBRARIES} -lpthread -lsqlite3)

#add the source files to the client executable
add_executable(REST_CLIENT ${SRC_CLIENT_FILES} )

#extra libraries for the executable
target_link_libraries(REST_CLIENT ${CURL_LIBRARIES} -lpthread)





#documentation
if(${DOXYGEN_FOUND})


add_custom_target(DOXYGEN_DOCUMENTATION_TARGET_REST
	DEPENDS "${CMAKE_BINARY_DIR}/rest/generated-doc/html/index.html")
	
	
add_custom_command(
    OUTPUT "${CMAKE_BINARY_DIR}/rest/generated-doc/html/index.html"
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/../doc/Doxyfile.rest
    COMMENT "GENERATING DOC: ${CMAKE_BINARY_DIR}/rest/generated-doc/html/index.html"
)

add_dependencies(REST_SERVER DOXYGEN_DOCUMENTATION_TARGET_REST)


endif(${DOXYGEN_FOUND})

