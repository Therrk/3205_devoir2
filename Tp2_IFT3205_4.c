/*------------------------------------------------------*/
/* Prog    : Tp2_IFT3205-2-4.c                          */
/* Auteur  : Élie Leblanc, Justin Veilleux              */
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
#include "fonctions.c"
/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_VISUALISER "display "
#define NAME_IMG_IN1  "UdM_1"
#define NAME_IMG_IN2  "UdM_2"
#define NAME_IMG_OUT1 "image-4"

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

    float** udem1_r = LoadImagePgm(NAME_IMG_IN1, &length, &width);
    float** udem1_i = fmatrix_allocate_2d(length, width);
    fmatrix_zero(length, width, udem1_i);
    FFTDD(udem1_r, udem1_i, length, width);

    float** udem1_mod = fmatrix_allocate_2d(length, width);
    fmatrix_zero(length, width, udem1_mod);
    // On calcule le module de l'image 1 (F)
    fmatrix_module(length, width, udem1_r, udem1_i, udem1_mod);

    float** udem2 = LoadImagePgm(NAME_IMG_IN2, &length, &width);
    
    float best_epsilon = 1e100;
    float best_theta = -1;

    for (float theta = -PI / 16.0;theta < PI / 16;theta += 0.002) {
        float** udem2_r = fmatrix_allocate_2d(length, width);
        fmatrix_zero(length, width, udem2_r);
        fmatrix_move(length, width, udem2, udem2_r);

        float** udem2_r_rot = fmatrix_allocate_2d(length, width);
        float** udem2_i_rot = fmatrix_allocate_2d(length, width);

        fmatrix_zero(length, width, udem2_r_rot);
        fmatrix_zero(length, width, udem2_i_rot);
        rotation(length, width, theta, udem2_r, udem2_r_rot);

        FFTDD(udem2_r_rot, udem2_i_rot, length, width);


        float** udem2_mod = fmatrix_allocate_2d(length, width);
        fmatrix_zero(length, width, udem2_mod);
        //On calcule le module de l'image 1 (F)
        fmatrix_module(length, width, udem2_r_rot, udem2_i_rot, udem2_mod);

        double epsilon = 0.0;
        for (int i = 0;i < length;i++) {
            for (int j = 0;j < width;j++) {
                double diff = fabs(udem2_mod[i][j] - udem1_mod[i][j]);
                epsilon += diff;

            }

        }

        if (epsilon < best_epsilon) {
            best_epsilon = epsilon;
            best_theta = theta;
        }
    }

    
    float** udem2_rot = fmatrix_allocate_2d(length, width);
    fmatrix_zero(length, width, udem2_rot);

    rotation(length, width, best_theta, udem2, udem2_rot);

    printf("best theta: %f\n", best_theta);

    /* free(MatriceImg1); */
    /* MatriceImg1 = MatriceImgM1; */

    /* //Sauvegarde */
    SaveImagePgm(NAME_IMG_OUT1, udem2_rot,length, width);
    // SaveImagePgm(NAME_IMG_OUT2,MatriceImg2,length,width);

    //Commande systeme: VISU
    strcpy(BufSystVisuImg,NAME_VISUALISER);
    strcat(BufSystVisuImg,NAME_IMG_OUT1);
    strcat(BufSystVisuImg,".pgm&");
    printf(" %s",BufSystVisuImg);
    system(BufSystVisuImg);
    // strcpy(BufSystVisuImg,NAME_VISUALISER);
    
    /* // strcat(BufSystVisuImg,NAME_IMG_OUT2); */
    /* // strcat(BufSystVisuImg,".pgm&"); */
    /* // printf(" %s",BufSystVisuImg); */
    /* // system(BufSystVisuImg); */


    /* //==End========================================================= */

    /* //Liberation memoire */
    /* free_fmatrix_2d(MatriceImg1); */
    /* //retour sans probleme */
    /* printf("\n C'est fini ... \n\n"); */
    return 0;
}
