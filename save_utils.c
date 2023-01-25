// Copyright Tudor Cristian-Andrei 311CAa 2022 - 2023
#include "save_utils.h"

// daca functia returneaza 0 se salveaza in binar
// iar daca returneaza 1 se salveaza in text
int how_to_save(char *filename)
{
	if (!strstr(filename, "ascii"))
		return 0;
	else
		return 1;
}

// functie care salveaza o imagine in format text
void save_text(char *filename, img image)
{
	FILE *picture = fopen(filename, "wt");
	if (!picture) {
		printf("Failed to save\n");
		return;
	}

	// scriu pe prima linie formatul de plain text
	if (image.format == 2 || image.format == 5)
		fprintf(picture, "P2\n"); // daca era grayscale
	else
		fprintf(picture, "P3\n"); // daca era color

	// scriu pe a doua linie intai width, dupa height
	fprintf(picture, "%d %d\n", image.width, image.height);

	// scriu pe a treia linie valoarea maxima
	fprintf(picture, "%d\n", image.maxvalue);

	// scriu valorile
	for (int i = 0; i < image.height; i++) {
		for (int j = 0; j < image.width; j++) {
			if (image.format == 2 || image.format == 5) {
				fprintf(picture, "%d ", image.gray[i][j]);
			} else {
				fprintf(picture, "%d ", image.pixel[i][j].red);
				fprintf(picture, "%d ", image.pixel[i][j].green);
				fprintf(picture, "%d ", image.pixel[i][j].blue);
			}
		}
		fprintf(picture, "\n");
	}
	fclose(picture);
}

// functie care salveaza o imagine in format binar
void save_binary(char *filename, img image)
{
	FILE *picture = fopen(filename, "wb");
	if (!picture) {
		printf("Failed to save\n");
		return;
	}

	// scriu pe prima linie in ascii formatul
	if (image.format == 2 || image.format == 5)
		fwrite("P5\n", 3, 1, picture);
	else
		fwrite("P6\n", 3, 1, picture);

	// scriu pe a doua linie in ascii width si height
	char ascii_number[11];
	sprintf(ascii_number, "%d %d\n", image.width, image.height);
	fwrite(ascii_number, strlen(ascii_number), 1, picture);

	// scriu pe a treia linie valoarea maxima
	sprintf(ascii_number, "%d\n", image.maxvalue);
	fwrite(ascii_number, strlen(ascii_number), 1, picture);

	// scriu culorile
	for (int i = 0; i < image.height; i++) {
		for (int j = 0; j < image.width; j++) {
			if (image.format == 2 || image.format == 5) {
				fwrite(&image.gray[i][j], 1, 1, picture);
			} else {
				fwrite(&image.pixel[i][j].red, 1, 1, picture);
				fwrite(&image.pixel[i][j].green, 1, 1, picture);
				fwrite(&image.pixel[i][j].blue, 1, 1, picture);
			}
		}
	}

	fclose(picture);
}

// functia apelata in main
void save(char *filename, img image)
{
	if (image.format == 0) {
		printf("No image loaded\n");
		return;
	}

	int mode = how_to_save(filename);
	// scot spatiile de dupa input daca exista
	for (int i = strlen(filename) - 1; i > 0; i--) {
		if (filename[i] == ' ' && filename[i - 1] != ' ') {
			filename[i] = '\0';
			break;
		}
	}

	// extrag numele fisierului
	if (mode == 1) {
		for (int i = strlen(filename) - 1; i > 0; i--) {
			if (filename[i] == ' ') {
				filename[i] = '\0';
				break;
			}
		}
	}

	if (mode == 0)
		save_binary(filename, image);
	else
		save_text(filename, image);

	printf("Saved %s\n", filename);
}
