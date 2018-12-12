#include "ac.h"

Ac* createAC(int dimension, int m, int n, int neighbourhood)
{
  Ac* novoac = malloc(sizeof(Ac));
  novoac->lin = m;
  novoac->col = n;
  novoac->domain = malloc(2*sizeof(Cell**));
  // ANOTAÇÃO
  // 2 simboliza .... ?
  // Talvez uma constante global pra esclarecer?

  novoac->dimension = dimension;
  novoac->neighbourhood = neighbourhood;
  for (int i=0; i < 2; i++)
  {
    novoac->domain[i] = malloc(m*sizeof(Cell*));
    for (int j=0; j < m; j++)
      novoac->domain[i][j] = malloc(n*sizeof(Cell));
  }
  for (int i=0; i < 2; i++)
    for (int j=0; j < m; j++)
      for (int k=0; k < n; k++)
      {
        novoac->domain[i][j][k].id = j*n + k;
        novoac->domain[i][j][k].state = -1;
        novoac->domain[i][j][k].x = j;
        novoac->domain[i][j][k].y = k;
        //Define numberofrules for each AC
        novoac->domain[i][j][k].numberofrules = 0;
        //novoac->domain[i][j][k].rules = (ComplexRule*)malloc(50*sizeof(ComplexRule));
        novoac->domain[i][j][k].rules = (Rule*)malloc(N_RULES*sizeof(Rule));
        novoac->domain[i][j][k].complexrules = (ComplexRule*)malloc(N_RULES*sizeof(ComplexRule));
        // ANOTAÇÃO
        // Alocar diretamente 50 espaços por célula pode ser custoso para
        // Automatos maiores. Talvez um `int` para representar a quantidade presente
        // de regras e realocar sempre que estivermos prestes a estourar esse
        // limite seja mais vantajoso

        // novoac->domain[i][j][k].rules = (Rule**)malloc(N_RULES*sizeof(Rule*));
        // novoac->domain[i][j][k].complexrules = (ComplexRule**)malloc(N_RULES*sizeof(ComplexRule*));
        // for (int m=0; m < N_RULES; m++)
        // {
        //     novoac->domain[i][j][k].rules[m] = (Rule*)malloc(sizeof(Rule));
        //     novoac->domain[i][j][k].complexrules[m] = (ComplexRule*)malloc(sizeof(ComplexRule));
        // }
        initializeNeighbourhood(novoac, &novoac->domain[i][j][k], i);
      }
    novoac->t = 0;
    novoac->t_pre = 0;
    novoac->t_cur = 1;
    novoac->fp = fopen("acconf.txt", "w");
    return novoac;
}

