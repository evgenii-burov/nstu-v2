// Лабораторная работа №2. Растеризация примитивов.
// Вариант 3: правильные шестиугольники, логические операции AND + NOT AND (NAND).
// Дополнительные задания: (1) сглаживание (суперсэмплинг),
//                         (3) растеризация объектов после модельно-видовых преобразований.
//
// Окно разделено на две панели:
//   слева  - обычная отрисовка средствами OpenGL (цвет, текстура, glLogicOp);
//   справа - крупная сетка "условных пикселей", на которой объекты растеризуются вручную.

// Запуск без консольного окна (только для Visual Studio / MSVC).
// Подсистема Windows + прежняя точка входа main(), поэтому код менять не нужно.
#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#endif

#include <GL/freeglut.h>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstdlib>
using namespace std;

const double PI = 3.14159265358979323846;
const int SCENE = 512;      // логический размер каждой панели (512 x 512 условных единиц)
const int NV = 6;           // число вершин у шестиугольника
const int SS = 4;           // сглаживание: каждая клетка делится на SS x SS подточек
const int TEX_COUNT = 3;    // число процедурных текстур
const int TEX_SIZE = 64;    // размер текстуры в текселях

int WinW = 1024, WinH = 512;

/* ===================== СТРУКТУРЫ ДАННЫХ ===================== */

/* Один правильный шестиугольник и все его атрибуты */
struct Hexagon
{
	double cx, cy;          // центр (задан первым кликом мыши)
	double r;               // "радиус" (расстояние от центра до второго клика)
	double tx, ty;          // перенос (glTranslatef)
	double angle;           // угол поворота в градусах (glRotatef)
	double sx, sy;          // масштаб по осям X и Y (glScalef)
	int colR, colG, colB;   // цвет, 0..255
	int texture;            // 0 - без текстуры, 1..TEX_COUNT - номер текстуры

	Hexagon() : cx(0), cy(0), r(10), tx(0), ty(0), angle(0), sx(1), sy(1),
		colR(255), colG(255), colB(255), texture(0) {}
};

vector<Hexagon> Objects;    // все объекты
int CurrentObj = -1;        // номер текущего (активного) объекта, -1 - объектов нет

/* Создание объекта двумя кликами */
bool WaitingForRadius = false;  // true: центр уже задан, ждём клик для радиуса
double CenterX = 0, CenterY = 0;
double MouseX = 0, MouseY = 0;  // последняя позиция мыши (для предпросмотра)

/* Режимы */
int  LogicMode = 0;         // 0 - выкл, 1 - AND, 2 - NOT AND (NAND)
bool FilledMode = true;     // true - "с заполнением", false - "контур"
bool AntiAlias = false;     // сглаживание вкл/выкл
int  CellSize = 16;         // размер клетки сетки в условных единицах (8, 16, 32, 64)

/* Сетка условных пикселей: GridN x GridN клеток, 3 байта (RGB) на клетку */
vector<GLubyte> Grid;
int  GridN = SCENE / 16;
bool GridDirty = true;      // true - сетку нужно перестроить

GLuint TexIds[TEX_COUNT];
const char* TexNames[TEX_COUNT + 1] = { "none", "checker", "stripes", "gradient" };
const char* LogicNames[3] = { "off", "AND", "NAND" };

/* Палитра для новых объектов (цвета со старшими битами - чтобы AND/NAND давали заметный результат) */
const int Palette[6][3] = {
	{ 255, 128, 128 },{ 128, 255, 128 },{ 128, 128, 255 },
	{ 255, 255, 128 },{ 255, 128, 255 },{ 128, 255, 255 }
};
int PaletteIdx = 0;

