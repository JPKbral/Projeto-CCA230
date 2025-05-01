#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <unistd.h>

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
  registro->nome = strdup(nome);
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

void limpar_buffer(){
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
}

ELista *procurar_paciente(Lista *lista){
  char nome[50];
  fgets(nome, sizeof(nome), stdin);
  nome[strcspn(nome, "\n")] = 0;
  ELista *atual = lista->inicio;
  while(atual != NULL && strcmp(nome, atual->dados->nome) != 0){
    atual = atual->proximo;
  }
  return atual;
}

void cadastrar_manual(Lista *lista){
  int dia, mes, ano, idade;
  char nome[50];
  long rg;
  printf("Qual o nome? ");
  scanf(" %[^\n]", nome);
  printf("Qual a idade?");
  scanf(" %d", &idade);
  printf("Qual o rg?");
  scanf(" %ld", &rg);
  if(rg/1000000000 != 0 || rg/100000000 == 0 || rg<0){
    printf("RG invalido\n");
    Sleep(1500);
    return;
  }
  printf("Qual o dia de entrada?");
  scanf(" %d", &dia);
  printf("Qual o mes de entrada?");
  scanf(" %d", &mes);
  printf("Qual o ano de entrada?");
  scanf(" %d", &ano);
  limpar_buffer();
  Data *entrada = cria_data(dia, mes, ano);
  Registro *registro = cria_registro(nome, idade, rg, entrada);
  ELista *elista = cria_elista(registro);
  if(lista->qtde != 0){
    elista->proximo = lista->inicio;
  }
  lista->inicio = elista;
  lista->qtde++;
}

void cadastrar_automatico(Lista *lista, const char *nome, int idade, long rg, int dia, int mes, int ano){
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
  printf("\n--------------------------------\n\n");
  while(atual != NULL){
    imprimir_registro(atual->dados);
    atual = atual->proximo;
  }
  printf("--------------------------------\n");
}

void atualizar_dados(Lista *lista){
  mostrar_lista(lista);
  printf("De quem voce deseja alterar os dados?");
  ELista *atual = procurar_paciente(lista);
  if(atual == NULL){
    printf("Pessoa nao foi encontrada");
    Sleep(1500);
    return;
  }
  printf("\n");
  char escolha[10];
  imprimir_registro(atual->dados);
  printf("Qual dado deseja alterar?\n");
  fgets(escolha, sizeof(escolha), stdin);
  switch (escolha[0])
  {
  case 'n':case 'N':
    printf("Qual o novo nome? ");
    char novo_nome[50];
    scanf(" %[^\n]", novo_nome);
    limpar_buffer();
    atual->dados->nome = strdup(novo_nome);
    break;

  case 'i':case 'I':
    printf("Qual a idade?");
    scanf(" %d", &atual->dados->idade);
    limpar_buffer();
    break;

  case 'r':case 'R':
    printf("Qual o rg?");
    long verificarRg;
    scanf(" %ld", &verificarRg);
    limpar_buffer();
    if(verificarRg/1000000000 != 0 || verificarRg/100000000 == 0 || verificarRg<0){
      printf("RG invalido\n");
      Sleep(1500);
      return;
    }
    atual->dados->rg = verificarRg;
    break;

  case 'e':case 'E':
    printf("Qual o dia de entrada?");
    scanf(" %d", &atual->dados->entrada->dia);
    printf("Qual o mes de entrada?");
    scanf(" %d", &atual->dados->entrada->mes);
    printf("Qual o ano de entrada?");
    scanf(" %d", &atual->dados->entrada->ano);
    limpar_buffer();
    break;
  
  default:
    printf("Opcao Invalida\n");
    Sleep(1500);
    break;
  }

}

void remover_paciente(Lista *lista){
  if(lista->qtde == 0){
    printf("A lista esta vazia\n");
    Sleep(1500);
    return;
  }
  mostrar_lista(lista);
  printf("Quem deseja excluir?");
  char nome[50];
  fgets(nome, sizeof(nome), stdin);
  nome[strcspn(nome, "\n")] = 0;
  ELista *atual = lista->inicio;
  ELista *anterior = NULL;
  while(atual != NULL && strcmp(nome, atual->dados->nome) != 0){
    anterior = atual;
    atual = atual->proximo;
  }
  if(atual == NULL){
    printf("Pessoa nao foi encontrada\n");
    Sleep(1500);
    return;
  }
  if(lista->qtde == 1){
    lista->inicio = NULL;
  }
  else if(atual == lista->inicio){
    lista->inicio = atual->proximo;
  }
  else{
    anterior->proximo = atual->proximo;
  }
  free(atual);
  lista->qtde--;
}

void consultar_paciente(Lista *lista){
  mostrar_lista(lista);
  printf("De quem voce obter os dados?");
  ELista *atual = procurar_paciente(lista);
  if(atual == NULL){
    printf("Pessoa nao foi encontrada");
    Sleep(1500);
    return;
  }
  printf("\n--------------------------------\n\n");
  imprimir_registro(atual->dados);
  printf("--------------------------------\n");
}

int main(){
  Lista *lista = cria_lista();
  cadastrar_automatico(lista, "Geno Erti", 22, 928374920, 30, 4, 2025);
  cadastrar_automatico(lista, "Buno Gano", 23, 927394825, 23, 02, 2025);
  cadastrar_automatico(lista, "Ben Or", 67, 120923875, 11, 9, 2024);
  atualizar_dados(lista);
  atualizar_dados(lista);
  atualizar_dados(lista);
  mostrar_lista(lista);

}