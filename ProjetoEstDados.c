#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <unistd.h>
#include <stdbool.h>

#define LEN 20
#define NAME 50

//Definindo estruturas utilizadas

typedef struct Data{
  int dia;
  int mes;
  int ano;
}Data;

typedef struct Registro{
  const char *nome;
  int idade;
  long rg;
  Data *entrada;
}Registro;

typedef struct ELista{
    Registro *dados;
    struct ELista *proximo;
}ELista;

typedef struct Lista{
    int qtde;
    ELista *inicio;
}Lista;

typedef struct EFila{
    struct EFila *proximo;
    Registro *dados;
}EFila;

typedef struct Fila{
    int qtde;
    EFila *head;
    EFila *tail;
}Fila;

typedef struct Heap{
  Registro *dados[LEN];
  int qtde;
}Heap;

typedef struct EABB{
  Registro *dados;
  struct EABB *filho_esq;
  struct EABB *filho_dir;
  struct EABB *pai;
}EABB;

typedef struct ABB{
  EABB *raiz;
  int qtde;
}ABB;

typedef struct EPilha{
  struct EPilha *proximo;
  EFila *ficha;
  char acao;
}EPilha;

typedef struct Pilha{
  EPilha *topo;
  int qtde;
}Pilha;

//Criando construtores das estruturas

Data *cria_data(int dia, int mes, int ano){
  Data *data = malloc(sizeof(Data));
  data->dia = dia;
  data->mes = mes;
  data->ano = ano;
  return data;
}

Registro *cria_registro(const char *nome, int idade, long rg, Data *entrada){
  Registro *registro = malloc(sizeof(Registro));
  registro->nome = strdup(nome);
  registro->idade = idade;
  registro->rg = rg;
  registro->entrada = entrada;
  return registro;
}

Registro *cria_registro_vazio(){
  Registro *registro = malloc(sizeof(Registro));
  Data *data = cria_data(0, 0, 0);
  registro->nome = NULL;
  registro->idade = 0;
  registro->rg = 0;
  registro->entrada = data;
  return registro;
}

ELista *cria_elista(Registro *registro){
  ELista *elista = malloc(sizeof(ELista));
  elista->dados = registro;
  elista->proximo = NULL;
  return elista;
}

Lista *cria_lista(){
  Lista *lista = malloc(sizeof(Lista));
  lista->inicio = NULL;
  lista->qtde = 0;
  return lista;
}

EFila *cria_efila(Registro *dados){
  EFila *efila = malloc(sizeof(EFila));
  efila->dados = dados;
  efila->proximo = NULL;
  return efila;
}

Fila *cria_fila(){
  Fila *fila = malloc(sizeof(Fila));
  fila->head = NULL;
  fila->tail = NULL;
  fila->qtde = 0;
  return fila;
}

Heap *cria_heap(){
  Heap *heap = malloc(sizeof(Heap));
  int i;
  for(i = 0; i < LEN; i++){
    heap->dados[i] = NULL;
  }
  heap->qtde = 0;
  return heap;
}

EABB *cria_eabb(Registro *dados){
  EABB *novo = malloc(sizeof(EABB));
  novo->dados = dados;
  novo->filho_esq = NULL;
  novo->filho_dir = NULL;
  novo->pai = NULL;

  return novo;
}

ABB *cria_abb(){
  ABB *arvore = malloc(sizeof(ABB));
  arvore->raiz = NULL;
  arvore->qtde = 0;
  return arvore;
}

EPilha *cria_epilha(){
  EPilha *epilha = malloc(sizeof(EPilha));
  epilha->proximo = NULL;
  epilha->ficha = NULL;
  epilha->acao = '0';
  return epilha;
}

Pilha *cria_pilha(){
  Pilha *pilha = malloc(sizeof(Pilha));
  pilha->topo = NULL;
  pilha->qtde = 0;
  return pilha;
}

//Usuário digita o RG do paciente e o código cria um ponteiro ELista para o paciente se existir
//Esse ponteiro é utilizado em outras funções
ELista *procurar_paciente(Lista *lista){
  int rg;
  scanf(" %d", &rg);
  ELista *atual = lista->inicio;
  while(atual != NULL && rg != atual->dados->rg){
    atual = atual->proximo;
  }
  return atual;
}

//Função para verificar se um RG existe na lista
boolean verificar_rg_lista(Lista *lista, int rg){
  ELista *atual = lista->inicio;
  int i;
  for(i = 0; i < lista->qtde; i++){
    if(atual->dados->rg == rg){
      return FALSE;
    }
    atual = atual->proximo;
  }
  return TRUE;
}

//Função para verificar se um RG existe na Fila
boolean verificar_rg_fila(Fila *fila, int rg){
  EFila *atual = fila->head;
  int i;
  for(i = 0; i < fila->qtde; i++){
    if(atual->dados->rg == rg){
      return FALSE;
    }
    atual = atual->proximo;
  }
  return TRUE;

}

//Função para verificar se um RG existe na Fila prioritária
boolean verificar_rg_fila_prioritaria(Heap *heap, int rg){
  int idx;
  for(idx = 0; idx < heap->qtde; idx++){
    if(heap->dados[idx]->rg == rg){
      return FALSE;
    }
  }
  return TRUE;

}

//Formatação correta para um RG de 6 a 9 dígitos numéricos, mais o dígito verificador
void imprimir_rg(long rg){
  if(rg/1000000000 != 0){
    printf("%03d.%03d.%03d-%d", (rg/10000000), (rg/10000)%1000, (rg/10)%1000, rg%10);
  }
  else if(rg/100000000 != 0){
    printf("%02d.%03d.%03d-%d", (rg/10000000), (rg/10000)%1000, (rg/10)%1000, rg%10);
  }
  else if(rg/10000000 != 0){
    printf("%01d.%03d.%03d-%d", (rg/10000000), (rg/10000)%1000, (rg/10)%1000, rg%10);
  }
  else{
    printf("%03d.%03d-%d", (rg/10000)%1000, (rg/10)%1000, rg%10);
  }
}

//Formatação correta para a data
void imprimir_data(Data *data){
  printf("%d/%d/%d", data->dia, data->mes, data->ano);
}

