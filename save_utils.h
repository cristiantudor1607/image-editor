// Copyright Tudor Cristian-Andrei 311CAa 2022 - 2023
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "save_utils.h"

int how_to_save(char *filename);

void save_text(char *filename, img image);

void save_binary(char *filename, img image);

void save(char *filename, img image);
