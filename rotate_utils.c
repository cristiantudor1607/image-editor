// Copyright Tudor Cristian-Andrei 311CAa 2022 - 2023
#include "rotate_utils.h"

// ROTATE
// rotatia cu +90 este acceasi cu rotatia -270
// rotatia cu -90 este acceasi cu rotatia +270
// +360 si -360 nu fac nimic (la fel si 0)
// -180 si +180 este la fel

// functie care roteste o selectie cu -90 de grade
void rotate_selection90negative(img *image, coordinates selection)
{
	// am nvoie de aceste 2 informatii, deoarece, trebuie sa plec si cu i
	// si cu j de la 0, pentru a putea roti
	int height = selection.row_stop - selection.row_start;
	int width = selection.coll_stop - selection.coll_start;
	// ma gandesc ca as face transpusa unei matrici, si pentru a respecta
	// regulile trebuie sa plec de la 0, altfel ar fi existat posibilitatea
	// ca unii pixeli sa nu se roteasca (ex daca plecam de la pixelul 200 500,
	// multi pixeli nu s-ar fi rotit, deoarece 500 < 200 fals, 500 < 201 fals,
	// etc)
	// spoiler alert : mi-a luat ceva sa descopar asta si generam imagini misto
	// dar fara niciun sens
	for (int i = 0; i < height; i++) {
		int idx_r = selection.row_start + i;
		for (int j = 0; j < width; j++) {
			int idx_c = selection.coll_start + j;
			if (i < j) {
				int ch_row = selection.row_start + j;
				int ch_coll = selection.coll_start + i;
				if (image->format == 2 || image->format == 5) {
					int auxgray = image->gray[idx_r][idx_c];
					image->gray[idx_r][idx_c] = image->gray[ch_row][ch_coll];
					image->gray[ch_row][ch_coll] = auxgray;
				} else {
					RGB auxcolor = image->pixel[idx_r][idx_c];
					image->pixel[idx_r][idx_c] = image->pixel[ch_row][ch_coll];
					image->pixel[ch_row][ch_coll] = auxcolor;
				}
			}
		}
	}

	// interschimb liniile cu cele opuse ca sa elimin efectul de oglinda
	// la fel ca la primul pas, trebuie sa iau height separat, pentru a
	// determina numarul de pasi
	// spoiler alert : si aici greseam
	int row = selection.row_start;
	for (int i = 0; i < height / 2; i++) {
		int opposite = selection.row_stop - i - 1;
		for (int j = selection.coll_start; j < selection.coll_stop; j++) {
			if (image->format == 2 || image->format == 5) {
				int auxgray = image->gray[row][j];
				image->gray[row][j] = image->gray[opposite][j];
				image->gray[opposite][j] = auxgray;
			} else {
				RGB auxcolor = image->pixel[row][j];
				image->pixel[row][j] = image->pixel[opposite][j];
				image->pixel[opposite][j] = auxcolor;
			}
		}
		row++;
	}
}

// functie care roteste toata imaginea cu -90
img entire90negative(img image)
{
	// de data asta aloc o structura noua, care va deveni imaginea
	img rotated;
	rotated.format = image.format;
	rotated.height = image.width;
	rotated.width = image.height;
	rotated.maxvalue = image.maxvalue;

	if (rotated.format == 2 || rotated.format == 5) {
		rotated.gray = gray_matrix(rotated.height, rotated.width);
		if (!rotated.gray) {
			rotated.format = 0;
			return rotated;
		}
	} else {
		rotated.pixel = color_matrix(rotated.height, rotated.width);
		if (!rotated.pixel) {
			rotated.format = 0;
			return rotated;
		}
	}

	// fac transpusa
	for (int i = 0; i < rotated.height; i++) {
		for (int j = 0; j < rotated.width; j++) {
			if (rotated.format == 2 || rotated.format == 5)
				rotated.gray[i][j] = image.gray[j][i];
			else
				rotated.pixel[i][j] = image.pixel[j][i];
		}
	}

	// interschimb liniile
	for (int i = 0; i < rotated.height / 2; i++) {
		int opposite = rotated.height - 1 - i;
		for (int j = 0; j < rotated.width; j++) {
			if (rotated.format == 2 || rotated.format == 5) {
				int auxgray = rotated.gray[i][j];
				rotated.gray[i][j] = rotated.gray[opposite][j];
				rotated.gray[opposite][j] = auxgray;
			} else {
				RGB auxcolor = rotated.pixel[i][j];
				rotated.pixel[i][j] = rotated.pixel[opposite][j];
				rotated.pixel[opposite][j] = auxcolor;
			}
		}
	}

	return rotated;
}