//Formatação para um registro completo
void imprimir_registro(Registro *registro){
  printf("Nome: %s\n", registro->nome);
  printf("Idade: %d\n", registro->idade);
  printf("RG: ");
  imprimir_rg(registro->rg);
  printf("\n");
  printf("Entrada: ");
  imprimir_data(registro->entrada);
  printf("\n");
}

// Pesquisa com arvore binária =============================

//inserção da idade
void inserir_abb_idade(ABB *abb, Registro *registro) {
  EABB *novo = cria_eabb(registro);
  if (abb->raiz == NULL) {
      abb->raiz = novo;
  } else {
      EABB *atual = abb->raiz;
      EABB *anterior = NULL;
      while (atual != NULL) {
          anterior = atual;
          if (registro->idade <= atual->dados->idade) {
              atual = atual->filho_esq;
          } else {
              atual = atual->filho_dir;
          }
      }
      if (registro->idade <= anterior->dados->idade) {
          anterior->filho_esq = novo;
      } else {
          anterior->filho_dir = novo;
      }
      novo->pai = anterior;
  }
  abb->qtde++;
}

//inserção do ano
void inserir_abb_ano(ABB *abb, Registro *registro) {
  EABB *novo = cria_eabb(registro);
  if (abb->raiz == NULL) {
      abb->raiz = novo;
  } else {
      EABB *atual = abb->raiz;
      EABB *anterior = NULL;
      while (atual != NULL) {
          anterior = atual;
          if (registro->entrada->ano <= atual->dados->entrada->ano) {
              atual = atual->filho_esq;
          } else {
              atual = atual->filho_dir;
          }
      }
      if (registro->entrada->ano <= anterior->dados->entrada->ano) {
          anterior->filho_esq = novo;
      } else {
          anterior->filho_dir = novo;
      }
      novo->pai = anterior;
  }
  abb->qtde++;
}

//inserção do mês
void inserir_abb_mes(ABB *abb, Registro *registro) {
  EABB *novo = cria_eabb(registro);
  if (abb->raiz == NULL) {
      abb->raiz = novo;
  } else {
      EABB *atual = abb->raiz;
      EABB *anterior = NULL;
      while (atual != NULL) {
          anterior = atual;
          if (registro->entrada->mes <= atual->dados->entrada->mes) {
              atual = atual->filho_esq;
          } else {
              atual = atual->filho_dir;
          }
      }
      if (registro->entrada->mes <= anterior->dados->entrada->mes) {
          anterior->filho_esq = novo;
      } else {
          anterior->filho_dir = novo;
      }
      novo->pai = anterior;
  }
  abb->qtde++;
}

//inserção do dia
void inserir_abb_dia(ABB *abb, Registro *registro) {
  EABB *novo = cria_eabb(registro);
  if (abb->raiz == NULL) {
      abb->raiz = novo;
  } else {
      EABB *atual = abb->raiz;
      EABB *anterior = NULL;
      while (atual != NULL) {
          anterior = atual;
          if (registro->entrada->dia <= atual->dados->entrada->dia) {
              atual = atual->filho_esq;
          } else {
              atual = atual->filho_dir;
          }
      }
      if (registro->entrada->dia <= anterior->dados->entrada->dia) {
          anterior->filho_esq = novo;
      } else {
          anterior->filho_dir = novo;
      }
      novo->pai = anterior;
  }
  abb->qtde++;
}

void mostrar_in_ordem(EABB *raiz) {
  if (raiz != NULL) {
      mostrar_in_ordem(raiz->filho_esq);
      imprimir_registro(raiz->dados);
      printf("\n"); 
      mostrar_in_ordem(raiz->filho_dir);
  }
}

void mostrar_pre_ordem(EABB *raiz) {
  if (raiz != NULL) {
    imprimir_registro(raiz->dados); 
    mostrar_pre_ordem(raiz->filho_esq);
    mostrar_pre_ordem(raiz->filho_dir);
  }
}

void mostrar_pos_ordem(EABB *raiz) {
  if (raiz != NULL) {
    mostrar_pos_ordem(raiz->filho_esq);
    mostrar_pos_ordem(raiz->filho_dir);
    imprimir_registro(raiz->dados); 
  }
}

//Encontrar o RG na árvore
//comparar é o valor a ser encontrado na árvore e deve confirmar com o rg
//verificador difere entre idade, dia, mes e ano
//atual será o ponto de partida para realizar a busca
EABB *busca_no_abb(EABB *atual, int comparar, char verificador, int rg) {
  //switch para cada caso de idade('i'), dia('d'), mes('m') e ano('a')
  switch(verificador){
    case 'i':
      while (atual != NULL) {
        if (atual->dados->idade == comparar){
          if (atual->dados->rg == rg) {
            return atual;
          } else {
            // Verifica dos dois lados
            //considera o filho esquerdo do atual como a raiz para buscar o RG
            EABB *esq = busca_no_abb(atual->filho_esq, atual->dados->idade, 'i', rg);
            if(esq != NULL){ 
              return esq;
            }
            atual = atual->filho_dir;
          }
        } else if (comparar < atual->dados->idade) {
          atual = atual->filho_esq;
        } else {
          atual = atual->filho_dir;
        }
      }
      break;

    case 'd':
      while (atual != NULL) {
        if (atual->dados->entrada->dia == comparar){
          if (atual->dados->rg == rg) {
            return atual;
          } else {
            // Verifica dos dois lados
            EABB *esq = busca_no_abb(atual->filho_esq, atual->dados->entrada->dia, 'd', rg);
            if(esq != NULL){ 
              return esq;
            }
            atual = atual->filho_dir;
          }
        } else if (comparar < atual->dados->entrada->dia) {
          atual = atual->filho_esq;
        } else {
          atual = atual->filho_dir;
        }
      }
      break;
    
    case 'm':
      while (atual != NULL) {
        if (atual->dados->entrada->mes == comparar){
          if (atual->dados->rg == rg) {
            return atual;
          } else {
            // Verifica dos dois lados
            EABB *esq = busca_no_abb(atual->filho_esq, atual->dados->entrada->mes, 'm', rg);
            if(esq != NULL){ 
              return esq;
            }
            atual = atual->filho_dir;
          }
        } else if (comparar < atual->dados->entrada->mes) {
          atual = atual->filho_esq;
        } else {
          atual = atual->filho_dir;
        }
      }
      break;
  
    case 'a':
      while (atual != NULL) {
        if (atual->dados->entrada->ano == comparar){
          if (atual->dados->rg == rg) {
            return atual;
          } else {
            // Verifica dos dois lados
            EABB *esq = busca_no_abb(atual->filho_esq, atual->dados->entrada->ano, 'a', rg);
            if(esq != NULL){ 
              return esq;
            }
            atual = atual->filho_dir;
          }
        } else if (comparar < atual->dados->entrada->ano) {
          atual = atual->filho_esq;
        } else {
          atual = atual->filho_dir;
        }
      }
      break;

    default:
      break;

  }
  return NULL;
}

