/*******************************************
 * Implementação de biblioteca para grafos.
 *
 * Algoritmos em Grafos e Otimização
 * Departamento de Informática - UFPR
 * prof. Guilherme Derenievicz
 *******************************************/

#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
//---------------------------------------------------------
// getters:

int vertice_id(vertice v)
{
  return v->id;
}
char *vertice_rotulo(vertice v)
{
  return v->rotulo;
}
int vertice_particao(vertice v)
{
  return v->particao;
}
int custo(vertice v)
{
  return v->custo;
}
int estado(vertice v)
{
  return v->estado;
}
vertice pai(vertice v)
{
  return v->pai;
}
lista fronteira_entrada(vertice v)
{
  return v->fronteira_entrada;
}
lista fronteira_saida(vertice v)
{
  return v->fronteira_saida;
}
int aresta_id(aresta e)
{
  return e->id;
}
vertice vertice_u(aresta e)
{
  return e->u;
}
vertice vertice_v(aresta e)
{
  return e->v;
}
int grafo_id(grafo G)
{
  return G->id;
}
lista vertices(grafo G)
{
  return G->vertices;
}
lista arestas(grafo G)
{
  return G->arestas;
}

//---------------------------------------------------------
// funcoes para construcao/desconstrucao do grafo:

// cria grafo vazio e o retorna
grafo cria_grafo(int id)
{
  printf("entrou \n");
  grafo G = (grafo)malloc(sizeof(t_grafo));
  if (!G)
    exit(1);
  G->id = id; // Atribua o valor de id ao grafo
  G->vertices = cria_lista();
  G->arestas = cria_lista();

  // Agora você pode imprimir o valor de id após atribuí-lo ao grafo
  printf("%d\n", grafo_id(G));

  return G;
}

// destroi grafo G (desaloca toda a memoria)
void destroi_grafo(grafo G)
{
  lista vertices_list = vertices(G);
  lista aresta_list = arestas(G);
  while (!vazio(vertices_list))
  {
    desempilha(vertices_list);
  }
  while (!vazio(aresta_list))
  {
    desempilha(aresta_list);
  }
}

void adiciona_vertice(int id, char *rotulo, int particao, grafo G) {
  // Verificar se o grafo é nulo
  if (G == NULL) {
    fprintf(stderr, "Erro: Grafo é nulo.\n");
    return;
  }


vertice v = (vertice)busca_chave_str(rotulo, vertices(G), (char *(*)(obj))vertice_rotulo);

if (v == NULL){
  // O vértice não foi encontrado
  vertice v = (vertice)malloc(sizeof(t_vertice));
  //printf("Vértice com rótulo %s não encontrado.\n", rotulo);

  v->id = id;
  strcpy(v->rotulo, rotulo);
  v->particao = particao;
  v->custo = 100000;
  v->estado = ABERTO;
  v->pai = NULL;
  v->fronteira_entrada = cria_lista();
  v->fronteira_saida = cria_lista();
  v->valido = false;
  empilha(v, vertices(G));
 
}
}

void remove_vertice(int id, grafo G)
{
  vertice v = busca_chave_int(id, vertices(G), (int (*)(void *))vertice_id);
  if (v)
  {
    // Remove and destroy incident edges
    lista fronteira_list_entrada = fronteira_entrada(v);
    lista fronteira_list_saida = fronteira_saida(v);
    while (!vazio(fronteira_list_saida))
    {
      aresta e = desempilha(fronteira_list_saida);
      remove_aresta(e->id, G);
      // Não é necessário liberar a memória da aresta aqui, pois isso será feito posteriormente
    }
    while (!vazio(fronteira_list_entrada))
    {
      aresta e = desempilha(fronteira_list_entrada);
      remove_aresta(e->id, G);
      // Não é necessário liberar a memória da aresta aqui, pois isso será feito posteriormente
    }
    free(fronteira_list_entrada); // Libere a memória da lista de fronteira
    free(fronteira_list_saida);
    // Remove the vertex from the graph
    remove_chave_int(id, vertices(G), (int (*)(void *))vertice_id);
    free(v); // Libere a memória do vértice
  }
}

