#inclucde <gcc-plugin.h>
#include "plugin-version.h"


int plugin_is_GPL_compatible;

enum pass_positioning_ops
{
  PASS_POS_INSERT_AFTER,  // Insert after the reference pass.
  PASS_POS_INSERT_BEFORE, // Insert before the reference pass.
  PASS_POS_REPLACE        // Replace the reference pass.
};

struct register_pass_info
{
  struct opt_pass *pass;            /* New pass provided by the plugin.  */
  const char *reference_pass_name;  /* Name of the reference pass for hooking
                                       up the new pass.  */
  int ref_pass_instance_number;     /* Insert the pass at the specified
                                       instance number of the reference pass.  */
                                    /* Do it for every instance if it is 0.  */
  enum pass_positioning_ops pos_op; /* how to insert the new pass.  */
};

typedef void (*plugin_callback_func)(void *gcc_data, void *user_data)
PLUGIN_PASS_MANAGER_SETUP
