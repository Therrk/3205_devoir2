/*------------------------------------------------------*/
/* Prog    : Tp2_IFT3205-2-4.c                          */
/* Auteur  : Élie Leblanc                               */
/* Date    : --/--/2010                                 */
/* version :                                            */ 
/* langage : C                                          */
/* labo    : DIRO                                       */
/*------------------------------------------------------*/

/*------------------------------------------------*/
/* FICHIERS INCLUS -------------------------------*/
/*------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "FonctionDemo2.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_VISUALISER "display "
#define NAME_IMG_IN1  "lena"
#define NAME_IMG_IN2  "UdM_2"
#define NAME_IMG_OUT1 "lena_rotated"
#define NAME_IMG_OUT2 "image-Out2"

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/
/*------------------------------------------------*/

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/
/*------------------------------------------------*/
int main(int argc,char **argv)
{
    int length,width;
    // On doit convertir en radians
    float Theta0 = 22.5 / 360.0 * (2.0 * PI);
    char BufSystVisuImg[100];

    //Constante
    length=512;
    width=512;

    //Allocation Memoire
    float** MatriceImgI1=fmatrix_allocate_2d(length,width);
    float** MatriceImgM1=fmatrix_allocate_2d(length,width);
    float** MatriceImgR1=fmatrix_allocate_2d(length,width);
    float** MatriceImgI2=fmatrix_allocate_2d(length,width);
    float** MatriceImgM2=fmatrix_allocate_2d(length,width);
    float** MatriceImgR2=fmatrix_allocate_2d(length,width);
    float** MatriceImgI3=fmatrix_allocate_2d(length,width);
    float** MatriceImgM3=fmatrix_allocate_2d(length,width);
    float** MatriceImgR3=fmatrix_allocate_2d(length,width);
    float** MatriceImg3=fmatrix_allocate_2d(length,width);

    //Lecture Image
    float** MatriceImg1=LoadImagePgm(NAME_IMG_IN1,&length,&width);
    // float** MatriceImg2=LoadImagePgm(NAME_IMG_IN2,&length,&width);


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
            float x_src = (x_centered*cos(Theta0) + y_centered*sin(Theta0)) + half_width;
            float y_src = (x_centered*-sin(Theta0) + y_centered*cos(Theta0)) + half_length;


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
                float down_avg = MatriceImg1[down][left] + x_off * (MatriceImg1[down][right] - MatriceImg1[down][left]);
                float up_avg = MatriceImg1[up][left] + x_off * (MatriceImg1[up][right] - MatriceImg1[up][left]);


                // interpolation entre les deux interpolations. le up, down est
                // inversé, car la composante verticale pointe vers le bas.
                float vert_avg = up_avg + y_off * (down_avg - up_avg);

                MatriceImgM1[i][j]= (int)vert_avg;
            } else {
                MatriceImgM1[i][j]= 0;
            }
        }
    }
    MatriceImg1 = MatriceImgM1;

    //Sauvegarde
    SaveImagePgm(NAME_IMG_OUT1,MatriceImg1,length,width);
    // SaveImagePgm(NAME_IMG_OUT2,MatriceImg2,length,width);

    //Commande systeme: VISU
    strcpy(BufSystVisuImg,NAME_VISUALISER);
    strcat(BufSystVisuImg,NAME_IMG_OUT1);
    strcat(BufSystVisuImg,".pgm&");
    printf(" %s",BufSystVisuImg);
    system(BufSystVisuImg);
    // strcpy(BufSystVisuImg,NAME_VISUALISER);
    // strcat(BufSystVisuImg,NAME_IMG_OUT2);
    // strcat(BufSystVisuImg,".pgm&");
    // printf(" %s",BufSystVisuImg);
    // system(BufSystVisuImg);


    //==End=========================================================

    //Liberation memoire
    free_fmatrix_2d(MatriceImgR1);
    free_fmatrix_2d(MatriceImgI1);
    free_fmatrix_2d(MatriceImgM1);
    free_fmatrix_2d(MatriceImgR2);
    free_fmatrix_2d(MatriceImgI2);
    free_fmatrix_2d(MatriceImgM2);
    free_fmatrix_2d(MatriceImgR3);
    free_fmatrix_2d(MatriceImgI3);
    free_fmatrix_2d(MatriceImgM3);
    free_fmatrix_2d(MatriceImg1);
    // free_fmatrix_2d(MatriceImg2);
    free_fmatrix_2d(MatriceImg3);

    //retour sans probleme
    printf("\n C'est fini ... \n\n");
    return 0;
}
