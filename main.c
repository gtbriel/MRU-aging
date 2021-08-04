#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#define MEMSIZE 10
#define MSB 0b10000000
#define BINARYSIZE 8
#define DISPLAYSIZE 5
#define INITIALBINARY 0b00000000
int index_display;
int priority;

typedef struct tItem {
	uint64_t id;
  int id_display;
	uint64_t paramA;
  uint64_t paramB;
  uint64_t paramC;
} Item;

typedef struct tCell {
	Item itemCell;
	struct tCell* proxCell;
} Cell;

typedef struct tCell* tApontador;

typedef struct tFila {
	tApontador frente, tras;
	int tamanho;
} Fila;

Item createNode() {
  Item newNode;
  newNode.id = INITIALBINARY;  
  newNode.id_display = index_display;
  index_display += 1;
  newNode.paramA = rand();
  newNode.paramB = rand();
  newNode.paramC = rand();
  return newNode;
}

void IniciaFila(Fila* pF) {
	pF->frente = pF->tras = NULL;
	pF->tamanho = 0;
}

int FilaEhVazia(Fila* pF) {
	return pF->tamanho == 0;
}

int Enfileira(Fila* pF, Item i) {
	tApontador novaCell;
	novaCell = (tApontador)malloc(sizeof(Cell));
	novaCell->itemCell = i;
	novaCell->proxCell = NULL;
	if (FilaEhVazia(pF))
		pF->frente = novaCell;
	else
		pF->tras->proxCell = novaCell;
	pF->tras = novaCell;
	pF->tamanho++;
	return 1;
}

int Desenfileira(Fila* pF, Item* pI) {
	tApontador apontAux;
	if (FilaEhVazia(pF))
		return 0;
	apontAux = pF->frente;
	pF->frente = apontAux->proxCell;
	*pI = apontAux->itemCell;
	pF->tamanho--;
	return 1;
}

void showbits( unsigned int x ) {
  int i=0;
  for (i = BINARYSIZE - 1; i >= 0; i--)  {
    putchar(x & (1u << i) ? '1' : '0');
  }
  printf("\n");
}

void fillDisplay(Fila* paginasExistentes, Item *display) {
  int i;
  for(i=0;i < DISPLAYSIZE;i++) {
    Item _ref;
    Desenfileira(paginasExistentes, &_ref);
    display[i] = _ref;
  }
}

void printDisplay(int i, Item *display) {  
  printf("Página %d:\n",display[i].id_display);
  showbits(display[i].id);
  //printf("ParamA = %lu\nParamB = %lu\nParamC = %lu\n",display[i].paramA,display[i].paramB,display[i].paramC);
  printf("-----------\n");
}

void aging(int i, Item *display) {  
  int R_priority = ((double)rand()/(double)RAND_MAX) * 2; // Como se fosse os R Bits;
  display[i].id >>= 1;
  if(R_priority) {    
    display[i].id |= MSB;
  }
  printDisplay(i,display);

}

int findLessPriority(Item *display) {
  int index_less = display[0].id_display;
  int i;

  for(i=0;i < DISPLAYSIZE;i++) {
    if(display[index_less].id >= display[i].id) {
      index_less = i;
    }
  }

  return index_less;
}

void resetingPriority(Item *display) {

  int i;

  for(i=0;i < DISPLAYSIZE;i++) {
    display[i].id = INITIALBINARY;
  }

}

void MRU(Fila* paginasExistentes, Item *display) { 

  int nBreaks = ((double)rand()/(double)RAND_MAX) * 10;
  int i,j,runningTimes = 10,k;  

  while(k <= runningTimes) {

    if(k % 5 == 0) {
      printf("\n--**RESETANDO PRIORIDADE**-- \n\n");
      resetingPriority(display);
    }
    
    for(i = 0;i < nBreaks;i++) {
      printf("------- Envelhecendo (%d) -------\n",i);
      for(j=0;j < DISPLAYSIZE;j++) {
        aging(j,display);
      }
    }

    printf("------- Finalizando envelhecendo  -------\n");

    printf("------- Trocando Páginas -------\n");

    Item _inserted;
    Desenfileira(paginasExistentes, &_inserted);
    int index_p = findLessPriority(display);
    Item _removed = display[index_p];
    _removed.id = INITIALBINARY;
    Enfileira(paginasExistentes, _removed);
    display[index_p] = _inserted;
    printf("Página %d inserida no local da página %d\n",_inserted.id_display,_removed.id_display);
    k++;
  }

}




int main(void) {
  int i=0;
  Fila paginasExistentes;
  Item display[DISPLAYSIZE];
  index_display = 0;  
  IniciaFila(&paginasExistentes);

  for(i=0;i < MEMSIZE;i++) {
    Enfileira(&paginasExistentes, createNode());
  }

  fillDisplay(&paginasExistentes,display);
  printf("{Display inicial}\n");
  
  for(i=0;i < DISPLAYSIZE;i++) {
    printDisplay(i,display);
  }
  
  MRU(&paginasExistentes, display);
  return 0;
}
