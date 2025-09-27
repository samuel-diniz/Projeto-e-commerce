#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> 

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

Produto* buscarPorCodigo(No* raiz, int codigo){
    if (raiz == NULL) return NULL;
    if (codigo == raiz->item.codigo)
        return &raiz->item;
    if (codigo < raiz->item.codigo)
        return buscarPorCodigo(raiz->esq, codigo);
    else
        return buscarPorCodigo(raiz->dir, codigo);
}

void listarProduto(No* raiz){
    if (raiz != NULL){
        listarProduto(raiz->esq);
        printf("Código: %d\n", raiz->item.codigo);
        listarProduto(raiz->dir);
    }
}

int altura(No*raiz) {
    if (raiz == NULL) return 0;
    int altEsq = altura(raiz->esq);
    int altDir = altura(raiz->dir);
    return (altEsq > altDir ? altEsq : altDir) + 1;
}

void comprarProduto(No** arvoreCodigo, No** carrinho, int codigo){
    Produto* p = buscarPorCodigo(*arvoreCodigo, codigo);
    if (p != NULL && p->quantidade > 0) {
        p->quantidade--;
        *carrinho = inserirPorCodigo(*carrinho, *p);
    }
}

void devolverProduto(No*arvoreCodigo, No** carrinho, int codigo){
    Produto* p = buscarPorCodigo(arvoreCodigo, codigo);
    Produto* c = buscarPorCodigo(*carrinho, codigo);
    if (p != NULL && c != NULL){
        p->quantidade++;
    }
}

void gerarNome(char* nome, int id){
    sprintf(nome, "Produto_%d", id);
}

int main(){

    srand(time(NULL)); 

    No *arvoreCodigo = NULL, *arvorePreco = NULL, *carrinho = NULL;
    int totalProdutos = 10000;

    for (int i = 1; i <= totalProdutos; i++) {
        char nome[50];
        gerarNome(nome, i);
        
        int codigoAleatorio = rand() % (totalProdutos*10);
        float precoAleatorio = (rand() % 10000) / 100.0;
        int quantidadeAleatoria = rand() % 50 + 1;

        Produto p = criarProduto(
            codigoAleatorio, nome, precoAleatorio, quantidadeAleatoria
        );
        
        arvoreCodigo = inserirPorCodigo(arvoreCodigo, p);
        arvorePreco = inserirPorPreco(arvorePreco, p);
    }
    printf("Altura da arvore por codigo: %d\n", altura(arvoreCodigo));
    printf("Altura da arvore por preco: %d\n",altura(arvorePreco));

    for (int i = 0; i < 500; i++){
        int codigoCompra = rand() % totalProdutos + 1;
        comprarProduto(&arvoreCodigo, &carrinho, codigoCompra);
    }
    
    printf("Altura da arvore do carrinho: %d\n", altura(carrinho));

    return 0;
}
