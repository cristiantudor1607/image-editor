// Copyright Tudor Cristian-Andrei 311CAa 2022 - 2023
#include "grayscale_utils.h"

// functie care calculeaza frecventa pe un anumit numar de intervale
// de preferat putere a lui 2, altfel nu mai merge :))
double *frequency(img image, int bins)
{
	double *freq = (double *)malloc(bins * sizeof(double));
	if (!freq)
		return NULL;

	int len = 256 / bins;
	// initializez valorile cu 0, ca sa pot incrementa
	for (int i = 0; i < bins; i++)
		freq[i] = 0;

	for (int i = 0; i < image.height; i++) {
		for (int j = 0; j < image.width; j++) {
			for (int k = 0; k < bins; k++) {
				// incadrez valoarea intr-un interval
				if (len * k <= image.gray[i][j] &&
					image.gray[i][j] < len * (k + 1))
					freq[k]++;
			}
		}
	}

	return freq;
}

// functie care returneaza un vector, in care salvez numarul de
// stelute ce trebuie printate
int *asterisk(img image, int max_asterisk, int bins)
{
	double *freq = frequency(image, bins);
	if (!freq)
		return NULL;

	// calculex maximul din vectorul de frecventa
	double max = freq[0];
	for (int i = 1; i < bins; i++) {
		if (freq[i] > max)
			max = freq[i];
	}

	int *printable = (int *)malloc(bins * sizeof(int));
	if (!printable)
		return NULL;

	// aplic formula din pdf
	for (int i = 0; i < bins; i++) {
		double aux = freq[i] / max;
		aux *= max_asterisk;
		printable[i] = (int)aux;
	}

	// eliberez vectorul de frecventa
	free(freq);
	return printable;
}

// functie care afiseaza histograma
void print_histogram(img image, int max, int bins)
{
	int *print = asterisk(image, max, bins);
	if (!print) {
		printf("Failed to histogram\n");
		return;
	}

	for (int i = 0; i < bins; i++) {
		printf("%d\t|\t", print[i]);
		for (int j = 0; j < print[i]; j++)
			printf("*");
		printf("\n");
	}

	free(print);
}

// functie care calculeaza suma de la 0 la a, H(i), din pdf
double sum_from_zero_to_a(double *freq, int a)
{
	double result = 0;
	for (int i = 0; i <= a; i++)
		result += freq[i];

	return result;
}

// functie care aplica formula din pdf si returneaza noua valoare
// a pixelului
int equalized_pixel(img image, double *freq, int a)
{
	double h = sum_from_zero_to_a(freq, a);

	double area = image.height * image.width;

	double aux = (h * 255) / area;

	int result = round(aux);

	// sper sa fie 255 si nu image.maxvalue :))
	return clamp(result, 255);
}

// functia apelata in main pentru histograma
void do_histogram(img image, char *type)
{
	int max, bins;
	char *p = strtok(type, " ");
	max = atoi(p);
	p = strtok(NULL, " ");

	// verific sa am fix 2 argumente date la HISTOGRAM
	if (!p) {
		printf("Invalid command\n");
		return;
	}

	bins = atoi(p);
	p = strtok(NULL, " ");
	if (p) {
		printf("Invalid command\n");
		return;
	}

	// verific daca am imagine incarcata
	if (image.format == 0) {
		printf("No image loaded\n");
		return;
	}

	// verific daca imaginea este alb-negru
	if (image.format == 3 || image.format == 6) {
		printf("Black and white image needed\n");
		return;
	}

	print_histogram(image, max, bins);
}

// functia apelata in main pentru egalizare
void do_equalize(img *image)
{
	if (image->format == 0) {
		printf("No image loaded\n");
		return;
	}

	if (image->format == 3 || image->format == 6) {
		printf("Black and white image needed\n");
		return;
	}

	double *freq = frequency(*image, 256);
	if (!freq) {
		printf("Failed to equalize\n");
		return;
	}

	// se calculeaza noua valoare a pixelului pentru fiecare pixel
	for (int i = 0; i < image->height; i++) {
		for (int j = 0; j < image->width; j++) {
			int aux = equalized_pixel(*image, freq, image->gray[i][j]);
			image->gray[i][j] = aux;
		}
	}

	free(freq);
	printf("Equalize done\n");
}
