/*******************************************
 * Biblioteca para manipulação de redes
 * metabólicas.
 *
 * Algoritmos em Grafos e Otimização
 * Departamento de Informática - UFPR
 * prof. Guilherme Derenievicz
 *******************************************/

#include "grafo.h"
#include "rede_metabolica.h"
#include <stdio.h>
#include <stdlib.h>

void add_arco_direcionado(int id, char *token, char *reacao, grafo G, int flag) {
  int u_id = vertice_id(busca_chave_str(reacao, vertices(G), (str_f_obj)vertice_rotulo));
  int v_id = vertice_id(busca_chave_str(token, vertices(G), (str_f_obj)vertice_rotulo));

  if (flag == 1)
    adiciona_aresta(id, u_id, v_id, G);
  else
    adiciona_aresta(id, v_id, u_id, G);
}


grafo le_rede_metabolica(FILE *f_met)
{
  
  char linha[1000];
  char linha_Aux[1000];
  char *token;
  char *reacoes[100];     // Armazena os nomes das reações
  char *metabolitos[100]; // Armazena os metabólitos
  char *enzimas[100];     // Armazena as enzimas
  int num_reacoes = 0;
  int num_metabolitos = 0;
  int num_enzimas = 0;
  int n_vertice = 0;
  int flag = -1; // M->R   antes da reacao
  //flag = 1 R->M   depois da reacao
  int n_arestas = 0;
 
  grafo G = cria_grafo(1);
  
    while (fgets(linha, sizeof(linha), f_met) != NULL)
    {
        // Remova o caractere de nova linha, se presente
        if (linha[strlen(linha) - 1] == '\n')
        {
            linha[strlen(linha) - 1] = '\0';
        }

        // Dividir a linha em tokens separados por espaços
        token = strtok(linha, " ");

        while (token != NULL)
        {
            // Verifique se o token começa com "R" para identificar uma reação
            if (token[0] == 'R')
            {
                reacoes[num_reacoes] = token;
                adiciona_vertice(n_vertice, token, 3, G);
                num_reacoes++;
                printf("Reação: %s\n", token);
                 n_vertice++;
                // Processar a reação, se necessário
            }
            else
            {
                // Se o token não começa com "R", é considerado ou um metabólito ou uma enzima
                // Você pode verificar se o token começa com "_" para identificar uma enzima
                if (token[0] == '_')
                {
                    enzimas[num_enzimas] = token;
                  
                    printf("Enzima: %s\n", token);
                    
                    vertice v = (vertice)busca_chave_str(token, vertices(G), (str_f_obj)vertice_rotulo);
                    if (v != NULL){
                  
                    printf("v id %d \n",v->id);
                    //add_arco_direcionado(n_arestas,reacoes[num_reacoes],enzimas[num_enzimas],G,flag);
                    printf("Enzimas: %s\n", token); 
                    }
                    else {
                     adiciona_vertice(n_vertice, token, 2, G);
                     //add_arco_direcionado(n_arestas,reacoes[num_reacoes],enzimas[num_enzimas],G,flag);
                     num_enzimas++;
                      n_vertice++;
                    }
                  n_arestas++; 
                }
                else if (token[0] == 'M')
                {
                    metabolitos[num_metabolitos] = token;
                   
                  
                    vertice v = (vertice)busca_chave_str(token, vertices(G), (str_f_obj)vertice_rotulo);
                    if (v != NULL){
                    //add_arco_direcionado(n_arestas,reacoes[num_reacoes],metabolitos[num_metabolitos],G,flag);
                    printf("Metabolito: %s\n", token); 
                    }
                    else{
                     adiciona_vertice( n_vertice, token, 1, G);
                     //add_arco_direcionado(n_arestas,reacoes[num_reacoes],enzimas[num_enzimas],G,flag);
                     num_metabolitos++;
                      n_vertice++;
                    }
                    n_arestas++;
                }    
                else{
                  if (token[0] == '+')
                    printf("reacao enzima metabolito %s\n", token);
                  else if (token[0] == '=>'){
                    flag = 1;
                    printf("reacao %s\n", token);
                  }
                }
            }
           
            token = strtok(NULL, " ");
            
        }


        flag = -1;
    }
    
    return G;
}



