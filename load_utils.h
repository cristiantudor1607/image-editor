// Copyright Tudor Cristian-Andrei 311CAa 2022 - 2023
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "utils.h"

void skip_comments_txt(FILE * f);

void skip_comments_bin(FILE *f);

int determine_format(char *filename);

int get_value(FILE *f);

img load_bin(char *filename);

img load_txt(char *filename);

img load_image(char *filename);