//Remove um RG da árvore de busca
int remover_abb(ABB *arvore, EABB *registro) {
  if (arvore->raiz == NULL || registro == NULL) {
    return 0; // Árvore vazia ou registro inválido
  }

  int filhos = 0;
  if(registro->filho_esq != NULL) {filhos++;}
  if(registro->filho_dir != NULL) {filhos++;}

  if (filhos == 0) {
    // nó sem filhos

    if (arvore->raiz == registro) {
      arvore->raiz = NULL;
    }else if (registro->pai->filho_esq == registro) {
        registro->pai->filho_esq = NULL;
    }else {
        registro->pai->filho_dir = NULL;
    }
    free(registro);
    arvore->qtde--;

  }else if (filhos == 1) {
    // nó com um filho
    EABB *filho = NULL;
    if (registro->filho_esq != NULL) {
      filho = registro->filho_esq;
    } else {
      filho = registro->filho_dir;
    }
    if (registro->pai == NULL) {
      arvore->raiz = filho;
      filho->pai = NULL;
    }else if (registro->pai->filho_esq == registro) {
        registro->pai->filho_esq = filho;
        filho->pai = registro->pai;
    }else {
        registro->pai->filho_dir = filho;
        filho->pai = registro->pai;
    }
    free(registro);
    arvore->qtde--;
    
  } else {
    EABB *atual = registro->filho_esq;
    while(atual->filho_dir != NULL) {
        atual = atual->filho_dir;
    }
    registro->dados = atual->dados;
    remover_abb(arvore, atual);
  }
  return 1; // Remoção bem-sucedida

}

//CADASTRAR ==========================================================

//Faz as perguntas necessárias para criar um cadastro completo e o insere na lista
void cadastrar_manual(Lista *lista, ABB *abb_idade, ABB *abb_ano, ABB *abb_mes, ABB *abb_dia){
  int dia, mes, ano, idade;
  char nome[NAME];
  long rg;
  printf("Qual o nome?\n");
  scanf(" %[^\n]", nome);
  printf("Qual a idade?\n");
  scanf(" %d", &idade);
  printf("Qual o rg?\n");
  scanf(" %ld", &rg);
  //Verifica se o RG possui de 6 a 9 dígitos numéricos, mais o verificador
  if(rg<1000000 || rg>99999999999){
    printf("RG invalido\n");
    Sleep(1500);
    return;
  }
  //Verifica se o RG já existe na lista
  if(verificar_rg_lista(lista, rg) == FALSE){
    printf("Esse RG ja possui um cadastro\n");
    return;
  }
  printf("Qual o dia de entrada?\n");
  scanf(" %d", &dia);
  printf("Qual o mes de entrada?\n");
  scanf(" %d", &mes);
  printf("Qual o ano de entrada?\n");
  scanf(" %d", &ano);
  Data *entrada = cria_data(dia, mes, ano);
  Registro *registro = cria_registro(nome, idade, rg, entrada);
  ELista *elista = cria_elista(registro);
  inserir_abb_idade(abb_idade, registro);
  inserir_abb_ano(abb_ano, registro);
  inserir_abb_mes(abb_mes, registro);
  inserir_abb_dia(abb_dia, registro);
  if(lista->qtde != 0){
    elista->proximo = lista->inicio;
  }
  lista->inicio = elista;
  lista->qtde++;
  printf("Cadastro realizado\n");
}

//Cadastro para utilizar com o carregar arquivo
void cadastrar_automatico(Lista *lista, const char *nome, int idade, long rg, int dia, int mes, int ano){
  if(verificar_rg_lista(lista, rg) == FALSE){
    return;
  }
  Data *entrada = cria_data(dia, mes, ano);
  Registro *registro = cria_registro(nome, idade, rg, entrada);
  ELista *elista = cria_elista(registro);
  //Insere o novo cadastro no final da lista, para manter a ordem no arquivo de salvar e no terminal
  if(lista->qtde == 0){
    lista->inicio = elista;
  }
  else{
    ELista *aux = lista->inicio;
    ELista *anterior = NULL;
    while(aux != NULL){
      anterior = aux;
      aux = aux->proximo;
    }
    anterior->proximo = elista;
  }

  lista->qtde++;
}

//Formatação para um registro com numeração para escolha
void imprimir_registro_escolha(Registro *registro){
  printf("1-)Nome: %s\n", registro->nome);
  printf("2-)Idade: %d\n", registro->idade);
  printf("3-)RG: ");
  imprimir_rg(registro->rg);
  printf("\n");
  printf("4-)Entrada: ");
  imprimir_data(registro->entrada);
  printf("\n");
}

//Mostra a lista completa
void mostrar_lista(Lista *lista){
  if(lista->qtde == 0){
    printf("Ninguem esta na lista\n");
    return;
  }

  ELista *atual = lista->inicio;
  printf("\n--------------------------------\n\n");
  while(atual != NULL){
    imprimir_registro(atual->dados);
    atual = atual->proximo;
    printf("\n");
  }
  printf("--------------------------------\n");
}

