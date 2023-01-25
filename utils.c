// Copyright Tudor Cristian-Andrei 311CAa 2022 - 2023
#include "utils.h"

// functie care aloca o matrice de structuri de tipul RGB
RGB **color_matrix(int height, int width)
{
	RGB **result;
	result = (RGB **)malloc(height * sizeof(RGB *));
	if (!result)
		return NULL;

	for (int i = 0; i < height; i++) {
		result[i] = (RGB *)malloc(width * sizeof(RGB));
		if (!result[i]) {
			for (int j = 0; j < i; j++)
				free(result[j]);
			free(result);
			return NULL;
		}
	}
	return result;
}

// functie care aloca o matrice de int-uri (pentru pozele gri)
int **gray_matrix(int height, int width)
{
	int **result;
	result = (int **)malloc(height * sizeof(int *));
	if (!result)
		return NULL;

	for (int i = 0; i < height; i++) {
		result[i] = (int *)malloc(width * sizeof(int));
		if (!result[i]) {
			for (int j = 0; j < i; j++)
				free(result[j]);
			free(result);
			return NULL;
		}
	}

	return result;
}

// functia clamp
int clamp(int value, int maxvalue)
{
	if (value > maxvalue)
		return maxvalue;
	if (value < 0)
		return 0;

	return value;
}

// functie care citeste si aloca dinamic un sir de caractere
void read_string(char **string)
{
	int size = 0;
	// plec de la o capacitate initiala
	int capacity = 32;
	*string = (char *)malloc(capacity * sizeof(char));
	char c;
	do {
		// citesc caracter cu caracter pana ajung la \n
		c = fgetc(stdin);
		// daca se depaseste capacitatea maresc sirul
		if (size >= capacity) {
			capacity += 32;
			*string = (char *)realloc(*string, capacity * sizeof(char));
		}
		// copiez in sir caracterul
		*(*string + size) = c;
		size++;
	} while (c != '\n');
	// realoc la dimensiunea care trebuie si elimin enter-ul
	*string = (char *)realloc(*string, size * sizeof(char));
	*(*string + size - 1) = '\0';
}

// functie care elibereaza o imagine, daca este cazul
void free_image(img image)
{
	// daca este 0 inseamna ca nu este incarcat nimic in memorie
	if (image.format == 0)
		return;

	for (int i = 0; i < image.height; i++) {
		if (image.format == 2 || image.format == 5)
			free(image.gray[i]);
		else
			free(image.pixel[i]);
	}

	if (image.format == 2 || image.format == 5)
		free(image.gray);
	else
		free(image.pixel);

	image.format = 0;
}

// functie care initializeaza imaginea
void initialize_img(img *image)
{
	// daca nu este incarcata nicio imagine toate sunt 0
	image->format = 0;
	image->height = 0;
	image->width = 0;
	image->maxvalue = 0;
	image->pixel = NULL;
	image->gray = NULL;
}

// functie care initializeaza selectia
void initialize_coordinates(img image, coordinates *selection)
{
	selection->row_start = 0;
	selection->coll_start = 0;
	if (image.format == 0) {
		selection->row_stop = 0;
		selection->coll_stop = 0;
		return;
	}

	selection->row_stop = image.height;
	selection->coll_stop = image.width;
}
