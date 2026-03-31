#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <functional>
#include <string>

    using namespace std;

// Структура для точки
struct Point {
    double x, y;
    Point(double _x = 0, double _y = 0) : x(_x), y(_y) {}
    Point operator+(const Point& other) const { return Point(x + other.x, y + other.y); }
    Point operator-(const Point& other) const { return Point(x - other.x, y - other.y); }
    Point operator*(double scalar) const { return Point(x * scalar, y * scalar); }
    friend ostream& operator<<(ostream& out, const Point& p)
    {
        out << '(' << p.x << ", " << p.y << ')';
        return out;
    }
};

// Глобальные счетчики для статистики
int func_evaluations = 0;
int iterations_count = 0;
bool verbose = false; // Включить подробный вывод для отчета

// Базовый класс функции
class Function {
public:
    virtual double operator()(const Point& p) = 0;
    virtual Point grad(const Point& p) { return Point(0, 0); } // По умолчанию 0
    virtual string name() = 0;
    virtual ~Function() {}
};

// 1. Функция Розенброка (Минимизация)
// f(x) = 100*(y - x^2)^2 + (1 - x)^2
class Rosenbrock : public Function {
public:
    double operator()(const Point& p) override {
        func_evaluations++;
        return 100.0 * pow(p.y - p.x * p.x, 2) + pow(1.0 - p.x, 2);
    }
    Point grad(const Point& p) override {
        // Градиент функции Розенброка
        double dfdx = -400.0 * p.x * (p.y - p.x * p.x) - 2.0 * (1.0 - p.x);
        double dfdy = 200.0 * (p.y - p.x * p.x);
        return Point(dfdx, dfdy);
    }
    string name() override { return "Rosenbrock"; }
};

// 2. Функция Варианта 4 (Максимизация -> Минимизация -f)
class Variant4 : public Function {
    // Параметры из таблицы для варианта 4
    const double A1 = 2, A2 = 1;
    const double a1 = 1, a2 = 3;
    const double b1 = 2, b2 = 3;
    const double c1 = 2, c2 = 1;
    const double d1 = 1, d2 = 3;

    double calcF(const Point& p) {
        double term1 = 1.0 + pow((p.x - a1) / b1, 2) + pow((p.y - c1) / d1, 2);
        double term2 = 1.0 + pow((p.x - a2) / b2, 2) + pow((p.y - c2) / d2, 2);
        return (A1 / term1) + (A2 / term2);
    }
public:
    double operator()(const Point& p) override {
        func_evaluations++;
        // Возвращаем отрицательное значение, т.к. ищем максимум через минимизацию
        return -calcF(p);
    }
    Point grad(const Point& p) override {
        // Градиент f(x,y) (без минуса)
        double t1 = 1.0 + pow((p.x - a1) / b1, 2) + pow((p.y - c1) / d1, 2);
        double t2 = 1.0 + pow((p.x - a2) / b2, 2) + pow((p.y - c2) / d2, 2);

        // df/dx для исходной функции f
        double dfdx = -A1 * (2.0 * (p.x - a1) / (b1 * b1)) / (t1 * t1)
            - A2 * (2.0 * (p.x - a2) / (b2 * b2)) / (t2 * t2);

        // df/dy для исходной функции f
        double dfdy = -A1 * (2.0 * (p.y - c1) / (d1 * d1)) / (t1 * t1)
            - A2 * (2.0 * (p.y - c2) / (d2 * d2)) / (t2 * t2);

        // Возвращаем градиент для минимизации -f, то есть -grad(f)
        return Point(-dfdx, -dfdy);
    }
    string name() override { return "Variant4 (Max)"; }
};

class QuadraticFunction : public Function
{
public:
    double operator()(const Point& p) override {
        func_evaluations++;
        return 100.0 * pow(p.y - p.x, 2) + pow(1.0 - p.x, 2);
    }
    Point grad(const Point& p) override {
        // Градиент функции
        double dfdx = 200.0 * (p.x - p.y) - 2.0 * (1.0 - p.x);
        double dfdy = 200.0 * (p.y - p.x);
        return Point(dfdx, dfdy);
    }
    string name() override { return "Quadratic"; }
};

// Одномерный поиск (Метод золотого сечения)
// Минимизирует func(start + lambda * direction)
double goldenSection(Function& func, const Point& start, const Point& direction, double eps = 1e-4) {
    int counter = 0;
    auto f_lambda = [&](double l) {
        Point p = start + direction * l;
        return func(p);
    };
    // Поиск интервала, содержащего минимум
    double a = 0, delta = 1, h = delta;
    double b = 1;
    while (f_lambda(a) > f_lambda(a + h))
    {
        b = a + h;
        h *= 2;
    }
    b = 4;
    std::cout << b << '\n';
    // Одномерный поиск
    double phi = (1.0 + sqrt(5.0)) / 2.0;
    double x1 = b - (b - a) / phi;
    double x2 = a + (b - a) / phi;

    double y1 = f_lambda(x1);
    double y2 = f_lambda(x2);

    while (abs(b - a) > eps) {
        if (y1 < y2) {
            b = x2;
            x2 = x1;
            y2 = y1;
            x1 = b - (b - a) / phi;
            y1 = f_lambda(x1);
        }
        else {
            a = x1;
            x1 = x2;
            y1 = y2;
            x2 = a + (b - a) / phi;
            y2 = f_lambda(x2);
        }
        counter++;
    }
    std::cout << "counter: " << counter << '\n';
    return (a + b) / 2.0;
}