void initializeNeighbourhood(Ac* ac, Cell* c, int t)
{
  int num_neighbours = 0;
  if (ac->dimension == 1 && ac->neighbourhood == 1)
  {
    c->neighbours = (Cell**)malloc(2*sizeof(Cell*));
    // ANOTAÇÃO
    // Valor 2 reaparece aqui

    if (c->x > 0)
    {
      c->neighbours[num_neighbours] = &ac->domain[t][c->x-1][c->y];
      num_neighbours++;
    }
    if (c->x < ac->lin-1)
    {
      c->neighbours[num_neighbours] = &ac->domain[t][c->x+1][c->y];
      num_neighbours++;
    }
  }
  else if (ac->dimension == 1 && ac->neighbourhood == 2)
  {
    c->neighbours = (Cell**)malloc(4*sizeof(Cell*));
    // ANOTAÇÃO
    // Então 2 * ac->neighbourhood * ac->dimension ?

    if (c->x > 0)
    {
      c->neighbours[num_neighbours] = &ac->domain[t][c->x-1][c->y];
      num_neighbours++;
    }
    if (c->x < ac->lin-1)
    {
      c->neighbours[num_neighbours] = &ac->domain[t][c->x+1][c->y];
      num_neighbours++;
    }
    if (c->x > 1)
    {
      c->neighbours[num_neighbours] = &ac->domain[t][c->x-2][c->y];
      num_neighbours++;
    }
    if (c->x < ac->lin-2)
    {
      c->neighbours[num_neighbours] = &ac->domain[t][c->x+2][c->y];
      num_neighbours++;
    }
  }
  if (ac->dimension == 2 && ac->neighbourhood == 1)
  {
    c->neighbours = (Cell**)malloc(4*sizeof(Cell*));
    if (c->x > 0)
    {
      c->neighbours[num_neighbours] = &ac->domain[t][c->x-1][c->y];
      num_neighbours++;
    }
    if (c->x < ac->lin-1)
    {
      c->neighbours[num_neighbours] = &ac->domain[t][c->x+1][c->y];
      num_neighbours++;
    }
    if (c->y > 0)
    {
      c->neighbours[num_neighbours] = &ac->domain[t][c->x][c->y-1];
      num_neighbours++;
    }
    if (c->y < ac->col-1)
    {
      c->neighbours[num_neighbours] = &ac->domain[t][c->x][c->y+1];
      num_neighbours++;
    }
  }
  if (ac->dimension == 2 && ac->neighbourhood == 2)
  {
    c->neighbours = (Cell**)malloc(8*sizeof(Cell*));
    if (c->y > 0)
    {
      c->neighbours[num_neighbours] = &ac->domain[t][c->x][c->y-1];
      num_neighbours++;
      if (c->x > 0)
      {
        c->neighbours[num_neighbours] = &ac->domain[t][c->x-1][c->y-1];
        num_neighbours++;
      }
    }
    if (c->x > 0)
    {
      c->neighbours[num_neighbours] = &ac->domain[t][c->x-1][c->y];
      num_neighbours++;
      if (c->y < ac->col-1)
      {
        c->neighbours[num_neighbours] = &ac->domain[t][c->x-1][c->y+1];
        num_neighbours++;
      }
    }
    if (c->y < ac->col-1)
    {
      c->neighbours[num_neighbours] = &ac->domain[t][c->x][c->y+1];
      num_neighbours++;
      if (c->x < ac->lin-1)
      {
        c->neighbours[num_neighbours] = &ac->domain[t][c->x+1][c->y+1];
        num_neighbours++;
      }
    }
    if (c->x < ac->lin-1)
    {
      c->neighbours[num_neighbours] = &ac->domain[t][c->x+1][c->y];
      num_neighbours++;
      if (c->y > 0)
      {
        c->neighbours[num_neighbours] = &ac->domain[t][c->x+1][c->y-1];
        num_neighbours++;
      }
    }
  }
  c->numberofneighbours = num_neighbours;
}

void randomACInitializer(Ac* ac)
{
    srand((unsigned)time(NULL));
    for (int j=0; j < ac->lin; j++)
        for (int k=0; k < ac->col; k++)
            ac->domain[ac->t_cur][j][k].state = rand()%N_STATES;
    incrementTimeStep(ac);
}

void incrementTimeStep(Ac* ac)
{
  for (int j=0; j < ac->lin; j++)
    for (int k=0; k < ac->col; k++)
        ac->domain[ac->t_pre][j][k].state = ac->domain[ac->t_cur][j][k].state;
  ac->t++;
  if (ac->t % 2 == 0)
  {
    ac->t_pre = 0;
    ac->t_cur = 1;
  }
  else
  {
    ac->t_pre = 1;
    ac->t_cur = 0;
  }
}

void updateState(Ac* ac, int x, int y, int newstate)
{
  ac->domain[ac->t_cur][x][y].state = newstate;
}

/* Passa a posição do vizinho para a regra e ela encontra quem é
 Consulta-se a posição x e y de cada um dos possíveis vizinhos para determinar quem é
 e guarda a referência para ele no atributo neighbour_cell
 */
