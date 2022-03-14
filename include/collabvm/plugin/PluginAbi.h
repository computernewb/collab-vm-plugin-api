//
// CollabVM Plugin Supplementary
//
// This file is licensed under the GNU Lesser General Public License Version 3
// for external usage in plugins. Text is provided in LICENSE-PLUGIN.
// If you cannot access it, please visit https://www.gnu.org/licenses/lgpl-3.0.html.
//
//
// Defines and shared support code for a server->plugin (or other way round!) ABI
// for CollabVM plugin interfaces (basically classes with just vtable members).
// Essentially "CollabVM COM", except without a baked in factory pattern or true-baseclass pattern.
//
// Some funsies:
//
// - CollabVM ABI objects are value-polymorphic, although it's not reccomended to use them like that
//	(in english: `abiObj.Method()` will always call the assigned vtfunc even when copying it)
//
// - CollabVM ABI objects have no vtable.
//	What would commonly be put in the vtable is instead laid out in the data layout of the class.
//	This is mostly a performance to object size tradeoff, since we only need to do one pointer fetch
//	instead of two to call a vtfunc.
//	Note that big objects with many vtfuncs may spill a cache line.

// TODO: This file should be installed with the collab-vm-server
// 	in e.g: $PREFIX/include/collabvm/
//
// 	EASTL should be installed in $PREFIX/include/collabvm/EASTL
// 	and EABase should be installed in $PREFIX/include/collabvm/EABase

#ifndef COLLAB_VM_SERVER_PLUGINABI_H
#define COLLAB_VM_SERVER_PLUGINABI_H

#include <cstdint>
//#include <utility> // std::forward
//#include "VtFunction.h"

// Make a detail name of a vtfunc.
// Do not use this to call a vtfunc,
// use the wrapper COLLABVM_PLUGINABI_DEFINE_VTFUNC adds to the interface.
#define _COLLABVM_PLUGINABI_NAME_VTFUNC(Name) \
	Name##__vtbl_pointer_do_not_use_by_hand

// Define a vtfunc, and a C++-side wrapper which calls
// the given vtfunc automatically.
// "args" is varadic and can be omitted.
//
// Unlike C++ native vtables, it is possible for a value to be polymorphic like this.
// Not that it's a good idea to pass by value vt objects, but you can do it I guess.
#define COLLABVM_PLUGINABI_DEFINE_VTFUNC(This, Retty, name, args...)                  \
	Retty (*_COLLABVM_PLUGINABI_NAME_VTFUNC(name))(This*, ##args) { nullptr };        \
	template<class... Args>                                                           \
	constexpr Retty name(Args&&... a) {                                               \
		return _COLLABVM_PLUGINABI_NAME_VTFUNC(name)(this, static_cast<Args&&>(a)...); \
	}

// Assign a vtfunc. Should be used to assign functions to an interface's vtfuncs
// in the constructor of an implementing class.
// NOTE: It is UB if you do not completely implement an interface.
#define COLLABVM_PLUGINABI_ASSIGN_VTFUNC(name, pointer) \
	_COLLABVM_PLUGINABI_NAME_VTFUNC(name) = collabvm::plugin::detail::_vtfunc_cast<decltype(_COLLABVM_PLUGINABI_NAME_VTFUNC(name))>(pointer)


// Expected ABI C exports for a CollabVM 3.0 server plugin:
// int collabvm_plugin_abi_version() - returns ABI version. If not matching the server's PluginAbi header, plugin is unloaded.

#ifdef _WIN32
	#define COLLABVM_PLUGINABI_EXPORT _declspec(dllexport)
#else
	#define COLLABVM_PLUGINABI_EXPORT __attribute__((visibility("default")))
#endif

namespace collabvm::plugin {

#ifdef __cpp_char8_t
	// Use C++20 char8_t to indicate we can use UTF-8.
	using utf8char = char8_t;
#else
	#error Please use a C++20 compiler.
#endif

	/**
	 * The version of the CollabVM Plugin ABI.
	 *
	 * Bump if you're going to make incompatible changes to the plugin ABI
	 * which will break existing plugins.
	 *
	 * Compatiable changes to the ABI do not warrant a bump. 
	 *
	 * History (if you're really curious:)
	 * 	v0 - Early working draft/prototype, no EASTL/other fun stuff.
	 * 	v1 - EASTL allowed, new types, VM plugins, ... (v3.0.0-pre to TBA)
	 *
	 */
	constexpr static auto PLUGIN_ABI_VERSION = 1;

	// TODO?: A root object for all plugin side objects?
	// don't wanna go full-NanoCOM though.

	namespace detail {
		/**
		 * Perform a UB cast for ABI vtable function pointers.
		 */
		template<class Dest, class Src>
		constexpr Dest _vtfunc_cast(Src src) {
			union {
				Src src;
				Dest dest;
			} cast_union { .src = src };

			return cast_union.dest;
		}
	} // namespace detail

} // namespace collabvm::plugin

#endif //COLLAB_VM_SERVER_PLUGINABI_H
