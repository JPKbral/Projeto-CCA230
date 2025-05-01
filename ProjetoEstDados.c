#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <unistd.h>

#define LEN 20

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
}EABB;

typedef struct ABB{
  EABB *raiz;
  int qtde;
}ABB;

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

//Limpar o buffer do "\n" após utilizar scanf para o fgets funcionar
void limpar_buffer(){
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
}

//Usuário digita o nome do paciente e o código cria um ponteiro ELista para o paciente, se existir
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

//Faz as perguntas necessárias para criar um cadastro completo e o insere na lista
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
  //Verifica se o RG possui 9 números
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

//Para utilizar com o arquivo
void cadastrar_automatico(Lista *lista, const char *nome, int idade, long rg, int dia, int mes, int ano){
  Data *entrada = cria_data(dia, mes, ano);
  Registro *registro = cria_registro(nome, idade, rg, entrada);
  ELista *elista = cria_elista(registro);
  //Insere o novo cadastro no início da lista
  if(lista->qtde != 0){
    elista->proximo = lista->inicio;
  }
  lista->inicio = elista;
  lista->qtde++;
}

//Formatação correta para a data
void imprimir_data(Data *data){
  printf("%d/%d/%d", data->dia, data->mes, data->ano);
}

//Formatação correta para o RG
void imprimir_rg(long rg){
  int parte1 = (rg/10000000)%100;
  int parte2 = (rg/10000)%1000;
  int parte3 = (rg/10)%1000;
  int parte4 = rg%10;
  printf("%d.%d.%d-%d", parte1, parte2, parte3, parte4);
}

//Formatação para um registro
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

//Mostra apenas os nomes na lista
void mostrar_nome_lista(Lista *lista){
  if(lista->qtde == 0){
    printf("Ninguem esta na lista\n\n");
    return;
  }

  ELista *atual = lista->inicio;
  printf("\n--------------------------------\n");
  while(atual != NULL){
    printf(" %s\n", atual->dados->nome);
    atual = atual->proximo;
  }
  printf("--------------------------------\n");
}

//Mostra a lista completa
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

//Verificar quem deve ter os dados atualizados e qual dado
void atualizar_dados(Lista *lista){
  mostrar_nome_lista(lista);
  printf("De quem voce deseja alterar os dados?");
  ELista *atual = procurar_paciente(lista);
  if(atual == NULL){
    printf("Pessoa nao foi encontrada");
    Sleep(1500);
    return;
  }
  printf("\n");
  //10 de char para evitar overflow
  char escolha[10];
  imprimir_registro(atual->dados);
  printf("Qual dado deseja alterar?\n");
  fgets(escolha, sizeof(escolha), stdin);
  //Casos para cada dado de um registro que pode ser alterado
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

//Exclui o paciente que o usuário digitar o nome
void remover_paciente(Lista *lista){
  if(lista->qtde == 0){
    printf("A lista esta vazia\n");
    Sleep(1500);
    return;
  }
  mostrar_nome_lista(lista);
  printf("Quem deseja excluir?");
  //Não utiliza da função de procura por utilizar do ponteiro 'anterior'
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
  free(atual);
  lista->qtde--;
}

//O usuário digita o nome do paciente para ter o registro mais completo
void consultar_paciente(Lista *lista){
  mostrar_nome_lista(lista);
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

//Usuário digita o nome do paciente a ser adicionado na fila para o atendimento
void enfileirar_paciente(Lista *lista, Fila *fila){
  mostrar_nome_lista(lista);
  printf("Quem voce deseja inserir no antendimento?");
  ELista *aux = procurar_paciente(lista);
  if(aux == NULL){
    printf("Pessoa nao foi encontrada");
    Sleep(1500);
    return;
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
}

//Retira o primeiro paciente que foi adicionado na fila
void desenfileirar_paciente(Fila *fila){
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
  free(aux);
  fila->qtde--;
}

//Mostra toda a fila na sequência de atendimento
void mostrar_fila(Fila *fila){
  if(fila->qtde == 0){
    printf("Ninguem esta na fila\n\n");
    return;
  }
  EFila *atual = fila->head;
  int idx = 1;
  printf("\n--------------------------------\n");
  while(atual != NULL){
    printf("%d -)", idx);
    printf(" %s\n", atual->dados->nome);
    atual = atual->proximo;
    idx++;
  }
  printf("--------------------------------\n");
}

int main(){
  Lista *lista = cria_lista();
  Fila *fila = cria_fila();
  cadastrar_automatico(lista, "Geno Erti", 22, 928374920, 30, 4, 2025);
  cadastrar_automatico(lista, "Buno Gano", 23, 927394825, 23, 02, 2025);
  cadastrar_automatico(lista, "Ben Or", 67, 120923875, 11, 9, 2024);
  cadastrar_manual(lista);
  mostrar_lista(lista);
  mostrar_fila(fila);
  enfileirar_paciente(lista, fila);
  enfileirar_paciente(lista, fila);
  enfileirar_paciente(lista, fila);
  mostrar_fila(fila);
  desenfileirar_paciente(fila);
  mostrar_fila(fila);
  desenfileirar_paciente(fila);
  mostrar_fila(fila);
  desenfileirar_paciente(fila);
  desenfileirar_paciente(fila);
  mostrar_fila(fila);
  

}