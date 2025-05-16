#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------------------------- ÁRVORE RUBRO-NEGRA (Produtos) -------------------------
typedef enum { VERMELHO, PRETO } Cor;

typedef struct Produto {
    int codigo;
    char nome[100];
    int quantidade;
    float preco;
} Produto;

typedef struct NodoRN {
    Produto produto;
    Cor cor;
    struct NodoRN* esquerda;
    struct NodoRN* direita;
    struct NodoRN* pai;
} NodoRN;

int ehPreto(NodoRN* n) {
    return n == NULL || n->cor == PRETO;
}

NodoRN* criarProduto(Produto produto) {
    NodoRN* novo = (NodoRN*)malloc(sizeof(NodoRN));
    novo->produto = produto;
    novo->cor = VERMELHO;
    novo->esquerda = novo->direita = novo->pai = NULL;
    return novo;
}

NodoRN* rotacaoEsquerdaRN(NodoRN* raiz, NodoRN* x) {
    NodoRN* y = x->direita;
    x->direita = y->esquerda;
    if (y->esquerda) y->esquerda->pai = x;
    y->pai = x->pai;

    if (x->pai == NULL)
        raiz = y;
    else if (x == x->pai->esquerda)
        x->pai->esquerda = y;
    else
        x->pai->direita = y;

    y->esquerda = x;
    x->pai = y;

    return raiz;
}

NodoRN* rotacaoDireitaRN(NodoRN* raiz, NodoRN* y) {
    NodoRN* x = y->esquerda;
    y->esquerda = x->direita;
    if (x->direita) x->direita->pai = y;
    x->pai = y->pai;

    if (y->pai == NULL)
        raiz = x;
    else if (y == y->pai->esquerda)
        y->pai->esquerda = x;
    else
        y->pai->direita = x;

    x->direita = y;
    y->pai = x;

    return raiz;
}

