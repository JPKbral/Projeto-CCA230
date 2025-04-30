#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 20

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
}EABB;

typedef struct ABB{
  EABB *raiz;
  int qtde;
}ABB;

Data *cria_data(int dia, int mes, int ano){
  Data *data = malloc(sizeof(Data));
  data->dia = dia;
  data->mes = mes;
  data->ano = ano;
  return data;
}

Registro *cria_registro(const char *nome, int idade, long rg, Data *entrada){
  Registro *registro = malloc(sizeof(Registro));
  registro->nome = nome;
  registro->idade = idade;
  registro->rg = rg;
  registro->entrada = entrada;
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
  fila->head = NULL;
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
  EABB *eabb = malloc(sizeof(EABB));
  eabb->dados = dados;
  eabb->filho_esq = NULL;
  eabb->filho_dir = NULL;
  return eabb;
}

ABB *cria_abb(){
  ABB *abb = malloc(sizeof(ABB));
  abb->raiz = NULL;
  abb->qtde = 0;
  return abb;
}

void cadastrar(Lista *lista, const char *nome, int idade, long rg, int dia, int mes, int ano){
  Data *entrada = cria_data(dia, mes, ano);
  Registro *registro = cria_registro(nome, idade, rg, entrada);
  ELista *elista = cria_elista(registro);
  if(lista->qtde != 0){
    elista->proximo = lista->inicio;
  }
  lista->inicio = elista;
  lista->qtde++;
}

void imprimir_data(Data *data){
  printf("%d/%d/%d", data->dia, data->mes, data->ano);
}

void imprimir_rg(long rg){
  int parte1 = (rg/10000000)%100;
  int parte2 = (rg/10000)%1000;
  int parte3 = (rg/10)%1000;
  int parte4 = rg%10;
  printf("%d.%d.%d-%d", parte1, parte2, parte3, parte4);
}

void imprimir_registro(Registro *registro){
  printf("Nome: %s\n", registro->nome);
  printf("Idade: %d\n", registro->idade);
  printf("RG: ");
  imprimir_rg(registro->rg);
  printf("\n");
  printf("Entrada: ");
  imprimir_data(registro->entrada);
  printf("\n\n");
}

void mostrar_lista(Lista *lista){
  if(lista->qtde == 0){
    printf("Ninguem esta na lista\n\n");
    return;
  }

  ELista *atual = lista->inicio;
  printf("--------------------------------\n\n");
  while(atual != NULL){
    imprimir_registro(atual->dados);
    atual = atual->proximo;
  }
  printf("--------------------------------\n");
}

void atualizar_dados(Lista *lista, const char *nome){
  ELista *atual = lista->inicio;
  while(nome != atual->dados->nome && atual != NULL){
    atual = atual->proximo;
  }
  if(atual == NULL){
    printf("Pessoa nao foi encontrada");
    return;
  }
  char c;
  printf("Qual dado deseja alterar?");
  imprimir_registro(atual->dados);
  scanf(" %c", &c);
  switch (c)
  {
  case 'n':case 'N':
    printf("Qual o novo nome?");
    scanf(" %c", &atual->dados->nome);
    break;

  case 'i':case 'I':
    printf("Qual a idade?");
    scanf(" %c", &atual->dados->idade);
    break;

  case 'r':case 'R':
    printf("Qual o rg?");
    scanf(" %c", &atual->dados->rg);
    break;

  case 'd':case 'D':
    printf("Qual a data de entrada?");
    scanf(" %c", &atual->dados->entrada);
    break;
  
  default:
    break;
  }

}

int main(){
  Lista *lista = cria_lista();
  cadastrar(lista, "Gabriel Ueno Vertamatti", 22, 928374920, 30, 4, 2025);
  mostrar_lista(lista);
  cadastrar(lista, "Gatti", 23, 927394825, 23, 02, 2025);
  mostrar_lista(lista);

}