# PluginCommon library - shared ABI implementation code
# for the CollabVM Plugin Interface

add_library(collabvm_plugincommon
		AbiImplementation.cpp
        )

target_link_libraries(collabvm_plugincommon collabvm::plugin_api)

set_property(TARGET collabvm_plugincommon
		PROPERTY POSITION_INDEPENDENT_CODE ON)

add_library(collabvm::plugin_common ALIAS collabvm_plugincommon)