Rule* createRuleWithPosition(Cell* cur_cell, int stateCurCell, int incx, int incy, int state, int newstate)
{
    Rule* newrule = malloc(sizeof(Rule));
    //newrule->id = cur_cell->numberofrules;
    newrule->type = -1; //Invalid value for debbuging
    //newrule->cur_cell = cur_cell;
    newrule->stateCurCell = stateCurCell;
    newrule->neighbour_cell = NULL;
    newrule->numberofneighbours = 1;
    newrule->state = state;
    newrule->newstate = newstate;
    newrule->op_cmp = -1; //Invalid value for debbuging
    newrule->incx = incx;
    newrule->incy = incy;
    // //cur_cell->rules[cur_cell->numberofrules] = *newrule;
    //cur_cell->numberofrules++;
    return newrule;
}

Rule* createRule(int type, Cell* cur_cell, int stateCurCell, int numberofneighbours, int state, int op_cmp, int newstate) //
{
    Rule* newrule = malloc(sizeof(Rule));
    //newrule->id = cur_cell->numberofrules;
    newrule->type = type;
    //newrule->cur_cell = cur_cell;
    newrule->stateCurCell = stateCurCell;
    newrule->neighbour_cell = NULL;
    newrule->numberofneighbours = numberofneighbours;
    newrule->state = state;
    newrule->op_cmp = op_cmp;
    newrule->newstate = newstate;
    //cur_cell->rules[cur_cell->numberofrules] = *newrule;
    //cur_cell->numberofrules++;
    return newrule;
}

ComplexRule* createComplexRule(int op, Cell* cur_cell, int stateCurCell, int newstate)
{
    ComplexRule* cr = (ComplexRule*)malloc(sizeof(ComplexRule));
    //cr->id = 0;
    cr->rules = (Rule**)malloc(10*sizeof(Rule*));
    for (int i=0; i < 10; i++)
        cr->rules[i] = (Rule*)malloc(sizeof(Rule));
    cr->numberofrules = 0;
    cr->op = op;
    cr->newstate = newstate;
    return cr;
}

// ComplexRule* createComplexRuleToAllCells(int op, Cell* cur_cell, int stateCurCell, int newstate)
// {}

ComplexRule* addRuleToComplexRule(Rule* r, ComplexRule* cr)
{
    //cr->rules[cr->numberofrules] = *r;
    cr->rules[cr->numberofrules] = r;
    cr->numberofrules++;
    return cr;
}

// Add a rule to all cells of the domain
void addRuleToAllCells(Ac* ac, Rule* r)
{
    Cell* c;
    Rule myr;
    for (int i=0; i < 2; i++)
        for (int j=0; j < ac->lin; j++)
            for (int k=0; k < ac->col; k++)
            {
                c = &(ac->domain[i][j][k]);
                myr = *r;
                r->neighbour_cell = findNeighbourCell(c, c->x + r->incx, c->y + r->incy);
                r->cur_cell = c;
                c->rules[c->numberofrules] = *r;
                c->numberofrules++;
            }
}

// Add a complex rule to all cells of the domain
// void addComplexRuleToAllCells(Ac* ac, ComplexRule* cr)
// {
//     Cell* c;
//     //Each cell has its own copy of the rules <- Verify this
//     Rule r;
//     ComplexRule mycr;
//     for (int i=0; i < 2; i++)
//         for (int j=0; j < ac->lin; j++)
//             for (int k=0; k < ac->col; k++)
//             {
//                 c = &(ac->domain[i][j][k]); //Define the current cell
//                 printf("Analyzed cell (%d,%d)\n", c->x, c->y);
//                 /*Para cada regra pertencente à regra complexa, é necessário verificar se existem vizinhos
//                 definidos com base em incrementos.
//                 */
//                 for (int m=0; m < cr->numberofrules; m++)
//                 {
//                     r = *(cr->rules[m]);
//                     r.neighbour_cell = findNeighbourCell(c, c->x + r.incx, c->y + r.incy);
//                     if (r.neighbour_cell!=NULL)
//                         printf("Neighbour found: (%d,%d)\n", r.neighbour_cell->x, r.neighbour_cell->y);
//                     r.cur_cell = c;
//                     /*Não posso adicionar no vetor rules da célula corrente
//                     porque as regras devem ser avaliadas conjuntamente.
//                     */
//                 }
//                 //Copy all rules
//                 mycr = *(cr); //esta errado
//                 c->complexrules[c->numberofcomplexrules] = mycr; //Add a new complex rule to current cell
//                 c->numberofcomplexrules++;
//             }
// }

