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
#define NAME_IMG_OUT1 "image-Out1"
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
    int mask_size= 50;

    float** udem1 = LoadImagePgm(NAME_IMG_IN1, &length, &width);
    float** udem1_backup = fmatrix_allocate_2d(length, width);
    float** udem1_r = fmatrix_allocate_2d(length, width);
    float** udem1_i = fmatrix_allocate_2d(length, width);
    fmatrix_move(length, width, udem1, udem1_backup);

    float** udem2 = LoadImagePgm(NAME_IMG_IN2, &length, &width);
    float** udem2_backup = fmatrix_allocate_2d(length, width);
    fmatrix_move(length, width, udem2, udem2_backup);

    // les rebords noirs de l'image tournée cause des difficultés lorsqu'on
    // trouve best_theta, on ajoute donc un cadre noir pour rendre les rebords
    // des deux images pareilles
    for (int i = 0; i < length; i++) {
	    for (int j = 0; j < width; j++) {
	        if (i<mask_size || j<mask_size || i>length-mask_size || j>width-mask_size) {
	            udem1[i][j]=0;
            }   
        }
    }
    
    fmatrix_move(length, width, udem1, udem1_r);
    fmatrix_zero(length, width, udem1_i);
    
    FFTDD(udem1_r, udem1_i, length, width);

    float** udem1_mod = fmatrix_allocate_2d(length, width);
    fmatrix_zero(length, width, udem1_mod);
    // On calcule le module de l'image 1 (F)
    Mod(udem1_mod, udem1_r, udem1_i, length, width);

    float best_epsilon = 1e100;
    float best_theta = -1;

    for (float theta = -PI / 16.0;theta < PI / 16;theta += 0.005) {

        float** udem2_r = fmatrix_allocate_2d(length, width);
        fmatrix_zero(length, width, udem2_r);
        fmatrix_move(length, width, udem2, udem2_r);

        float** udem2_r_rot = fmatrix_allocate_2d(length, width);
        float** udem2_i_rot = fmatrix_allocate_2d(length, width);

        fmatrix_zero(length, width, udem2_r_rot);
        fmatrix_zero(length, width, udem2_i_rot);
        rotation(length, width, theta, udem2_r, udem2_r_rot);

        // cadre noir sur l'image tournée
        for (int i = 0; i < length; i++) {
	    for (int j = 0; j < width; j++) {
	        if (i<mask_size || j<mask_size || i>length-mask_size || j>width-mask_size) {
	            udem2_r_rot[i][j]=0;
            }   
        }
        }

        FFTDD(udem2_r_rot, udem2_i_rot, length, width);

        float** udem2_mod = fmatrix_allocate_2d(length, width);
        fmatrix_zero(length, width, udem2_mod);
        //On calcule le module de l'image 1 (F)
        Mod(udem2_mod, udem2_r_rot, udem2_i_rot, length, width);

        double epsilon = 0.0;
        for (int i = 0;i < length;i++) {
            for (int j =0;j < width;j++) {
                double diff = fabs(udem2_mod[i][j] - udem1_mod[i][j]);
                epsilon += diff;
            }

        }

        if (epsilon < best_epsilon) {
            best_epsilon = epsilon;
            best_theta = theta;
        }
    }

    printf("best theta: %f\n", best_theta);

    float** udem2_straight = fmatrix_allocate_2d(length, width);
    float** udem2_r = fmatrix_allocate_2d(length, width);
    float** udem2_i = fmatrix_allocate_2d(length, width);
    float** udem2_mod = fmatrix_allocate_2d(length, width);
    float** udem2_mod_i = fmatrix_allocate_2d(length, width);
    fmatrix_zero(length, width, udem2_mod_i);
    
    rotation(length, width, best_theta, udem2_backup, udem2_straight);
    fmatrix_move(length, width, udem2_straight, udem2);
    fmatrix_move(length, width, udem2, udem2_r);
    fmatrix_zero(length, width, udem2_i);

    FFTDD(udem2_r, udem2_i, length, width);

    Mod(udem2_mod, udem2_r, udem2_i, length, width);
    // pour trouver le conjugué de G
    for (int i = 0; i < length; i++) {
	for (int j = 0; j < width; j++) {
	    udem2_i[i][j] = -udem2_i[i][j];
    }
    }

    float** vector_r = fmatrix_allocate_2d(length,width);
    float** vector_i = fmatrix_allocate_2d(length,width);

    MultMatrix(vector_r, vector_i, udem1_r, udem1_i, udem2_r, udem2_i, length, width);

    for (int i = 0; i < length; i++) {
	for (int j = 0; j < width; j++) {
	    udem2_mod[i][j] = 1/(SQUARE(udem2_mod[i][j]));
    }
    }


    MultMatrix(vector_r, vector_i, vector_r, vector_i, udem2_mod, udem2_mod_i, length, width);
    // IFFTDD(vector_r, vector_i, length, width);
    // Recal(vector_r, length, width);
    // Mult(vector_r, 100, length, width);
    CenterImg(vector_r, length, width);
    CenterImg(vector_i, length, width);
    /* //Sauvegarde */
    SaveImagePgm(NAME_IMG_OUT1,vector_r,length, width);
    SaveImagePgm(NAME_IMG_OUT2,vector_i,length,width);

     //Commande systeme: VISU */
     strcpy(BufSystVisuImg,NAME_VISUALISER);
     strcat(BufSystVisuImg,NAME_IMG_OUT1);
     strcat(BufSystVisuImg,".pgm&");
     printf(" %s",BufSystVisuImg);
     system(BufSystVisuImg);
     strcpy(BufSystVisuImg,NAME_VISUALISER);
    
     strcat(BufSystVisuImg,NAME_IMG_OUT2);
     strcat(BufSystVisuImg,".pgm&");
     printf(" %s",BufSystVisuImg);
     system(BufSystVisuImg);


    /* //==End========================================================= */

    /* //retour sans probleme */
    printf("\n C'est fini ... \n\n");
    return 0;
}
