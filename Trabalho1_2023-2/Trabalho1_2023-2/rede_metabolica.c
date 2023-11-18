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

//adiciona aresta R->M se a flag for 1 
//ou M->R ou E->R se a flag nao for 1
void add_arco_direcionado(int id, char *token, char *reacao, grafo G, int flag) {
  int u_id = vertice_id(busca_chave_str(reacao, vertices(G), (str_f_obj)vertice_rotulo));
  int v_id = vertice_id(busca_chave_str(token, vertices(G), (str_f_obj)vertice_rotulo));
  
  if (flag == 1)
    adiciona_aresta(id, u_id, v_id, G);
  else
    adiciona_aresta(id, v_id, u_id, G);
}

// Cria grafo G e lê a rede metabólica do arquivo apontado por <f_met>,
// adicionando em G as reações, enzimas e metabólitos, com as respectivas conexões.
// Devolve o grafo G.
grafo le_rede_metabolica(FILE *f_met) {
  char linha[1000];
  char * reacao;
  char *token;
  char *reacoes[100];     // Armazena os nomes das reações
  char *metabolitos[100]; // Armazena os metabólitos
  char *enzimas[100];     // Armazena as enzimas
  int num_reacoes = 1;
  int num_metabolitos = 1;
  int num_enzimas = 1;
  int n_vertice = 0;//id dos vertices
  int flag = -1; // M->R   antes da reacao
  //flag = 1 R->M   depois da reacao
 
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
    n_vertice++;

    //guarda a reacao numa varivel e adiciona ela no grafo
    reacao = token;
    adiciona_vertice(n_vertice, reacao, 1, G);
    token = strtok(NULL, " ");
    while (token != NULL)
    {

        //se o token for uma enzima
        if (token[0] == '_')
        {
          enzimas[num_enzimas-1] = token;

          //verifica se a enzima ja existe no grafo          
          vertice v = (vertice)busca_chave_str(token, vertices(G), (str_f_obj)vertice_rotulo);
          if (v){
            add_arco_direcionado(num_reacoes,token,reacao,G,flag);
          }
          else {
            n_vertice++;
            adiciona_vertice(n_vertice, token, 2, G);
            add_arco_direcionado(num_reacoes, token, reacao, G, flag);
            num_enzimas++;
            
          }
        }

        //se o token for um metabolito
        else if (token[0] == 'M')
        {
          metabolitos[num_metabolitos-1] = token;    

          //verifica se o metabolito ja existe no grafo   
          vertice v = (vertice)busca_chave_str(token, vertices(G), (str_f_obj)vertice_rotulo);
          if (v){     
            add_arco_direcionado(num_reacoes,token,reacao,G,flag);
          }
          else {
            n_vertice++;
            adiciona_vertice(n_vertice, token, 2, G);
            add_arco_direcionado(num_reacoes, token, reacao, G, flag);
            num_metabolitos++;
          } 
        }   
        else
          //quando o token chega no '=>', a flag vira 1 
          //para mudar a direção das arestas de M->R para R->M(produto)
          if (token[0] == '='){
            flag = 1;
          }
        token = strtok(NULL, " ");
      }
      num_reacoes++;
      flag = -1;     
    } 
  return G;
}

// Cria lista L e lê da entrada padrão os substratos iniciais, adicionando
// em L os vértices que representam os substratos.
// Devolve a lista de vértices L.
lista le_substratos(grafo G) {
  lista L = cria_lista();
  char subs[10];
  char *token;
  vertice v1;
  fgets(subs, 10, stdin);
  token = strtok(subs, " ");
  while (token != NULL && strcmp(token, "FIM") != 0) {
    v1 = busca_chave_str(token, vertices(G), (str_f_obj)vertice_rotulo);
    empilha(v1, L);
    token = strtok(NULL, " ");
  }
  return L;
}


