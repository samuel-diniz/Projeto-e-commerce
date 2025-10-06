#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct Produto {
    int codigo;
    char nome[50];
    float preco;
    int quantidade;
} Produto;

typedef struct No {
    Produto item;
    struct No *esq;
    struct No *dir;
} No;

Produto criarProduto(int codigo, char nome[], float preco, int quantidade) {
    Produto p;
    p.codigo = codigo;
    strcpy(p.nome, nome);
    p.preco = preco;
    p.quantidade = quantidade;
    return p;
}

No* criarNo(Produto p) {
    No* novo = (No*) malloc(sizeof(No));
    novo->item = p;
    novo->esq = novo->dir = NULL;
    return novo;
}

No* inserirPorCodigo(No* raiz, Produto p) {
    if (raiz == NULL) return criarNo(p);
    if (p.codigo < raiz->item.codigo)
        raiz->esq = inserirPorCodigo(raiz->esq, p);
    else if (p.codigo > raiz->item.codigo)
        raiz->dir = inserirPorCodigo(raiz->dir, p);
    return raiz;
}

No* inserirPorPreco(No* raiz, Produto p) {
    if (raiz == NULL) return criarNo(p);
    if (p.preco < raiz->item.preco)
        raiz->esq = inserirPorPreco(raiz->esq, p);
    else if (p.preco > raiz->item.preco)
        raiz->dir = inserirPorPreco(raiz->dir, p);
    return raiz;
}

Produto* buscarPorCodigo(No* raiz, int codigo) {
    if (raiz == NULL) return NULL;
    if (codigo == raiz->item.codigo)
        return &raiz->item;
    if (codigo < raiz->item.codigo)
        return buscarPorCodigo(raiz->esq, codigo);
    else
        return buscarPorCodigo(raiz->dir, codigo);
}

int altura(No* raiz) {
    if (raiz == NULL) return 0;
    int altEsq = altura(raiz->esq);
    int altDir = altura(raiz->dir);
    return (altEsq > altDir ? altEsq : altDir) + 1;
}

void listarProduto(No* raiz) {
    if (raiz != NULL) {
        listarProduto(raiz->esq);
        printf("Codigo: %d | Nome: %s | Preco: R$ %.2f | Quantidade: %d\n",
            raiz->item.codigo, raiz->item.nome,
            raiz->item.preco, raiz->item.quantidade);
        listarProduto(raiz->dir);
    }
}

void comprarProduto(No** arvoreCodigo, No** carrinho, int codigo) {
    Produto* p = buscarPorCodigo(*arvoreCodigo, codigo);
    if (p != NULL && p->quantidade > 0) {
        p->quantidade--;

        Produto* jaNoCarrinho = buscarPorCodigo(*carrinho, codigo);
        if (jaNoCarrinho == NULL) {
            *carrinho = inserirPorCodigo(*carrinho, *p);
        }

        printf("Produto '%s' adicionado ao carrinho!\n", p->nome);
    } else {
        printf("Produto nao encontrado ou sem estoque.\n");
    }
}

No* encontrarMinimo(No* raiz) {
    while (raiz && raiz->esq != NULL)
        raiz = raiz->esq;
    return raiz;
}

No* removerPorCodigo(No* raiz, int codigo) {
    if (raiz == NULL) return NULL;

    if (codigo < raiz->item.codigo)
        raiz->esq = removerPorCodigo(raiz->esq, codigo);
    else if (codigo > raiz->item.codigo)
        raiz->dir = removerPorCodigo(raiz->dir, codigo);
    else {
        if (raiz->esq == NULL) {
            No* temp = raiz->dir;
            free(raiz);
            return temp;
        } else if (raiz->dir == NULL) {
            No* temp = raiz->esq;
            free(raiz);
            return temp;
        }

        No* temp = encontrarMinimo(raiz->dir);
        raiz->item = temp->item;
        raiz->dir = removerPorCodigo(raiz->dir, temp->item.codigo);
    }
    return raiz;
}

void devolverProduto(No** arvoreCodigo, No** carrinho, int codigo) {
    Produto* p = buscarPorCodigo(*arvoreCodigo, codigo);
    Produto* c = buscarPorCodigo(*carrinho, codigo);

    if (p != NULL && c != NULL) {
        p->quantidade++;
        *carrinho = removerPorCodigo(*carrinho, codigo);
        printf("Produto '%s' devolvido ao estoque.\n", p->nome);
    } else {
        printf("Produto nao encontrado no carrinho.\n");
    }
}

void importarProdutos(const char *nomeArquivo, No **arvoreCodigo, No **arvorePreco) {
    FILE *fp = fopen(nomeArquivo, "r");
    if (!fp) {
        perror("Erro abrindo arquivo CSV");
        return;
    }

    char linha[256];
    fgets(linha, sizeof(linha), fp); 

    while (fgets(linha, sizeof(linha), fp)) {
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

int main() {
    
    srand(time(NULL));

    No *arvoreCodigo = NULL, *arvorePreco = NULL, *carrinho = NULL;

    importarProdutos("C:\\Users\\cdcon\\ProjetoUnicid\\produtos_pecas_carro.csv", &arvoreCodigo, &arvorePreco);

    int opcao, codigo;

    do {
        printf("\n===== MENU E-COMMERCE =====\n");
        printf("1. Listar produtos\n");
        printf("2. Adicionar produto ao carrinho\n");
        printf("3. Ver carrinho\n");
        printf("4. Devolver produto ao estoque\n");
        printf("5. Ver alturas das arvores\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        printf("\n");

        switch(opcao) {
            case 1:
                printf("--- Produtos disponiveis ---\n");
                listarProduto(arvoreCodigo);
                break;

            case 2:
                printf("Digite o codigo do produto para adicionar: ");
                scanf("%d", &codigo);
                comprarProduto(&arvoreCodigo, &carrinho, codigo);
                break;

            case 3:
                printf("--- Carrinho ---\n");
                listarProduto(carrinho);
                break;

            case 4:
                printf("Digite o codigo do produto para devolver: ");
                scanf("%d", &codigo);
                devolverProduto(&arvoreCodigo, &carrinho, codigo);
                break;

            case 5:
                printf("Altura da arvore por codigo: %d\n", altura(arvoreCodigo));
                printf("Altura da arvore por preco: %d\n", altura(arvorePreco));
                printf("Altura da arvore do carrinho: %d\n", altura(carrinho));
                break;

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

        printf("\n");
        system("pause");
        system("cls");

    } while (opcao != 0);

    return 0;
}
