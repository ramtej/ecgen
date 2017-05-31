/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "order.h"
#include "io/input.h"

GENERATOR(order_gen_input) {
	pari_sp ltop = avma;
	GEN ord = input_int("order", cfg->bits);
	if (gequalm1(ord)) {
		avma = ltop;
		return -4;
	} else {
		curve->order = ord;
		obj_insert_shallow(curve->curve, 1, ord);
		return 1;
	}
}

GENERATOR(order_gen_any) {
	GEN ord = ellff_get_card(curve->curve);
	if (isclone(ord)) {
		curve->order = gcopy(ord);
	} else {
		curve->order = ord;
	}
	return 1;
}

GENERATOR(order_gen_sea) {
	pari_sp ltop = avma;
	GEN order = ellsea(curve->curve, 0);
	if (gequal0(order)) {
		avma = ltop;
		return -4;
	} else {
		curve->order = order;
		obj_insert_shallow(curve->curve, 1, order);
		return 1;
	}
}

GENERATOR(order_gen_smallfact) {
	if (!args) {
		fprintf(stderr, "No args to an arged function. order_gen_smallfact\n");
		return INT_MIN;
	}

	pari_ulong smallfact = *(pari_ulong *)args->args;
	pari_sp ltop = avma;
	GEN fact = mpfact(smallfact);
	if (lgefint(fact) > 3) {
		smallfact = 0;
	} else {
		smallfact = itou(fact);
	}

	GEN order = ellsea(curve->curve, smallfact);
	if (gequal0(order) || gequal1(gcdii(order, fact))) {
		avma = ltop;
		return -4;
	} else {
		curve->order = order;
		obj_insert_shallow(curve->curve, 1, curve->order);
		return 1;
	}
}

GENERATOR(order_gen_prime) {
	pari_sp ltop = avma;
	GEN order = ellsea(curve->curve, 1);
	if (gequal0(order) || !(isprime(order))) {
		avma = ltop;
		return -4;
	} else {
		curve->order = order;
		obj_insert_shallow(curve->curve, 1, curve->order);
		return 1;
	}
}