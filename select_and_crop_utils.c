// Copyright Tudor Cristian-Andrei 311CAa 2022 - 2023
#include "select_and_crop_utils.h"

void do_select(char *input, img image, coordinates *selection)
{
	if (image.format == 0) {
		printf("No image loaded\n");
		return;
	}

	if (strstr(input, "ALL")) {
		selection->row_start = 0;
		selection->coll_start = 0;
		selection->row_stop = image.height;
		selection->coll_stop = image.width;
		printf("Selected ALL\n");
		return;
	}

	// daca inputul nu este "ALL", incerc sa extrag numere din el
	int coord[4];
	int idx = 0;
	char *p = strtok(input, " ");
	while (p) {
		for (unsigned long i = 0; i < strlen(p); i++) {
			// daca gasesc litere, comanda este invalida
			if (isalpha(p[i]) != 0) {
				printf("Invalid command\n");
				return;
			}
		}
		coord[idx] = atoi(p); // transform in int
		idx++;
		p = strtok(NULL, " ");
	}

	if (idx < 4) {
		printf("Invalid command\n");
		return;
	}

	for (int i = 0; i < 4; i++) {
		if (coord[i] < 0) {
			printf("Invalid set of coordinates\n");
			return;
		}
	}

	// le interschimb daca nu sunt ordonate
	if (coord[0] > coord[2]) {
		int aux = coord[0];
		coord[0] = coord[2];
		coord[2] = aux;
	}

	if (coord[1] > coord[3]) {
		int aux = coord[1];
		coord[1] = coord[3];
		coord[3] = aux;
	}

	// verific ca niciuna dinte coordonate sa iasa in afara imaginii
	if (coord[0] > image.width || coord[2] > image.width) {
		printf("Invalid set of coordinates\n");
		return;
	}

	if (coord[1] > image.height || coord[3] > image.height) {
		printf("Invalid set of coordinates\n");
		return;
	}

	if (coord[0] == coord[2] || coord[1] == coord[3]) {
		printf("Invalid set of coordinates\n");
		return;
	}

	selection->coll_start = coord[0];
	selection->row_start = coord[1];
	selection->coll_stop = coord[2];
	selection->row_stop = coord[3];

	printf("Selected %d %d %d %d\n", coord[0], coord[1], coord[2], coord[3]);
}

img crop(img image, coordinates selection)
{
	// aloc o imagine auxiliara, care va deveni noua imagine
	img result;
	// copiez "metadatele" despre poza in structura
	result.format = image.format;
	result.maxvalue = image.maxvalue;
	result.height = selection.row_stop - selection.row_start;
	result.width = selection.coll_stop - selection.coll_start;

	// aloc matricea de pixeli corespunzatoare
	if (result.format == 2 || result.format == 5) {
		result.gray = gray_matrix(result.height, result.width);
		if (!result.gray) {
			printf("Failed to crop\n");
			return image;
		}
	} else {
		result.pixel = color_matrix(result.height, result.width);
		if (!result.pixel) {
			printf("Failed to crop\n");
			return image;
		}
	}

	// cu 2 variabile pornesc de la 0 pentru a parcurge matricea auxiliara
	int idx_r = 0;
	int idx_c = 0;
	// cu alte 2 variabile parcurg selectia
	for (int i = selection.row_start; i < selection.row_stop; i++) {
		idx_c = 0;
		for (int j = selection.coll_start; j < selection.coll_stop; j++) {
			if (image.format == 2 || image.format == 5) {
				// copiez in matricea rezultat valoarea corespunzatoare
				result.gray[idx_r][idx_c] = image.gray[i][j];
			} else {
				result.pixel[idx_r][idx_c].red = image.pixel[i][j].red;
				result.pixel[idx_r][idx_c].green = image.pixel[i][j].green;
				result.pixel[idx_r][idx_c].blue = image.pixel[i][j].blue;
			}
			idx_c++;
		}
		idx_r++;
	}

	return result;
}

void do_crop(img *image, coordinates *selection)
{
	// verific daca este incarcata vreo imagine
	if (image->format == 0) {
		printf("No image loaded\n");
		return;
	}

	// copiez selectia in imaginea auxiliara, si fac astfel incat
	// imaginea mea sa indice catre adresa noii imagini
	img aux = crop(*image, *selection);
	free_image(*image);
	*image = aux;
	initialize_coordinates(*image, selection);
	printf("Image cropped\n");
}
