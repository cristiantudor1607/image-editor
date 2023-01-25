// Copyright Tudor Cristian-Andrei 311CAa 2022 - 2023
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "struct.h"
#include "utils.h"

double *frequency(img image, int bins);

int *asterisk(img image, int max_asterisk, int bins);

void print_histogram(img image, int max, int bins);

double sum_from_zero_to_a(double *freq, int a);

int equalized_pixel(img image, double *freq, int a);

void do_histogram(img image, char *type);

void do_equalize(img *image);
