// Copyright Tudor Cristian-Andrei 311CAa 2022 - 2023
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "struct.h"
#include "utils.h"

RGB convultion_sum(img image, const ker kernel, int i, int j);

void do_convultion(img *image, coordinates selection, const ker kernel);

int effect(char *kernel);

void apply(img *image, coordinates selection, char *kernel);