// void addRuleToCell(Cell* cur_cell, Rule* r)
// {
//     cur_cell->rules[cur_cell->numberofrules] = r;
//     cur_cell->numberofrules++;
//     r->cur_cell = cur_cell;
// }
//
// void addComplexRuleToCell(Cell* cur_cell, ComplexRule* r)
// {
//     cur_cell->complexrules[cur_cell->numberofcomplexrules] = r;
//     cur_cell->numberofcomplexrules++;
//     r->cur_cell = cur_cell;
// }

void createACRule(Ac* ac, int type, int stateCurCell, int numberofneighbours, int state, int op_cmp, int newstate)
{
    for (int i=0; i < 2; i++)
        for (int j=0; j < ac->lin; j++)
            for (int k=0; k < ac->col; k++)
                createRule(type,&ac->domain[i][j][k],stateCurCell,numberofneighbours,state,op_cmp,newstate);
}

//passar pos relativa do vizinho
void createACRuleWithPosition(Ac* ac, int stateCurCell, int incx, int incy, int state, int newstate)
{
    Cell* c;
    for (int i=0; i < 2; i++)
      for (int j=0; j < ac->lin; j++)
        for (int k=0; k < ac->col; k++)
        {
            c = findNeighbourCell(&ac->domain[i][j][k], ac->domain[i][j][k].x+incx, ac->domain[i][j][k].y+incy);
            //if (c != NULL)
                //createRuleWithPosition(&ac->domain[i][j][k],stateCurCell,c,state,newstate);
        }
}

// void createACComplexRuleWithPosition(Ac* ac, int op, int stateCurCell, int incx, int incy, int state, int newstate)
// {
//     Cell* c, Rule;
//     for (int i=0; i < 2; i++)
//       for (int j=0; j < ac->lin; j++)
//         for (int k=0; k < ac->col; k++)
//         {
//             c = findNeighbourCell(&ac->domain[i][j][k], ac->domain[i][j][k].x+incx, ac->domain[i][j][k].y+incy);
//             if (c != NULL)
//             {
//                 r = createRuleWithPosition(&ac->domain[i][j][k],stateCurCell,c,state,newstate);
//             }
//         }
// }

Cell* findNeighbourCell(Cell* cell, int x, int y)
{
    //printf("find(%d,%d)\n", x, y);
    for (int i=0; i < cell->numberofneighbours; i++)
    {
        if (cell->neighbours[i]->x == x && cell->neighbours[i]->y == y)
        {
            printf("Neighbour found: (%d,%d)\n", cell->neighbours[i]->x, cell->neighbours[i]->y);
            return cell->neighbours[i];
        }
    }
    printf("Error! A neighbour with position (%d,%d) was not found.\n", x, y);
    //Imprimir os erros em um arquivo de log
    return NULL;
}

