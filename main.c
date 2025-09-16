#include <stdio.h>

typedef struct Produto{
    int codigo;
    char nome[50];
    float preco;
    int quantidade;
} Produto;

typedef struct No{
    Produto item;
    struct No *esq;
    struct No *dir;
} No;
