#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <unistd.h>

#define LEN 20
#define NAME 50

//Carregar/Salvar

long rg_numeros(char rg_string[14]) {
  long numeros = 0;
  int i = 0, j = 0;

  for (i = 0; i < 13; i++) {
      if (rg_string[i] != '.' && rg_string[i] != '-' && rg_string[i] != '\0') {
        numeros = numeros*10 + (rg_string[i] - 48);
        j++;
      }
  }
  return numeros;
}

void carregar_arquivo(FILE *arquivo){
  arquivo = fopen("cadatros_pacientes.txt", "r");
  //Verifica se o arquivo foi aberto
  if(arquivo == NULL){
    printf("Erro ao abrir o arquivo\n");
  }

  char verificar;
  char nome_arquivo[100];
  int dia, mes, ano, idade;
  const char *nome;
  char rg[14];
  //Lê o arquivo até a última linha ou encontrar um erro
  while (fscanf(arquivo, " %c", &verificar) != EOF) {
    //Se a linha começa com '-' ou '\n' toda linha é descartada
    if (verificar == '-' || verificar == '\n') {
        //Lê a linha e a descarta
        fscanf(arquivo, "%*[^\n]\n");
        //Volta para o começo do While
        continue;
    }

    //Salva o nome no registro novo
    //%[^\n] lê sem considerar '\n'
    fscanf(arquivo, "ome: %[^\n]", nome_arquivo);
    //strdup para alocar corretamente, pois nome é um ponteiro
    nome = strdup(nome_arquivo);
    //Salva o idade no registro novo
    fscanf(arquivo, "dade: %d\n", &idade);
    //Salva o RG no registro novo
    //Garante que vão passar o rg como 13 char para a função
    fscanf(arquivo, "G: %c\n", &rg);
    //Converte string em long se linha não estiver vazia
    //Salva a entrada no registro novo
    fscanf(arquivo, "ntrada: %d/%d/%d\n", &dia, &mes, &ano);
    
    //Salva o registro na lista
    if(nome != NULL && idade != 0 && rg[0] != '0' && dia != 0){
      //Se tiver todos os dados salva como um cadastro
      printf("%s, %d, %s, %d, %d, %d\n", nome, idade, rg, dia, mes, ano);
      //Limpa a estrutura para recerber outros cadastros
      nome = NULL;
      idade = 0;
      rg[0] = '0';
      dia = 0;
    }
  }  
  //Fecha o arquivo
  fclose(arquivo);
  printf("Dados carregados com sucesso\n");
}


int main(){
  FILE *arquivo;
  carregar_arquivo(arquivo);
  
}