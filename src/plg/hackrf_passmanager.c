#include "../../include/hakrf_passmanager.h"


/* Sample plugin code that registers a new pass.  */
int
plugin_init (struct plugin_name_args *plugin_info,
             struct plugin_gcc_version *version)
{


  if (!plugin_default_version_check (version, &gcc_version))
    return 1;

  struct register_pass_info pass_info;
  //...

  /* Code to fill in the pass_info object with new pass information.  */

  //...

  /* Register the new pass.  */
  register_callback (plugin_info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &pass_info);

 // ...
}

/* The prototype for a plugin callback function.
     gcc_data  - event-specific data provided by GCC
     user_data - plugin-specific data provided by the plug-in.  */
typedef void (*plugin_callback_func)(void *gcc_data, void *user_data);
