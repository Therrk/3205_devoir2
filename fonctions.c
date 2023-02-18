#include <math.h>

void rotation(int length, int width, float angle, float ** mat_src, float ** mat_dst) {

    // Le code
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {

            int half_width = width/2;
            int half_length = length/2;

            // Les coordonnées centrés
            float x_centered = j - half_width;
            float y_centered = i - half_length;


            // La matrice de rotation est la suivante:


            // | cos(θ) -sin(θ) |
            // | sin(θ)  cos(θ) |
            
            // Puisqu'elle est orthonormale, son inverse est sa transposée

            // Les coordonnées où on devrait prendre les pixels
            float x_src = (x_centered*cos(angle) + y_centered*sin(angle)) + half_width;
            float y_src = (x_centered*-sin(angle) + y_centered*cos(angle)) + half_length;


            int up = (int)floor(y_src);
            int down = (int)ceil(y_src);
            int left = (int)floor(x_src);
            int right = (int)ceil(x_src);

            if (left>=0 && right < width && up >= 0 && down < length) {


                // Les quatre pixels réels qui encadrent notre pixel destination



                // à quel distance somme nous du point entier en haut à gauche?
                float x_off = x_src - (float)left;
                float y_off = y_src - (float)up;

                // interpolation linéaire pour le côté supérieur et inférieur du pixel

                /*           up          */
                /*        +------+       */
                /*   left |  le  | right */
                /*        | pixel|       */
                /*        +------+       */
                /*          down         */
                float down_avg = mat_src[down][left] + x_off * (mat_src[down][right] - mat_src[down][left]);
                float up_avg = mat_src[up][left] + x_off * (mat_src[up][right] - mat_src[up][left]);


                // interpolation entre les deux interpolations. le up, down est
                // inversé, car la composante verticale pointe vers le bas.
                float vert_avg = up_avg + y_off * (down_avg - up_avg);

                mat_dst[i][j]= (int)vert_avg;
            } else {
                mat_dst[i][j]= 0;
            }
        }
    }
}

void fmatrix_move(int length, int width, float ** src, float ** dst) {
    for (int i = 0;i < length;i++) {
        for (int j = 0;j < width;j++) {
            dst[i][j] = src[i][j];
        }
    }
}

void fmatrix_module(int length, int width, float ** r, float ** im, float ** dst) {
        for (int i = 0;i < length;i++) {
        for (int j = 0;j < width;j++) {
            int real = r[i][j];
            int ima = im[i][j];
            dst[i][j] = sqrtf(real * real + ima * ima);
        }
    }
}
