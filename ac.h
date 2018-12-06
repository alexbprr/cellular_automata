#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define T 20
#define N_STATES 2
#define MAX_NEIGHBOURS 8
#define N_RULES 50

// ASTNode *super = new_ASTNode(nome,"Expr");
// Expr *novo = malloc(sizeof(Expr));
// build(novo, super);
// free(super);
// #define build(dest, src) memcpy(dest,src,sizeof(*src))

struct TCell
{
    int id;
    int state;
    int x;
    int y;
    int numberofneighbours;
    struct TCell **neighbours;//list of neighbours
    int numberofrules;
    struct TRule *rules;//every cell has rules associated with it
    //struct TComplexRule* rules;
    int numberofcomplexrules;
    struct TComplexRule* complexrules;//ponteiro de ponteiro
};

struct TAc
{
    struct TCell ***domain;
    int lin;
    int col;
    int t;
    int t_pre;
    int t_cur;
    int dimension;
    int neighbourhood;
    char* name;
    FILE* fp;
};

struct TRule
{
    //struct TComplexRule* super; //TRule herda de TComplexRule
    int id;
    int type; //0: test all neighbours; 1: test specific neighbours; 2: test a numberofneighbours
    struct TCell* cur_cell; //current cell
    int stateCurCell; //current cell state to be tested
    int neighbourid;
    struct TCell* neighbour_cell; //Referência para um vizinho específico
    int numberofneighbours;
    int state;//state value to be tested
    int newstate;//new state received by the current cell; função para calcular novo estado
    int op_cmp;//0: ==; 1: <; 2: >;
    int incx;
    int incy;
    /*Lista de incrementos e lista de estados
    neighbour_cell: Vetor de structs (vizinhos)
    Criar uma nova struct para conter as informações do vizinho: incx, incy, state,
        111: incrementsx[0] = -1; incrementsy[0] = 0; states[0] = 1;
            incrementsx[1] = 1; incrementsy[1] = 0; states[1] = 1;
            for ()
                vetor_structs = findNeighbourCell(c, c->x + c->incrementsx[i], c->y + c->incrementsy[i]);
            verifyIfRuleSatisfied(){


        }
    */
};

//A Complex rule is formed by more than one rule. Cada regra pode se referir a uma célula em outro domínio.
//The complex rule is satisfied when all of its constituting rules are satisfied (and operator) or
//when one of the rules is satisfied (or operator)
struct TComplexRule
{
    int id;
    struct TRule** rules; //Each complex rule has its own copy of the rules
    int numberofrules;
    int op;//op = 0: and; op = 1: or;
    struct TCell* cur_cell;
    int stateCurCell;
    int newstate;
    // int id;
    // int type; //0: test all neighbours; 1: test a single neighbour; 2: test a numberofneighbours
    // struct TCell* cur_cell; //current cell
    // int stateCurCell; //current cell state to be tested
    // int neighbourid;
    // struct TCell* neighbour_cell; //Referência para um vizinho específico
    // int numberofneighbours;
    // int state;//state value to be tested
    // int newstate;//new state received by the current cell; função para calcular novo estado
    // int op_cmp;//0: ==; 1: <; 2: >;
};

//For each cellular type we instantiate struct TCellularType.
struct TCellularType {
    struct TAc* ac; //Every cellular type has a pointer to its AC
    //We add references to cells from another Cellular Type in the field struct TCell **neighbours (that is the list of neighbours)
};

typedef struct TCell Cell;
typedef struct TAc Ac;
typedef struct TRule Rule;
typedef struct TComplexRule ComplexRule;

Ac* createAC(int dimension, int m, int n, int neighbourhood);

void initializeNeighbourhood(Ac* ac, Cell* c, int t);

//TODO
void addNeighboursFromOtherAC(Ac* other_ac, Cell* c, int t);

void initializeBoundary(Ac* ac);

void randomACInitializer(Ac* ac);

void incrementTimeStep(Ac* ac);

void updateState(Ac* ac, int x, int y, int newstate);

Rule* createRuleWithPosition(Cell* cur_cell, int stateCurCell, int incx, int incy, int state, int newstate);

Rule* createRule(int type, Cell* cur_cell, int stateCurCell, int numberofneighbours, int state, int op_cmp, int newstate); //;

ComplexRule* createComplexRule(int op, Cell* cur_cell, int stateCurCell, int newstate);

ComplexRule* addRuleToComplexRule(Rule* r, ComplexRule* cr);

void addRuleToAllCells(Ac* ac, Rule* r);

void addComplexRuleToAllCells(Ac* ac, ComplexRule* r);

void createACRule(Ac* ac, int type, int stateCurCell, int numberofneighbours, int state, int op_cmp, int newstate);

void createACRuleWithPosition(Ac* ac, int stateCurCell, int incx, int incy, int state, int newstate);

Cell* findNeighbourCell(Cell* cell, int x, int y);

void applyAllComplexRules(Ac* ac, Cell* c);

void applyComplexRule(Ac* ac, ComplexRule* cr);

int verifyIfRuleSatisfied(Rule* r);

void testRuleForAllCells(Ac* ac, int rule_number);

void applyRules(Ac* ac, Cell* c);

void updateAC(Ac* ac);

void printAc(Ac* ac);

void saveAc(Ac* ac);

void printCells(Cell** cells);

void printNeighboursStates(Cell c);

int findRandomPosition(Ac* ac);

int isTrue(float prob);

Cell* chooseRandomNeighbour(Ac* ac, Cell* c);
//
// int* neighboursThatSatisfyRule(Rule* r);
//
// void cellsThatSatisfyRule(Rule* r);


/*createCellularType():
    Every cellular type has a reference to struct TAc.
*/

/*
  createMollecularType():
    Create a PDE for a molecule
*/

 //   def InitializeNeighbour(self, c, cells, x, y, t):
 //
 //   def initializeBoundary(self):
 //       for t in range(0,2):
 //           for i in range(0,SIZE_X): #Inicializa as celulas "fantasmas" da matriz
 //               self.Ac.cells[t][0][i].x = 0
 //               self.Ac.cells[t][0][i].y = i
 //               self.Ac.cells[t][0][i].state = -2
 //
 //               self.Ac.cells[t][SIZE_X-1][i].x = SIZE_X-1
 //               self.Ac.cells[t][SIZE_X-1][i].y = i
 //               self.Ac.cells[t][SIZE_X-1][i].state = -2
 //
 //               self.Ac.cells[t][i][0].x = i
 //               self.Ac.cells[t][i][0].y = 0
 //               self.Ac.cells[t][i][0].state = -2
 //
 //               self.Ac.cells[t][i][SIZE_Y-1].x = i
 //               self.Ac.cells[t][i][SIZE_Y-1].y = SIZE_Y-1
 //               self.Ac.cells[t][i][SIZE_Y-1].state = -2
 //