/* Коды команд - общие для клавиатуры и меню */
enum Action
{
	ACT_NEXT_OBJ = 1, ACT_PREV_OBJ, ACT_DELETE_OBJ, ACT_CLEAR_ALL,
	ACT_MOVE_LEFT, ACT_MOVE_RIGHT, ACT_MOVE_UP, ACT_MOVE_DOWN,
	ACT_ROT_CCW, ACT_ROT_CW, ACT_SCALE_UP, ACT_SCALE_DOWN,
	ACT_STRETCH_X_UP, ACT_STRETCH_X_DOWN, ACT_STRETCH_Y_UP, ACT_STRETCH_Y_DOWN,
	ACT_R_UP, ACT_R_DOWN, ACT_G_UP, ACT_G_DOWN, ACT_B_UP, ACT_B_DOWN,
	ACT_TEXTURE_NEXT,
	ACT_LOGIC_OFF, ACT_LOGIC_AND, ACT_LOGIC_NAND, ACT_LOGIC_CYCLE,
	ACT_MODE_TOGGLE, ACT_AA_TOGGLE, ACT_GRID_BIGGER, ACT_GRID_SMALLER,
	ACT_HOME, ACT_RESET
};

/* ===================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ===================== */

double ClampD(double v, double lo, double hi) { return v < lo ? lo : (v > hi ? hi : v); }
int    ClampI(int v, int lo, int hi) { return v < lo ? lo : (v > hi ? hi : v); }

/* Локальная вершина k шестиугольника (до преобразований): правильный шестиугольник радиуса r */
void LocalVertex(const Hexagon& h, int k, double& lx, double& ly)
{
	double a = k * PI / 3.0;
	lx = h.r * cos(a);
	ly = h.r * sin(a);
}

/* Модельно-видовое преобразование "вручную": M = T * R * S.
   Совпадает с тем, что делают glTranslatef -> glRotatef -> glScalef в DrawHexagon. */
void Transform(const Hexagon& h, double lx, double ly, double& wx, double& wy)
{
	double a = h.angle * PI / 180.0;
	double x = h.sx * lx;                         // S - масштаб
	double y = h.sy * ly;
	wx = h.cx + h.tx + x * cos(a) - y * sin(a);   // R - поворот, T - перенос
	wy = h.cy + h.ty + x * sin(a) + y * cos(a);
}

/* Вершины объекта (уже преобразованные) в координатах сетки: 1 единица = 1 клетка */
void GridVertices(const Hexagon& h, double vx[], double vy[])
{
	for (int k = 0; k < NV; k++)
	{
		double lx, ly, wx, wy;
		LocalVertex(h, k, lx, ly);
		Transform(h, lx, ly, wx, wy);
		vx[k] = wx / CellSize;
		vy[k] = wy / CellSize;
	}
}

/* ===================== РАСТЕРИЗАЦИЯ НА СЕТКЕ ===================== */

/* Метод углов: точка внутри многоугольника, если сумма углов между
   направлениями на соседние вершины равна 360 градусов. */
bool PointInPolygon(double px, double py, const double vx[], const double vy[], int n)
{
	double sum = 0;
	for (int i = 0; i < n; i++)
	{
		int j = (i + 1) % n;
		double ax = vx[i] - px, ay = vy[i] - py;
		double bx = vx[j] - px, by = vy[j] - py;
		double la = sqrt(ax * ax + ay * ay);
		double lb = sqrt(bx * bx + by * by);
		if (la < 1e-9 || lb < 1e-9) return true;   // точка совпала с вершиной
		double c = (ax * bx + ay * by) / (la * lb); // косинус угла по формуле скалярного произведения
		if (c > 1) c = 1;
		if (c < -1) c = -1;
		sum += acos(c);
	}
	return fabs(sum - 2 * PI) < 1e-4;
}

/* Расстояние от точки до отрезка */
double DistToSegment(double px, double py, double x0, double y0, double x1, double y1)
{
	double dx = x1 - x0, dy = y1 - y0;
	double len2 = dx * dx + dy * dy;
	double t = (len2 > 0) ? ((px - x0) * dx + (py - y0) * dy) / len2 : 0;
	if (t < 0) t = 0;
	if (t > 1) t = 1;
	double qx = x0 + t * dx, qy = y0 + t * dy;
	return sqrt((px - qx) * (px - qx) + (py - qy) * (py - qy));
}

/* Точка лежит рядом с контуром (не дальше halfWidth от какого-либо ребра) */
bool NearPolygonEdge(double px, double py, const double vx[], const double vy[], int n, double halfWidth)
{
	for (int i = 0; i < n; i++)
	{
		int j = (i + 1) % n;
		if (DistToSegment(px, py, vx[i], vy[i], vx[j], vy[j]) <= halfWidth) return true;
	}
	return false;
}