//Verificar quem deve ter os dados atualizados e qual o dado a ser alterado
void atualizar_dados(Lista *lista, ABB *abb_idade, ABB *abb_ano, ABB *abb_mes, ABB *abb_dia){
  int dado_novo = 0, dado_ano = 0, dado_mes = 0, dado_dia = 0;
  printf("Qual o RG da pessoa que voce deseja alterar os dados?\n");
  ELista *atual = procurar_paciente(lista);
  if(atual == NULL){
    printf("Pessoa nao foi encontrada\n");
    return;
  }
  printf("\n");
  
  int escolha;
  imprimir_registro_escolha(atual->dados);
  printf("Qual dado deseja alterar?\n");
  scanf("%d", &escolha);
  
  int rg = atual->dados->rg;
  //Casos para cada dado de um registro que pode ser alterado
  switch (escolha)
  {
  case 1: //altera o nome
    printf("Qual o novo nome?\n");
    char novo_nome[NAME];
    scanf(" %[^\n]", novo_nome);
    atual->dados->nome = strdup(novo_nome);
    printf("Dados atualizados\n");
    break;

  case 2: //altera a idade
    printf("Qual a idade?\n");
    scanf(" %d", &dado_novo);
    //Remove o cadastro das árvores de busca
    EABB *no_idade = busca_no_abb(abb_idade->raiz, atual->dados->idade, 'i', rg);
    if(no_idade != NULL) remover_abb(abb_idade, no_idade);
    //Atualiza a idade
    atual->dados->idade = dado_novo;
    //Insere os dados atuais
    inserir_abb_idade(abb_idade, atual->dados);
    printf("Dados atualizados\n");
    break;

  case 3: //altera o RG
    printf("Qual o rg?\n");
    long verificarRg;
    scanf(" %ld", &verificarRg);
    //Verifica se o RG possui de 6 a 9 dígitos numéricos, mais o verificador
    if(verificarRg<1000000 || verificarRg>9999999999){
      printf("RG invalido\n");
      return;
    }
    //Verifica se o RG já existe na lista
    if(verificar_rg_lista(lista, verificarRg) == FALSE){
      printf("Esse RG ja possui um cadastro\n");
      return;
    }
    atual->dados->rg = verificarRg;
    printf("Dados atualizados\n");
    break;

  case 4: //altera a data
    printf("Qual o dia de entrada?\n");
    scanf(" %d", &dado_dia);
    printf("Qual o mes de entrada?\n");
    scanf(" %d", &dado_mes);
    printf("Qual o ano de entrada?\n");
    scanf(" %d", &dado_ano);
    
    //Remove o cadastro das árvores de busca
    EABB *no_dia = busca_no_abb(abb_dia->raiz, atual->dados->entrada->dia, 'd', rg);
    if(no_dia != NULL) remover_abb(abb_dia, no_dia);
    //Atualiza a dia
    atual->dados->entrada->dia = dado_dia;
    //Insere os dados atuais
    inserir_abb_dia(abb_dia, atual->dados);
    
    //Remove o cadastro das árvores de busca
    EABB *no_mes = busca_no_abb(abb_mes->raiz, atual->dados->entrada->mes, 'm', rg);
    if(no_mes != NULL) remover_abb(abb_mes, no_mes);
    //Atualiza a mes
    atual->dados->entrada->mes = dado_mes;
    //Insere os dados atuais
    inserir_abb_mes(abb_mes, atual->dados);
    
    //Remove o cadastro das árvores de busca
    EABB *no_ano = busca_no_abb(abb_ano->raiz, atual->dados->entrada->ano, 'a', rg);
    if(no_ano != NULL) remover_abb(abb_ano, no_ano);
    //Atualiza a ano
    atual->dados->entrada->ano = dado_ano;
    //Insere os dados atuais
    inserir_abb_ano(abb_ano, atual->dados);
    printf("Dados atualizados\n");
    break;
  
  default:
    printf("Opcao Invalida\n");
    break;
  }

}

//Exclui o paciente que o usuário digitar o nome
void remover_paciente(Lista *lista, Fila *fila, Heap *heap, ABB *abb_idade, ABB *abb_ano, ABB *abb_mes, ABB *abb_dia){
  if(lista->qtde == 0){
    printf("A lista esta vazia\n");
    return;
  }
  printf("Qual o RG de quem deseja excluir?\n");
  //Não utiliza da função 'procurar_paciente' por precisar utilizar do ponteiro 'anterior', já que não é uma lista duplamente encadeada
  int rg;
  scanf(" %d", &rg);
  ELista *atual = lista->inicio;
  ELista *anterior = NULL;
  while(atual != NULL && rg != atual->dados->rg){
    anterior = atual;
    atual = atual->proximo;
  }
  if(atual == NULL){
    printf("Pessoa nao foi encontrada\n");
    return;
  }

  //Verifica se a cadastro a ser removido está em alguma fila e pede confirmação
  int confirmar = 0;
  if(verificar_rg_fila_prioritaria(heap, rg) == FALSE && verificar_rg_fila(fila, rg) == FALSE){
    printf("O cadastro esta registrado na fila prioritaria e na fila\n 1-)Seguir para remover o paciente\n 2-)Cancelar\n");
    scanf(" %d", &confirmar);
    if(confirmar != 1){
      printf("Remocao cancelada");
      return;
    }
  }
  else if(verificar_rg_fila(fila, rg) == FALSE){
    printf("O cadastro esta registrado na fila\n 1-)Seguir para remover o paciente\n 2-)Cancelar\n");
    scanf(" %d", &confirmar);
    if(confirmar != 1){
      printf("Remocao cancelada");
      return;
    }
  }
  else if(verificar_rg_fila_prioritaria(heap, rg) == FALSE){
    printf("O cadastro esta registrado na fila prioritaria\n 1-)Seguir para remover o paciente\n 2-)Cancelar\n");
    scanf(" %d", &confirmar);
    if(confirmar != 1){
      printf("Remocao cancelada");
      return;
    }
  }

  //Remove o cadastro das árvores de busca
  EABB *no_idade = busca_no_abb(abb_idade->raiz, atual->dados->idade, 'i', rg);
  if(no_idade != NULL) remover_abb(abb_idade, no_idade);
  
  EABB *no_ano = busca_no_abb(abb_ano->raiz, atual->dados->entrada->ano, 'a', rg);
  if(no_ano != NULL) remover_abb(abb_ano, no_ano);
  
  EABB *no_mes = busca_no_abb(abb_mes->raiz, atual->dados->entrada->mes, 'm', rg);
  if(no_mes != NULL) remover_abb(abb_mes, no_mes);
  
  EABB *no_dia = busca_no_abb(abb_dia->raiz, atual->dados->entrada->dia, 'd', rg);
  if(no_dia != NULL) remover_abb(abb_dia, no_dia);

  //A lista fica vazia se possuir apenas o cadastro a ser removimo
  if(lista->qtde == 1){
    lista->inicio = NULL;
  }
  //Se o primeiro da lista aponta para o atual a ser removido esse ponteiro passa para o proximo
  else if(atual == lista->inicio){
    lista->inicio = atual->proximo;
  }
  //No caso de estar no meio da lista
  else{
    anterior->proximo = atual->proximo;
  }

  //Confirma a remoção do cadastro
  printf("%s teve o cadastro removido\n", atual->dados->nome);
  free(atual);
  lista->qtde--;
}

