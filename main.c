#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <stdbool.h>
#include "fractal.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <SDL/SDL.h>
#include <float.h>


pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
pthread_mutex_t mutex3; //mutex pour proteger la variable flagLecture
sem_t empty1;
sem_t full1;
sem_t empty2;
sem_t full2;
float le_plus_grand=FLT_MIN;
int compteur_de_fichier;
int flagLecture;
int amettre1;
int amettre2;
struct fractal **tab1;
//tab1->value=0;//place dans le tableau a laquelle il faut placer la prochaine fractal dont il faut calculer la valeur des pixels.
struct fractal **tab2;
//tab2->value=0;//place dans le tableau a laquelle il faut placer la prochaine fractal dont la valeur des pixels est deja calculee.


int nbrFractale;
int nbrFractaleCalcule;
int nbrFractaleConso;
void append (char* s,char c){
    int len=strlen(s);
    s[len]=c;
    s[len+1]='\0';
}

/*
 * @check: nombre de fichier que l'on met en arguments
 * @flag: true si -d est en argument de la main
 * @output: Si le fichier est -, alors on insere ds le tableau la fractal mise dans l'entree std
 *
 *
 */

void *producer1 (void * param) {
    printf("producteur\n");
    char *check=(char *)param;
    char tiret = '-';
    char fin = '\0';


    /*
     * Si le fichier est un tiret
     */
    if(*check == tiret){
        char lpBuffer[500];


        printf("Entrez une chaine de caracteres : ");

        fgets(lpBuffer, 500*sizeof(char), stdin);

        int i = 0;
        char diese = '#';
        if (*lpBuffer == diese) {
            fprintf(stderr, "%s\n", strerror(errno));
        }

        char* nom =&fin;


        char vide = ' ';
        while (*(lpBuffer+i) != vide) {

            append(nom, *(lpBuffer+i));//Il faut cheker la valeure de retour??
            i++;
        }

        i++;
        char fin2='\0';
        char* largeur = &fin2;


        while (*(lpBuffer+i) != vide){


            append(largeur, *(lpBuffer+i));
            i++;
        }

        int largeur2 = atoi(largeur);
        i++;
        char fin3='\0';
        char* hauteur = &fin3;
        while (*(lpBuffer+i) != vide){
            append(hauteur, *(lpBuffer+i));
            i++;
        }

        int hauteur2 = atoi(hauteur);
        i++;
        char fin4='\0';
        char* reelle = &fin4;
        while (*(lpBuffer+i) != vide){
            append(reelle, *(lpBuffer+i));
            i++;
        }

        double reelle2 = atof(reelle);
        i++;
        char fin5='\0';
        char* imaginaire = &fin5;
        char n = '\n';
        while (*(lpBuffer+i) != n) {
            append(imaginaire, *(lpBuffer+i));
            i++;
        }

        double imaginaire2 = atof(imaginaire);
        //if(!dejaRegistre){
        struct fractal *fractal_a_creer;
        int place;

        sem_wait(&empty1);
        pthread_mutex_lock(&mutex1);
        fractal_a_creer = fractal_new(nom, largeur2, hauteur2, reelle2, imaginaire2);
        nbrFractale++;




        tab1[amettre1] = fractal_a_creer;
        amettre1++;
        //tab1->value=tab1->value+1;
        pthread_mutex_unlock(&mutex1);
        sem_post(&full1);

        printf("%f\n", fractal_get_a(tab1[0]));

    } //fin de la lecture depuis une entree standard

    else{//lecture depuis un fichier texte
        printf("producteur 1-1\n");
        FILE* fichier = NULL;
        char lpBuffer[500] = "";
        fichier = fopen(check, "r");
        if(fichier != NULL){
            char* c=fgets(lpBuffer, 500 * sizeof(char), fichier);
            while(c != NULL){
                int i = 0;
                char diese = '#';
                if (*lpBuffer == diese) {
                    fprintf(stderr, "%s\n", strerror(errno));
                }

                char fin = '\0';
                char* nom = &fin;
                char vide = ' ';

                while (*(lpBuffer+i) != vide) {
                    append(nom, lpBuffer[i]);//Il faut cheker la valeure de retour??
                    i++;
                }





                i++;
                char fin2='\0';
                char* largeur = &fin2;


                while (*(lpBuffer+i) != vide){


                    append(largeur, *(lpBuffer+i));
                    i++;
                }

                int largeur2 = atoi(largeur);
                i++;
                char fin3='\0';
                char* hauteur = &fin3;
                while (*(lpBuffer+i) != vide){
                    append(hauteur, *(lpBuffer+i));
                    i++;
                }

                int hauteur2 = atoi(hauteur);
                i++;
                char fin4='\0';
                char* reelle = &fin4;
                while (*(lpBuffer+i) != vide){
                    append(reelle, *(lpBuffer+i));
                    i++;
                }

                double reelle2 = atof(reelle);
                i++;
                char fin5='\0';
                char* imaginaire = &fin5;
                char n = '\n';
                while (*(lpBuffer+i) != n) {
                    append(imaginaire, *(lpBuffer+i));
                    i++;
                }

                double imaginaire2 = atof(imaginaire);
                //if(!dejaRegistre){
                struct fractal *fractal_a_creer;
                int place;
                printf("producteru 2\n");
                sem_wait(&empty1);
                pthread_mutex_lock(&mutex1);
                fractal_a_creer = fractal_new(nom, largeur2, hauteur2, reelle2, imaginaire2);
             
                nbrFractale++;

                printf("A Prod %d\n", amettre1);
                tab1[amettre1] = fractal_a_creer;
                amettre1++;
                c=fgets(lpBuffer, 500 * sizeof(char), fichier);
                if(c==NULL){
                    flagLecture++;
                }
             
                //tab1->value=tab1->value+1;
                pthread_mutex_unlock(&mutex1);
                sem_post(&full1);


            }
            fclose(fichier);
        }
    }
 
}

