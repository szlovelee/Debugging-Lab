#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 

typedef struct {
    int width;
    int height;
    int channels;
    size_t nbytes;              
    unsigned char *px;
} Image;

static Image *image_new(int width, int height, int channels) {
    Image *img = malloc(sizeof *img);
    if (!img) { perror("malloc"); exit(1); }
    img->width = width;
    img->height = height;
    img->channels = channels;

    img->nbytes = (size_t)width * (size_t)height * (size_t)channels;
    img->px = malloc((size_t)img->nbytes);
    if (!img->px) { perror("malloc px"); exit(1); }
    return img;
}

static void image_fill(Image *img, unsigned char value) {
    size_t total = img->nbytes;
    for (size_t i = 0; i < total; i++) {
        printf("%ld / %ld\n", i, total);
        img->px[i] = value;                     
    }
}

int main(void) {
    Image *img = image_new(65536, 65536, 4);
    printf("allocated nbytes(int)=%d for %dx%d x%d\n",
           img->nbytes, img->width, img->height, img->channels);

    image_fill(img, 0xFF);                       

    printf("px[0]=%u\n", img->px[0]);
    free(img->px);
    free(img);
    return 0;
}