void applyComplexRule(Ac* ac, ComplexRule* cr)
{
//     int satisfied[cr->numberofrules], i;
//     for (i=0; i < cr->numberofrules; i++)
//     {
//         satisfied[i] = 0;
//         if (cr->rules[i]->cur_cell != NULL && cr->rules[i]->neighbour_cell != NULL)
//             printf("cur_cell state: %d, neighbour cell state: %d, newstate: %d\n", cr->rules[i]->stateCurCell, cr->rules[i]->neighbour_cell->state, cr->rules[i]->newstate);
//         if (cr->rules[i]->neighbour_cell != NULL)
//         {
//             printf("Neighbour cell data:\n");
//             printf("\t pos %d,%d\n", cr->rules[i]->neighbour_cell->x, cr->rules[i]->neighbour_cell->y);
//             printf("\t state %d\n", cr->rules[i]->neighbour_cell->state);
//         }
//         satisfied[i] = verifyIfRuleSatisfied(cr->rules[i]);
//     }//end for
//     int result;
//     if (cr->op == 0) //and
//     {
//         result = 1;
//         i = 0;
//         while (i < cr->numberofrules)
//         {
//             result = result && satisfied[i];
//             printf("result: %d\n", result);
//             if (result == 0)
//                 break;
//             i++;
//         }
//     }
//     else if (cr->op == 1) //or
//     {
//         result = 0;
//         i = 0;
//         while (i < cr->numberofrules)
//         {
//             result = result || satisfied[i];
//             if (result == 0)
//                 break;
//             i++;
//         }
//     }
//     if (result)
//     {
//         printf("Complex rule satisfied.\n");
//         printf("Pos of current cell: (%d,%d)\n", cr->rules[0]->cur_cell->x, cr->rules[0]->cur_cell->y);
//         updateState(ac,cr->rules[0]->cur_cell->x,cr->rules[0]->cur_cell->y,
//             cr->newstate);
//     }
}

void applyAllComplexRules(Ac* ac, Cell* c)
{
    printf("Applying complex rule for cell (%d,%d)\n", c->x, c->y);
    for (int i=0; i < c->numberofcomplexrules; i++)
    {
        applyComplexRule(ac,&(c->complexrules[i]));
    }
}

int verifyIfRuleSatisfied(Rule* r)
{
    Cell** cells = (Cell**) malloc(MAX_NEIGHBOURS*sizeof(Cell*));
    int count = 0, satisfied = 0;
    for(int i=0; i < MAX_NEIGHBOURS; i++)
    {
        cells[i] = (Cell*)malloc(sizeof(Cell));
    }

    if (r->cur_cell->state == r->stateCurCell)
    {
        if (r->neighbour_cell != NULL) //Test a specific neighbour
        {
            if (r->neighbour_cell->state == r->state)
                satisfied = 1;
            else
                satisfied = 0;
        }
        else
        {
            for (int j=0; j < r->cur_cell->numberofneighbours; j++)
            {
                if (r->cur_cell->neighbours[j]->state == r->state)
                {
                    cells[count] = (r->cur_cell->neighbours[j]);
                    count++;
                }
            }
            if (r->type == 0)
            {
                if (r->op_cmp == 0)
                {
                    if (count == r->cur_cell->numberofneighbours)
                        satisfied = 1;
                    else
                        satisfied = 0;
                }
                else if (r->op_cmp == 1)
                {
                    if (count < r->cur_cell->numberofneighbours)
                        satisfied = 1;
                    else
                        satisfied = 0;
                }
                else if (r->op_cmp == 2)
                {
                    if (count > r->cur_cell->numberofneighbours)
                        satisfied = 1;
                    else
                        satisfied = 0;
                }
            } //End if r->type == 0
            else if (r->type == 1)
            {}
            else if (r->type == 2)
            {
                if (r->op_cmp == 0)
                {
                    if (count == r->numberofneighbours)
                        satisfied = 1;
                    else
                        satisfied = 0;
                }
                else if (r->op_cmp == 1)
                {
                    if (count < r->numberofneighbours)
                        satisfied = 1;
                    else
                        satisfied = 0;
                }
                else if (r->op_cmp == 2)
                {
                    if (count > r->numberofneighbours)
                        satisfied = 1;
                    else
                        satisfied = 0;
                }
            }//End if r->type == 2
        } //End else
    } //End if test stateCurCell
    if (satisfied)
    {
        //printf("Neighbors cells that satisfy rule id %d of Cell %d\n", r->id, r->cur_cell->id);
        //printCells(cells);
    }
    return satisfied;
}

// //Return one or more cells that satisfy the rule passed as argument
// void cellsThatSatisfyRule(Rule* r)
// {
//   Cell** cells = malloc(sizeof(Cell*));
//   if (verifyIfRuleSatisfied(r,cells))
//   {
//     printf("Neighbors cells that satisfy rule id %d of Cell %d\n", r->id, r->cur_cell->id);
//     printCells(*cells);
//     //return cells;
//   }
//   // else
//   //   return NULL;
//   //free(cells);
// }

