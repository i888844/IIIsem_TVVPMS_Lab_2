#include <iostream>
#include <windows.h>
#include <time.h>

using namespace std;

class cube
{
private:
    int sides;
    double* probability, * ort;
public:
    cube()
    {
        sides = 6;
        probability = new double[sides];
        ort = new double[sides];
        double sum = 0;
        for (int i = 0; i < sides - 1; i++)
        {
            probability[i] = 1.0 / sides;
            sum += probability[i];
            ort[i] = sum;
        }
        probability[sides - 1] = 1 - sum;
        ort[sides - 1] = 1;
    }
    cube(int cube_sides)
    {
        if (cube_sides > 1)
        {
            sides = cube_sides;
        }
        else
        {
            sides = 6;
        }
        probability = new double[sides];
        ort = new double[sides];
        double sum = 0;
        for (int i = 0; i < sides - 1; i++)
        {
            probability[i] = 1.0 / sides;
            sum += probability[i];
            ort[i] = sum;
        }
        probability[sides - 1] = 1 - sum;
        ort[sides - 1] = 1;
    }
    cube(int cube_sides, int dominant_side, double probability_side)
    {
        double sum = 0;
        if (cube_sides > 1 && dominant_side > 0 && dominant_side <= cube_sides && probability_side > 0 && probability_side < 1)
        {
            sides = cube_sides;
            probability = new double[sides];
            ort = new double[sides];
            double p = (1 - probability_side) / (sides - 1);
            for (int i = 0; i < sides - 1; i++)
            {
                if ((i + 1) == dominant_side)
                {
                    probability[i] = probability_side;
                }
                else
                {
                    probability[i] = p;
                }
                sum += probability[i];
                ort[i] = sum;
            }
        }
        else
        {
            sides = 6;
            probability = new double[sides];
            ort = new double[sides];
            for (int i = 0; i < sides - 1; i++)
            {
                probability[i] = 1.0 / sides;
                sum += probability[i];
                ort[i] = sum;
            }
        }
        probability[sides - 1] = 1 - sum;
        ort[sides - 1] = 1;

    }
    cube(cube& object)
    {
        sides = object.sides;
        probability = new double[sides];
        ort = new double[sides];
        for (int i = 0; i < sides; i++)
        {
            probability[i] = object.probability[i];
            ort[i] = object.ort[i];
        }
    }
    ~cube()
    {
        delete[]probability;
        delete[]ort;
    }
    int get_sides()
    {
        return sides;
    }
    double get_probability_side(int side_number)
    {
        if (side_number > 0 && side_number <= sides)
        {
            return (probability[side_number - 1]);
        }
        return 0;
    }
    void output()
    {
        cout << "Количество сторон: " << sides << endl;
        for (int i = 0; i < sides; i++)
        {
            cout << i + 1 << "     " << probability[i] << endl;
        }
    }
    int roll()
    {
        double x = (double)rand() / RAND_MAX;
        for (int i = 0; i < sides; i++)
        {
            if (x <= ort[i])
            {
                return (i + 1);
            }
        }
        return 1;
    }
};

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand(time(NULL));
    int i = 0, j = 0, cmd = 0, sides_a = 0, sides_b = 0, roll_amount = 0, * results = nullptr;
    double dominant_side_a = 0, probability_side_a = 0, dominant_side_b = 0, probability_side_b = 0, hi2 = 0, * theoretical_probabilities = nullptr, * practical_probabilities = nullptr;
    cout << "Выберите кубы:" << endl;
    cout << "1 - С равным весом граней" << endl;
    cout << "2 - С доминирующей гранью" << endl;
    cout << "Введите номер команды: ";
    cin >> cmd;
    while (cmd != 1 && cmd != 2)
    {
        cout << "[Ошибка]: введён неверный номер команды. Введите номер команды: ";
        cin >> cmd;
    }
    switch (cmd)
    {
        case 1:
        {
            cout << "Введите количество граней первого куба: ";
            cin >> sides_a;
            cube a(sides_a);
            cout << "Введите количество граней второго куба: ";
            cin >> sides_b;
            cube b(sides_b);
            results = new int[a.get_sides() + b.get_sides() - 1];
            theoretical_probabilities = new double[a.get_sides() + b.get_sides() - 1];
            practical_probabilities = new double[a.get_sides() + b.get_sides() - 1];
            for (i = 0; i < a.get_sides() + b.get_sides() - 1; i++)
            {
                results[i] = 0;
                theoretical_probabilities[i] = 0;
            }
            cout << "Введите количество бросков: ";
            cin >> roll_amount;
            while (roll_amount <= 0)
            {
                cout << "[Ошибка]: количество бросков должно быть больше 0. Введите количество бросков: ";
                cin >> roll_amount;
            }
            a.output();
            b.output();
            for (i = 0; i < a.get_sides(); i++)
            {
                for (j = 0; j < b.get_sides(); j++)
                {
                    theoretical_probabilities[i + j] += a.get_probability_side(i + 1) * b.get_probability_side(j + 1);
                }
            }
            for (i = 0; i < roll_amount; i++)
            {
                results[a.roll() + b.roll() - 2]++;
            }
            double sum_theoretical_probabilities = 0, sum_practical_probabilities = 0;
            cout << "\nСумма     Количество бросков     Вероятность выпадения (теор.)     Вероятность выпадения (практ.)     Отклонение" << endl;
            for (i = 0; i < a.get_sides() + b.get_sides() - 1; i++)
            {
                practical_probabilities[i] = (double)results[i] / roll_amount;
                cout << "      " << i + 2 << "                 " << results[i] << "                 " << theoretical_probabilities[i] << "                           " << practical_probabilities[i] << "                           " << fabs(theoretical_probabilities[i] - practical_probabilities[i]) << endl;
                sum_theoretical_probabilities += theoretical_probabilities[i];
                sum_practical_probabilities += practical_probabilities[i];
                hi2 += powf(theoretical_probabilities[i] - practical_probabilities[i], 2) / theoretical_probabilities[i];
            }
            hi2 *= roll_amount;
            cout << "                                          " << sum_theoretical_probabilities << "                           " << sum_practical_probabilities << endl;
            cout << "Хи-квадрат: " << hi2 << endl;
            break;
        }
        case 2:
        {
            cout << "введите количество граней первого куба: ";
            cin >> sides_a;
            cout << "введите номер доминантной грани первого куба: ";
            cin >> dominant_side_a;
            cout << "ведите вес доминантной грани первого куба: ";
            cin >> probability_side_a;
            cube a(sides_a, dominant_side_a, probability_side_a);
            cout << "введите количество граней второго куба: ";
            cin >> sides_b;
            cout << "введите номер доминантной грани второго куба: ";
            cin >> dominant_side_b;
            cout << "ведите вес доминантной грани второго куба: ";
            cin >> probability_side_b;
            cube b(sides_b, dominant_side_b, probability_side_b);
            results = new int[a.get_sides() + b.get_sides() - 1];
            theoretical_probabilities = new double[a.get_sides() + b.get_sides() - 1];
            practical_probabilities = new double[a.get_sides() + b.get_sides() - 1];
            for (i = 0; i < a.get_sides() + b.get_sides() - 1; i++)
            {
                results[i] = 0;
                theoretical_probabilities[0] = 0;
            }
            cout << "Введите количество бросков: ";
            cin >> roll_amount;
            while (roll_amount <= 0)
            {
                cout << "[Ошибка]: количество бросков должно быть больше 0. Введите количество бросков: ";
                cin >> roll_amount;
            }
            a.output();
            b.output();
            for (i = 0; i < a.get_sides(); i++)
            {
                for (j = 0; j < b.get_sides(); j++)
                {
                    theoretical_probabilities[i + j] += a.get_probability_side(i + 1) * b.get_probability_side(j + 1);
                }
            }
            for (i = 0; i < roll_amount; i++)
            {
                results[a.roll() + b.roll() - 2]++;
            }
            double sum_theoretical_probabilities = 0, sum_practical_probabilities = 0;
            cout << "\nСумма     Количество бросков     Вероятность выпадения (теор.)     Вероятность выпадения (практ.)     Отклонение" << endl;
            for (i = 0; i < a.get_sides() + b.get_sides() - 1; i++)
            {
                practical_probabilities[i] = (double)results[i] / roll_amount;
                cout << "  " << i + 2 << "                 " << results[i] << "                 " << theoretical_probabilities[i] << "                           " << practical_probabilities[i] << "                           " << fabs(theoretical_probabilities[i] - practical_probabilities[i]) << endl;
                sum_theoretical_probabilities += theoretical_probabilities[i];
                sum_practical_probabilities += practical_probabilities[i];
                hi2 += powf(theoretical_probabilities[i] - practical_probabilities[i], 2) / theoretical_probabilities[i];
            }
            hi2 *= roll_amount;
            cout << "                                          " << sum_theoretical_probabilities << "                           " << sum_practical_probabilities << endl;
            cout << "Хи-квадрат: " << hi2 << endl;
            break;
        }
    }
    delete[]results;
    delete[]theoretical_probabilities;
    delete[]practical_probabilities;
    return 0;
}