// pentru ca functia de rotire a intregii imagini intoarce o structura
// am facut si o functie care inlocuieste in main imaginea veche cu cea noua
void rotate_entire90negative(img *image, coordinates *selection, int *ok)
{
	img aux = entire90negative(*image);
	// daca alocarea nu s-a reusit
	if (aux.format == 0) {
		printf("Failed to rotate\n");
		// un contor prin care stiu daca trebuie sa printez mesajul la final
		*ok = -1;
		return;
	}

	free_image(*image);
	*image = aux;
	initialize_coordinates(*image, selection);
}

void rotate_entire180(img *image, coordinates *selection, int *ok)
{
	// in cazul primei rotiri daca nu se reuseste nu se modifica nimic
	// dar in cazul celei de-a 2-a, daca nu se reuseste vrem ca imaginea
	// sa fie cea initiala, nu rotita o data la stanga
	img backup = *image;

	// prima rotire
	img aux = entire90negative(*image);
	if (aux.format == 0) {
		printf("Failed to rotate\n");
		*ok = -1;
		return;
	}

	free_image(*image);
	*image = aux;
	initialize_coordinates(*image, selection);

	// a doua rotire
	img secondaux = entire90negative(*image);
	if (secondaux.format == 0) {
		printf("Failed to rotate\n");
		*ok = -1;
		free_image(*image);
		*image = backup;
		initialize_coordinates(*image, selection);
		return;
	}

	free_image(*image);
	*image = secondaux;
	initialize_coordinates(*image, selection);
}

void rotate_entire270(img *image, coordinates *selection, int *ok)
{
	// acelasi principiu ca la 180
	img backup = *image;

	// prima rotire
	img aux = entire90negative(*image);
	if (aux.format == 0) {
		printf("Failed to rotate\n");
		*ok = -1;
		return;
	}

	free_image(*image);
	*image = aux;
	initialize_coordinates(*image, selection);

	// a doua rotire
	img secondaux = entire90negative(*image);
	if (secondaux.format == 0) {
		printf("Failed to rotate\n");
		*ok = -1;
		free_image(*image);
		*image = backup;
		initialize_coordinates(*image, selection);
		return;
	}

	free_image(*image);
	*image = secondaux;
	initialize_coordinates(*image, selection);

	// a treia rotire
	img thirdaux = entire90negative(*image);
	if (thirdaux.format == 0) {
		printf("Failed to rotate\n");
		*ok = -1;
		free_image(*image);
		*image = backup;
		initialize_coordinates(*image, selection);
		return;
	}

	free_image(*image);
	*image = thirdaux;
	initialize_coordinates(*image, selection);
}

// functie care codifica unghiurile, dar imi spune si de cate ori
// ar trebui sa rotesc o selectie pentru fiecare unghi
int angle(int value)
{
	if (value == -90 || value == 270)
		// rotesc o data selectia
		return 1;
	if (value == -180 || value == 180)
		// rotesc de 2 ori selectia
		return 2;
	if (value == -270 || value == 90)
		// rotesc de 3 ori selectia
		return 3;
	if (value == 360 || value == -360 || value == 0)
		// nu o rotesc deloc
		return 0;

	return -1;
}

// functia apelata in main
void do_rotate(img *image, coordinates *selection, int value)
{
	int print = 0;

	if (image->format == 0) {
		printf("No image loaded\n");
		return;
	}

	int height = selection->row_stop - selection->row_start;
	int width = selection->coll_stop - selection->coll_start;

	if (height != image->height || width != image->width) {
		if (height != width) {
			printf("The selection must be square\n");
			return;
		}
	}

	// se face un switch la rotirea care trebuie aplicata
	switch (angle(value)) {
	case 0:
		break;
	case 1:
		if (height != image->height || width != image->width)
			rotate_selection90negative(image, *selection);
		else
			rotate_entire90negative(image, selection, &print);
		break;
	case 2:
		if (height != image->height || width != image->width) {
			rotate_selection90negative(image, *selection);
			rotate_selection90negative(image, *selection);
		} else {
			rotate_entire180(image, selection, &print);
		}
		break;
	case 3:
		if (height != image->height || width != image->width) {
			rotate_selection90negative(image, *selection);
			rotate_selection90negative(image, *selection);
			rotate_selection90negative(image, *selection);
		} else {
			rotate_entire270(image, selection, &print);
		}
		break;
	case -1:
		printf("Unsupported rotation angle\n");
		print = -1;
		break;
	}

	if (print != -1)
		printf("Rotated %d\n", value);
}