/* Отметить клетку (i, j) как покрытую на cov (с проверкой границ) */
void SetCoverage(vector<float>& cov, int i, int j, float value)
{
	if (i < 0 || j < 0 || i >= GridN || j >= GridN) return;
	cov[j * GridN + i] = value;
}

/* Алгоритм Брезенхема для отрезка между клетками (x0,y0) и (x1,y1) - для любого наклона */
void BresenhamLine(int x0, int y0, int x1, int y1, vector<float>& cov)
{
	int dx = abs(x1 - x0), sx = (x0 < x1) ? 1 : -1;
	int dy = -abs(y1 - y0), sy = (y0 < y1) ? 1 : -1;
	int err = dx + dy;
	while (true)
	{
		SetCoverage(cov, x0, y0, 1.0f);
		if (x0 == x1 && y0 == y1) break;
		int e2 = 2 * err;
		if (e2 >= dy) { err += dy; x0 += sx; }
		if (e2 <= dx) { err += dx; y0 += sy; }
	}
}

/* Для объекта вычисляет "покрытие" каждой клетки сетки: от 0 (пусто) до 1 (клетка закрашена полностью).
   Без сглаживания покрытие бывает только 0 или 1, со сглаживанием - доля подточек, попавших в объект. */
void ComputeCoverage(const Hexagon& h, vector<float>& cov)
{
	cov.assign(GridN * GridN, 0.0f);

	double vx[NV], vy[NV];
	GridVertices(h, vx, vy);

	/* Контур без сглаживания: Брезенхем по каждому ребру */
	if (!FilledMode && !AntiAlias)
	{
		for (int k = 0; k < NV; k++)
		{
			int n = (k + 1) % NV;
			BresenhamLine((int)floor(vx[k]), (int)floor(vy[k]),
				(int)floor(vx[n]), (int)floor(vy[n]), cov);
		}
		return;
	}

	/* Ограничивающий прямоугольник - чтобы не просматривать всю сетку */
	double minx = vx[0], maxx = vx[0], miny = vy[0], maxy = vy[0];
	for (int k = 1; k < NV; k++)
	{
		if (vx[k] < minx) minx = vx[k];
		if (vx[k] > maxx) maxx = vx[k];
		if (vy[k] < miny) miny = vy[k];
		if (vy[k] > maxy) maxy = vy[k];
	}
	int i0 = ClampI((int)floor(minx) - 1, 0, GridN - 1);
	int i1 = ClampI((int)floor(maxx) + 1, 0, GridN - 1);
	int j0 = ClampI((int)floor(miny) - 1, 0, GridN - 1);
	int j1 = ClampI((int)floor(maxy) + 1, 0, GridN - 1);

	/* Без сглаживания - одна проба в центре клетки; со сглаживанием - SS x SS проб */
	int sub = AntiAlias ? SS : 1;
	for (int j = j0; j <= j1; j++)
	{
		for (int i = i0; i <= i1; i++)
		{
			int hit = 0;
			for (int sj = 0; sj < sub; sj++)
			{
				for (int si = 0; si < sub; si++)
				{
					double px = i + (si + 0.5) / sub;
					double py = j + (sj + 0.5) / sub;
					bool inside = FilledMode
						? PointInPolygon(px, py, vx, vy, NV)
						: NearPolygonEdge(px, py, vx, vy, NV, 0.5);   // контур толщиной 1 клетка
					if (inside) hit++;
				}
			}
			if (hit > 0) cov[j * GridN + i] = (float)hit / (float)(sub * sub);
		}
	}
}

/* Наложение покрытия объекта на сетку с учётом логической операции.
   s - цвет объекта (source), d - цвет, уже лежащий в клетке (destination). */
void Composite(const vector<float>& cov, const Hexagon& h)
{
	int src[3] = { h.colR, h.colG, h.colB };
	for (int j = 0; j < GridN; j++)
	{
		for (int i = 0; i < GridN; i++)
		{
			float c = cov[j * GridN + i];
			if (c <= 0.0f) continue;

			GLubyte* d = &Grid[3 * (j * GridN + i)];
			for (int k = 0; k < 3; k++)
			{
				int s = src[k], dd = d[k], result;
				if (LogicMode == 1)      result = s & dd;                 // AND
				else if (LogicMode == 2) result = (~(s & dd)) & 0xFF;     // NOT AND
				else                     result = s;                      // операции выключены
				d[k] = (GLubyte)(dd * (1.0f - c) + result * c + 0.5f);    // c < 1 - сглаженный край
			}
		}
	}
}

