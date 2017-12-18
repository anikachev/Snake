//Змейка с препятствиями. Версия 1.0
//Подготовил Никачев Александр, 721 группа
#include <GL/glut.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#define N 20 // кол-во столбцов
#define M 20 // кол-во строк
#define A 25 // размер ячейки
#define B 1 // НЕ ИЗМЕНЯТЬ
#define FRUCTNUM 6  // количество ягод
#define RFRUCTNUM 10 // кол-во ловушек
#define SWAPTIME 4 //период перемешивания ловушек
#define TRAP 4 // штраф за поедание ловушки

// 'p' - пауза, 'c' - продолжить, "+" - ускорить змейку, "-" - замедлить змейку

void timer(int value);

int w = N*(A+B) + B; // кол-во пикселей в высоту
int h = M*(A+B) + B;
int dir = 0, dir_head = 0, num = 2, redfruct = 0, tickstat = 1, ticktime = 100, maxlen = 2; // поворот змеи / длина змеи / генераци¤ kjdeitr / таймер / рекорд длины

struct fruct // координаты фруктов
{
  int x;
  int y;
} m[FRUCTNUM];
struct red_fruct // координаты ловушек
{
  int x;
  int y;
  int stat;
} rm[RFRUCTNUM];

struct point // координаты точек змеи и переменная новой точки
{
    int x;
    int y;
} s[M*N], np;

struct point newf() // функция создаёт новую точку, проверяя непересекаемость с другими точками
{
    int i, k = 1; // проверка на то, чтобы фрукты не появлялись внутри змеи
    struct point np;
    while(k == 1)
    {
        k = 0;
        np.x = rand()%N;
        np.y = rand()%M;
        for (i = 0; i < num; ++i)
        {
            if (s[i].x == np.x && s[i].y == np.y)
            {
                k = 1;
                break;
            }
        }
        for (i = 0; i < FRUCTNUM; ++i)
            if (m[i].x == np.x && m[i].y == np.y)
            {
                k = 1;
                break;
            }
        for (i = 0; i < RFRUCTNUM; ++i)
            if (rm[i].x == np.x && rm[i].y == np.y)
            {
                k = 1;
                break;
            }
    }
    return np;
}

void DrawFructs(void) // рисование фруктов
  {
    int i;
    for (i = 0; i < FRUCTNUM; ++i)
    {
        glColor3f(0.0, 1.0, 1.0);
        glRectf(m[i].x*(A + B) + B, m[i].y*(A + B) + B, m[i].x*(A + B) + A + B, m[i].y*(A + B) + A + B);
    }
    for (i = 0; i < RFRUCTNUM; ++i)
    {
        glColor3ub(0, 165, 230);
        glRectf(rm[i].x*(A + B) + B, rm[i].y*(A + B) + B, rm[i].x*(A + B) + A + B, rm[i].y*(A + B) + A + B);
    }

  }

void DrawLines() // рисование поля
{
  int i;
  glColor3ub(90, 45, 0);
  glBegin(GL_LINES);
  for (i = 0; i < w; i+= A + B)
  {
  glVertex2f(i, 0); glVertex2f(i, h - 1);
  }
  for (i = 0; i < h; i += A + B)
  {
  glVertex2f(0, i); glVertex2f(w - 1, i);
  }
  for (i = h; i < h + 4; ++i)
  {
    glVertex2f(0, i); glVertex2f(w, i);
  }
  glEnd();

  /* glColor3ub(255, 238, 142);
  glRectf(0, h + 4, w, h + 99); */ // заготовка для табло
}

void tick() // шаг работы программы
{
    for (int i = num; i > 0; --i)
  {
  s[i].x = s[i - 1].x;
  s[i].y = s[i - 1].y;
  }

  if (dir == 0) s[0].y += 1; // поворот змеи
  if (dir == 1) s[0].x -= 1;
  if (dir == 2) s[0].x += 1;
  if (dir == 3) s[0].y -= 1;

  if (s[0].x > N - 1) s[0].x = 0; // перенос змеи при уходе за границы
  if (s[0].y > M - 1) s[0].y = 0;
  if (s[0].x < 0) s[0].x = N - 1;
  if (s[0].y < 0) s[0].y = M - 1;

  for (int i = 0; i < FRUCTNUM; i++) // поедание фрукта
    if ((s[0].x == m[i].x) && (s[0].y == m[i].y))
  {
  num++;
  np = newf();
  m[i].x = np.x;
  m[i].y = np.y;
  if(num > maxlen)
    maxlen = num;
  }

  for (int i = 1; i < num; i++) // поедание змеёй себя
  if (s[0].x == s[i].x && s[0].y == s[i].y)
  num = i;

  for (int i = 0; i < RFRUCTNUM; i++) // поедание ловушки
    if ((s[0].x == rm[i].x) && (s[0].y == rm[i].y))
  {
  if (num > TRAP)
    num -= TRAP;
  else
  if(num > 1)
    num--;
  rm[i].x = -2;
  rm[i].y = -2;
  }

  if (redfruct % (SWAPTIME*1000/ticktime) == 0) // перемешивание ловушек. Раз в SWAPTIME секунд.
    for(int i = 0; i < RFRUCTNUM; ++i)
    {
        np = newf();
        rm[i].x = np.x;
        rm[i].y = np.y;
    }
  redfruct++;
}