//O usuário digita o nome do paciente para ter o registro mais completo
void consultar_paciente(Lista *lista){
  printf("Qual o RG de quem voce deseja obter os dados?\n");
  ELista *atual = procurar_paciente(lista);
  if(atual == NULL){
    printf("Pessoa nao foi encontrada\n");
    return;
  }
  printf("\n--------------------------------\n");
  imprimir_registro(atual->dados);
  printf("--------------------------------\n");
}
//==========================================================


//DESFAZER =================================================

//Coloca na pilha uma ação de adicionar um cadastro na fila
void salvar_adicionar_fila(EFila *ficha, Pilha *pilha){
  EPilha *novo = cria_epilha();
  novo->ficha = ficha;
  //Caractere define que foi uma ação de 'a' (adicionar)
  novo->acao = 'a';
  if(pilha->qtde != 0){
    novo->proximo = pilha->topo;
  }
  pilha->topo = novo;
  pilha->qtde++;
}

//Coloca na pilha uma ação de remover um cadastro na fila
void salvar_remover_fila(EFila *ficha, Pilha *pilha){
  EPilha *novo = cria_epilha();
  novo->ficha = ficha;
  //Caractere define que foi uma ação de 'r' (remover)
  novo->acao = 'r';
  if(pilha->qtde != 0){
    novo->proximo = pilha->topo;
  }
  pilha->topo = novo;
  pilha->qtde++;
}

//Mostra todas ações realizadas na pilha, difere se foi adicionado ou removido pelo caractere
void operacoes_na_fila(Pilha *pilha){
  if(pilha->qtde == 0){
    printf("Nenhuma acao para desfazer na fila\n");
    return;
  }
  EPilha *atual = pilha->topo;
  int idx = 0;
  for (idx = 0; idx < pilha->qtde; idx++)
  {
    if(atual->acao == 'a'){
      printf("%s foi adicionado da fila\n", atual->ficha->dados->nome);
    }
    else{
      printf("%s foi removido da fila\n", atual->ficha->dados->nome);
    }
    atual = atual->proximo;
  } 
}

//Função para remover da pilha e desfazer uma ação na fila
void desfazer_acao(Pilha *pilha, Fila *fila) {
  int confirmacao = 0;
  if (pilha->qtde > 0) {
    EPilha *topo = pilha->topo;
    //Se alguém foi 'a' (adicionado), ao desfazer deve ser removido na fila
    if(topo->acao == 'a'){
      printf("%s sera removido da fila, tem certeza dessa acao? \n 1-)Sim \n 2-)Nao\n", topo->ficha->dados->nome);
      scanf(" %d", &confirmacao);
      if(confirmacao != 1){
        return;
      }
      //Se for o último item na fila passa o head e tail para nulos
      if(fila->qtde == 1){
        fila->head = NULL;
        fila->tail = NULL;
      }
      else{
        EFila *atual = fila->head;
        EFila *anterior = NULL;
        //Percorre a fila interia para alterar o tail para o anterior na fila, por ser um encadeamento simples
        while(atual != NULL && atual != topo->ficha){
          anterior = atual;
          atual = atual->proximo;
        }
        fila->tail = anterior;
        anterior->proximo = NULL;
      }
      fila->qtde--;
    }
    //Se alguém foi 'r' (removido), ao desfazer deve ser adicionado na fila
    else{
      printf("%s sera adicionado novamente da fila, tem certeza dessa acao? \n 1-)Sim \n 2-)Nao\n", topo->ficha->dados->nome);
      scanf(" %d", &confirmacao);
      if(confirmacao != 1){
        return;
      }
      //Se não tiver nenhum outro item na fila esse se torna o head e tail
      if(fila->qtde == 0){
        fila->head = topo->ficha;
        fila->tail = topo->ficha;
      }
      //É colocado na head da fila
      else{
        topo->ficha->proximo = fila->head;
        fila->head = topo->ficha;
      }
      fila->qtde++;  
    }
    pilha->topo = pilha->topo->proximo;
    pilha->qtde--;
    free(topo);
    return;
  } 
  else{
    printf("Nao existem acoes para desfazer\n");
    Sleep(1500);
    return;
  }
}
//=========================================================


//ATENDIMENTO =============================================

//Usuário digita o nome do paciente a ser adicionado na fila para o atendimento
void enfileirar_paciente(Lista *lista, Fila *fila, Pilha *pilha, Heap *heap){
  printf("Qual o RG de quem voce deseja inserir no antendimento?\n");
  ELista *aux = procurar_paciente(lista);
  if(aux == NULL){
    printf("Pessoa nao foi encontrada\n");
    return;
  }
  //Verifica se esse cadastro já está na fila
  if(verificar_rg_fila(fila, aux->dados->rg) == FALSE){
    printf("Esse RG ja esta na fila\n");
    return;
  }
  //Verifica se esse cadastro já está na fila prioritária e pede confirmação para essa ação
  int confirmar = 0;
  if(verificar_rg_fila_prioritaria(heap, aux->dados->rg) == FALSE){
    printf("Esse cadastro ja esta na fila prioritaria, ainda deseja adiciona-lo na fila?\n 1-)Adicionar o paciente\n 2-)Cancelar\n");
    scanf(" %d", &confirmar);
    if(confirmar != 1){
      printf("O paciente nao foi adicionado na fila\n");
      return;
    }
  }
  EFila *novo = cria_efila(aux->dados);
  //Se for o único item na lista
  if(fila->qtde == 0){
    fila->head = novo;
  }
  //Caso já existam outros pacientes na fila
  else{
    fila->tail->proximo = novo;
  }
  fila->tail = novo;
  fila->qtde++;
  salvar_adicionar_fila(novo, pilha);
  printf("%s foi adicionado na fila para atendimento\n", aux->dados->nome);
}