// Метод 1: Хука-Дживса (0-й порядок)
void hookJeeves(Function& func, Point start, double eps) {
    cout << "\n--- Method: Hook-Jeeves (0th Order) ---" << endl;
    cout << "Function: " << func.name() << ", Start: (" << start.x << ", " << start.y << ")" << endl;

    func_evaluations = 0;
    iterations_count = 0;

    double step = 1.0;
    double alpha = 0.5; // Коэффициент уменьшения шага
    Point x_curr = start;
    double f_curr = func(x_curr);

    cout << fixed << setprecision(6);
    if (verbose) cout << "Iter\tX\t\tY\t\tF\t\tStep" << endl;

    while (step > eps) {
        iterations_count++;
        Point x_new = x_curr;

        // Исследующий поиск по координатам
        double f_temp = func(x_new);

        // По X
        Point x_trial = x_new + Point(step, 0);
        if (func(x_trial) < f_temp) {
            x_new = x_trial;
            f_temp = func(x_new);
        }
        else {
            x_trial = x_new + Point(-step, 0);
            if (func(x_trial) < f_temp) {
                x_new = x_trial;
                f_temp = func(x_new);
            }
        }

        // По Y
        x_trial = x_new + Point(0, step);
        if (func(x_trial) < f_temp) {
            x_new = x_trial;
            f_temp = func(x_new);
        }
        else {
            x_trial = x_new + Point(0, -step);
            if (func(x_trial) < f_temp) {
                x_new = x_trial;
                f_temp = func(x_new);
            }
        }

        // Успешный шаг?
        if (x_new.x != x_curr.x || x_new.y != x_curr.y) {
            // Движение по образцу
            Point pattern = x_new + (x_new - x_curr);
            if (func(pattern) < f_temp) {
                x_curr = pattern;
            }
            else {
                x_curr = x_new;
            }
        }
        else {
            // Неудача, уменьшаем шаг
            step *= alpha;
            x_curr = x_new;
        }

        f_curr = func(x_curr);
        if (verbose) cout << iterations_count << "\t" << x_curr.x << "\t" << x_curr.y << "\t" << f_curr << "\t" << step << endl;
    }

    cout << "Result: (" << x_curr.x << ", " << x_curr.y << ")" << endl;
    cout << "Min Value: " << f_curr << " (Max Original: " << -f_curr << ")" << endl;
    cout << "Iterations: " << iterations_count << ", Func Evals: " << func_evaluations << endl;
}

// Метод 2: Наискорейший спуск (1-й порядок)
void gradientDescent(Function& func, Point start, double eps) {
    cout << "\n--- Method: Gradient Descent (1st Order) ---" << endl;
    cout << "Function: " << func.name() << ", Start: (" << start.x << ", " << start.y << ")" << endl;

    func_evaluations = 0;
    iterations_count = 0;

    Point x_curr = start;
    double f_curr = func(x_curr);

    cout << fixed << setprecision(6);
    if (verbose) cout << "Iter\tX\t\tY\t\tF\t\tGradNorm\tLambda" << endl;

    while (true) {
        iterations_count++;
        Point g = func.grad(x_curr);
        double gradNorm = sqrt(g.x * g.x + g.y * g.y);
        g = g * (1 / gradNorm);

        if (gradNorm < eps) break;

        // Направление спуска: антиградиент
        Point direction = g * (-1.0);

        // Одномерный поиск шага lambda
        double lambda = goldenSection(func, x_curr, direction);

        Point x_new = x_curr + direction * lambda;
        double f_new = func(x_new);

        if (verbose) cout << iterations_count << "\t" << x_new.x << "\t" << x_new.y << "\t" << f_new << "\t" << gradNorm << "\t" << lambda << "\t" << g  <<endl;

        // Критерий остановки по изменению функции или точки
        if (abs(f_curr - f_new) < eps && abs(x_curr.x - x_new.x) < eps && abs(x_curr.y - x_new.y) < eps) {
            x_curr = x_new;
            f_curr = f_new;
            break;
        }

        x_curr = x_new;
        f_curr = f_new;

        // Защита от зацикливания
        if (iterations_count > 100) break;
    }

    cout << "Result: (" << x_curr.x << ", " << x_curr.y << ")" << endl;
    cout << "Min Value: " << f_curr << " (Max Original: " << -f_curr << ")" << endl;
    cout << "Iterations: " << iterations_count << ", Func Evals: " << func_evaluations << endl;
}

int main() {
    // Настройка вывода
    cout << fixed << setprecision(6);

    // Выбор режима вывода (true для подробностей, false для краткости)
    verbose = true;

    // Инициализация функций
    Rosenbrock rosen;
    Variant4 var4;
    QuadraticFunction qf;

    // Начальные точки (как минимум две разные)
    vector<Point> startPoints = { Point(5, 1)};

    cout << "==========================================" << endl;
    cout << "LAB WORK 2: OPTIMIZATION METHODS" << endl;
    cout << "==========================================" << endl;

    // Тестирование на функции Розенброка
    //for (const auto& sp : startPoints) {
    //    hookJeeves(rosen, sp, 1e-4);
    //    gradientDescent(rosen, sp, 1e-4);
    //}

    //// Тестирование на функции Варианта 4
    //for (const auto& sp : startPoints) {
    //    hookJeeves(var4, sp, 1e-4);
    //    gradientDescent(var4, sp, 1e-4);
    //}

    for (const auto& sp : startPoints) {
    //hookJeeves(qf, sp, 1e-4);
    gradientDescent(qf, sp, 1e-4);
    }

    return 0;
}