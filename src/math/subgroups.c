/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "subgroups.h"

/**
 * @brief All prime divisors of a given integer.
 * @param order
 * @return
 */
static GEN subgroups_factors(GEN order) {
	GEN factors = Z_factor(order);
	return gel(factors, 1);
}

/**
 * @brief All prime divisors of a given integer with multiplicity.
 * @param order
 * @return
 */
static GEN __attribute__((unused)) subgroups_divisors(GEN order) {
	GEN factors = Z_factor(order);
	GEN primes = gel(factors, 1);
	GEN multiples = gel(factors, 2);
	long uniqs = glength(primes);

	long size = 0;
	for (long i = 1; i <= uniqs; ++i) {
		size += itos(gel(multiples, i));
	}
	GEN result = gtovec0(gen_0, size);

	long count = 0;
	for (long i = 1; i <= uniqs; ++i) {
		long multiple = itos(gel(multiples, i));
		for (long j = 1; j <= multiple; ++j) {
			gel(result, ++count) = gel(primes, i);
		}
	}
	return result;
}

GEN subgroups_prime(const curve_t *curve, const config_t *cfg) {
	if (cfg->prime || isprime(curve->order)) {
		return gtovec(curve->order);
	} else {
		return subgroups_factors(curve->order);
	}
}

static GEN subgroups_2n(GEN factors, size_t min_bits) {
	long nprimes = glength(factors);
	if (nprimes == min_bits) return NULL;
	GEN amount = int2n(nprimes);
	GEN groups = gtovec0(gen_0, itos(amount) - (min_bits * nprimes) - 1);

	size_t i = 0;
	for (size_t count = 1; count < (size_t)(1 << nprimes); ++count) {
		pari_sp btop = avma;
		GEN result = gen_1;
		size_t bits = 0;
		for (long bit = 0; bit < nprimes; ++bit) {
			size_t mask = (size_t)(1 << bit);
			if (count & mask) {
				result = mulii(result, gel(factors, bit + 1));
				bits++;
			}
		}
		if (bits > min_bits) {
			gel(groups, ++i) = result;
		} else {
			avma = btop;
		}
	}
	return groups;
}

GEN subgroups_nonprime(const curve_t *curve, const config_t *cfg) {
	if (cfg->prime || isprime(curve->order)) {
		return NULL;
	} else {
		if (curve->ngens == 1) {
			GEN factors = subgroups_factors(curve->order);
			return subgroups_2n(factors, 1);
		} else {
			GEN one_factors = subgroups_factors(curve->generators[0]->order);
			GEN one = subgroups_2n(one_factors, 1);
			GEN other_factors = subgroups_factors(curve->generators[1]->order);
			GEN other = subgroups_2n(other_factors, 1);
			if (!one) {
				return other;
			}
			if (!other) {
				return one;
			}
			GEN result = gtovec0(gen_0, glength(one) + glength(other));
			for (long i = 1; i <= glength(result); ++i) {
				if (i <= glength(one)) {
					gel(result, i) = gel(one, i);
				} else {
					gel(result, i) = gel(other, i - glength(one));
				}
			}
			return result;
		}
	}
}

GEN subgroups_all(const curve_t *curve, const config_t *cfg) {
	if (cfg->prime || isprime(curve->order)) {
		return gtovec(curve->order);
	} else {
		GEN factors = subgroups_factors(curve->order);
		return subgroups_2n(factors, 0);
	}
}
