// Copyright Tudor Cristian-Andrei 311CAa 2022 - 2023
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "struct.h"
#include "utils.h"

void rotate_selection90negative(img * image, coordinates selection);

img entire90negative(img image);

void rotate_entire90negative(img *image, coordinates *selection, int *ok);

void rotate_entire180(img *image, coordinates *selection, int *ok);

void rotate_entire270(img *image, coordinates *selection, int *ok);

int angle(int value);

void do_rotate(img *image, coordinates *selection, int value);