// Obtém o identificador exclusivo para a próxima aresta
// Função auxiliar que adiciona na rede metabólica uma reação falsa que gera
// todos os substratos iniciais (dado pela lista de vértices <substratos>).
// Deve adicionar um novo vértice com rótulo "SF" representando o substrato
// falso inicial e um novo vértice com rótulo "RF" representando a reação falsa.
// Deve também adicionar as arestas necessárias.
// ATENÇÃO: os rótulos "SF" e "RF" são usados no método imprime_reacoes_minimas().
void adiciona_reacao_falsa(lista substratos, grafo G) {
  char *rotulo1 = "SF";
  char *rotulo2 = "RF";
  int i = 100;
  adiciona_vertice(100, rotulo1, 2, G);
  adiciona_vertice(101, rotulo2, 1, G);

  vertice sub;
  lista subs = cria_lista();
  
  // Preenche a lista de substratos com os elementos da lista de entrada
  no n = primeiro_no(substratos);
  while (n) {
    vertice v = conteudo(n);
    empilha(v, subs);
    n = proximo(n);
  }
  // Inicialize subAux como o primeiro nó de subs
  no subAux = primeiro_no(subs);
  
  while (subAux->proximo !=NULL) {
    sub = desempilha(subs);
    adiciona_aresta(i, 101, vertice_id(sub), G);
    printf("Dentro do while\n");
    subAux = proximo(subAux);  // Avance para o próximo nó
  }

  // Lembre-se de liberar a memória alocada para a lista subs
  free(subs);
}


  // Função auxiliar que inicializa os custos e pais dos vértices do grafo G para
// iniciar busca em largura. O custo de uma reação é a quantidade de enzimas que
// a cataliza.
// Devolve uma lista de vértices para ser a Fila inicial do algoritmo de busca.
lista inicializa_custos(grafo G) {
  lista F = cria_lista();
  no aux = vertices(G)->primeiro_no;

  // Inicializa custos e pais dos vértices no grafo
  

    vertice v = conteudo(aux);
    
    while (aux->proximo !=NULL){
      v->custo = 0;
      v->pai = NULL;
        empilha(v, F);  // Inicializa a fila com as enzimas sem reações de entrada
    
      vertice v = conteudo(aux);
      aux = aux->proximo;
    }
    printf("%d \n",v->id);
  return F;
}




// Algoritmo variante de Dijkstra que encontra o conjunto de reações de custo
// mínimo (número de enzimas) que produz todos os metabólitos da rede, considerando
// como substratos iniciais os metabólitos representados pelos vértices na lista
// substratos.
// Ao final do algoritmo, cada metabólito deve ter como pai (na estrutura vértice->pai)
// a reação "ótima" que o produz.
void processa(lista substratos, grafo G) {
  // Adiciona uma reação falsa para iniciar a busca
  //printf(" entrou  \n");
  adiciona_reacao_falsa(substratos, G);
  //printf(" SAIU  \n");
  // Inicializa custos, pais e fila inicial da busca F
  lista F = inicializa_custos(G);
  
  // Variante do Algoritmo de Dijkstra para resolver o problema
  if (vazio(F)== NULL)
    printf(" VAZIO\n");
  while (!vazio(F)) {
    vertice u = desempilha(F);
    
    // Itera sobre as arestas de saída do vértice u
    for (no n = primeiro_no(fronteira_saida(u)); n; n = proximo(n)) {
      aresta e = conteudo(n);
      vertice v = vertice_v(e);
      int custo_novo = u->custo + 1;  // Custo é o número de enzimas
      printf(" entrou  \n");
      if (v != NULL && vertice_particao(v) == REACAO) {  // Verifique se o vértice existe e é uma reação
        if (custo_novo < v->custo) {
          v->custo = custo_novo;
          v->pai = u;
          empilha(v, F);
        }
      }
    }
  }
}



// Imprime resultados com base nos pais dos vértices (na estrutura vértice->pai),
// calculados pelo método processa().
void imprime_reacoes_minimas(grafo G) {
  
  // Para cada metabólito, imprime as reações "ótimas"
  for (no n = primeiro_no(vertices(G)); n; n = proximo(n)) {
    vertice v = conteudo(n);
    if (vertice_particao(v) == METABOLITO && pai(v)) {
      printf("%s: ", vertice_rotulo(v));
      
      // cria lista de reações necessárias para sua produção
      lista R = cria_lista();
      empilha(pai(v), R);
      
      // seta todas as reações como ABERTOS, para não repetir
      for (no p = primeiro_no(vertices(G)); p; p = proximo(p)) {
        vertice w = conteudo(p);
        if (vertice_particao(w) == REACAO)
          w->estado = ABERTO;
      }
      
      // Enquanto tiver reações ainda não processadas, imprime
      while (!vazio(R)) {
        vertice w = desempilha(R);
        if (estado(w) == ABERTO && strcmp(vertice_rotulo(w), "RF")) { // não inclui a reação falsa
          printf("%s ", vertice_rotulo(w));
          w->estado = FECHADO;
          
          // coloca na lista as reações antecessoras
          for (no p = primeiro_no(fronteira_entrada(w)); p; p = proximo(p)) {
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
