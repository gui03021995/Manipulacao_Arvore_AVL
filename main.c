/*
Nome do autor : Guilherme Tadeu Gon�alves Morais
Algoritmo desenvolvido para pratica de arvore binaria AVL,
aplicado ent�o fun��o de inser��o e remo��o, de balanceamento de arvore e
tamb�m rota��o simples e composta, rota��es nas quais s�o essencial
para balancear a arvore.
*/
#include <stdio.h>
#include <stdlib.h>


typedef struct no {
    int valor;
    struct no *esq , *dir;
    short alt;
}No;

/* Fun��o que cria um novo n�
    x -> valor a ser inserido no n�
    Retorna: o endere�o do n� criado
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
        printf("\nErro ao alocar n� em novoNo!\n");
    return novo;

}

/*
    Retorna o maior dentre dois valores
    a,b -> altura de dois n�s da �rvore
*/
short maior (short a, short b){
    return (a > b)? a: b;
}

//Retorna a altura de um n� ou -1 caso ele seja null
short alturaDoNo(No *no){
    if(no == NULL)
        return -1;
    else
        return no->alt;
}

//Calcula e retorna o fator de balanceamento de um n�
short fatorDeBalanceamento(No *no){
    if(no)
        return (alturaDoNo(no->esq) - alturaDoNo(no->dir));
    else
        return 0;
}

// ------------------------Rota��es -----------------------

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

// Funcao para a rota��o � direita
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
    Funcao para realizar o balanceamento da �rvore ap�s uma inser��o ou remo��o
    recebe o n� que est� desbalanceada e retorna a nova raiz ap�s o balanceamento

*/

No* balancear(No *raiz){
    short fb = fatorDeBalanceamento(raiz);

    //Rota��o � esquerda = esq
    if(fb < -1 && fatorDeBalanceamento(raiz-> dir) <= 0)
        raiz = rotacaoEsquerda(raiz);


    //Rota��o � direita = dir
    else if(fb > 1 && fatorDeBalanceamento(raiz-> esq) >= 0)
        raiz = rotacaoDireita(raiz);

    //Rota��o dupla � esquerda = esq
    else if(fb > 1 && fatorDeBalanceamento(raiz-> esq) < 0)
        raiz = rotacaoEsquerdaDireita(raiz);

    //Rota��o dupla � direita = dir
    else if(fb < -1 && fatorDeBalanceamento(raiz-> dir) > 0)
        raiz = rotacaoDireitaEsquerda(raiz);

    return raiz;

}

/*
    Insere um novo n� na �rvore
    raiz -> raiz da �rvore
    x -> valor a ser inserido
    Retorno : endere�o do novo n� ou nova raiz ap�s o balanceamento
*/
No* inserir(No *raiz, int x){
    if(raiz == NULL) //�vore vazia
        return novoNo(x);
    else{ //inser��o ser� � esquerda ou � direita
            if(x < raiz->valor)
                raiz->esq = inserir(raiz->esq, x);
            else if(x > raiz->valor)
                raiz->dir = inserir(raiz->dir,x);
            else
                printf("\nInsercao n�o realizada!\n O elemento %d a existe!\n",x);
    }

    //Recalcula a altura de todos os n�s entre a raiz e o novo n� inserido
    raiz->alt = maior(alturaDoNo(raiz->esq), alturaDoNo(raiz->dir))+1;

    //Verifica a necessidade de rabalancear a �rvore
    raiz = balancear(raiz);

    return raiz;
}
/*
    Fun��o para remover um n� da �rvore bin�ria
    Pode ser necess�rio rebalancear a �rvore e a raiz pode ser alterada
    por isso precisamos retornar a raiz
*/

No* remover(No *raiz, int chave){
    if(raiz == NULL){
        printf("Valor n�o encontrado!\n");
        return NULL;
    } else { //procura o n� a remover
        if(raiz->valor == chave){
            //remove n�s folhas (n�s sem filhos)
            if(raiz->esq == NULL && raiz->dir == NULL){
                free(raiz);
                printf("Elemento folha removido: %d! \n", chave);
                return NULL;
            }
            else{
                //remover n�s que possuem 2 filhos
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
                //remover n�s que possuem apenas um filho
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

        //Recalcula a altura de todos os n�s entre a raiz e o novo n� inserido
        raiz->alt = maior(alturaDoNo(raiz->esq), alturaDoNo(raiz->dir)) + 1;

        //Verifica a necessidade de rebalancear a �rvore
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