// cria aresta com id <id> incidente a vertices com ids
// <u_id> e <v_id> e adiciona ao grafo G
void adiciona_aresta(int id, int u_id, int v_id, grafo G)
{
  aresta a = (aresta)malloc(sizeof(t_aresta));

  a->id = id;
  a->u = busca_chave_int(u_id, vertices(G), (int_f_obj)vertice_id);
  a->v = busca_chave_int(v_id, vertices(G), (int_f_obj)vertice_id);
  empilha(a, arestas(G));
  empilha(a, fronteira_saida(a->u));
  empilha(a, fronteira_entrada(a->v));

}
// remove aresta com id <id> do grafo G e a destroi
void remove_aresta(int id, grafo G)
{
  aresta a = busca_chave_int(id, arestas(G), (int (*)(void *))aresta_id);

  if (a)
  {
    remove_chave_int(id, arestas(G), (int (*)(void *))aresta_id);
    remove_chave_int(id, fronteira_entrada(vertice_u(a)), (int (*)(void *))aresta_id);
    remove_chave_int(id, fronteira_saida(vertice_v(a)), (int (*)(void *))aresta_id);

    free(a);
  }
}

//---------------------------------------------------------
// funcoes para operacoes com o grafo pronto:

// calcula e devolve os graus do vertice v
int grau_entrada(vertice v)
{
  int d_v = 0;
  for (no n = primeiro_no(fronteira_entrada(v)); n; n = proximo(n))
    ++d_v;
  return d_v;
}
int grau_saida(vertice v)
{
  int d_v = 0;
  for (no n = primeiro_no(fronteira_saida(v)); n; n = proximo(n))
    ++d_v;
  return d_v;
}

// imprime o grafo G
void imprime_grafo(grafo G)
{

  if (G == NULL)
  {
    printf("Grafo nulo\n");
    return;
  }

  printf("%d\n", grafo_id(G));
  printf("\nVertices: ");
  /*no aux = (vertices(G)->primeiro_no);
  vertice v = conteudo(aux);
  //printf("%s \n",v->rotulo);
  */
  imprime_lista(vertices(G), (void_f_obj)imprime_vertice);
  printf("\nArestas: ");
  imprime_lista(arestas(G), (void_f_obj) imprime_aresta);
  printf("\n");
  printf("\nEstrutura:\n");

  imprime_lista(arestas(G), (void_f_obj) imprime_estrutura_aresta);
  
}

// imprime o vertice v
void imprime_vertice(vertice v)
{
  if (v == NULL)
  {
    printf("Vertice nulo\n");
    return;
  }

  printf("(id:%d \n", vertice_id(v));

  if (fronteira_entrada(v) != NULL)
  {
    imprime_lista(fronteira_entrada(v), (void_f_obj)imprime_aresta);
  }
  printf("}, grau_saida:%d, fronteira_saida:{ ", grau_saida(v));
  if (fronteira_saida(v) != NULL)
  {
    imprime_lista(fronteira_saida(v), (void_f_obj)imprime_aresta);
  }
  printf("})");
}

// imprime a aresta e
void imprime_aresta(aresta e)
{
  int u_id = vertice_id(vertice_u(e));
  int v_id = vertice_id(vertice_v(e));
  printf("(id:%d {%d,%d})", aresta_id(e), u_id, v_id);
}

// imprime aresta e no formato de entrada para
// https://graphonline.ru/en/create_graph_by_edge_list
void imprime_estrutura_aresta(aresta e)
{
  char *u_rot = vertice_rotulo(vertice_u(e));
  char *v_rot = vertice_rotulo(vertice_v(e));
  int u_id = vertice_id(vertice_u(e));
  int v_id = vertice_id(vertice_v(e));
  printf("%d:%s > %d:%s\n", u_id, u_rot, v_id, v_rot);
}