// Cria lista L e lê da entrada padrão os substratos iniciais, adicionando
// em L os vértices que representam os substratos.
// Devolve a lista de vértices L.
lista le_substratos(grafo G)
{

  lista L = cria_lista();

  // TODO: implementar...

  return L;
}

// Função auxiliar que adiciona na rede metabólica uma reação falsa que gera
// todos os substratos iniciais (dado pela lista de vértices <substratos>).
// Deve adicionar um novo vértice com rótulo "SF" representando o substrato
// falso inicial e um novo vértice com rótulo "RF" representando a reação falsa.
// Deve também adicionar as arestas necessárias.
// ATENÇÃO: os rótulos "SF" e "RF" são usados no método imprime_reacoes_minimas().
void adiciona_reacao_falsa(lista substratos, grafo G)
{

  // TODO: implementar...
}

// Função auxiliar que inicializa os custos e pais dos vértices do grafo G para
// iniciar busca em largura. O custo de uma reação é a quantidade de enzimas que
// a cataliza.
// Devolve uma lista de vértices para ser a Fila inicial do algoritmo de busca.
lista inicializa_custos(grafo G)
{
  lista F = cria_lista();

  // TODO: implementar...

  return F;
}

// Algoritmo variante de Dijkstra que encontra o conjunto de reações de custo
// mínimo (número de enzimas) que produz todos os metabólitos da rede, considerando
// como substratos iniciais os metabólitos representados pelos vértices na lista
// substratos.
// Ao final do algoritmo, cada metabólito deve ter como pai (na estrutura vértice->pai)
// a reação "ótima" que o produz.
void processa(lista substratos, grafo G)
{

  // adiciona uma reação falsa para iniciar a busca
  adiciona_reacao_falsa(substratos, G);

  // inicializa custos, pais e fila inicial da busca F
  lista F = inicializa_custos(G);

  // variante do Algoritmo de Dijkstra para resolver o problema
  while (!vazio(F))
  {

    // TODO: implementar
  }
}

// Imprime resultados com base nos pais dos vértices (na estrutura vértice->pai),
// calculados pelo método processa().
void imprime_reacoes_minimas(grafo G)
{

  // Para cada metabólito, imprime as reações "ótimas"
  for (no n = primeiro_no(vertices(G)); n; n = proximo(n))
  {
    vertice v = conteudo(n);
    if (vertice_particao(v) == METABOLITO && pai(v))
    {
      printf("%s: ", vertice_rotulo(v));

      // cria lista de reações necessárias para sua produção
      lista R = cria_lista();
      empilha(pai(v), R);

      // seta todas as reações como ABERTOS, para não repetir
      for (no p = primeiro_no(vertices(G)); p; p = proximo(p))
      {
        vertice w = conteudo(p);
        if (vertice_particao(w) == REACAO)
          w->estado = ABERTO;
      }

      // Enquanto tiver reações ainda não processadas, imprime
      while (!vazio(R))
      {
        vertice w = desempilha(R);
        if (estado(w) == ABERTO && strcmp(vertice_rotulo(w), "RF"))
        { // não inclui a reação falsa
          printf("%s ", vertice_rotulo(w));
          w->estado = FECHADO;

          // coloca na lista as reações antecessoras
          for (no p = primeiro_no(fronteira_entrada(w)); p; p = proximo(p))
          {
            aresta e = conteudo(p);
            vertice u = vertice_u(e);
            if (vertice_particao(u) == METABOLITO && pai(u))
              empilha(pai(u), R);
          }
        }
      }
      printf("\n");
      free(R);
    }
  }
}