NodoRN* corrigirInsercao(NodoRN* raiz, NodoRN* z) {
    while (z->pai && z->pai->cor == VERMELHO) {
        if (z->pai == z->pai->pai->esquerda) {
            NodoRN* y = z->pai->pai->direita;
            if (y && y->cor == VERMELHO) {
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->direita) {
                    z = z->pai;
                    raiz = rotacaoEsquerdaRN(raiz, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                raiz = rotacaoDireitaRN(raiz, z->pai->pai);
            }
        } else {
            NodoRN* y = z->pai->pai->esquerda;
            if (y && y->cor == VERMELHO) {
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->esquerda) {
                    z = z->pai;
                    raiz = rotacaoDireitaRN(raiz, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                raiz = rotacaoEsquerdaRN(raiz, z->pai->pai);
            }
        }
    }
    raiz->cor = PRETO;
    return raiz;
}

NodoRN* inserirProduto(NodoRN* raiz, Produto produto) {
    if (buscarProduto(raiz, produto.codigo) != NULL) {
        printf("Código já existente! Produto não inserido.\n");
        return raiz;
    }

    NodoRN* novo = criarProduto(produto);
    NodoRN* y = NULL;
    NodoRN* x = raiz;

    while (x != NULL) {
        y = x;
        if (produto.codigo < x->produto.codigo)
            x = x->esquerda;
        else
            x = x->direita;
    }

    novo->pai = y;
    if (y == NULL)
        raiz = novo;
    else if (produto.codigo < y->produto.codigo)
        y->esquerda = novo;
    else
        y->direita = novo;

    return corrigirInsercao(raiz, novo);
}

void listarProdutos(NodoRN* raiz) {
    if (raiz != NULL) {
        listarProdutos(raiz->esquerda);
        printf("Código: %d | Nome: %s | Qtd: %d | Preço: %.2f | Cor: %s\n",
            raiz->produto.codigo, raiz->produto.nome,
            raiz->produto.quantidade, raiz->produto.preco,
            raiz->cor == VERMELHO ? "Vermelho" : "Preto");
        listarProdutos(raiz->direita);
    }
}

NodoRN* buscarProduto(NodoRN* raiz, int codigo) {
    if (raiz == NULL || raiz->produto.codigo == codigo)
        return raiz;
    if (codigo < raiz->produto.codigo)
        return buscarProduto(raiz->esquerda, codigo);
    else
        return buscarProduto(raiz->direita, codigo);
}

NodoRN* minimoNodoRN(NodoRN* nodo) {
    while (nodo->esquerda != NULL)
        nodo = nodo->esquerda;
    return nodo;
}

NodoRN* substituirNodoRN(NodoRN* raiz, NodoRN* u, NodoRN* v) {
    if (u->pai == NULL)
        raiz = v;
    else if (u == u->pai->esquerda)
        u->pai->esquerda = v;
    else
        u->pai->direita = v;

    if (v != NULL)
        v->pai = u->pai;

    return raiz;
}

NodoRN* corrigirRemocao(NodoRN* raiz, NodoRN* x) {
    while (x != raiz && ehPreto(x)) {
        if (x == x->pai->esquerda) {
            NodoRN* w = x->pai->direita;
            if (w->cor == VERMELHO) {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                raiz = rotacaoEsquerdaRN(raiz, x->pai);
                w = x->pai->direita;
            }
            if (ehPreto(w->esquerda) && ehPreto(w->direita)) {
                w->cor = VERMELHO;
                x = x->pai;
            } else {
                if (ehPreto(w->direita)) {
                    if (w->esquerda) w->esquerda->cor = PRETO;
                    w->cor = VERMELHO;
                    raiz = rotacaoDireitaRN(raiz, w);
                    w = x->pai->direita;
                }
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                if (w->direita) w->direita->cor = PRETO;
                raiz = rotacaoEsquerdaRN(raiz, x->pai);
                x = raiz;
            }
        } else {
            NodoRN* w = x->pai->esquerda;
            if (w->cor == VERMELHO) {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                raiz = rotacaoDireitaRN(raiz, x->pai);
                w = x->pai->esquerda;
            }
            if (ehPreto(w->esquerda) && ehPreto(w->direita)) {
                w->cor = VERMELHO;
                x = x->pai;
            } else {
                if (ehPreto(w->esquerda)) {
                    if (w->direita) w->direita->cor = PRETO;
                    w->cor = VERMELHO;
                    raiz = rotacaoEsquerdaRN(raiz, w);
                    w = x->pai->esquerda;
                }
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                if (w->esquerda) w->esquerda->cor = PRETO;
                raiz = rotacaoDireitaRN(raiz, x->pai);
                x = raiz;
            }
        }
    }
    if (x) x->cor = PRETO;
    return raiz;
}

NodoRN* removerProduto(NodoRN* raiz, int codigo) {
    NodoRN* z = buscarProduto(raiz, codigo);
    if (z == NULL) {
        printf("Produto não encontrado para remoção.\n");
        return raiz;
    }

    NodoRN* y = z;
    Cor corOriginal = y->cor;
    NodoRN* x;

    if (z->esquerda == NULL) {
        x = z->direita;
        raiz = substituirNodoRN(raiz, z, z->direita);
    } else if (z->direita == NULL) {
        x = z->esquerda;
        raiz = substituirNodoRN(raiz, z, z->esquerda);
    } else {
        y = minimoNodoRN(z->direita);
        corOriginal = y->cor;
        x = y->direita;

        if (y->pai == z) {
            if (x) x->pai = y;
        } else {
            raiz = substituirNodoRN(raiz, y, y->direita);
            y->direita = z->direita;
            y->direita->pai = y;
        }

        raiz = substituirNodoRN(raiz, z, y);
        y->esquerda = z->esquerda;
        y->esquerda->pai = y;
        y->cor = z->cor;
    }

    free(z);
    if (corOriginal == PRETO)
        raiz = corrigirRemocao(raiz, x);
    return raiz;
}

// ------------------------- MAIN -------------------------
int main() {
    NodoRN* raizProdutos = NULL;
    int opcao, subopcao;

    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Sistema de Inventário (Rubro-Negra)\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                do {
                    printf("\n--- Menu de Inventário ---\n");
                    printf("1. Cadastrar Produto\n");
                    printf("2. Listar Produtos\n");
                    printf("3. Buscar Produto\n");
                    printf("4. Remover Produto\n");
                    printf("0. Voltar\n");
                    printf("Escolha: ");
                    scanf("%d", &subopcao);
                    getchar();

                    if (subopcao == 1) {
                        Produto p;
                        printf("Código: ");
                        scanf("%d", &p.codigo); getchar();
                        printf("Nome: ");
                        fgets(p.nome, 100, stdin);
                        p.nome[strcspn(p.nome, "\n")] = '\0';
                        printf("Quantidade: ");
                        scanf("%d", &p.quantidade); getchar();
                        printf("Preço: ");
                        scanf("%f", &p.preco); getchar();
                        raizProdutos = inserirProduto(raizProdutos, p);
                    } else if (subopcao == 2) {
                        if (raizProdutos == NULL)
                            printf("Nenhum produto cadastrado.\n");
                        else
                            listarProdutos(raizProdutos);
                    } else if (subopcao == 3) {
                        int codigoBusca;
                        printf("Digite o código do produto para buscar: ");
                        scanf("%d", &codigoBusca); getchar();
                        NodoRN* produtoEncontrado = buscarProduto(raizProdutos, codigoBusca);
                        if (produtoEncontrado) {
                            printf("Produto encontrado:\n");
                            printf("Código: %d | Nome: %s | Qtd: %d | Preço: %.2f | Cor: %s\n",
                                   produtoEncontrado->produto.codigo, produtoEncontrado->produto.nome,
                                   produtoEncontrado->produto.quantidade, produtoEncontrado->produto.preco,
                                   produtoEncontrado->cor == VERMELHO ? "Vermelho" : "Preto");
                        } else {
                            printf("Produto não encontrado.\n");
                        }
                    } else if (subopcao == 4) {
                        int codigoRemover;
                        printf("Digite o código do produto para remover: ");
                        scanf("%d", &codigoRemover); getchar();
                        raizProdutos = removerProduto(raizProdutos, codigoRemover);
                    }
                } while (subopcao != 0);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