void testRuleForAllCells(Ac* ac, int rule_number)
{
  for (int j=0; j < ac->lin; j++)
    for (int k=0; k < ac->col; k++)
    {
       verifyIfRuleSatisfied(&(ac->domain[ac->t_pre][j][k].rules[rule_number]));
    }
}

//Return one or more neighbours positions that satisfy the rule passed as argument
// int* neighboursThatSatisfyRule(Rule* r)
// {
//   int count = 0, satisfied = 0;
//   if (r->cur_cell->state == r->stateCurCell)
//   {
//     int positions[MAX_NEIGHBOURS];
//     for(int i=0; i < MAX_NEIGHBOURS; i++)
//       positions[i] = -1;
//     if (r->type != 1)
//     {
//       for (int j=0; j < r->cur_cell->numberofneighbours; j++)
//       {
//         if (r->cur_cell->neighbours[j]->state == r->state)
//         {
//           positions[count] = j;
//           count++;
//         }
//       }
//     }
//     satisfied = verifyIfRuleSatisfied(r);
//   }
// }

void applyRules(Ac* ac, Cell* c)
{
  int satisfied = 0;
  for (int i=0; i < c->numberofrules; i++)
  {
    satisfied = 0;
    satisfied = verifyIfRuleSatisfied(&(c->rules[i]));
    if (satisfied)
      updateState(ac,c->x,c->y,c->rules[i].newstate);
  }//end for
}

void updateAC(Ac* ac)
{
  for (int j=0; j < ac->lin; j++)
    for (int k=0; k < ac->col; k++)
    {
        applyRules(ac,&(ac->domain[ac->t_pre][j][k]));
        //applyAllComplexRules(ac,&(ac->domain[ac->t_pre][j][k]));
    }
  incrementTimeStep(ac);
}

void printAc(Ac* ac)
{
  printf("AC domain view:\n");
  for (int j=0; j < ac->lin; j++)
  {
    for (int k=0; k < ac->col; k++)
      printf("%d ", ac->domain[ac->t_cur][j][k].state);
    printf("\n");
  }
}

//Save AC configuration to a file
void saveAc(Ac* ac)
{
    if (ac->dimension == 1)
    {
        if (ac->col > 1)
        {
            printf("Error! Dimension must be one in y axis.\n");
            return;
        }
        for (int j=0; j < ac->lin; j++)
        {
            fprintf(ac->fp, "%d ", ac->domain[ac->t_cur][j][0].state);
        }
        fprintf(ac->fp,"\n");
    }
    else
    {
        for (int j=0; j < ac->lin; j++)
        {
          for (int k=0; k < ac->col; k++)
            fprintf(ac->fp, "%d ", ac->domain[ac->t_cur][j][k].state);
          fprintf(ac->fp,"\n");
        }
        fprintf(ac->fp,"---\n");
    }
}

// testa se 0 -> rgb (255,255,255), 1 -> rgb (0,0,0)
// void generateFigure()
// {
//     fprintf(fd, "%s%i.%i%s\n", "\\setlength{\\unitlength}{", size_i, size_d, "mm}");
//     fprintf(fd, "%s%i,%i%s\n", "\\begin{picture}(", width, height, ")(0,0)");
//     // print frame
//     print_frame(fd, height, width);
//     fprintf(fd, "%s%i%s%i%s\n", "\\put(0,", 0, "){\\line(1,0){", w, "}}");
//     fprintf(fd, "%s%i%s%i%s\n", "\\put(0,", h, "){\\line(1,0){", w, "}}");
//     fprintf(fd, "%s%i%s%i%s\n", "\\put(", 0, ",0){\\line(0,1){", h, "}}");
//     fprintf(fd, "%s%i%s%i%s\n", "\\put(", w, ",0){\\line(0,1){", h, "}}");
//     print_cell_aut(fd, A, width, height, B);
//     fprintf(fd, "%s\n", "\\end{picture}");
// }

