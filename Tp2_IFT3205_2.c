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
  int i,j,k;
  int length,width;
  float Theta0 = 22.5;
  int x0,y0;
  char BufSystVisuImg[100];

  int half_width,half_length,x_p,y_p, r_x, r_y;
  float bot_avg, top_avg, pix_avg,x,y;

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
  half_width = width/2;
  half_length = length/2;
  for (i = 0; i < length; i++) {
    for (j = 0; j < width; j++) {
 
	   x_p = half_width-i;
	   y_p = half_length-j;
	   x = (x_p*cos(Theta0) + y_p*sin(Theta0));
	   y = (y_p*cos(Theta0) - x_p*sin(Theta0));
	   x = x+half_width;
	   y = y+half_length;
	   r_x = (int)x;
	   r_y = (int)y;
	   if (r_y>=0&&r_y<length-1&&r_x>=0&&r_x<width-1) {
	      bot_avg= MatriceImg1[r_x][r_y] + (x-r_x) * (MatriceImg1[r_x+1][r_y]-MatriceImg1[r_x][r_y]);
	      top_avg = MatriceImg1[r_x][r_y+1] + (x-r_x) * (MatriceImg1[r_x+1][r_y+1]-MatriceImg1[r_x][r_y+1]);
	      pix_avg = bot_avg + (y-r_y) * (bot_avg-top_avg);
	      MatriceImgM1[i][j]= pix_avg;
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


