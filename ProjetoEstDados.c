#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 20

typedef struct Registro{
  string nome;
  int idade;
  string rg;
  Data entrada;
}Registro;

typedef struct Data{
  int dia;
  int mes;
  int ano;
}Data;

typedef struct Lista{
    int qtde;
    ELista *inicio;
}Lista;

typedef struct ELista{
    Registro *dados;
    ELista *proximo;
}ELista;

typedef struct Fila{
    int qtde;
    EFila *head;
    EFila *tail;
}Fila;

typedef struct EFila{
    EFila *proximo;
    Registro dados;
}EFila;

typedef struct Heap{
  Registro dados[LEN];
  int qtde;
}Heap;