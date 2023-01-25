// Copyright Tudor Cristian-Andrei 311CAa 2022 - 2023
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "struct.h"

RGB * *color_matrix(int height, int width);

int **gray_matrix(int height, int width);

int clamp(int value, int maxvalue);

void read_string(char **string);

void free_image(img image);

void initialize_img(img *image);

void initialize_coordinates(img image, coordinates *selection);
