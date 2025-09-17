#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

Produto criarProduto(int codigo, char nome[], float preco, int quantidade){
    Produto p;
    p.codigo = codigo;
    strcpy(p.nome,nome);
    p.preco = preco;
    p.quantidade = quantidade;
    return p;
}

No* criarNo(Produto p){
    No* novo = (No*) malloc(sizeof(No));
    novo->item = p;
    novo->esq = novo->dir = NULL;
    return novo;
}

No* inserirPorCodigo(No* raiz, Produto p){
    if (raiz == NULL) return criarNo(p);
    if (p.codigo < raiz->item.codigo)
        raiz->esq = inserirPorCodigo(raiz->esq, p);
    else if (p.codigo > raiz->item.codigo)
        raiz->dir = inserirPorCodigo(raiz->dir, p);
    return raiz;
}

No* inserirPorPreco(No* raiz, Produto p){
    if (raiz == NULL) return criarNo(p);
    if (p.preco < raiz ->item.preco)
        raiz->esq = inserirPorPreco(raiz->esq, p);
    else if (p.preco > raiz->item.preco)
        raiz->dir = inserirPorPreco(raiz->dir, p);
    return raiz;
}