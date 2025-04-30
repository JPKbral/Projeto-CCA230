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
  char nome[50];
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

Registro *cria_registro(char nome[50], int idade, long rg, Data *entrada){
  Registro *registro = malloc(sizeof(Registro));
  strcpy(registro->nome, nome);
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

void cadastrar(Lista *lista, ELista *elista){
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

int main(){
  Data *entrada = cria_data(30, 4, 2025);
  Registro *registro = cria_registro("Gabriel Ueno Vertamatti", 22, 928374920, entrada);
  Registro *registro2 = cria_registro("Gatti", 23, 927394825, entrada);
  Lista *lista = cria_lista();
  ELista *gabriel = cria_elista(registro);
  ELista *dendo = cria_elista(registro2);
  cadastrar(lista, gabriel);
  mostrar_lista(lista);
  cadastrar(lista, dendo);
  mostrar_lista(lista);

}