/* Полная перестройка сетки: белый фон + все объекты по очереди */
void RebuildGrid()
{
	GridN = SCENE / CellSize;
	Grid.assign(GridN * GridN * 3, 255);

	vector<float> cov;
	for (size_t o = 0; o < Objects.size(); o++)
	{
		ComputeCoverage(Objects[o], cov);
		Composite(cov, Objects[o]);
	}
	GridDirty = false;
}

/* ===================== ОТРИСОВКА СРЕДСТВАМИ OpenGL ===================== */

void InitTextures()
{
	glGenTextures(TEX_COUNT, TexIds);
	static GLubyte data[TEX_SIZE * TEX_SIZE * 3];
	for (int t = 0; t < TEX_COUNT; t++)
	{
		for (int y = 0; y < TEX_SIZE; y++)
		{
			for (int x = 0; x < TEX_SIZE; x++)
			{
				int v;
				if (t == 0)      v = ((x / 8 + y / 8) % 2) ? 255 : 60;   // шахматная доска
				else if (t == 1) v = ((y / 8) % 2) ? 255 : 90;           // горизонтальные полосы
				else             v = (x + y) * 2;                        // диагональный градиент
				GLubyte* p = &data[3 * (y * TEX_SIZE + x)];
				p[0] = p[1] = p[2] = (GLubyte)v;
			}
		}
		glBindTexture(GL_TEXTURE_2D, TexIds[t]);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TEX_SIZE, TEX_SIZE, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);   // текстура умножается на цвет объекта
}

/* Рисует шестиугольник с его модельно-видовыми преобразованиями (заливка или контур) */
void DrawHexagon(const Hexagon& h, bool outline)
{
	glPushMatrix();
	glTranslatef((GLfloat)(h.cx + h.tx), (GLfloat)(h.cy + h.ty), 0);
	glRotatef((GLfloat)h.angle, 0, 0, 1);
	glScalef((GLfloat)h.sx, (GLfloat)h.sy, 1);

	if (outline)
	{
		glBegin(GL_LINE_LOOP);
		for (int k = 0; k < NV; k++)
		{
			double lx, ly;
			LocalVertex(h, k, lx, ly);
			glVertex2d(lx, ly);
		}
		glEnd();
	}
	else
	{
		glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(0.5f, 0.5f);
		glVertex2f(0, 0);
		for (int k = 0; k <= NV; k++)
		{
			double a = k * PI / 3.0;
			glTexCoord2d(0.5 + 0.5 * cos(a), 0.5 + 0.5 * sin(a));
			glVertex2d(h.r * cos(a), h.r * sin(a));
		}
		glEnd();
	}
	glPopMatrix();
}

/* Вывод строки текста (растровым шрифтом) в точке (x, y) */
void PrintText(float x, float y, const char* s)
{
	glRasterPos2f(x, y);
	for (; *s; s++) glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *s);
}

