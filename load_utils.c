// Copyright Tudor Cristian-Andrei 311CAa 2022 - 2023
#include "load_utils.h"

// functie care da skip la comentariile din fisierele text
void skip_comments_txt(FILE *f)
{
	// se citeste primul caracter de pe linie si verific daca este #
	char c = fgetc(f);
	while (c == '#') {
		// citesc intr-un buffer pana la sfarsitul liniei toate caracterele
		char falsein;
		do {
			falsein = fgetc(f);
		} while (falsein != '\n');
		// citesc iar primul caracter de pe linie
		c = fgetc(f);
	}

	// ma duc inapoi cu o pozitie, pentru ca ultimul caracter citit nu
	// a fost #, si este o linie valida, fara comentarii
	fseek(f, -1, SEEK_CUR);
}

// functie care da skip la comentariile din fisiere binare
void skip_comments_bin(FILE *f)
{
	// functioneaza la fel ca la text, doar ca se foloseste fread, in loc
	// de fgetc
	char c;
	fread(&c, 1, 1, f);
	while (c == '#') {
		char falsein;
		do {
			fread(&falsein, 1, 1, f);
		} while (falsein != '\n');
		fread(&c, 1, 1, f);
	}

	fseek(f, -1, SEEK_CUR);
}

// asociaza fiecarui format un numar, mai exact numarul de care este insotit P
// in cazul in care nu este incarcata nicio imagine sau nu s-a reusit
// incarcarea in memorie, formatul este 0
int determine_format(char *filename)
{
	char format[3];

	// se deschide fisierul, se citeste formatul si se inchide imediat
	FILE *picture = fopen(filename, "rb");
	if (!picture) {
		printf("Failed to load %s\n", filename);
		return 0;
	}

	fread(format, sizeof(char), 2, picture);
	format[2] = '\0';
	fclose(picture);

	if (strcmp(format, "P2") == 0)
		return 2;
	if (strcmp(format, "P3") == 0)
		return 3;
	if (strcmp(format, "P5") == 0)
		return 5;
	if (strcmp(format, "P6") == 0)
		return 6;

	return 0;
}

// transforma valorile scrise in ascii din fisierul binar in
// numere de tip int
int get_value(FILE *f)
{
	// int are maxim 10 cifre + '\0' = 11;
	char aux[11];
	char c;
	int size = 0;
	do {
		fread(&c, sizeof(char), 1, f);
		aux[size] = c;
		size++;
	} while (c != ' ' && c != '\n');
	aux[size - 1] = '\0';
	int result = atoi(aux);
	return result;
}

// functie care incarca o imagine binara
img load_bin(char *filename)
{
	img image_bin;
	image_bin.format = determine_format(filename);

	FILE *picture = fopen(filename, "rb");
	if (!picture) {
		// in caz de fail, mereu se seteaza valoarea lui format la 0, si se
		// returneaza imaginea asa, doar cu metadatele incarcate
		printf("Failed to load %s\n", filename);
		image_bin.format = 0;
		return image_bin;
	}

	// pozitia in fisier acum este 0, trec peste format care este citit si \n
	// dupa fiecare citire se cauta daca exista comentarii
	fseek(picture, 3, SEEK_CUR);
	skip_comments_bin(picture);
	image_bin.width = get_value(picture);
	image_bin.height = get_value(picture);
	skip_comments_bin(picture);
	image_bin.maxvalue = get_value(picture);
	skip_comments_bin(picture);

	if (image_bin.format == 5) {
		// aloc o matricea gray din structura si matricea pixel ramane
		// nefolosita
		image_bin.gray = gray_matrix(image_bin.height, image_bin.width);
		if (!image_bin.gray) {
			image_bin.format = 0;
			printf("Failed to load %s\n", filename);
			fclose(picture);
			return image_bin;
		}

	} else {
		// aloc o matrice de structuri-uri RGB pentru valorile pixelilor si
		// matricea gray ramane nefolosita
		image_bin.pixel = color_matrix(image_bin.height, image_bin.width);
		if (!image_bin.pixel) {
			image_bin.format = 0;
			printf("Failed to load %s\n", filename);
			fclose(picture);
			return image_bin;
		}
	}

	// se incarca pixelii in matrice
	for (int i = 0; i < image_bin.height; i++) {
		for (int j = 0; j < image_bin.width; j++) {
			if (image_bin.format == 5) {
				image_bin.gray[i][j] = 0;
				fread(&image_bin.gray[i][j], 1, 1, picture);
			} else {
				image_bin.pixel[i][j].red = 0;
				fread(&image_bin.pixel[i][j].red, 1, 1, picture);
				image_bin.pixel[i][j].green = 0;
				fread(&image_bin.pixel[i][j].green, 1, 1, picture);
				image_bin.pixel[i][j].blue = 0;
				fread(&image_bin.pixel[i][j].blue, 1, 1, picture);
			}
		}
	}

	printf("Loaded %s\n", filename);
	fclose(picture);
	return image_bin;
}

// functie care incarca o imagine text in memorie
// e asemanatoare celei binare, doar ca se foloseste fscanf
img load_txt(char *filename)
{
	img image_txt;
	image_txt.format = determine_format(filename);

	FILE *picture = fopen(filename, "rt");
	if (!picture) {
		printf("Failed to load %s\n", filename);
		image_txt.format = 0;
		return image_txt;
	}

	// trec peste 3 pozitii, pentru ca formatul este deja citit si apoi vine \n
	// nu mai trebuie sa folosesc functia get_value, cu fscanf e mai usor
	fseek(picture, 3, SEEK_CUR);
	skip_comments_txt(picture);
	fscanf(picture, "%d%d", &image_txt.width, &image_txt.height);
	fseek(picture, 1, SEEK_CUR);
	skip_comments_txt(picture);
	fscanf(picture, "%d", &image_txt.maxvalue);
	fseek(picture, 1, SEEK_CUR);
	skip_comments_txt(picture);

	if (image_txt.format == 2) {
		image_txt.gray = gray_matrix(image_txt.height, image_txt.width);
		if (!image_txt.gray) {
			image_txt.format = 0;
			printf("Failed to load %s\n", filename);
			fclose(picture);
			return image_txt;
		}

	} else {
		image_txt.pixel = color_matrix(image_txt.height, image_txt.width);
		if (!image_txt.pixel) {
			image_txt.format = 0;
			printf("Failed to load %s\n", filename);
			fclose(picture);
			return image_txt;
		}
	}

	for (int i = 0; i < image_txt.height; i++) {
		for (int j = 0; j < image_txt.width; j++) {
			if (image_txt.format == 2) {
				image_txt.gray[i][j] = 0;
				fscanf(picture, "%d", &image_txt.gray[i][j]);
			} else {
				image_txt.pixel[i][j].red = 0;
				fscanf(picture, "%d", &image_txt.pixel[i][j].red);
				image_txt.pixel[i][j].green = 0;
				fscanf(picture, "%d", &image_txt.pixel[i][j].green);
				image_txt.pixel[i][j].blue = 0;
				fscanf(picture, "%d", &image_txt.pixel[i][j].blue);
			}
		}
	}

	printf("Loaded %s\n", filename);
	fclose(picture);
	return image_txt;
}

// o functie de "switch", oarecum, prin care se decide ce
// functie trebuie apelata
img load_image(char *filename)
{
	img result;
	int format = determine_format(filename);
	if (format == 0)
		result.format = 0;
	else if (format == 5 || format == 6)
		result = load_bin(filename);
	else
		result = load_txt(filename);

	return result;
}
