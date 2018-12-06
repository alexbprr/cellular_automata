#include "ac.h"
#include <sys/resource.h>

// Jogo da vida - As regras são simples e elegantes:
//
//    1. Qualquer célula viva com menos de dois vizinhos vivos morre de solidão.
//    2. Qualquer célula viva com mais de três vizinhos vivos morre de superpopulação.
//    3. Qualquer célula com exatamente três vizinhos vivos se torna uma célula viva.
//    4. Qualquer célula com dois vizinhos vivos continua no mesmo estado para a próxima geração.

/*
  Cada tipo de célula possui o seu próprio AC e um conjunto de regras
*/

/*
Criação de regras complexas testando cada vizinho específico
Regra 30 do automato elementar de Wolfram:
111 -> 0
110 -> 0
101 -> 0
http://mathworld.wolfram.com/Rule30.html

*/
//void createACRule(Ac* ac, int type, int stateCurCell, int numberofneighbours, int state, int op_cmp, int newstate);
//void createACRuleWithPosition(Ac* ac, int stateCurCell, int incx, int incy, int state, int newstate);
//ComplexRule* createComplexRule(int op, Cell* cur_cell, int stateCurCell, int newstate)
//Rule* createRuleWithPosition(Cell* cur_cell, int stateCurCell, Cell* neighbour_cell, int state, int newstate)
/*
    Se uma regra está dentro de uma regra complexa, ela não pode ser testada separadamente.
    Possibilidade: adicionar uma regra simples a uma regra complexa mesmo se houver somente uma regra simples
    para ser avaliada.
*/
int main()
{
  struct rusage resources;
  int rc;
  double utime, stime, total_time;
  Ac* ac = createAC(1,10,1,1);

  ComplexRule *cr1 = createComplexRule(0,NULL,-1,0);//op=and, new state = 0
  printf("Passei 1\n");
  Rule* r1_e = createRuleWithPosition(NULL,1,-1,0,1,0);//teste 1 esq
  printf("Passei 2\n");
  Rule* r1_d = createRuleWithPosition(NULL,1,1,0,1,0);//teste 1 dir
  printf("Passei 3\n");
  cr1 = addRuleToComplexRule(r1_e,cr1);
  printf("Passei 4\n");
  cr1 = addRuleToComplexRule(r1_d,cr1);
  printf("Passei 5\n");
  //addComplexRuleToAllCells(ac,cr1);
  printf("Passei 6\n");
  //applyAllComplexRules(ac,c);

  //int op_cmp, int newstate); type = 2 (test a number of neighbors)
  // createACRule(ac,2,1,2,1,1,0);//teste 1
  // createACRule(ac,2,1,3,1,2,0);//regra 2
  // createACRule(ac,2,0,3,1,0,1);//regra 3 para célula morta
  // createACRule(ac,2,1,3,1,0,1);//regra 3 para célula viva
  // createACRule(ac,2,0,2,1,0,0);//regra 4 para célula morta
  // createACRule(ac,2,1,2,1,0,1);//regra 4 para célula viva
  // //createACRule(ac,0,0,4,0,0,1);//regra 5
  randomACInitializer(ac);
  //printAc(ac);
  saveAc(ac);
  for (ac->t=1; ac->t < T; ac->t++)
  {
    //testRuleForAllCells(ac,0);
    //printNeighboursStates(ac->domain[ac->t_pre][8][0]);
    updateAC(ac);
    //printAc(ac);
    saveAc(ac);
  }
  fclose(ac->fp);

  if((rc = getrusage(RUSAGE_SELF, &resources)) != 0) {
      perror("getrusage failed");
  }
  utime = (double) resources.ru_utime.tv_sec + 1.e-6 * (double) resources.ru_utime.tv_usec;
  stime = (double) resources.ru_stime.tv_sec + 1.e-6 * (double) resources.ru_stime.tv_usec;
  total_time = utime+stime;
  printf("User time %.3f, System time %.3f, Total Time %.3f\n",utime, stime, total_time);
  return 0;
}
