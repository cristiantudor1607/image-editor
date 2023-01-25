// Copyright Tudor Cristian-Andrei 311CAa 2022 - 2023
#include "apply_utils.h"

// disclaimer : imi cer scuze pentru numele gresit, trebuia sa fie convolution,
// dar am pus deja peste tot functia cu numele asta
// aplic formula pentru pixelul [i][j]
RGB convultion_sum(img image, const ker kernel, int i, int j)
{
	RGB new_value;
	double sum = 0;
	// fac inmultirile si adun tot la suma
	sum = sum + kernel.template[1][1] * (double)image.pixel[i][j].red;
	sum = sum + kernel.template[0][0] * (double)image.pixel[i - 1][j - 1].red;
	sum = sum + kernel.template[0][1] * (double)image.pixel[i - 1][j].red;
	sum = sum + kernel.template[0][2] * (double)image.pixel[i - 1][j + 1].red;
	sum = sum + kernel.template[1][0] * (double)image.pixel[i][j - 1].red;
	sum = sum + kernel.template[1][2] * (double)image.pixel[i][j + 1].red;
	sum = sum + kernel.template[2][0] * (double)image.pixel[i + 1][j - 1].red;
	sum = sum + kernel.template[2][1] * (double)image.pixel[i + 1][j].red;
	sum = sum + kernel.template[2][2] * (double)image.pixel[i + 1][j + 1].red;

	// la final impart la divideby, care o sa fie 1, 9 sau 16
	sum  = sum / kernel.divideby;
	// aplic round, si dupa clamp
	new_value.red = clamp(round(sum), image.maxvalue);

	sum = 0;
	sum = sum + kernel.template[1][1] * (double)image.pixel[i][j].green;
	sum = sum + kernel.template[0][0] * (double)image.pixel[i - 1][j - 1].green;
	sum = sum + kernel.template[0][1] * (double)image.pixel[i - 1][j].green;
	sum = sum + kernel.template[0][2] * (double)image.pixel[i - 1][j + 1].green;
	sum = sum + kernel.template[1][0] * (double)image.pixel[i][j - 1].green;
	sum = sum + kernel.template[1][2] * (double)image.pixel[i][j + 1].green;
	sum = sum + kernel.template[2][0] * (double)image.pixel[i + 1][j - 1].green;
	sum = sum + kernel.template[2][1] * (double)image.pixel[i + 1][j].green;
	sum = sum + kernel.template[2][2] * (double)image.pixel[i + 1][j + 1].green;

	sum = sum / kernel.divideby;
	new_value.green = clamp(round(sum), image.maxvalue);

	sum = 0;
	sum = sum + kernel.template[1][1] * (double)image.pixel[i][j].blue;
	sum = sum + kernel.template[0][0] * (double)image.pixel[i - 1][j - 1].blue;
	sum = sum + kernel.template[0][1] * (double)image.pixel[i - 1][j].blue;
	sum = sum + kernel.template[0][2] * (double)image.pixel[i - 1][j + 1].blue;
	sum = sum + kernel.template[1][0] * (double)image.pixel[i][j - 1].blue;
	sum = sum + kernel.template[1][2] * (double)image.pixel[i][j + 1].blue;
	sum = sum + kernel.template[2][0] * (double)image.pixel[i + 1][j - 1].blue;
	sum = sum + kernel.template[2][1] * (double)image.pixel[i + 1][j].blue;
	sum = sum + kernel.template[2][2] * (double)image.pixel[i + 1][j + 1].blue;

	sum = sum / kernel.divideby;
	new_value.blue = clamp(round(sum), image.maxvalue);

	return new_value;
}

void do_convultion(img *image, coordinates selection, const ker kernel)
{
	// o matrice auxiliara de tipul RGB
	RGB **aux;
	int height = selection.row_stop - selection.row_start;
	int width = selection.coll_stop - selection.coll_start;

	aux = color_matrix(height, width);
	if (!aux) {
		printf("Failed to apply\n");
		return;
	}

	// cu 2 variabile plec de la 0, pentru a parcurge imaginea auxiliara
	int idx_r = 0;
	int idx_c = 0;
	// in for plec cu alte 2 variabile pentru a parcurge selectia
	for (int i = selection.row_start; i < selection.row_stop; i++) {
		idx_c = 0;
		for (int j = selection.coll_start; j < selection.coll_stop; j++) {
			if (i == 0 || j == 0) {
				// daca e prima sau ultima coloana, las pixeli in pace
				aux[idx_r][idx_c] = image->pixel[i][j];
			} else if (i == image->height - 1 || j == image->width - 1) {
				// daca este ultima sau prima coloana, las pixeli in pace
				aux[idx_r][idx_c] = image->pixel[i][j];
			} else {
				// pentru restul de pixeli, aplic convolution_sum, pentru a
				// calcula noua valoare a pixelilor
				aux[idx_r][idx_c] = convultion_sum(*image, kernel, i, j);
			}

			idx_c++;
		}
		idx_r++;
	}

	// copiez inapoi in selectie noii pixeli
	idx_r = 0;
	for (int i = selection.row_start; i < selection.row_stop; i++) {
		idx_c = 0;
		for (int j = selection.coll_start; j < selection.coll_stop; j++) {
			image->pixel[i][j] = aux[idx_r][idx_c];
			idx_c++;
		}
		idx_r++;
	}

	// eliberez imaginea auxiliara
	for (int i = 0; i < height; i++)
		free(aux[i]);
	free(aux);
}

// o functie care codifica efectele pentru apply
int effect(char *kernel)
{
	if (strcmp(kernel, "EDGE") == 0)
		return 1;
	if (strcmp(kernel, "SHARPEN") == 0)
		return 2;
	if (strcmp(kernel, "BLUR") == 0)
		return 3;
	if (strcmp(kernel, "GAUSSIAN_BLUR") == 0)
		return 4;

	return 0;
}

// functia apelata in main
void apply(img *image, coordinates selection, char *kernel)
{
	// declar cele 4 kerneluri, ca si const, pentru a nu le modifica din
	// greseala in program
	const ker edge = { { { -1, -1, -1 }, { -1, 8, -1 }, {-1, -1, -1 } }, 1.0};
	const ker sharpen = { { { 0, -1, 0 }, { -1, 5, -1 }, { 0, -1, 0 } }, 1.0};
	const ker blur = { { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 } }, 9.0};
	const ker gauss = { { { 1, 2, 1 }, { 2, 4, 2 }, { 1, 2, 1 } }, 16.0};

	// verific daca am o imagine incarcata
	if (image->format == 0) {
		printf("No image loaded\n");
		return;
	}

	// verific daca efectul este unul valid
	if (effect(kernel) == 0) {
		printf("APPLY parameter invalid\n");
		return;
	}

	// verific daca imaginea este color
	if (image->format == 2 || image->format == 5) {
		printf("Easy, Charlie Chaplin\n");
		return;
	}

	// aplic unul dintre efecte
	switch (effect(kernel)) {
	case 1:
		do_convultion(image, selection, edge);
		break;
	case 2:
		do_convultion(image, selection, sharpen);
		break;
	case 3:
		do_convultion(image, selection, blur);
		break;
	case 4:
		do_convultion(image, selection, gauss);
		break;
	}

	printf("APPLY %s done\n", kernel);
}
