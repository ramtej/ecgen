/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_SEED_H
#define ECGEN_SEED_H

#include "io/cli.h"
#include "math/types.h"

/**
 *
 * @param seed
 */
void seed_free(seed_t **seed);

/**
 *
 * @param curve
 * @param config
 * @param ...
 * @return
 */
int seed_random(curve_t *curve, config_t *config, ...);

/**
 *
 * @param curve
 * @param config
 * @param ...
 * @return
 */
int seed_argument(curve_t *curve, config_t *config, ...);

/**
 *
 * @param curve
 * @param config
 * @param ...
 * @return
 */
int seed_input(curve_t *curve, config_t *config, ...);

#endif  // ECGEN_SEED_H