void *proconsumer(void * param){
    printf("proconsumer\n");
    int place=0;
    int place2=0;
    struct fractal* fractal_a_retirer;


    while(flagLecture!=compteur_de_fichier || nbrFractale!=nbrFractaleCalcule){//a changer en while(le producer a fini && le tableau est vide)
        printf("wait-proconsumer\n");
        sem_wait(&full1);
        pthread_mutex_lock(&mutex1);

        nbrFractaleCalcule++;
        fractal_a_retirer = tab1[amettre1-1];
        amettre1--;
         
            
 
        //tab1->value=tab1->value-1;  //prend une fractal + decremente la valeur du tableau et remplace l'endoit du tableau dont on a pri la fract par NULL
        int e = pthread_mutex_unlock(&mutex1);   
        sem_post (&empty1);

            for(int largeur=0;largeur<fractal_get_height(fractal_a_retirer);largeur++){
            for(int longeur=0;longeur<fractal_get_width(fractal_a_retirer);longeur++){
   
                fractal_compute_value(fractal_a_retirer,longeur,largeur);//ATTENTION REVOIE UN INT
            }

        }
   
 

//fin du calcul des fractal.
        printf("wait-proconsumer 2\n");
        sem_wait(&empty2);
        pthread_mutex_lock(&mutex2);
     
        printf("amettre 2 %d\n", amettre2);
        tab2[amettre2]=fractal_a_retirer;
        amettre2++;
        //tab2->value=tab2->value+1;
        pthread_mutex_unlock(&mutex2);
        sem_post(&full2);     
    }
}

void *consumer1 (void * param){// pas de -d ds la main
    printf("consumer 1\n");
    char *doc_final=(char *) param;
    struct fractal *fractal_moyenne;
    struct fractal *fractal_a_free;
    struct fractal *fractal_a_conserver;
    int premier=1;

 
 

 
    while(flagLecture!=compteur_de_fichier || nbrFractaleCalcule!=nbrFractale || nbrFractaleCalcule!=nbrFractaleConso){//a changer en while(le proconsummer a fini && le tableau est vide)
        printf("ammettre3 %d\n", amettre2);
        sem_wait(&full2);
        pthread_mutex_lock(&mutex2);
       printf("ammettre4 %d\n", amettre2);
        nbrFractaleConso++;
        fractal_moyenne=tab2[amettre2-1];
        printf("aaaaaaaa\n");
        for(int i=0; i<fractal_get_width(fractal_moyenne); i++){
            for(int j=0; j<fractal_get_height(fractal_moyenne); j++){
                fractal_set_value(fractal_moyenne, i, j, fractal_get_value(fractal_moyenne, i, j));
            }
        }
        amettre2--;
        printf("ammettre5 %d\n", amettre2);



        //tab2->value=tab2->value-1;//prend une fractal dont il faut calculer la moyenne +decremente et remplace l'endroit du tableau dont on a pris la fract par NULL

        pthread_mutex_unlock(&mutex2);
        sem_post(&empty2);


        float max_tableau=fractal_get_height(fractal_moyenne) * fractal_get_width(fractal_moyenne);
        float a_comparer=0;
        for(int i=0;i<fractal_get_width(fractal_moyenne); i++){
            for(int j=0; j<fractal_get_height(fractal_moyenne); j++){
                a_comparer = a_comparer + fractal_get_value(fractal_moyenne, i, j);
            }
        }

        a_comparer=a_comparer/(max_tableau);//a cheker si on mettrait pas plutot un double et une division plus précise
        printf("a_comparer %f\n", a_comparer);
        if(a_comparer>le_plus_grand){
            if(premier){
                le_plus_grand = a_comparer;
                fractal_a_conserver=fractal_moyenne;
                premier=0;
            }
            else {
                le_plus_grand = a_comparer;
                fractal_free(fractal_a_conserver);//normalement c'est ok nn?
                fractal_a_conserver = fractal_moyenne;
            }
        }
        else{
            fractal_free(fractal_moyenne);
        }
 
    }
    printf("fin\n");
    int fin = write_bitmap_sdl(fractal_a_conserver,doc_final);
    fractal_free(fractal_a_conserver);
    printf("fin fin\n");



}

