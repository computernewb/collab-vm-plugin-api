# This gets the same version as EASTL.
# Bump it on EASTL change.

project(eastl_support LANGUAGES CXX VERSION 3.18.00)

add_library(collabvm_eastl_support SHARED vsprintf.cpp)

target_link_libraries(collabvm_eastl_support PRIVATE EABase)

set_target_properties(collabvm_eastl_support PROPERTIES
	# SO version garbage
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}

	# Build as standard C++20
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)
