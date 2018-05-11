#include <stdlib.h>
#include "fractal.h"
#include <stdio.h>

//struct fractal {
  //  char *name;
    //int width;
//    int height;
  //  double a;
//    double b;
//    int *value;// a voir si c'est pas plutot des int.
//}

struct fractal *fractal_new(const char *name, int width, int height, double a, double b)
{
    struct fractal *new;
    new=(struct fractal *) malloc(sizeof(struct fractal));
    if(new==NULL){
        return NULL;
    }
    new->name=name;
    new->width=width;
    new->height=height;
    new->a=a;
    new->b=b;
    int **tab;
    tab = malloc(width*sizeof(*tab));
    int i;
    for(i=0; i<width; i++){
        tab[i] = malloc(height * sizeof(**tab));
    }
    new->value=tab; 
    return new;
}

void fractal_free(struct fractal *f)
{
    free(f);
}

const char *fractal_get_name(const struct fractal *f)
{
    return (f->name);
}

int fractal_get_value(const struct fractal *f, int x, int y)
{
    return f->value[x][y];
}

void fractal_set_value(struct fractal *f, int x, int y, int val)
{
    *((f->value[x])+y) = val;
}

int fractal_get_width(const struct fractal *f)
{
    return (f->width);
}

int fractal_get_height(const struct fractal *f)
{
    return(f->height);
}

double fractal_get_a(const struct fractal *f)
{
    return (f->a);
}

double fractal_get_b(const struct fractal *f)
{
    return (f->b);
}





