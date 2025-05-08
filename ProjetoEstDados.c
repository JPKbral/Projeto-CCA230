#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <unistd.h>

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
  int rg;
  scanf(" %d", &rg);
  limpar_buffer();
  ELista *atual = lista->inicio;
  while(atual != NULL && rg != atual->dados->rg){
    atual = atual->proximo;
  }
  return atual;
}

//CADASTRAR

//Faz as perguntas necessárias para criar um cadastro completo e o insere na lista
void cadastrar_manual(Lista *lista){
  int dia, mes, ano, idade;
  char nome[NAME];
  long rg;
  printf("Qual o nome? ");
  scanf(" %[^\n]", nome);
  printf("Qual a idade?");
  scanf(" %d", &idade);
  printf("Qual o rg?");
  scanf(" %ld", &rg);
  //Verifica se o RG possui de 7 a 10 números
  if(rg<1000000 || rg>99999999999){
    printf("%d", rg);
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

//Formatação para um registro
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
  printf("\n--------------------------------\n");
  while(atual != NULL){
    imprimir_registro(atual->dados);
    atual = atual->proximo;
    printf("\n");
  }
  printf("--------------------------------\n");
}

//Verificar quem deve ter os dados atualizados e qual dado
void atualizar_dados(Lista *lista){
  printf("Qual o RG da pessoa que voce deseja alterar os dados?");
  ELista *atual = procurar_paciente(lista);
  if(atual == NULL){
    printf("Pessoa nao foi encontrada");
    Sleep(1500);
    return;
  }
  printf("\n");
  
  int escolha;
  imprimir_registro_escolha(atual->dados);
  printf("Qual dado deseja alterar?\n");
  scanf("%d", &escolha);
  limpar_buffer();
  //Casos para cada dado de um registro que pode ser alterado
  switch (escolha)
  {
  case 1:
    printf("Qual o novo nome? ");
    char novo_nome[NAME];
    scanf(" %[^\n]", novo_nome);
    limpar_buffer();
    atual->dados->nome = strdup(novo_nome);
    break;

  case 2:
    printf("Qual a idade?");
    scanf(" %d", &atual->dados->idade);
    limpar_buffer();
    break;

  case 3:
    printf("Qual o rg?");
    long verificarRg;
    scanf(" %ld", &verificarRg);
    limpar_buffer();
    //Verifica se o RG possui de 7 a 10 números
    if(verificarRg<1000000 || verificarRg>9999999999){
      printf("RG invalido\n");
      Sleep(1500);
      return;
    }
    atual->dados->rg = verificarRg;
    break;

  case 4:
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
  printf("Qual o RG de quem deseja excluir?");
  //Não utiliza da função de procura por utilizar do ponteiro 'anterior'
  int rg;
  scanf(" %d", &rg);
  limpar_buffer();
  ELista *atual = lista->inicio;
  ELista *anterior = NULL;
  while(atual != NULL && rg != atual->dados->rg){
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
  printf("%s teve o cadastro removido\n", atual->dados->nome);
  free(atual);
  lista->qtde--;
}

//O usuário digita o nome do paciente para ter o registro mais completo
void consultar_paciente(Lista *lista){
  printf("Qual o RG de quem voce deseja obter os dados?");
  ELista *atual = procurar_paciente(lista);
  if(atual == NULL){
    printf("Pessoa nao foi encontrada");
    Sleep(1500);
    return;
  }
  printf("\n--------------------------------\n");
  imprimir_registro(atual->dados);
  printf("--------------------------------\n");
}

//ATENDIMENTO

//Usuário digita o nome do paciente a ser adicionado na fila para o atendimento
void enfileirar_paciente(Lista *lista, Fila *fila){
  printf("Qual o RG de quem voce deseja inserir no antendimento?\n");
  ELista *aux = procurar_paciente(lista);
  if(aux == NULL){
    printf("Pessoa nao foi encontrada\n");
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
  printf("%s foi adicionado na fila para atendimento\n", aux->dados->nome);
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
  printf("%s foi removido da fila de atendimento\n", aux->dados->nome);
  free(aux);
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
    printf("%d -) %s\n", idx, atual->dados->nome);
    atual = atual->proximo;
    idx++;
  }
  printf("--------------------------------\n");
}

//ATENDIMENTO PRIORITÁRIO

int filho_esq(int pai){
  return 2*pai+1;
}

int filho_dir(int pai){
  return 2*pai+2;
}

int pai(int filho){
  return (filho - 1)/2;
}

int ultimo_pai(Heap *heap){
  if(heap->qtde <= 1){
    return 0;
  }
  return (heap->qtde/2)-1;
}

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

void construir(Heap *heap) {
  int idx;
  for(idx = ultimo_pai(heap); idx >= 0; idx--){
    peneirar(heap, idx);
  }
}

void mostrar_fila_prioriaria(Heap *heap) {
  if(heap->qtde == 0){
    printf("Ninguem esta na fila prioritaria\n");
    Sleep(1500);
    return;
  }
  int idx;
  for(idx = 0; idx < heap->qtde; idx++){
    printf("%d -) %s - Idade: %d\n", idx+1, heap->dados[idx]->nome, heap->dados[idx]->idade);
  }
  printf("\n");
}

void inserir_fila_prioriaria(Lista *lista, Heap *heap) {
  if(heap->qtde == LEN){
    return;
  }
  printf("Qual o RG de quem deseja inserir na fila prioritaria?");
  ELista *atual = procurar_paciente(lista);
  if(atual == NULL){
    printf("Pessoa nao foi encontrada\n");
    Sleep(1500);
    return;
  }
  Registro *dado = atual->dados;
  heap->dados[heap->qtde] = dado;
  heap->qtde++;
  construir(heap);
  printf("%s foi adicionado na fila prioritaria\n", atual->dados->nome);
}

void remover_fila_prioriaria(Heap *heap) {
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

int main(){
  Lista *lista = cria_lista();
  Fila *fila = cria_fila();
  Heap *fila_prioritaria = cria_heap();
  int menuEscolha = 0, segundaEscolha = 0, sair = 0;
  cadastrar_automatico(lista, "Geno Erti", 22, 1234567, 30, 4, 2025);
  cadastrar_automatico(lista, "Buno Gano", 23, 12345678, 23, 02, 2025);
  cadastrar_automatico(lista, "Ben Or", 67, 123456789, 11, 9, 2024);
  while(sair == 0){
    printf("Menu: \n 1-)Cadastrar \n 2-)Atendimento \n 3-)Atendimento Prioritario \n 4-)Pesquisa \n 5-)Desfazer \n 6-)Carregar/Salvar \n 7-)Sobre \n 8-)Sair \n ");
    scanf(" %d", &menuEscolha);
    limpar_buffer();
    switch (menuEscolha)
    {
    case 1:
      //Cadastrar
      printf("Cadastrar: \n 1-)Cadastrar novo paciente \n 2-)Consultar paciente cadastrado \n 3-)Mostrar lista completa \n 4-)Atualizar dados de paciente \n 5-)Remover paciente \n");
      scanf(" %d", &segundaEscolha);
      limpar_buffer();
      switch (segundaEscolha)
      {
      case 1:
        //Cadastrar novo paciente
        cadastrar_manual(lista);
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
        atualizar_dados(lista);
        break;

      case 5:
        //Remover cadastro do paciente
        remover_paciente(lista);
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
      limpar_buffer();
      switch (segundaEscolha)
      {
      case 1:
        //Enfileirar paciente
        enfileirar_paciente(lista, fila);
        Sleep(1500);
        break;
        
      case 2:
        //Desenfileirar paciente
        desenfileirar_paciente(fila);
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
      limpar_buffer();
      switch (segundaEscolha)
      {
      case 1:
        //Enfileirar paciente
        inserir_fila_prioriaria(lista, fila_prioritaria);
        Sleep(1500);
        break;
        
      case 2:
        //Desenfileirar paciente
        remover_fila_prioriaria(fila_prioritaria);
        Sleep(1500);
        break;
        
      case 3:
        //Mostrar fila
        mostrar_fila_prioriaria(fila_prioritaria);
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
      break;
    
    case 5:
      //Desfazer
      break;
    
    case 6:
      //Carregar/Salvar
      break;
    
    case 7:
      //Sobre
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