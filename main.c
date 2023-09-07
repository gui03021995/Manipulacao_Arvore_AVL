/*
Nome do autor : Guilherme Tadeu Gonçalves Morais
Algoritmo desenvolvido para pratica de arvore binaria AVL,
aplicado então função de inserção e remoção, de balanceamento de arvore e
também rotação simples e composta, rotações nas quais são essencial
para balancear a arvore.
*/
#include <stdio.h>
#include <stdlib.h>


typedef struct no {
    int valor;
    struct no *esq , *dir;
    short alt;
}No;

/* Função que cria um novo nó
    x -> valor a ser inserido no nó
    Retorna: o endereço do nó criado
*/
No* novoNo(int x){
    No *novo = malloc (sizeof(No));

    if(novo){
        novo->valor = x;
        novo->esq = NULL;
        novo->dir = NULL;
        novo->alt = 0;
    }
    else
        printf("\nErro ao alocar nó em novoNo!\n");
    return novo;

}

/*
    Retorna o maior dentre dois valores
    a,b -> altura de dois nós da árvore
*/
short maior (short a, short b){
    return (a > b)? a: b;
}

//Retorna a altura de um nó ou -1 caso ele seja null
short alturaDoNo(No *no){
    if(no == NULL)
        return -1;
    else
        return no->alt;
}

//Calcula e retorna o fator de balanceamento de um nó
short fatorDeBalanceamento(No *no){
    if(no)
        return (alturaDoNo(no->esq) - alturaDoNo(no->dir));
    else
        return 0;
}

// ------------------------Rotações -----------------------

//funcao para a rotacao a esquerda (esq)

No* rotacaoEsquerda(No *r){
    No *y, *f;

    y = r->dir;
    f = y->esq;

    y->esq = r;
    r->dir = f;

    r->alt = maior(alturaDoNo(r->esq),alturaDoNo(r->dir)) + 1;
    y->alt = maior(alturaDoNo(y->esq),alturaDoNo(y->dir)) + 1;

    return y;
}

// Funcao para a rotação à direita
No* rotacaoDireita(No *r){
    No *y, *f;

    y = r->esq;
    f = y->dir;

    y->dir = r;
    r->esq = f;

    r->alt = maior(alturaDoNo(r->esq),alturaDoNo(r->dir)) + 1;
    y->alt = maior(alturaDoNo(y->esq),alturaDoNo(y->dir)) + 1;

    return y;
}

No* rotacaoEsquerdaDireita(No *r){
    r->esq = rotacaoEsquerda(r->esq);
    return rotacaoDireita(r);
}


No* rotacaoDireitaEsquerda(No *r){
    r->dir = rotacaoDireita(r->dir);
    return rotacaoEsquerda(r);
}

/*
    Funcao para realizar o balanceamento da árvore após uma inserção ou remoção
    recebe o nó que está desbalanceada e retorna a nova raiz após o balanceamento

*/

No* balancear(No *raiz){
    short fb = fatorDeBalanceamento(raiz);

    //Rotação à esquerda = esq
    if(fb < -1 && fatorDeBalanceamento(raiz-> dir) <= 0)
        raiz = rotacaoEsquerda(raiz);


    //Rotação à direita = dir
    else if(fb > 1 && fatorDeBalanceamento(raiz-> esq) >= 0)
        raiz = rotacaoDireita(raiz);

    //Rotação dupla à esquerda = esq
    else if(fb > 1 && fatorDeBalanceamento(raiz-> esq) < 0)
        raiz = rotacaoEsquerdaDireita(raiz);

    //Rotação dupla à direita = dir
    else if(fb < -1 && fatorDeBalanceamento(raiz-> dir) > 0)
        raiz = rotacaoDireitaEsquerda(raiz);

    return raiz;

}

/*
    Insere um novo nó na árvore
    raiz -> raiz da árvore
    x -> valor a ser inserido
    Retorno : endereço do novo nó ou nova raiz após o balanceamento
*/
No* inserir(No *raiz, int x){
    if(raiz == NULL) //ávore vazia
        return novoNo(x);
    else{ //inserção será à esquerda ou à direita
            if(x < raiz->valor)
                raiz->esq = inserir(raiz->esq, x);
            else if(x > raiz->valor)
                raiz->dir = inserir(raiz->dir,x);
            else
                printf("\nInsercao não realizada!\n O elemento %d a existe!\n",x);
    }

    //Recalcula a altura de todos os nós entre a raiz e o novo nó inserido
    raiz->alt = maior(alturaDoNo(raiz->esq), alturaDoNo(raiz->dir))+1;

    //Verifica a necessidade de rabalancear a árvore
    raiz = balancear(raiz);

    return raiz;
}
/*
    Função para remover um nó da Árvore binária
    Pode ser necessário rebalancear a árvore e a raiz pode ser alterada
    por isso precisamos retornar a raiz
*/

No* remover(No *raiz, int chave){
    if(raiz == NULL){
        printf("Valor não encontrado!\n");
        return NULL;
    } else { //procura o nó a remover
        if(raiz->valor == chave){
            //remove nós folhas (nós sem filhos)
            if(raiz->esq == NULL && raiz->dir == NULL){
                free(raiz);
                printf("Elemento folha removido: %d! \n", chave);
                return NULL;
            }
            else{
                //remover nós que possuem 2 filhos
                if(raiz->esq != NULL && raiz->dir != NULL){
                    No *aux = raiz ->esq;
                    while(aux->dir != NULL)
                        aux = aux->dir;
                    raiz->valor = aux -> valor;
                    aux-> valor = chave;
                    printf("Elemento trocado: %d !\n", chave);
                    raiz->esq = remover(raiz->esq, chave);
                    return raiz;
                }
            else {
                //remover nós que possuem apenas um filho
                No *aux;
                if(raiz->esq != NULL)
                    aux = raiz->esq;
                else
                    aux = raiz->dir;
                free(raiz);
                printf("Elemento com 1 filho removido: %d !\n");
                return aux;
            }
          }
        } else {
            if(chave < raiz -> valor)
                raiz -> esq = remover(raiz->esq, chave);
            else
                raiz -> dir = remover(raiz->dir , chave);
        }

        //Recalcula a altura de todos os nós entre a raiz e o novo nó inserido
        raiz->alt = maior(alturaDoNo(raiz->esq), alturaDoNo(raiz->dir)) + 1;

        //Verifica a necessidade de rebalancear a árvore
        raiz = balancear(raiz);

        return raiz;
    }
}
void imprimir(No *raiz, int nivel){
    int i;
    if(raiz){
        imprimir(raiz->dir, nivel +1 );
        printf("\n\n");

        for(i=0; i < nivel ; i++)
            printf("\t");

        printf("%d", raiz->valor);
        imprimir(raiz->esq, nivel + 1);
    }
}

int main(){
    int opcao, valor;
    No *raiz = NULL;

    do{
        printf("\n\n\t0 - Sair\n\t1 - Inserir\n\t2 - Remove\n\t3 - Imprimir\n\n");
        scanf("%d",&opcao);

        switch(opcao){
        case 0:
            printf("Saindo!!!");
            break;
        case 1 :
            printf("\tDigite o valor a ser inserido: ");
            scanf("%d",&valor);
            raiz = inserir(raiz, valor);
            break;
        case 2 :
            printf("\tDigite o valor a ser removido: ");
            scanf("%d", &valor);
            raiz = remover(raiz, valor);
            break;

        case 3 :
            imprimir(raiz, 1);
            break;
        default:
            printf("\nOpcao Invalida!!!\n");
        }
    }while(opcao != 0);
}
