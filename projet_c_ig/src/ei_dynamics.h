#ifndef EI_DYNAMICS_H
#define EI_DYNAMICS_H
#include "ei_types.h"
#include "ei_widget.h"
#include "ei_widgettypes.h"
#include "ei_params.h"
#include "ei_common.h"
#include "ei_utils.h"
#include "ei_gridder.h"
#include <stdlib.h>
#include <assert.h> 
void resize(ei_widget_t *widget, ei_size_t add_size);
void move(ei_widget_t *widget, ei_point_t where);


#endif
