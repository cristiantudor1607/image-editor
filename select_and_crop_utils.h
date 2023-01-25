// Copyright Tudor Cristian-Andrei 311CAa 2022 - 2023
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "struct.h"
#include "utils.h"

void do_select(char *input, img image, coordinates * selection);

img crop(img image, coordinates selection);

void do_crop(img *image, coordinates *selection);
