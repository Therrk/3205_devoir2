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
#include "fonctions.c"
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
    float** MatriceImgM1=fmatrix_allocate_2d(length,width);

    //Lecture Image
    float** MatriceImg1=LoadImagePgm(NAME_IMG_IN1,&length,&width);
    // float** MatriceImg2=LoadImagePgm(NAME_IMG_IN2,&length,&width);

    rotation(length, width, Theta0, MatriceImg1, MatriceImgM1);

    free(MatriceImg1);
    MatriceImg1 = MatriceImgM1;

    //Sauvegarde
    SaveImagePgm(NAME_IMG_OUT1,MatriceImg1,length, width);
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
    free_fmatrix_2d(MatriceImg1);
    //retour sans probleme
    printf("\n C'est fini ... \n\n");
    return 0;
}
