/* example1.c                                                      */
/*                                                                 */
/* This small program shows how to print a rotated string with the */
/* FreeType 2 library.                                             */

#include <math.h>
#include <stdio.h>
#include <string.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#define WIDTH 640
#define HEIGHT 480

/* origin is the upper left corner */
unsigned char image[HEIGHT][WIDTH];

/* Replace this function with something useful. */

void draw_bitmap(FT_Bitmap* bitmap, FT_Int x, FT_Int y) {
    FT_Int i, j, p, q;
    FT_Int x_max = x + bitmap->width;
    FT_Int y_max = y + bitmap->rows;

    for (i = x, p = 0; i < x_max; i++, p++) {
        for (j = y, q = 0; j < y_max; j++, q++) {
            if (i < 0 || j < 0 || i >= WIDTH || j >= HEIGHT) continue;
            image[j][i] |= bitmap->buffer[q * bitmap->width + p];
        }
    }
}

char i2c(unsigned char i) {
    return i < 128 ? '*' : '@';
    /*
    if(i < 10 ) {
        return '.';
    } else if(i < 50) {
        return '*';
    } else if(i < 100) {
        return '-';
    } else if(i < 150) {
        return '+';
    } else if(i < 200) {
        return '*';
    } else {
        return '@';
    }*/
}

void show_image(void) {
    int i, j;
    int xlb = 0;
    int xub = WIDTH;
    int ylb = 0;
    int yub = HEIGHT;

    for (i = 0; i < WIDTH; i++) {
        int flg = false;
        for (j = 0; j < HEIGHT; j++) {
            // h - w
            if (image[j][i] != 0) flg = true;
        }
        if (flg == true) {
            break;
        } else {
            xlb = i;
        }
    }

    for (i = WIDTH; i > 0; i--) {
        int flg = false;
        for (j = 0; j < HEIGHT; j++) {
            if (image[j][i] != 0) flg = true;
        }
        if (flg == true) {
            break;
        } else {
            xub = i;
        }
    }

    for (i = 0; i < HEIGHT; i++) {
        int flg = false;
        for (j = 0; j < WIDTH; j++) {
            if (image[i][j] != 0) flg = true;
        }
        if (flg == true) {
            break;
        } else {
            ylb = i;
        }
    }

    for (i = HEIGHT; i > 0; i--) {
        int flg = false;
        for (j = 0; j < WIDTH; j++) {
            if (image[i][j] != 0) flg = true;
        }
        if (flg == true) {
            break;
        } else {
            yub = i;
        }
    }

    for (i = ylb; i < yub; i++) {
        for (j = xlb; j < xub; j++) {
            // h - w
            putchar(image[i][j] == 0 ? ' ' : i2c(image[i][j]));
        }
        putchar('\n');
    }
}

int main(int argc, char** argv) {
    FT_Library library;
    FT_Face face;

    FT_GlyphSlot slot;
    FT_Matrix matrix; /* transformation matrix */
    FT_Vector pen;    /* untransformed origin  */
    FT_Error error;

    char* filename;
    char* text;

    double angle;
    int target_height;
    int n, num_chars;

    if (argc != 3) {
        fprintf(stderr, "usage: %s font sample-text\n", argv[0]);
        exit(1);
    }

    filename = argv[1]; /* first argument     */
    text = argv[2];     /* second argument    */
    num_chars = strlen(text);
    // angle = (25.0 / 360) * 3.14159 * 2; /* use 25 degrees     */
    angle = (5.0 / 360) * 3.14159 * 2; /* use 25 degrees     */
    target_height = HEIGHT;

    error = FT_Init_FreeType(&library); /* initialize library */
    /* error handling omitted */

    error = FT_New_Face(library, filename, 0, &face); /* create face object */
    /* error handling omitted */

    /* use 50pt at 100dpi */
    //error = FT_Set_Char_Size(face, 50 * 64, 0, 100, 0); /* set character size */
    error = FT_Set_Char_Size(face, 8 * 64, 0, 150, 0); /* set character size */
    /* error handling omitted */

    slot = face->glyph;

    /* set up matrix */
    matrix.xx = (FT_Fixed)(cos(angle) * 0x10000L);
    matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000L);
    matrix.yx = (FT_Fixed)(sin(angle) * 0x10000L);
    matrix.yy = (FT_Fixed)(cos(angle) * 0x10000L);

    /* the pen position in 26.6 cartesian space coordinates; */
    /* start at (300,200) relative to the upper left corner  */
    pen.x = 300 * 64;
    pen.y = (target_height - 200) * 64;

    for (n = 0; n < num_chars; n++) {
        /* set transformation */
        FT_Set_Transform(face, &matrix, &pen);

        /* load glyph image into the slot (erase previous one) */
        error = FT_Load_Char(face, text[n], FT_LOAD_RENDER);
        if (error) continue; /* ignore errors */

        /* now, draw to our target surface (convert position) */
        draw_bitmap(&slot->bitmap, slot->bitmap_left, target_height - slot->bitmap_top);

        /* increment pen position */
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
    }

    show_image();

    FT_Done_Face(face);
    FT_Done_FreeType(library);

    return 0;
}

/* EOF */