/* Подготовка панели: область вывода и система координат 512 x 512 */
void SetupPane(int x, int y, int w, int h)
{
	glViewport(x, y, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, SCENE, 0, SCENE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/* Левая панель: обычная отрисовка OpenGL */
void DrawScene()
{
	char buf[160];

	/* Логические операции над цветами */
	if (LogicMode == 1) { glEnable(GL_COLOR_LOGIC_OP); glLogicOp(GL_AND); }
	else if (LogicMode == 2) { glEnable(GL_COLOR_LOGIC_OP); glLogicOp(GL_NAND); }
	else glDisable(GL_COLOR_LOGIC_OP);

	for (size_t o = 0; o < Objects.size(); o++)
	{
		const Hexagon& h = Objects[o];
		glColor3ub((GLubyte)h.colR, (GLubyte)h.colG, (GLubyte)h.colB);
		if (h.texture > 0)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, TexIds[h.texture - 1]);
		}
		else glDisable(GL_TEXTURE_2D);
		DrawHexagon(h, false);
	}

	/* Служебные элементы рисуем уже без логических операций и текстур */
	glDisable(GL_COLOR_LOGIC_OP);
	glDisable(GL_TEXTURE_2D);

	/* Выделение текущего объекта */
	if (CurrentObj >= 0 && CurrentObj < (int)Objects.size())
	{
		glLineWidth(3);
		glColor3ub(0, 0, 0);
		DrawHexagon(Objects[CurrentObj], true);
	}

	/* Предпросмотр: центр уже задан, радиус тянем за мышью */
	if (WaitingForRadius)
	{
		Hexagon tmp;
		tmp.cx = CenterX;
		tmp.cy = CenterY;
		tmp.r = sqrt((MouseX - CenterX) * (MouseX - CenterX) + (MouseY - CenterY) * (MouseY - CenterY));
		if (tmp.r < 1) tmp.r = 1;
		glLineWidth(1);
		glColor3ub(0, 0, 0);
		DrawHexagon(tmp, true);
		glPointSize(5);
		glBegin(GL_POINTS);
		glVertex2d(CenterX, CenterY);
		glEnd();
	}

	/* Строка состояния */
	glColor3ub(0, 0, 0);
	snprintf(buf, sizeof(buf), "Obj %d/%d   Logic: %s   Tex: %s",
		CurrentObj + 1, (int)Objects.size(), LogicNames[LogicMode],
		(CurrentObj >= 0) ? TexNames[Objects[CurrentObj].texture] : "-");
	PrintText(6, 8, buf);
	PrintText(6, 24, WaitingForRadius ? "Click: point on radius" : "Click: hexagon center");
}

