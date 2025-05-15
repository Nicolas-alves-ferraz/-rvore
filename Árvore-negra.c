#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------------------------- ÁRVORE AVL (Usuários) -------------------------

typedef struct Usuario {
    char nome[100];     // chave para ordenação
    int id;
    char email[100];
} Usuario;

typedef struct NodoAVL {
    Usuario usuario;
    int altura;
    struct NodoAVL* esquerda;
    struct NodoAVL* direita;
} NodoAVL;

int max(int a, int b) {
    return (a > b) ? a : b;
}

int altura(NodoAVL* nodo) {
    return (nodo == NULL) ? 0 : nodo->altura;
}

int fatorBalanceamento(NodoAVL* nodo) {
    return (nodo == NULL) ? 0 : altura(nodo->esquerda) - altura(nodo->direita);
}

NodoAVL* rotacaoDireita(NodoAVL* y) {
    NodoAVL* x = y->esquerda;
    NodoAVL* T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    y->altura = 1 + max(altura(y->esquerda), altura(y->direita));
    x->altura = 1 + max(altura(x->esquerda), altura(x->direita));

    return x;
}

NodoAVL* rotacaoEsquerda(NodoAVL* x) {
    NodoAVL* y = x->direita;
    NodoAVL* T2 = y->esquerda;

    y->esquerda = x;
    x->direita = T2;

    x->altura = 1 + max(altura(x->esquerda), altura(x->direita));
    y->altura = 1 + max(altura(y->esquerda), altura(y->direita));

    return y;
}

NodoAVL* inserirUsuario(NodoAVL* raiz, Usuario usuario) {
    if (raiz == NULL) {
        NodoAVL* novo = (NodoAVL*)malloc(sizeof(NodoAVL));
        novo->usuario = usuario;
        novo->altura = 1;
        novo->esquerda = novo->direita = NULL;
        return novo;
    }

    if (strcmp(usuario.nome, raiz->usuario.nome) < 0)
        raiz->esquerda = inserirUsuario(raiz->esquerda, usuario);
    else if (strcmp(usuario.nome, raiz->usuario.nome) > 0)
        raiz->direita = inserirUsuario(raiz->direita, usuario);
    else
        return raiz;

    raiz->altura = 1 + max(altura(raiz->esquerda), altura(raiz->direita));
    int fb = fatorBalanceamento(raiz);

    if (fb > 1 && strcmp(usuario.nome, raiz->esquerda->usuario.nome) < 0)
        return rotacaoDireita(raiz);
    if (fb < -1 && strcmp(usuario.nome, raiz->direita->usuario.nome) > 0)
        return rotacaoEsquerda(raiz);
    if (fb > 1 && strcmp(usuario.nome, raiz->esquerda->usuario.nome) > 0) {
        raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
        return rotacaoDireita(raiz);
    }
    if (fb < -1 && strcmp(usuario.nome, raiz->direita->usuario.nome) < 0) {
        raiz->direita = rotacaoDireita(raiz->direita);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

void listarUsuarios(NodoAVL* raiz) {
    if (raiz != NULL) {
        listarUsuarios(raiz->esquerda);
        printf("Nome: %s | ID: %d | Email: %s\n", raiz->usuario.nome, raiz->usuario.id, raiz->usuario.email);
        listarUsuarios(raiz->direita);
    }
}

// ------------------------- ÁRVORE RUBRO-NEGRA (Produtos) -------------------------

typedef enum { VERMELHO, PRETO } Cor;

typedef struct Produto {
    int codigo;  // chave
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
    if (x->pai == NULL) raiz = y;
    else if (x == x->pai->esquerda) x->pai->esquerda = y;
    else x->pai->direita = y;
    y->esquerda = x;
    x->pai = y;
    return raiz;
}

NodoRN* rotacaoDireitaRN(NodoRN* raiz, NodoRN* y) {
    NodoRN* x = y->esquerda;
    y->esquerda = x->direita;
    if (x->direita) x->direita->pai = y;
    x->pai = y->pai;
    if (y->pai == NULL) raiz = x;
    else if (y == y->pai->esquerda) y->pai->esquerda = x;
    else y->pai->direita = x;
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
               raiz->produto.codigo, raiz->produto.nome, raiz->produto.quantidade,
               raiz->produto.preco, raiz->cor == VERMELHO ? "Vermelho" : "Preto");
        listarProdutos(raiz->direita);
    }
}

// ------------------------- MAIN -------------------------

int main() {
    NodoAVL* raizUsuarios = NULL;
    NodoRN* raizProdutos = NULL;
    int opcao, subopcao;
    int idContador = 1;

    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Sistema de Usuários (AVL)\n");
        printf("2. Sistema de Inventário (Rubro-Negra)\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                do {
                    printf("\n--- Menu de Usuários ---\n");
                    printf("1. Cadastrar Usuário\n");
                    printf("2. Listar Usuários\n");
                    printf("0. Voltar\n");
                    printf("Escolha: ");
                    scanf("%d", &subopcao);
                    getchar();

                    if (subopcao == 1) {
                        Usuario u;
                        printf("Nome: ");
                        fgets(u.nome, 100, stdin);
                        u.nome[strcspn(u.nome, "\n")] = '\0';

                        printf("Email: ");
                        fgets(u.email, 100, stdin);
                        u.email[strcspn(u.email, "\n")] = '\0';

                        u.id = idContador++;
                        raizUsuarios = inserirUsuario(raizUsuarios, u);
                        printf("Usuário cadastrado!\n");
                     }   else if (subopcao == 2) {
                        if (raizUsuarios == NULL) {
                        printf("Nenhum usuário cadastrado.\n");
                     } else {
                         listarUsuarios(raizUsuarios);
                            }
                    }
                } while (subopcao != 0);
                break;

            case 2:
                do {
                    printf("\n--- Menu de Inventário ---\n");
                    printf("1. Cadastrar Produto\n");
                    printf("2. Listar Produtos\n");
                    printf("0. Voltar\n");
                    printf("Escolha: ");
                    scanf("%d", &subopcao);
                    getchar();

                    if (subopcao == 1) {
                        Produto p;
                        printf("Código: ");
                        scanf("%d", &p.codigo);
                        getchar();

                        printf("Nome: ");
                        fgets(p.nome, 100, stdin);
                        p.nome[strcspn(p.nome, "\n")] = '\0';

                        printf("Quantidade: ");
                        scanf("%d", &p.quantidade);
                        printf("Preço: ");
                        scanf("%f", &p.preco);

                        raizProdutos = inserirProduto(raizProdutos, p);
                        printf("Produto cadastrado!\n");
                    }   else if (subopcao == 2) {
                        if (raizProdutos == NULL) {
                        printf("Nenhum Produto cadastrado.\n");
                     } else {
                         listarProdutos(raizProdutos);
                            }
                    }
                } while (subopcao != 0);
                break;
        }
    } while (opcao != 0);

    printf("Programa finalizado.\n");
    return 0;
}
