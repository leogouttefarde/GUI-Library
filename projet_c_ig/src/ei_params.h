/**
 *  @file       ei_params.h
 *
 *  @brief      Defines the different geometry parameter structures, one for each
 *              geometry manager, depending on their needs.
 *
 *  \author
 *  Created by Antoine DELAITE, Eric BUREL, Léo GOUTTEFARDE on 13.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 *
 */

#ifndef EI_PARAMS_H
#define EI_PARAMS_H

#include "ei_geometrymanager.h"
#include "ei_types.h"


/**
 * @brief       Placer geometry manager parameters.
 */
typedef struct ei_placer_param_t {
        ei_geometry_param_t geom;
        ei_anchor_t* anc;
        int* x;
        int* y;
        float* rel_x;
        float* rel_y;
        int* w;
        int* h;
        float* rel_w;
        float* rel_h;
} ei_placer_param_t;


/**
 * @brief       Gridder geometry manager parameters.
 */
typedef struct ei_gridder_param_t {
        ei_geometry_param_t geom;

        /* Position */
        int* lin;
        int* col;

        /* Taille */
        int* w;
        int* h;

        /* Pour forcer la taille de la grille */
        int* force_w;
        int* force_h;

        /* Pour retenir la taille du rectangle elementaire */
        float elem_w;
        float elem_h;
} ei_gridder_param_t;


#endif
