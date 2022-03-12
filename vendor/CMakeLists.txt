# Vendor cmakelists.

# We don't use the the EA* cmakelists because they assume
# builds are static and they're kinda polluteyy we don't want that (wherever possible, at least)
# so naaahhh.
#
# These will also setup to install into /usr/lib/collabvm/libcollabvm_EA*.so (+sover? pls help)
# IF they provide SO outputs. If it's a interface lib like EABase then
# it's just a interface target.
# That's just to make package maintainer's lives less of a pain though,
# since our EA* libs are special. Not that it matters, since
# no one actually ships EA* libs in any distro (Arch's AUR is the closest I've seen), but...... you never kno!w

# EABase seems to be alright. For installation, we might wanna end up
# modifying it to not do a global add_definitions(), and change it to use
# GNUInstallDirs.
add_subdirectory(EABase)

# EASTL support lib
add_subdirectory(eastl_support)

# Finally eastl itself
add_subdirectory(EASTL)