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
    cmplx *v, **arrels, *primer;
    cmplx aux, w0;
    int i, j, k, d;
    double a, b;
    
    w0.x = 0.1234;
    w0.y = 0.5678;
    
    
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
    
    /* La llista de nodes */
    v = (cmplx *) malloc(sizeof(cmplx));
    primer = (cmplx *) malloc(sizeof(cmplx));
    primer->seg = NULL;
    
    /* Llegim */
    i=0;
    while (fscanf(fp, "%lf %lf %d", &a, &b, &d) != EOF) {
        if ((sqrt(a*a + b*b) >= 1e-8) && (d >= 1)) {
            v = (cmplx *) realloc(v, i+1);
            
            
            
            aux.x = a;
            aux.y = b;
            polar(&aux);        // Obtenim les polars
            
            v[i] = aux;
            
            primer = afegir(primer, &v[i]);

            
            if (d == 1) {
                v = (cmplx *) realloc(v, i+3);
                aux = prod(v[i], w0);
                
                v[i+1] = aux;
                primer = afegir(primer, &v[i+1]);
                
                quocient(v[i], w0, &v[i+2]);
                primer = afegir(primer, &v[i+2]);
                
            } else if (d >= 2) {
                /* Ara hem de fer un vector de vectors per les arrels amb dynamic */
                arrels = (cmplx **) calloc(d, sizeof(cmplx *));
                if (arrels == NULL) {
                    printf("memory issues\n");
                    exit(1);
                }
                for (j=0; j < d; j++) {
                    arrels[j] = (cmplx *) malloc(sizeof(cmplx));
                    if (arrels[i] == NULL) {
                        printf("memory issues\n");
                        exit(1);
                    }
                    
                    arrels = nroot(v[i], d);
                    
                    for (k=0; k < d; k++) {
                        v[i+k+1] = *arrels[k];
                        primer = afegir(primer, &v[i+k+1]);
                    }
                    i += d + 1;
                    
                    
                    /* Ara alliberar la memòria */
                    for (j=0; j < d; j++) {
                        free(arrels[j]);
                    }
                    
                    free(arrels);
                }
            }
        }
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
