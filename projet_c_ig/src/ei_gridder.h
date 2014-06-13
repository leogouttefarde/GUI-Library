#ifndef EI_GRIDDER_H
#define EI_GRIDDER_H

#include "ei_geometrymanager.h"
#include "ei_utils.h"
#include <string.h>
#include <math.h>
#include "ei_common.h"
#include "ei_core.h"

/***** Gridder *****/

typedef struct ei_gridder_param_t {
        ei_geometry_param_t geom;
        int* lin;
        int* col;
        int* w;
        int* h;
        // Pour forcer la taille de la grille
        int* force_w;
        int* force_h;
        // Booleen pour definir les widgets a recalculer
        ei_bool_t seen;
} ei_gridder_param_t;

// Gestion des paramètres
void ei_grid(ei_widget_t *widget, int *col, int *lin, int *w, int *h, int
                *force_w, int *force_h);


// Runfunc du gridder
void ei_grid_runfunc(ei_widget_t *widget);


void ei_grid_releasefunc(struct ei_widget_t* widget);
// Register
/**
 * \brief       Registers the "gridder" geometry manager in the program. This must be called only
 *              once before the \ref ei_place function can be called.
 */
void  ei_register_gridder_manager();

#endif