//Retira o primeiro paciente que foi adicionado na fila
void desenfileirar_paciente(Fila *fila, Pilha *pilha){
  if(fila->qtde == 0){
    printf("A fila esta vazia\n");
    return;
  }
  EFila *aux = fila->head;
  if(fila->qtde == 1){
    fila->head = NULL;
    fila->tail = NULL;
  }
  else{
    fila->head = fila->head->proximo;
  }
  salvar_remover_fila(aux, pilha);
  printf("%s foi removido da fila de atendimento\n", aux->dados->nome);
  fila->qtde--;
}

//Mostra toda a fila na sequência de atendimento
void mostrar_fila(Fila *fila){
  if(fila->qtde == 0){
    printf("Ninguem esta na fila\n");
    return;
  }
  EFila *atual = fila->head;
  int idx = 1;
  printf("\n--------------------------------\n");
  while(atual != NULL){
    printf("%d -) %s, RG: ", idx, atual->dados->nome);
    imprimir_rg(atual->dados->rg);
    printf("\n");
    atual = atual->proximo;
    idx++;
  }
  printf("--------------------------------\n");
}

//==============================================================


//ATENDIMENTO PRIORITÁRIO ======================================

//Calculos para os filhos e pai de um item da heap
int filho_esq(int pai){
  return 2*pai+1;
}

int filho_dir(int pai){
  return 2*pai+2;
}

int pai(int filho){
  return (filho - 1)/2;
}

//Encontra o último pai da heap para realizar o construir a partir desse item
int ultimo_pai(Heap *heap){
  if(heap->qtde <= 1){
    return 0;
  }
  return (heap->qtde/2)-1;
}

//Corrige a posição dos itens na fila prioritária conforme mais cadastros serem adicionados
void peneirar(Heap *heap, int pai){
  int aux = pai;
  if (filho_esq(pai) < heap->qtde && heap->dados[filho_esq(pai)]->idade > heap->dados[aux]->idade){
      aux = filho_esq(pai);
  }
  if (filho_dir(pai) < heap->qtde && heap->dados[filho_dir(pai)]->idade > heap->dados[aux]->idade){
      aux = filho_dir(pai);
  }
  if (aux != pai){
      Registro *temp = heap->dados[pai];
      heap->dados[pai] = heap->dados[aux];
      heap->dados[aux] = temp;
      peneirar(heap,aux);
  }
}

//Realiza o peneirar em toda a heap
void construir(Heap *heap) {
  int idx;
  for(idx = ultimo_pai(heap); idx >= 0; idx--){
    peneirar(heap, idx);
  }
}

//Mostra a fila prioritária, com o RG e idade
void mostrar_fila_prioritaria(Heap *heap) {
  if(heap->qtde == 0){
    printf("Ninguem esta na fila prioritaria\n");
    return;
  }
  printf("\n--------------------------------\n");
  int idx;
  for(idx = 0; idx < heap->qtde; idx++){
    printf("%d -) %s - Idade: %d, RG: ", idx+1, heap->dados[idx]->nome, heap->dados[idx]->idade);
  imprimir_rg(heap->dados[idx]->rg);
  printf("\n");
  }
  printf("--------------------------------\n");
}

//Pergunta qual cadastro deve ser inserido na fila prioritária
void inserir_fila_prioritaria(Lista *lista, Heap *heap, Fila *fila) {
  if(heap->qtde == LEN){
    return;
  }
  printf("Qual o RG de quem deseja inserir na fila prioritaria?\n");
  ELista *atual = procurar_paciente(lista);
  if(atual == NULL){
    printf("Pessoa nao foi encontrada\n");
    return;
  }
  //Verifica se o cadastro já está na fila prioritária
  if(verificar_rg_fila_prioritaria(heap, atual->dados->rg) == FALSE){
    printf("Esse RG ja esta na fila prioritaria");
    return;
  }
  //Verifica se o cadastro já está na fila e pede confirmação dessa ação
  int confirmar = 0;
  if(verificar_rg_fila(fila, atual->dados->rg) == FALSE){
    printf("Esse cadastro ja esta na fila, ainda deseja adiciona-lo na fila prioritaria?\n 1-)Adicionar o paciente\n 2-)Cancelar\n");
    scanf(" %d", &confirmar);
    if(confirmar != 1){
      printf("O paciente nao foi adicionado na fila\n");
      return;
    }
  }
  Registro *dado = atual->dados;
  heap->dados[heap->qtde] = dado;
  heap->qtde++;
  //Corrige a ordem da heap e confirma a adição do cadastro na fila prioritária
  construir(heap);
  printf("%s foi adicionado na fila prioritaria\n", atual->dados->nome);
}

//Remove o primeiro dado da heap
void remover_fila_prioritaria(Heap *heap) {
  int i = 0;
  if(heap->qtde == 0){
    printf("Ninguem esta na fila prioritaria\n");
    return;
  }
    printf("%s foi removido da lista prioritaria\n", heap->dados[0]->nome);
    heap->dados[0] = heap->dados[heap->qtde-1];
    heap->qtde--;
  for(i = 0; i < heap->qtde; i++){
    peneirar(heap, 0);
  }
}

//==========================================================


//Carregar/Salvar ==========================================

//Passa o RG de uma string XXX.XXX.XXX-XX para um long XXXXXXXXXX
long rg_numeros(char rg_string[14]) {
  long numeros = 0;
  int i = 0, j = 0;
  while(rg_string[i] != '\0') {
      if (rg_string[i] != '.' && rg_string[i] != '-' && rg_string[i] != '\0') {
        numeros = numeros*10 + (rg_string[i] - 48);
        j++;
      }
      i++;
  }
  return numeros;
}

