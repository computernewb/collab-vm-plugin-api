add_library(helloworld SHARED main.cpp)

target_link_libraries(helloworld PRIVATE collabvm::plugin_common)

# default to hidden visibility
if(NOT WIN32)
	target_compile_options(helloworld PRIVATE "-fvisibility=hidden")
	target_link_libraries(helloworld PRIVATE "-fvisibility=hidden")
endif()

set_target_properties(helloworld PROPERTIES
		# CollabVM doesn't care about library prefix,
		# cause it's not a core libary
		PREFIX ""
#		LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
		)

