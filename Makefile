CC=gcc
CFLAGS=-Wall -Wextra -std=c99

TARGETS = image_editor

build: $(TARGETS)

image_editor: image_editor.c utils.c load_utils.c save_utils.c apply_utils.c \
				grayscale_utils.c rotate_utils.c select_and_crop_utils.c
	$(CC) $(CFLAGS) image_editor.c utils.c load_utils.c save_utils.c \
					apply_utils.c grayscale_utils.c rotate_utils.c \
					select_and_crop_utils.c -lm -o image_editor

clean:
	rm -f $(TARGETS)