void printCells(Cell** cells)
{
  for (int i=0; i < MAX_NEIGHBOURS; i++)
  {
    if(cells[i] != NULL)//if (cells[i].id != -1)
      printf("state(%d,%d):%d\n", cells[i]->x,cells[i]->y,cells[i]->state);
  }
}

void printNeighboursStates(Cell c)
{
  printf("Neighbours of %d:\n", c.x);
  //printf("Number of neighbours %d\n", c.numberofneighbours);
  for(int i=0; i < c.numberofneighbours; i++)
  {
    printf("state of (%d,%d): %d; ", c.neighbours[i]->x, c.neighbours[i]->y, c.neighbours[i]->state);
  }
  printf("\n");
}

int findRandomPosition(Ac* ac)
{
  srand((unsigned)time(NULL));
  if (ac->dimension == 1 && ac->neighbourhood == 1)//VonNeumann
    return rand()%2;
  if (ac->dimension == 1 && ac->neighbourhood == 2)//Moore
    return rand()%4;
  if (ac->dimension == 2 && ac->neighbourhood == 1)//VonNeumann
    return rand()%4;
  if (ac->dimension == 2 && ac->neighbourhood == 2)//Moore
    return rand()%8;
  return 0;
}

int isTrue(float prob)
{
  srand((unsigned)time(NULL));
  float p = rand()%1;
  if (p <= prob)
    return 1;
  else return 0;
}

Cell* chooseRandomNeighbour(Ac* ac, Cell* c)
{
  return c->neighbours[findRandomPosition(ac)];
}

// def searchWithCondition(self, x_ini, x_fim, y_ini, y_fim, x_pos, y_pos, cond, values, t):
//       #result = False
//       neighbours = []
//       for x in range(x_ini,x_fim+1):
//           for y in range(y_ini,y_fim+1):
//               if(x != x_pos or y != y_pos):
//                   if(cond == EQUAL):
//                       if(self.Ac.cells[t][x][y].state in values):
//                           #result = True
//                           neighbours.append([x,y])
//       return neighbours
//       #return result
//
//   def searchNeighboursWithCondition(self, neighbours, cond, values, t): #values
//       new_neighbours = []
//       for n in neighbours:
//           if(cond == EQUAL):
//               if(self.Ac.cells[t][n[0]][n[1]].state in values): #Usar operador in para verificar se o valor do estado está em uma lista de valores
//                   new_neighbours.append([n[0],n[1]])
//       return new_neighbours
//
//       def findNeighbourThatMeetConditions(self,x_ini,x_fim,y_ini,y_fim,x_pos,y_pos,conditions, t_ant): #conditions é uma lista de tuplas
//           c = conditions[0]
//   #        print('c[0]:',c[0])
//   #        print('c[1]:',c[1])
//   #        print('c[2]:',c[2])
//           if(c[0] == 'Bacteria'):
//               neighbours = bacteria.searchWithCondition(x_ini, x_fim, y_ini, y_fim, x_pos, y_pos, c[1], c[2], t_ant)
//           if(c[0] == 'Neutrophil'):
//               neighbours = neutrofilo.searchWithCondition(x_ini, x_fim, y_ini, y_fim, x_pos, y_pos, c[1], c[2], t_ant)
//           if(c[0] == 'Fibrin'):
//               neighbours = fibrina.searchWithCondition(x_ini, x_fim, y_ini, y_fim, x_pos, y_pos, c[1], c[2], t_ant)
//
//           for c in conditions[1:]:
//               if(c[0] == 'Bacteria'):
//                   neighbours = bacteria.searchNeighboursWithCondition(neighbours, c[1], c[2], t_ant)
//               if(c[0] == 'Neutrophil'):
//                   neighbours = neutrofilo.searchNeighboursWithCondition(neighbours, c[1], c[2], t_ant)
//               if(c[0] == 'Fibrin'):
//                   neighbours = fibrina.searchNeighboursWithCondition(neighbours, c[1], c[2], t_ant)
//           return neighbours
