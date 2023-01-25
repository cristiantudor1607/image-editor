// Copyright Tudor Cristian-Andrei 311CAa 2022 - 2023
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "struct.h"
#include "utils.h"
#include "save_utils.h"
#include "load_utils.h"
#include "grayscale_utils.h"
#include "apply_utils.h"
#include "rotate_utils.h"
#include "select_and_crop_utils.h"

int do_query(char command[20])
{
	if (strcmp(command, "LOAD") == 0)
		return 1;
	if (strcmp(command, "SELECT") == 0)
		return 2;
	if (strcmp(command, "EQUALIZE") == 0)
		return 3;
	if (strcmp(command, "CROP") == 0)
		return 4;
	if (strcmp(command, "APPLY") == 0)
		return 5;
	if (strcmp(command, "HISTOGRAM") == 0)
		return 6;
	if (strcmp(command, "SAVE") == 0)
		return 10;
	if (strcmp(command, "EXIT") == 0)
		return 0;
	if (strcmp(command, "ROTATE") == 0)
		return 100;

	return -1;
}

int jump_over(char c, img image)
{
	if (c == '\n') {
		if (image.format == 0)
			printf("No image loaded\n");
		else
			printf("Invalid command\n");
		return 1;
	}

	return 0;
}

int main(void)
{
	char command[20], c;
	char *input;
	int angle;
	img image;
	coordinates selection;
	int count = 0;
	initialize_img(&image);
	initialize_coordinates(image, &selection);
	scanf("%s", command);
	while (do_query(command) != 0 || count == 0) {
		switch (do_query(command)) {
		case 1:
			getchar();
			read_string(&input);
			free_image(image);
			image = load_image(input);
			initialize_coordinates(image, &selection);
			if (image.format != 0)
				count++;
			free(input);
			break;
		case 2:
			getchar();
			read_string(&input);
			do_select(input, image, &selection);
			free(input);
			break;
		case 3:
			do_equalize(&image);
			break;
		case 4:
			do_crop(&image, &selection);
			break;
		case 5:
			c = getchar();
			if (jump_over(c, image) == 1)
				break;
			read_string(&input);
			apply(&image, selection, input);
			free(input);
			break;
		case 6:
			c = getchar();
			if (jump_over(c, image) == 1)
				break;
			read_string(&input);
			do_histogram(image, input);
			free(input);
			break;
		case 10:
			getchar();
			read_string(&input);
			save(input, image);
			free(input);
			break;
		case 100:
			scanf("%d", &angle);
			do_rotate(&image, &selection, angle);
			break;
		case 0:
			printf("No image loaded\n");
			break;
		default:
			read_string(&input);
			printf("Invalid command\n");
			free(input);
			break;
		}
		scanf("%s", command);
	}
	if (image.format == 0)
		printf("No image loaded\n");
	free_image(image);
	return 0;
}