//Carrega os dados para a fila, evitando cadastros duplicados (com o mesmo RG)
void carregar_arquivo(FILE *arquivo, Lista *lista, ABB *abb_idade, ABB *abb_ano, ABB *abb_mes, ABB *abb_dia){
  arquivo = fopen("carregar_cadastros_pacientes.txt", "r");
  //Verifica se o arquivo foi aberto
  if(arquivo == NULL){
    printf("Erro ao abrir o arquivo\n");
  }

  Registro *novo = cria_registro_vazio();
  char verificar[80];
  char linha[14];
  char nome[80];
  int idade = 0, dia = 0, mes = 0, ano = 0;
  long rg = 0;
  linha[0] = 'v';

  //Lê o arquivo até a última linha ou até encontrar um erro
  while (fgets(verificar, sizeof(verificar), arquivo)) {
    //Se a linha começa com '-' ou '\n' toda linha é descartada
    if (verificar[0] == '-' || verificar[0] == '\n') {
        //Volta para o começo do While
        continue;
    }

    //Salva o nome para o registro novo
    //%[^\n] lê espeço e sem considerar '\n'
    if(sscanf(verificar, "Nome: %[^\n]", nome) == 1){}
    //Salva a idade para o registro novo
    else if(sscanf(verificar, "Idade: %d\n", &idade) == 1){}
    //Salva o RG para o registro novo
    //Garante que vão passar o rg como 13 char para a função
    else if(sscanf(verificar, "RG: %13s\n", &linha) == 1){
      //Converte string em long se linha não estiver vazia
      if(linha[0] != 'v'){
          rg = rg_numeros(linha);
          linha[0] = 'v';
      }
    }
    //Salva a data de entrada para o registro novo
    else if(sscanf(verificar, "Entrada: %d/%d/%d\n", &dia, &mes, &ano)){}
    
    //Salva o registro na lista se tiver todos os dados, considerando o ano a ser a última variável a ser alterada
    if(ano != 0){
      cadastrar_automatico(lista, nome, idade, rg, dia, mes, ano);
      // Recupera o registro inserido para as árvores de busca
      ELista *inserido = lista->inicio;
      while (inserido->proximo != NULL) {
          inserido = inserido->proximo;
      }
      // Insere nas arvores
      inserir_abb_idade(abb_idade, inserido->dados);
      inserir_abb_ano(abb_ano, inserido->dados);
      inserir_abb_mes(abb_mes, inserido->dados);
      inserir_abb_dia(abb_dia, inserido->dados);
      //Limpa a estrutura para recerber outros cadastros
      ano = 0;
    }
  }  
  //Fecha o arquivo
  fclose(arquivo);
  printf("Dados carregados com sucesso\n");
}

//Substitui todos os dados no arquivo pelo que estiverem na lista
void salvar_arquivo(FILE *arquivo, Lista *lista){
  arquivo = fopen("salvar_cadastros_pacientes.txt", "w");
  //Verifica se o arquivo foi aberto
  if(arquivo == NULL){
    printf("Erro ao abrir o arquivo\n");
    return;
  }
  //Salva os cadastros no arquivo
  //Realiza as mesmas ações que "mostrar_lista", porém para salvar no arquivo
  if(lista->qtde == 0){
    fprintf(arquivo, "Ninguem está na lista\n");
    //Fecha o arquivo
    fclose(arquivo);
    printf("Dados salvos com sucesso\n");
    return;
  }

  ELista *atual = lista->inicio;
  fprintf(arquivo, "\n--------------------------------\n\n");
  while(atual != NULL){
    fprintf(arquivo, "Nome: %s\n", atual->dados->nome);
    fprintf(arquivo, "Idade: %d\n", atual->dados->idade);
    fprintf(arquivo, "RG: ");
    if(atual->dados->rg/1000000000 != 0){
      fprintf(arquivo, "%03d.%03d.%03d-%d", (atual->dados->rg/10000000), (atual->dados->rg/10000)%1000, (atual->dados->rg/10)%1000, atual->dados->rg%10);
    }
    else if(atual->dados->rg/100000000 != 0){
      fprintf(arquivo, "%02d.%03d.%03d-%d", (atual->dados->rg/10000000), (atual->dados->rg/10000)%1000, (atual->dados->rg/10)%1000, atual->dados->rg%10);
    }
    else if(atual->dados->rg/10000000 != 0){
      fprintf(arquivo, "%01d.%03d.%03d-%d", (atual->dados->rg/10000000), (atual->dados->rg/10000)%1000, (atual->dados->rg/10)%1000, atual->dados->rg%10);
    }
    else{
      fprintf(arquivo, "%03d.%03d-%d", (atual->dados->rg/10000)%1000, (atual->dados->rg/10)%1000, atual->dados->rg%10);
    }
    fprintf(arquivo, "\n");
    fprintf(arquivo, "Entrada: ");
    fprintf(arquivo, "%d/%d/%d", atual->dados->entrada->dia, atual->dados->entrada->mes, atual->dados->entrada->ano);
    fprintf(arquivo, "\n");
    atual = atual->proximo;
    fprintf(arquivo, "\n");
  }
  fprintf(arquivo, "--------------------------------\n");
  //Fecha o arquivo
  fclose(arquivo);

  printf("Dados salvos com sucesso\n");
}

//==========================================================