void DrawSnake() //рисование змеи
{
  glColor3ub(50, 180, 20);
  glRectf(s[0].x*(A + 1), s[0].y*(A + 1), s[0].x*(A + 1) + A + 2, s[0].y*(A + 1) + A + 2);

  glColor3ub(0, 0, 0);
  switch(dir) // рисование глаз
  {
    case 0:
        glRectf(s[0].x*(A + 1) + 4*A/25 , s[0].y*(A + 1) + 18*A/25, s[0].x*(A + 1) + 9*A/25, s[0].y*(A + 1) + 22*A/25);
        glRectf(s[0].x*(A + 1) + 17*A/25 , s[0].y*(A + 1) + 18*A/25, s[0].x*(A + 1) + 22*A/25, s[0].y*(A + 1) + 22*A/25);
        break;
    case 3:
        glRectf(s[0].x*(A + 1) + 4*A/25 , s[0].y*(A + 1) + 7*A/25, s[0].x*(A + 1) + 9*A/25, s[0].y*(A + 1) + 11*A/25);
        glRectf(s[0].x*(A + 1) + 17*A/25 , s[0].y*(A + 1) + 7*A/25, s[0].x*(A + 1) + 22*A/25, s[0].y*(A + 1) + 11*A/25);
        break;
    case 1:
        glRectf(s[0].x*(A + 1) + 8*A/25 , s[0].y*(A + 1) + 5*A/25, s[0].x*(A + 1) + 12*A/25, s[0].y*(A + 1) + 10*A/25);
        glRectf(s[0].x*(A + 1) + 8*A/25 , s[0].y*(A + 1) + 18*A/25, s[0].x*(A + 1) + 12*A/25, s[0].y*(A + 1) + 23*A/25);
        break;
    case 2:
        glRectf(s[0].x*(A + 1) + 17*A/25 , s[0].y*(A + 1) + 5*A/25, s[0].x*(A + 1) + 21*A/25, s[0].y*(A + 1) + 10*A/25);
        glRectf(s[0].x*(A + 1) + 17*A/25 , s[0].y*(A + 1) + 18*A/25, s[0].x*(A + 1) + 21*A/25, s[0].y*(A + 1) + 23*A/25);
        break;

  }
  glColor3f(0.1, 1.0, 0.0);
  for (int i = 1; i < num; i+= 2)
  {
  glRectf(s[i].x*(A + 1), s[i].y*(A + 1), s[i].x*(A + 1) + A + 2, s[i].y*(A + 1) + A + 2);
  }

  glColor3ub(50, 220, 15);
  for (int i = 2; i < num; i+= 2)
  {
  glRectf(s[i].x*(A + 1), s[i].y*(A + 1), s[i].x*(A + 1) + A + 2, s[i].y*(A + 1) + A + 2);
  }
}

void Key(int key, int a, int b)
{
  switch (key)
  {
  case 101: dir = 0; break;
  case 102: dir = 2; break;
  case 100: dir = 1; break;
  case 103: dir = 3; break;
  }
}

void Keyboard(unsigned char key, int a, int b)
{
  switch (key)
  {
  case 'p':
      tickstat = 0; // пауза
      printf("%d %d\n", maxlen, num);
      break;
  case 'c': // продолжить
      if (tickstat == 0)
      {
          tickstat = 1;
          glutTimerFunc(ticktime, timer, 0);
      }
    break;
  case '+': // ускорить
    if (ticktime > 30)
        ticktime -= 10;
    break;
  case '-': // замедлить
    ticktime += 10;
    break;
  }
}

void Display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(1, 0.9372549019607843, 0.7, 1);

  DrawFructs();
  DrawLines();
  DrawSnake();

  glFlush();
  glutSwapBuffers();
}

void timer(int value)
{
    if (tickstat == 1)
    {
        glutPostRedisplay();
        tick();
        glutTimerFunc(ticktime, timer, 0);
    }
}

int main(int argc, char **argv)
{
  srand(time(NULL));
  for (int i = 0; i < FRUCTNUM; i++)
  {
      np = newf();
      m[i].x = np.x;
      m[i].y = np.y;
  }

  s[0].x = N - 1; // первоначальное направление змейки
  s[0].y = M - 1;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(w - 1, h + 4);
  glutCreateWindow("Snake 1.0");

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, w - 1, 0, h + 4);

  glutDisplayFunc(Display);
  glutSpecialFunc(Key);
  glutKeyboardFunc(Keyboard);

  glutTimerFunc(100, timer, 0);

  glutMainLoop();

  return 0;
}

