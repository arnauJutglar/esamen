//
//  main.c
//  esamen
//
//  Created by Arnau Jutglar Puig on 21/06/2021.
//  Copyright © 2021 Arnau Jutglar Puig. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14

typedef struct comp {
    int index;
    double x,y;
    double r, arg;
    struct comp *seg;
} cmplx;

cmplx prod(cmplx, cmplx);
int quocient(cmplx, cmplx, cmplx *);
cmplx **nroot(cmplx, int);
void polar(cmplx *);
void cartesia(cmplx *);
void escriuCmplx(FILE *, cmplx);
cmplx * afegir(cmplx *, cmplx *);
void alliberar(cmplx *);

int main(void) {
    FILE *fp;
    char nomf[10];
    cmplx *v;
    int i;
    
    printf("Entra nom arxiu.\n");
    scanf("%s", nomf);
    
    if (nomf[9] != '\0') {
        printf("Error de lectura en el nom de l'arxiu.\n");
        exit(1);
    }
    
    fp = fopen(nomf, "r");
    
    
    if (fp == NULL) {
        printf("Error obrint fitxer\n");
        exit(1);
    }
    
    v = (cmplx *) malloc(sizeof(cmplx));
    
    /* Llegim */
    i=1;
    while (fscanf(fp, "%lf %lf %d", &v[i-1].x, &v[i-1].y, &v[i-1].index) != EOF) {
        v = realloc(v, i+1);
    }
    
    return 0;
}


cmplx prod(cmplx z1, cmplx z2) {
    cmplx z;
    
    z.r = z1.r * z2.r;
    z.arg = z1.arg + z2.arg;
    
    return z;   // Retorna en polars
}

int quocient(cmplx z1, cmplx z2, cmplx *z) {
    if (z2.r < 1e-8)  return 0;
    
    z->r = z1.r / z2.r;
    z->arg = z1.arg - z2.arg;
    // Retorna en polars
    
    return 1;
}

cmplx **nroot(cmplx z1, int n) {
    cmplx **z;
    int i;
    
    z = (cmplx **) calloc(n, sizeof(cmplx *));
    if (z == NULL) {
        printf("memory issues\n");
        exit(1);
    }
    
    for (i=0; i < n; i++) {
        z[i]->r = pow(z1.r, 1/n);
        z[i]->arg = (z1.arg + 2 * PI * i) / n;
    }
    
    return z;
}

void polar(cmplx *z) {
    z->r = sqrt(z->x * z->x + z->y * z->y);
    z->arg = atan2(z->y, z->x);
}

void cartesia(cmplx *z) {
    z->x = z->r * cos(z->arg);
    z->y = z->r * sin(z->arg);
}

void escriuCmplx(FILE *f, cmplx z) {
    fprintf(f, "%2d (%+5le,%+5le)  %.5le %.5le", z.index, z.x, z.y, z.r, z.arg);
    
}

cmplx * afegir(cmplx *primer, cmplx *z) {
    int id;
    
    id = primer->index + 1;
    
    z->seg = primer;
    
    return z;
}

void alliberar(cmplx *primer) {
    cmplx *current, *aux;
    
    current = primer;
    
    while (current->seg != NULL) {
        aux = current;
        current = current->seg;
        free(aux);
    }
    free(current);
    
    return;
}
