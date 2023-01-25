// Copyright Tudor Cristian-Andrei 311CAa 2022 - 2023
#pragma once

// pentru imaginile color am nevoie de o matrice care sa retina 3 valori
// pentru fiecare element, valoarea lui rosu, valoarea lui verde si valoarea
// lui albastru
typedef struct {
	int red;
	int green;
	int blue;
} RGB;

// o imagine este caracterizata de format, dimensiuni (inaltime si lungime),
// valoarea maxima a pixelilor, si imaginea in sine, adica fie o matrice de
// tipul RGB, fie o matrice de valori de gri
typedef struct {
	int format;
	int height;
	int width;
	int maxvalue;
	RGB **pixel;
	int **gray;
} img;

// o structura care retine selectia
typedef struct {
	int row_start;
	int coll_start;
	int row_stop;
	int coll_stop;
} coordinates;

// structura care descrie un kernel
typedef struct {
	double template[9][9];
	double divideby;
} ker;