/* Правая панель: сетка условных пикселей */
void DrawGridPane()
{
	char buf[160];
	if (GridDirty) RebuildGrid();

	glBegin(GL_QUADS);
	for (int j = 0; j < GridN; j++)
	{
		for (int i = 0; i < GridN; i++)
		{
			const GLubyte* c = &Grid[3 * (j * GridN + i)];
			glColor3ub(c[0], c[1], c[2]);
			glVertex2i(i * CellSize, j * CellSize);
			glVertex2i((i + 1) * CellSize, j * CellSize);
			glVertex2i((i + 1) * CellSize, (j + 1) * CellSize);
			glVertex2i(i * CellSize, (j + 1) * CellSize);
		}
	}
	glEnd();

	/* Линии сетки */
	glLineWidth(1);
	glColor3ub(190, 190, 190);
	glBegin(GL_LINES);
	for (int i = 0; i <= GridN; i++)
	{
		glVertex2i(i * CellSize, 0);
		glVertex2i(i * CellSize, SCENE);
		glVertex2i(0, i * CellSize);
		glVertex2i(SCENE, i * CellSize);
	}
	glEnd();

	/* Разделитель панелей */
	glLineWidth(3);
	glColor3ub(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2i(0, 0);
	glVertex2i(0, SCENE);
	glEnd();

	snprintf(buf, sizeof(buf), "Grid %dx%d (cell %d)   %s   AA: %s",
		GridN, GridN, CellSize, FilledMode ? "filled" : "contour", AntiAlias ? "on" : "off");
	glColor3ub(0, 0, 0);
	PrintText(10, 8, buf);
}

/* ===================== КОМАНДЫ (общие для клавиатуры и меню) ===================== */

void DoAction(int a)
{
	Hexagon* h = (CurrentObj >= 0 && CurrentObj < (int)Objects.size()) ? &Objects[CurrentObj] : NULL;
	const double MOVE = 8, ROT = 5, K = 1.1;
	const int COL = 16;

	switch (a)
	{
	case ACT_NEXT_OBJ: if (!Objects.empty()) CurrentObj = (CurrentObj + 1) % (int)Objects.size(); break;
	case ACT_PREV_OBJ: if (!Objects.empty()) CurrentObj = (CurrentObj - 1 + (int)Objects.size()) % (int)Objects.size(); break;
	case ACT_DELETE_OBJ:
		if (h)
		{
			Objects.erase(Objects.begin() + CurrentObj);
			if (CurrentObj >= (int)Objects.size()) CurrentObj = (int)Objects.size() - 1;
		}
		break;
	case ACT_CLEAR_ALL: Objects.clear(); CurrentObj = -1; break;

		/* Перенос */
	case ACT_MOVE_LEFT:  if (h) h->tx -= MOVE; break;
	case ACT_MOVE_RIGHT: if (h) h->tx += MOVE; break;
	case ACT_MOVE_UP:    if (h) h->ty += MOVE; break;
	case ACT_MOVE_DOWN:  if (h) h->ty -= MOVE; break;
		/* Вернуть фигуру на экран: центр фигуры - в центр сцены (поворот и масштаб не меняются) */
	case ACT_HOME:
		if (h) { h->tx = SCENE / 2.0 - h->cx; h->ty = SCENE / 2.0 - h->cy; }
		break;
		/* Сбросить все преобразования: перенос, поворот, масштаб - в исходное состояние */
	case ACT_RESET:
		if (h) { h->tx = 0; h->ty = 0; h->angle = 0; h->sx = 1; h->sy = 1; }
		break;
		/* Поворот */
	case ACT_ROT_CCW: if (h) h->angle += ROT; break;
	case ACT_ROT_CW:  if (h) h->angle -= ROT; break;
		/* Масштаб (общий и по осям - последнее меняет форму) */
	case ACT_SCALE_UP:   if (h) { h->sx = ClampD(h->sx * K, 0.1, 10); h->sy = ClampD(h->sy * K, 0.1, 10); } break;
	case ACT_SCALE_DOWN: if (h) { h->sx = ClampD(h->sx / K, 0.1, 10); h->sy = ClampD(h->sy / K, 0.1, 10); } break;
	case ACT_STRETCH_X_UP:   if (h) h->sx = ClampD(h->sx * K, 0.1, 10); break;
	case ACT_STRETCH_X_DOWN: if (h) h->sx = ClampD(h->sx / K, 0.1, 10); break;
	case ACT_STRETCH_Y_UP:   if (h) h->sy = ClampD(h->sy * K, 0.1, 10); break;
	case ACT_STRETCH_Y_DOWN: if (h) h->sy = ClampD(h->sy / K, 0.1, 10); break;

		/* Цвет */
	case ACT_R_UP:   if (h) h->colR = ClampI(h->colR + COL, 0, 255); break;
	case ACT_R_DOWN: if (h) h->colR = ClampI(h->colR - COL, 0, 255); break;
	case ACT_G_UP:   if (h) h->colG = ClampI(h->colG + COL, 0, 255); break;
	case ACT_G_DOWN: if (h) h->colG = ClampI(h->colG - COL, 0, 255); break;
	case ACT_B_UP:   if (h) h->colB = ClampI(h->colB + COL, 0, 255); break;
	case ACT_B_DOWN: if (h) h->colB = ClampI(h->colB - COL, 0, 255); break;

		/* Текстура */
	case ACT_TEXTURE_NEXT: if (h) h->texture = (h->texture + 1) % (TEX_COUNT + 1); break;

		/* Логические операции */
	case ACT_LOGIC_OFF:   LogicMode = 0; break;
	case ACT_LOGIC_AND:   LogicMode = 1; break;
	case ACT_LOGIC_NAND:  LogicMode = 2; break;
	case ACT_LOGIC_CYCLE: LogicMode = (LogicMode + 1) % 3; break;

		/* Режимы растеризации и сетка */
	case ACT_MODE_TOGGLE: FilledMode = !FilledMode; break;
	case ACT_AA_TOGGLE:   AntiAlias = !AntiAlias; break;
	case ACT_GRID_BIGGER:  if (CellSize < 64) CellSize *= 2; break;
	case ACT_GRID_SMALLER: if (CellSize > 8)  CellSize /= 2; break;
	}

	GridDirty = true;
	glutPostRedisplay();
}

/* ===================== CALLBACK-ФУНКЦИИ GLUT ===================== */

void Display(void)
{
	glClearColor(1, 1, 1, 1);          // белый фон: для AND белый - "нейтральный" цвет
	glClear(GL_COLOR_BUFFER_BIT);

	int half = WinW / 2;
	SetupPane(0, 0, half, WinH);
	DrawScene();
	SetupPane(half, 0, WinW - half, WinH);
	DrawGridPane();

	glutSwapBuffers();
}

void Reshape(GLint w, GLint h)
{
	WinW = w;
	WinH = h;
}

/* Перевод координат окна (пиксели, y вниз) в координаты сцены 512 x 512 (y вверх).
   Клик в любой из двух панелей считается кликом в сцене. */
void WindowToScene(int x, int y, double& sx, double& sy)
{
	double half = WinW / 2.0;
	double px = (x >= half) ? (x - half) : x;
	sx = px * SCENE / half;
	sy = (WinH - y) * (double)SCENE / WinH;
}

void Mouse(int button, int state, int x, int y)
{
	if (state != GLUT_DOWN || button != GLUT_LEFT_BUTTON) return;   // правая кнопка - меню

	double sx, sy;
	WindowToScene(x, y, sx, sy);

	if (!WaitingForRadius)
	{
		CenterX = sx;
		CenterY = sy;
		MouseX = sx;
		MouseY = sy;
		WaitingForRadius = true;
	}
	else
	{
		double r = sqrt((sx - CenterX) * (sx - CenterX) + (sy - CenterY) * (sy - CenterY));
		if (r >= 4)
		{
			Hexagon h;
			h.cx = CenterX;
			h.cy = CenterY;
			h.r = r;
			h.colR = Palette[PaletteIdx % 6][0];
			h.colG = Palette[PaletteIdx % 6][1];
			h.colB = Palette[PaletteIdx % 6][2];
			PaletteIdx++;
			Objects.push_back(h);
			CurrentObj = (int)Objects.size() - 1;
			WaitingForRadius = false;
			GridDirty = true;
		}
	}
	glutPostRedisplay();
}

void PassiveMotion(int x, int y)
{
	WindowToScene(x, y, MouseX, MouseY);
	if (WaitingForRadius) glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{
	(void)x; (void)y;
	switch (key)
	{
	case 27: WaitingForRadius = false; glutPostRedisplay(); break;   // Esc - отменить создание объекта

	case 'n': DoAction(ACT_NEXT_OBJ); break;
	case 'p': DoAction(ACT_PREV_OBJ); break;
	case 127: case 8: DoAction(ACT_DELETE_OBJ); break;               // Delete / Backspace
	case 'c': DoAction(ACT_CLEAR_ALL); break;

	case 'a': DoAction(ACT_MOVE_LEFT); break;
	case 'd': DoAction(ACT_MOVE_RIGHT); break;
	case 'w': DoAction(ACT_MOVE_UP); break;
	case 's': DoAction(ACT_MOVE_DOWN); break;
	case 'h': DoAction(ACT_HOME); break;      // вернуть фигуру в центр окна
	case 'o': DoAction(ACT_RESET); break;     // сбросить все преобразования
	case 'e': DoAction(ACT_ROT_CCW); break;
	case 'q': DoAction(ACT_ROT_CW); break;
	case 'x': DoAction(ACT_SCALE_UP); break;
	case 'z': DoAction(ACT_SCALE_DOWN); break;

	case 'r': DoAction(ACT_R_UP); break;
	case 'R': DoAction(ACT_R_DOWN); break;
	case 'g': DoAction(ACT_G_UP); break;
	case 'G': DoAction(ACT_G_DOWN); break;
	case 'b': DoAction(ACT_B_UP); break;
	case 'B': DoAction(ACT_B_DOWN); break;

	case 't': DoAction(ACT_TEXTURE_NEXT); break;
	case 'l': DoAction(ACT_LOGIC_CYCLE); break;
	case 'm': DoAction(ACT_MODE_TOGGLE); break;
	case 'f': DoAction(ACT_AA_TOGGLE); break;
	case '+': case '=': DoAction(ACT_GRID_BIGGER); break;
	case '-': case '_': DoAction(ACT_GRID_SMALLER); break;
	}
}

/* Стрелки: растяжение/сжатие по осям (меняет форму шестиугольника) */
void SpecialKeys(int key, int x, int y)
{
	(void)x; (void)y;
	switch (key)
	{
	case GLUT_KEY_RIGHT: DoAction(ACT_STRETCH_X_UP); break;
	case GLUT_KEY_LEFT:  DoAction(ACT_STRETCH_X_DOWN); break;
	case GLUT_KEY_UP:    DoAction(ACT_STRETCH_Y_UP); break;
	case GLUT_KEY_DOWN:  DoAction(ACT_STRETCH_Y_DOWN); break;
	}
}

void MenuHandler(int value)
{
	DoAction(value);
}

/* Контекстное меню (правый клик) - дублирует все команды клавиатуры */
void BuildGlutMenu()
{
	int mObj = glutCreateMenu(MenuHandler);
	glutAddMenuEntry("Sleduyuschiy (n)", ACT_NEXT_OBJ);
	glutAddMenuEntry("Predyduschiy (p)", ACT_PREV_OBJ);
	glutAddMenuEntry("Udalit tekuschiy (Delete)", ACT_DELETE_OBJ);
	glutAddMenuEntry("Udalit vse (c)", ACT_CLEAR_ALL);

	int mTr = glutCreateMenu(MenuHandler);
	glutAddMenuEntry("Perenos vlevo (a)", ACT_MOVE_LEFT);
	glutAddMenuEntry("Perenos vpravo (d)", ACT_MOVE_RIGHT);
	glutAddMenuEntry("Perenos vverh (w)", ACT_MOVE_UP);
	glutAddMenuEntry("Perenos vniz (s)", ACT_MOVE_DOWN);
	glutAddMenuEntry("Vernut na ekran (h)", ACT_HOME);
	glutAddMenuEntry("Sbrosit preobrazovaniya (o)", ACT_RESET);
	glutAddMenuEntry("Povorot + (e)", ACT_ROT_CCW);
	glutAddMenuEntry("Povorot - (q)", ACT_ROT_CW);
	glutAddMenuEntry("Masshtab + (x)", ACT_SCALE_UP);
	glutAddMenuEntry("Masshtab - (z)", ACT_SCALE_DOWN);
	glutAddMenuEntry("Rastyanut X (Right)", ACT_STRETCH_X_UP);
	glutAddMenuEntry("Szhat X (Left)", ACT_STRETCH_X_DOWN);
	glutAddMenuEntry("Rastyanut Y (Up)", ACT_STRETCH_Y_UP);
	glutAddMenuEntry("Szhat Y (Down)", ACT_STRETCH_Y_DOWN);

	int mCol = glutCreateMenu(MenuHandler);
	glutAddMenuEntry("R + (r)", ACT_R_UP);
	glutAddMenuEntry("R - (R)", ACT_R_DOWN);
	glutAddMenuEntry("G + (g)", ACT_G_UP);
	glutAddMenuEntry("G - (G)", ACT_G_DOWN);
	glutAddMenuEntry("B + (b)", ACT_B_UP);
	glutAddMenuEntry("B - (B)", ACT_B_DOWN);
	glutAddMenuEntry("Sleduyuschaya tekstura (t)", ACT_TEXTURE_NEXT);

	int mLog = glutCreateMenu(MenuHandler);
	glutAddMenuEntry("Vykl", ACT_LOGIC_OFF);
	glutAddMenuEntry("AND", ACT_LOGIC_AND);
	glutAddMenuEntry("NOT AND", ACT_LOGIC_NAND);
	glutAddMenuEntry("Sleduyuschaya (l)", ACT_LOGIC_CYCLE);

	int mRas = glutCreateMenu(MenuHandler);
	glutAddMenuEntry("Kontur / Zapolnenie (m)", ACT_MODE_TOGGLE);
	glutAddMenuEntry("Sglazhivanie vkl/vykl (f)", ACT_AA_TOGGLE);
	glutAddMenuEntry("Setka krupnee (+)", ACT_GRID_BIGGER);
	glutAddMenuEntry("Setka melche (-)", ACT_GRID_SMALLER);

	glutCreateMenu(MenuHandler);
	glutAddSubMenu("Obyekty", mObj);
	glutAddSubMenu("Preobrazovaniya", mTr);
	glutAddSubMenu("Cvet i tekstura", mCol);
	glutAddSubMenu("Logicheskie operacii", mLog);
	glutAddSubMenu("Rastrirovanie i setka", mRas);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/* Головная программа */
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WinW, WinH);
	glutCreateWindow("Lab 2: Shestiugolniki");

	InitTextures();    // текстуры создаются после создания окна (нужен контекст OpenGL)

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeys);
	glutMouseFunc(Mouse);
	glutPassiveMotionFunc(PassiveMotion);
	BuildGlutMenu();

	glutMainLoop();
	return 0;
}
