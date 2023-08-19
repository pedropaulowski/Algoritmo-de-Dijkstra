#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define INF INT_MAX / 2

typedef struct
{
    int vertice;
    int distancia;
} Node;

typedef struct
{
    Node *array;
    int tamanho;
    int capacidade;
} MinHeap;

typedef struct AdjListNode
{
    int destino;
    int peso;
    struct AdjListNode *proximo;
} AdjListNode;

typedef struct
{
    AdjListNode *cabeca;
} ListaAdj;

MinHeap *criarMinHeap(int capacidade)
{
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    heap->array = (Node *)malloc(capacidade * sizeof(Node));
    heap->tamanho = 0;
    heap->capacidade = capacidade;
    return heap;
}

void trocarNodes(Node *a, Node *b)
{
    Node temp = *a;
    *a = *b;
    *b = temp;
}

void heapificar(MinHeap *heap, int idx)
{
    int menor = idx;
    int esquerda = 2 * idx + 1;
    int direita = 2 * idx + 2;

    if (esquerda < heap->tamanho && heap->array[esquerda].distancia < heap->array[menor].distancia)
        menor = esquerda;

    if (direita < heap->tamanho && heap->array[direita].distancia < heap->array[menor].distancia)
        menor = direita;

    if (menor != idx)
    {
        trocarNodes(&heap->array[idx], &heap->array[menor]);
        heapificar(heap, menor);
    }
}

void inserir(MinHeap *heap, int vertice, int distancia)
{
    if (heap->tamanho == heap->capacidade)
        return;

    Node novoNode;
    novoNode.vertice = vertice;
    novoNode.distancia = distancia;

    int i = heap->tamanho;
    heap->tamanho++;
    heap->array[i] = novoNode;

    while (i != 0 && heap->array[(i - 1) / 2].distancia > heap->array[i].distancia)
    {
        trocarNodes(&heap->array[i], &heap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

Node extrairMin(MinHeap *heap)
{
    if (heap->tamanho == 0)
    {
        Node NodeVazio = {-1, -1};
        return NodeVazio;
    }

    Node raiz = heap->array[0];
    heap->array[0] = heap->array[heap->tamanho - 1];
    heap->tamanho--;
    heapificar(heap, 0);
    return raiz;
}

void dijkstra(ListaAdj *grafo, int numVertices, int verticeInicial)
{
    int distancias[numVertices];
    bool visitados[numVertices];
    int anteriores[numVertices];

    for (int i = 0; i < numVertices; i++)
    {
        distancias[i] = INF;
        visitados[i] = false;
        anteriores[i] = -1;
    }

    distancias[verticeInicial] = 0;
    MinHeap *heap = criarMinHeap(numVertices);
    inserir(heap, verticeInicial, 0);

    while (heap->tamanho > 0)
    {
        Node atual = extrairMin(heap);
        int verticeAtual = atual.vertice;
        visitados[verticeAtual] = true;

        for (AdjListNode *vizinhoAtual = grafo[verticeAtual].cabeca; vizinhoAtual != NULL; vizinhoAtual = vizinhoAtual->proximo)
        {
            int vizinho = vizinhoAtual->destino;
            int peso = vizinhoAtual->peso;
            if (!visitados[vizinho] && distancias[verticeAtual] + peso < distancias[vizinho])
            {
                distancias[vizinho] = distancias[verticeAtual] + peso;
                anteriores[vizinho] = verticeAtual;
                inserir(heap, vizinho, distancias[vizinho]);
            }
        }
    }

    printf("Vertice\tDistancia\tCaminho\n");
    for (int i = 0; i < numVertices; i++)
    {
        printf("%d\t%d\t\t", i, distancias[i]);
        int atual = i;
        while (anteriores[atual] != -1)
        {
            printf("%d <- ", atual);
            atual = anteriores[atual];
        }
        printf("%d\n", verticeInicial);
    }
}

int main()
{
    int numVertices;
    printf("Informe o numero de vertices: ");
    scanf("%d", &numVertices);

    ListaAdj *grafo = (ListaAdj *)malloc(numVertices * sizeof(ListaAdj));
    for (int i = 0; i < numVertices; i++)
    {
        grafo[i].cabeca = NULL;
    }

    printf("Informe a matriz de adjacencias do grafo (%d x %d):\n", numVertices, numVertices);
    for (int i = 0; i < numVertices; i++)
    {
        for (int j = 0; j < numVertices; j++)
        {
            int peso;
            scanf("%d", &peso);
            if (peso > 0)
            {
                AdjListNode *novoNo = (AdjListNode *)malloc(sizeof(AdjListNode));
                novoNo->destino = j;
                novoNo->peso = peso;
                novoNo->proximo = grafo[i].cabeca;
                grafo[i].cabeca = novoNo;
            }
        }
    }

    int verticeInicial;
    printf("Informe o vertice de partida: ");
    scanf("%d", &verticeInicial);

    dijkstra(grafo, numVertices, verticeInicial);

    for (int i = 0; i < numVertices; i++)
    {
        AdjListNode *atual = grafo[i].cabeca;
        while (atual != NULL)
        {
            AdjListNode *proximo = atual->proximo;
            free(atual);
            atual = proximo;
        }
    }
    free(grafo);

    return 0;
}