int main(){
  //Criando as estruturas utilizadas no programa
  FILE *arquivo;
  Lista *lista = cria_lista();
  Fila *fila = cria_fila();
  Heap *fila_prioritaria = cria_heap();
  Pilha *desfazer = cria_pilha();
  ABB *abb_ano = cria_abb();
  ABB *abb_mes = cria_abb();
  ABB *abb_dia = cria_abb();
  ABB *abb_idade = cria_abb();
  int menuEscolha = 0, segundaEscolha = 0, sair = 0;
  //Fica no while enquanto o usuário desejar
  while(sair == 0){
    printf("\n\n");
    printf("\t/---------------------------------------- \n");
    printf("\t|                                        |\n");
    printf("\t|   /================================    |\n");
    printf("\t|   |                                |   |\n");
    printf("\t|   |    SISTEMA DE ATENDIMENTO      |   |\n");
    printf("\t|   |                                |   |\n");
    printf("\t|    ================================/   |\n");
    printf("\t|                                        |\n");
    printf("\t|   [1] Cadastrar paciente               |\n");
    printf("\t|   [2] Atendimento regular              |\n");
    printf("\t|   [3] Atendimento prioritario          |\n");
    printf("\t|   [4] Pesquisar registros              |\n");
    printf("\t|   [5] Desfazer ultima acao             |\n");
    printf("\t|   [6] Carregar/Salvar dados            |\n");
    printf("\t|   [7] Sobre                            |\n");
    printf("\t|   [8] Sair do programa                 |\n");
    printf("\t|                                        |\n");
    printf("\t ----------------------------------------/\n\n");
    printf("\tEscolha uma opcao: ");
    scanf(" %d", &menuEscolha);
    //Verifica as escolhas do usuário e realiza as funções necessárias
    //Uma para cada item no menu e uma segunda para as opções dentro de cada item no menu
    switch (menuEscolha)
    {
    case 1:
      //Cadastrar
      printf("Cadastrar: \n 1-)Cadastrar novo paciente \n 2-)Consultar paciente cadastrado \n 3-)Mostrar lista completa \n 4-)Atualizar dados de paciente \n 5-)Remover paciente \n");
      scanf(" %d", &segundaEscolha);
      switch (segundaEscolha)
      {
      case 1:
        //Cadastrar novo paciente
        cadastrar_manual(lista, abb_idade, abb_ano, abb_mes, abb_dia);
        Sleep(1500);
        break;
        
      case 2:
        //Consultar paciente cadastrado
        consultar_paciente(lista);
        Sleep(1500);
        break;
        
      case 3:
        //Mostrar lista completa
        mostrar_lista(lista);
        Sleep(1500);
        break;
        
      case 4:
        //Atualizar dados de paciente
        atualizar_dados(lista, abb_idade, abb_ano, abb_mes, abb_dia);
        Sleep(1500);
        break;

      case 5:
        //Remover cadastro do paciente
        remover_paciente(lista, fila, fila_prioritaria, abb_idade, abb_ano, abb_mes, abb_dia);
        Sleep(1500);
        break;
      
      default:
      printf("Opcao Invalida\n");
      Sleep(1500);
        break;
      }
      break;
    
    case 2:
      //Atendimento
      printf("Atendimento: \n 1-)Enfileirar paciente \n 2-)Desenfileirar paciente \n 3-)Mostrar fila \n");
      scanf(" %d", &segundaEscolha);
      switch (segundaEscolha)
      {
      case 1:
        //Enfileirar paciente
        enfileirar_paciente(lista, fila, desfazer, fila_prioritaria);
        Sleep(1500);
        break;
        
      case 2:
        //Desenfileirar paciente
        desenfileirar_paciente(fila, desfazer);
        Sleep(1500);
        break;
        
      case 3:
        //Mostrar fila
        mostrar_fila(fila);
        Sleep(1500);
        break;

      default:
      printf("Opcao Invalida\n");
      Sleep(1500);
        break;
      }
      break;
    
    case 3:
      //Atendimento Prioritario
      printf("Atendimento Prioritario: \n 1-)Enfileirar paciente \n 2-)Desenfileirar paciente \n 3-)Mostrar fila \n");
      scanf(" %d", &segundaEscolha);
      switch (segundaEscolha)
      {
      case 1:
        //Enfileirar paciente
        inserir_fila_prioritaria(lista, fila_prioritaria, fila);
        Sleep(1500);
        break;
        
      case 2:
        //Desenfileirar paciente
        remover_fila_prioritaria(fila_prioritaria);
        Sleep(1500);
        break;
        
      case 3:
        //Mostrar fila
        mostrar_fila_prioritaria(fila_prioritaria);
        Sleep(1500);
        break;

      default:
      printf("Opcao Invalida\n");
      Sleep(1500);
        break;
      }

      break;
    
    case 4:
      //Pesquisa
      printf("Ordenar por:\n1. Ano\n2. Mes\n3. Dia\n4. Idade\n");
      scanf("%d", &segundaEscolha);
      switch (segundaEscolha) {
          case 1:
            printf("--------------------------------\n");
            mostrar_in_ordem(abb_ano->raiz); // ABB ordenada por ano
            printf("--------------------------------");
            Sleep(1500);
            break;
          case 2:
            printf("--------------------------------\n");
            mostrar_in_ordem(abb_mes->raiz); // ABB ordenada por mês
            printf("--------------------------------");
            Sleep(1500);
            break;
          case 3:
            printf("--------------------------------\n");
            mostrar_in_ordem(abb_dia->raiz); // ABB ordenada por dia
            printf("--------------------------------");
            Sleep(1500);
            break;
          case 4:
            printf("--------------------------------\n");
            mostrar_in_ordem(abb_idade->raiz); // ABB ordenada por idade
            printf("--------------------------------");
            Sleep(1500);
            break;
    }

      break;
    
    case 5:
      //Desfazer
      printf("Atendimento Prioritario: \n 1-)Desfazer ultima acao \n 2-)Ver ultimas acoes \n");
      scanf(" %d", &segundaEscolha);
      switch (segundaEscolha)
      {
      case 1:
        //Enfileirar paciente
        desfazer_acao(desfazer, fila);
        break;
        
      case 2:
        //Desenfileirar paciente
        operacoes_na_fila(desfazer);
        Sleep(1500);
        break;

      default:
      printf("Opcao Invalida\n");
      Sleep(1500);
        break;
      }

      break;
    
    case 6:
      //Carregar/Salvar
      printf("Carregar/Salvar: \n 1-)Carregar um Arquivo \n 2-)Salvar um Arquivo \n");
      scanf(" %d", &segundaEscolha);
      switch (segundaEscolha)
      {
      case 1:
        //Carregar um Arquivo
        carregar_arquivo(arquivo, lista, abb_idade, abb_ano, abb_mes, abb_dia);
        Sleep(1500);
        break;
        
      case 2:
        //Salvar um Arquivo
        salvar_arquivo(arquivo, lista);
        Sleep(1500);
        break;

      default:
      printf("Opcao Invalida\n");
      Sleep(1500);
        break;
      }
      break;
    
    case 7:
      //Sobre
      printf("\n\t************************************************************************************\n");
      printf("\t*   Nome dos desenvolvedores: Gabriel Ueno Vertamatti e Joao Pedro Soares Cabral   *\n");
      printf("\t*   Ciclo: 7 Semestre                                                              *\n");
      printf("\t*   Curso: Engenharia de Robos                                                     *\n");
      printf("\t*   Disciplina: Estrutura de Dados e Analise de Algoritmos (CCA230)                *\n");
      printf("\t*   Data: Maio de 2025                                                             *\n");
      printf("\t************************************************************************************\n");
      Sleep(3000);
      break;
    
    case 8:
      //Sair
      sair = 1;
      break;
    
    default:
      printf("Opcao Invalida\n");
      Sleep(1500);
      break;
    }
  }
}