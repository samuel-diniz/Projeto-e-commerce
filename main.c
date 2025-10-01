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
        printf("Codigo: %d | Nome: %s | Preco: %.2f | Quantidade: %d\n", 
            raiz->item.codigo, raiz->item.nome,
        raiz->item.preco, raiz->item.quantidade);
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

void importarProdutos(const char *nomeArquivo, No **arvoreCodigo, No **arvorePreco){ 
    FILE *fp = fopen(nomeArquivo, "r"); 
    if (!fp){ 
        perror("Erro abrindo arquivo CSV"); 
        return; 
    }

    char linha[256];
    fgets(linha, sizeof(linha), fp);

    while (fgets(linha,sizeof(linha), fp)){
        Produto p;
        char *token;
        linha[strcspn(linha, "\n")] = 0;

        token = strtok(linha, ",");
        if (token == NULL) continue;
        p.codigo = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strncpy(p.nome, token, 49);
        p.nome[49] = '\0';

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        p.preco = atof(token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        p.quantidade = atoi(token);

        *arvoreCodigo = inserirPorCodigo(*arvoreCodigo, p);
        *arvorePreco = inserirPorPreco(*arvorePreco, p);
    }
    fclose(fp);
}

int main(){

    srand(time(NULL)); 

    No *arvoreCodigo = NULL, *arvorePreco = NULL, *carrinho = NULL;
    int totalProdutos = 800;

    /*
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
    } */

    importarProdutos("C:\\Users\\cdcon\\ProjetoUnicid\\produtos_pecas_carro.csv", &arvoreCodigo, &arvorePreco);

    printf("Altura da arvore por codigo: %d\n", altura(arvoreCodigo));
    printf("Altura da arvore por preco: %d\n",altura(arvorePreco));

    printf("\n--- Lista de Produtos (Ordenada por codigo) ---\n");
    listarProduto(arvoreCodigo);

    for (int i = 0; i < 500; i++){
        int codigoCompra = rand() % totalProdutos + 1;
        comprarProduto(&arvoreCodigo, &carrinho, codigoCompra);
    }
    
    printf("Altura da arvore do carrinho: %d\n", altura(carrinho));

    printf("\n--- Conteudo do Carrinho ---\n");
    listarProduto(carrinho);

    return 0;
}
