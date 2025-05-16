#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Usuario {
    char nome[100];
    int id;
    char email[100];
} Usuario;

typedef struct NodoAVL {
    Usuario usuario;
    struct NodoAVL *esq, *dir;
    int altura;
} NodoAVL;

int max(int a, int b) {
    return (a > b) ? a : b;
}

int altura(NodoAVL *nodo) {
    return nodo ? nodo->altura : 0;
}

int fatorBalanceamento(NodoAVL *nodo) {
    return nodo ? altura(nodo->esq) - altura(nodo->dir) : 0;
}

NodoAVL *rotacaoDireita(NodoAVL *y) {
    NodoAVL *x = y->esq;
    NodoAVL *T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    y->altura = max(altura(y->esq), altura(y->dir)) + 1;
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;

    return x;
}

NodoAVL *rotacaoEsquerda(NodoAVL *x) {
    NodoAVL *y = x->dir;
    NodoAVL *T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    x->altura = max(altura(x->esq), altura(x->dir)) + 1;
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;

    return y;
}

NodoAVL *criarNodo(Usuario u) {
    NodoAVL *nodo = (NodoAVL *)malloc(sizeof(NodoAVL));
    nodo->usuario = u;
    nodo->esq = nodo->dir = NULL;
    nodo->altura = 1;
    return nodo;
}

NodoAVL *inserir(NodoAVL *nodo, Usuario u) {
    if (!nodo) return criarNodo(u);

    if (strcmp(u.nome, nodo->usuario.nome) < 0)
        nodo->esq = inserir(nodo->esq, u);
    else if (strcmp(u.nome, nodo->usuario.nome) > 0)
        nodo->dir = inserir(nodo->dir, u);
    else {
        printf("\nNome já existente!\n");
        return nodo;
    }

    nodo->altura = 1 + max(altura(nodo->esq), altura(nodo->dir));
    int balance = fatorBalanceamento(nodo);

    // Casos de rotação
    if (balance > 1 && strcmp(u.nome, nodo->esq->usuario.nome) < 0)
        return rotacaoDireita(nodo);
    if (balance < -1 && strcmp(u.nome, nodo->dir->usuario.nome) > 0)
        return rotacaoEsquerda(nodo);
    if (balance > 1 && strcmp(u.nome, nodo->esq->usuario.nome) > 0) {
        nodo->esq = rotacaoEsquerda(nodo->esq);
        return rotacaoDireita(nodo);
    }
    if (balance < -1 && strcmp(u.nome, nodo->dir->usuario.nome) < 0) {
        nodo->dir = rotacaoDireita(nodo->dir);
        return rotacaoEsquerda(nodo);
    }

    return nodo;
}

NodoAVL *minValorNodo(NodoAVL *nodo) {
    NodoAVL *atual = nodo;
    while (atual->esq)
        atual = atual->esq;
    return atual;
}

NodoAVL *remover(NodoAVL *raiz, char nome[]) {
    if (!raiz) return raiz;

    if (strcmp(nome, raiz->usuario.nome) < 0)
        raiz->esq = remover(raiz->esq, nome);
    else if (strcmp(nome, raiz->usuario.nome) > 0)
        raiz->dir = remover(raiz->dir, nome);
    else {
        if (!raiz->esq || !raiz->dir) {
            NodoAVL *temp = raiz->esq ? raiz->esq : raiz->dir;
            if (!temp) {
                temp = raiz;
                raiz = NULL;
            } else {
                *raiz = *temp;
            }
            free(temp);
        } else {
            NodoAVL *temp = minValorNodo(raiz->dir);
            raiz->usuario = temp->usuario;
            raiz->dir = remover(raiz->dir, temp->usuario.nome);
        }
    }

    if (!raiz) return raiz;

    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));
    int balance = fatorBalanceamento(raiz);

    // Casos de rotação após remoção
    if (balance > 1 && fatorBalanceamento(raiz->esq) >= 0)
        return rotacaoDireita(raiz);
    if (balance > 1 && fatorBalanceamento(raiz->esq) < 0) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }
    if (balance < -1 && fatorBalanceamento(raiz->dir) <= 0)
        return rotacaoEsquerda(raiz);
    if (balance < -1 && fatorBalanceamento(raiz->dir) > 0) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

NodoAVL *buscar(NodoAVL *raiz, char nome[]) {
    if (!raiz || strcmp(nome, raiz->usuario.nome) == 0)
        return raiz;
    if (strcmp(nome, raiz->usuario.nome) < 0)
        return buscar(raiz->esq, nome);
    return buscar(raiz->dir, nome);
}

void listarEmOrdem(NodoAVL *raiz) {
    if (raiz) {
        listarEmOrdem(raiz->esq);
        printf("Nome: %s | ID: %d | Email: %s\n", raiz->usuario.nome, raiz->usuario.id, raiz->usuario.email);
        listarEmOrdem(raiz->dir);
    }
}

int main() {
    NodoAVL *raiz = NULL;
    int opcao, id;
    char nome[100], email[100];

    do {
        printf("\n1 - Cadastrar\n2 - Remover\n3 - Buscar\n4 - Listar\n0 - Sair\n> ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                printf("Nome: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0;

                printf("ID: ");
                scanf("%d", &id);
                getchar();

                printf("Email: ");
                fgets(email, sizeof(email), stdin);
                email[strcspn(email, "\n")] = 0;

                Usuario u = {"", id, ""};
                strcpy(u.nome, nome);
                strcpy(u.email, email);

                raiz = inserir(raiz, u);
                break;

            case 2:
                printf("Nome a remover: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0;

                raiz = remover(raiz, nome);
                break;

            case 3:
                printf("Nome a buscar: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0;

                NodoAVL *resultado = buscar(raiz, nome);
                if (resultado)
                    printf("Encontrado: Nome: %s | ID: %d | Email: %s\n", resultado->usuario.nome, resultado->usuario.id, resultado->usuario.email);
                else
                    printf("Usuário não encontrado.\n");
                break;

            case 4:
                if (raiz)
                    listarEmOrdem(raiz);
                else
                    printf("Nenhum usuário cadastrado.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}
