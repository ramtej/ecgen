/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_GENERATORS_H
#define ECGEN_GENERATORS_H

#include "math/types.h"

/**
 *
 * @param curve
 * @param config
 * @param generators
 * @param start_offset
 * @param end_offset
 * @return
 */
int exhaustive_gen(curve_t *curve, config_t *config, gen_t generators[],
                   int start_offset, int end_offset);

/**
 *
 * @param cfg
 * @return
 */
int exhaustive_do(config_t *cfg);

#endif  // ECGEN_GENERATORS_H
