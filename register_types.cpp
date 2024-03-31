/* register_types.cpp */

#include "register_types.h"
#include "icosphere.h"


void initialize_icooctree_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_CLASS(Icosphere);
}

void uninitialize_icooctree_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}