void *consumer2 (void * param){// -d ds la main
    printf("consumer 1\n");
    
    struct fractal *fractal_moyenne;
    struct fractal *fractal_a_free;
    struct fractal *fractal_a_conserver;
    int premier=1;

 
 

 
    while(flagLecture!=compteur_de_fichier || nbrFractaleCalcule!=nbrFractale || nbrFractaleCalcule!=nbrFractaleConso){//a changer en while(le proconsummer a fini && le tableau est vide)
        printf("ammettre3 %d\n", amettre2);
        sem_wait(&full2);
        pthread_mutex_lock(&mutex2);
       printf("ammettre4 %d\n", amettre2);
        nbrFractaleConso++;
        fractal_moyenne=tab2[amettre2-1];
        printf("aaaaaaaa\n");
        for(int i=0; i<fractal_get_width(fractal_moyenne); i++){
            for(int j=0; j<fractal_get_height(fractal_moyenne); j++){
                fractal_set_value(fractal_moyenne, i, j, fractal_get_value(fractal_moyenne, i, j));
            }
        }
        amettre2--;
        printf("ammettre5 %d\n", amettre2);



        //tab2->value=tab2->value-1;//prend une fractal dont il faut calculer la moyenne +decremente et remplace l'endroit du tableau dont on a pris la fract par NULL

        pthread_mutex_unlock(&mutex2);
        sem_post(&empty2);

        int fin = write_bitmap_sdl(fractal_a_conserver,fractal_get_name(fractal_a_conserver));
    }

}







int main (int argc, char *argv[]) {  //pas de -d
    int e;
    int error;
    int nthread;
    char *c = argv[1]; //nbr de thread



    amettre1=0;
    amettre2=0;
 
    nbrFractale=0;
    nbrFractaleCalcule=0;
    nbrFractaleConso=0;

    error = pthread_mutex_init(&mutex1, NULL);
    if (error != 0) {
        return (EXIT_FAILURE);
    }
    error = pthread_mutex_init(&mutex2, NULL);
    if (error != 0) {
        return (EXIT_FAILURE);
    }
    error = pthread_mutex_init(&mutex3, NULL);
    if (error != 0) {
        return (EXIT_FAILURE);
    }

    error = sem_init(&empty1, 0, 2*argc);// N a discuter
    if (error != 0) {
        return (EXIT_FAILURE);
    }

    error = sem_init(&full1, 0, 0);
    if (error != 0) {
        return (EXIT_FAILURE);
    }
    error = sem_init(&full2, 0, 0);
    if (error != 0) {
        return (EXIT_FAILURE);
    }




    tab1 =  malloc(2*argc*sizeof(struct fractal*));

    printf("thread_co\n");
    pthread_t thread_co;
    char *d="-d\n";
    int decalage;
    if(*c==*d){//-d
        printf("TIRETD\n");
        compteur_de_fichier = argc - 4;
        nthread = atoi(argv[2]);
        pthread_create(&thread_co, NULL, &consumer2, NULL);
        decalage=3;


        
    }
    else{
        compteur_de_fichier=argc-3;
        nthread=atoi(argv[1]);
        pthread_create(&thread_co,NULL,&consumer1,argv[compteur_de_fichier+2]);
        decalage=2;
    }
    
    
  
    error = sem_init(&empty2, 0, 2*nthread);//N a discuter
        if (error != 0) {
           return (EXIT_FAILURE);
        }
 

    tab2 = malloc(2*nthread*sizeof(struct fractal*)); //initialise le tableau 2

        //for(int i=0; i<2*nthread; i++){
        //    tab2[i] = malloc(sizeof(struct fractal**)); //initialise le premier tableau à 2* le nombre de fichier
        //}
    char* argument;
    pthread_t thread_prod[compteur_de_fichier];
    int z=0;


    while(z<compteur_de_fichier){
        (argument)=argv[decalage+z];
        error=pthread_create(&(thread_prod[z]),NULL,&producer1, argument);
        printf("thread_prod\n");
        if (error != 0) {
                    printf("erreur\n");
           return (EXIT_FAILURE);
        }
        z++;
    }

    int a=0;
    pthread_t thread_proco[nthread];
    while(a<nthread){
        error=pthread_create(&(thread_proco[a]),NULL,&proconsumer, NULL);
        printf("thread_proco\n");
        if (error != 0) {
                    printf("erreur\n");
           return (EXIT_FAILURE);
        }
        a++;
    }

    
    


    for(int w=0; w<compteur_de_fichier;w++){
        e=pthread_join(thread_prod[w],NULL);
        printf("joint prod\n");
            if(e!=0){
                fprintf(stderr, "%s\n", strerror(errno));
            }
        }

    for(int w=0; w<nthread;w++){  
          e=pthread_join(thread_proco[w],NULL);
          printf("joint proco\n");
            if(e!=0){
                fprintf(stderr, "%s\n", strerror(errno));
            }
      }


    e=pthread_join(thread_co, NULL);
    printf("joint co\n");
    if(e!=0){
        fprintf(stderr, "%s\n", strerror(errno));
    }



    return 5;
}


