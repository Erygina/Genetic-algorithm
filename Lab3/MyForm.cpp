#include "MyForm.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <string.h>
#include <sstream>
//using namespace NewTon;
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Diagnostics;
using namespace System::IO;
using namespace System::Drawing;
[STAThreadAttribute]

void main(array<String^ >^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	Lab3::MyForm form;
	Application::Run(% form);
}

std::ofstream resultFile;
int sumHelp = 0; int numberPuple = 0;
const int n = 100;    //Число особей в популяции !ЧЕТНОЕ!
const int m = 3;    //Максимальное количество 
const int number = 50;   //количество вращений рулетки ВОЗМОЖНО СТОИТ ПОМЕНЯТЬ НА l [0.3 ; 0.7]
const double Pm = 0.01; //Вероятность мутации = 1/длинна хромосомы
const double Pc = 1;  //Вероятность кроссинговера [0.6 ; 1]
class Individual {
public:
    int gen[3]; //у одной особи 3 гена
    void initial_gen() //инициализация 3х генов
    {
        for (int i = 0; i < 3; i++) gen[i] = rand() % (255);
    }

};

Individual purple;//фиолетовый цвет


class Population {
public:
    
    double population_fitness[n]; //Массив оценки популяции 
    double population_prob[n]; //Массив вероятностей скрещивания особи
    double sum; // сумма приспособленностей всех особей популяции
    double chose_individual[n]; //Список особей, отобранных для дальнейших этапов генетического алгоритма 
    Individual individ[n]; // популяция (наши x)
    Individual parent[number];  // предки       
    int count;  //количество отобраных предков

    //====Инициализация популяции=====
    void initial_population()
    {
        for (int i = 0; i < n; i++) individ[i].initial_gen();        
    }

    //====Вывод популяции====
    void Output_pop(int Count_pop)               //В этой функции надо будет печать графика
    {
        resultFile << "Популяция " << Count_pop << ": " << '\n';
        for (int i = 0; i < n; i++)
        {
            resultFile << i << ". [" << individ[i].gen[0] << "," << individ[i].gen[1] << "," << individ[i].gen[2] << "]" << '\n';
        }
    }

    //====функция приспособленности особей начальной популяции====
    bool fit_pop()
    {
        sum = 0;
        Individual helpPurple, help;//helpFit,
        for (int i = 0; i < n; i++)
        {
            population_fitness[i] = 0;
            helpPurple.gen[0] = purple.gen[0]; helpPurple.gen[1] = purple.gen[1]; helpPurple.gen[2] = purple.gen[2];
            help.gen[0] = individ[i].gen[0]; help.gen[1] = individ[i].gen[1]; help.gen[2] = individ[i].gen[2];
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 8; k++)
                {                   
                    if (helpPurple.gen[j] % 2 != help.gen[j] % 2)
                    {
                        population_fitness[i] ++;
                    }
                    helpPurple.gen[j] = helpPurple.gen[j] >> 1;
                    help.gen[j] = help.gen[j] >> 1;
                }
            }

            if (population_fitness[i] == 0){
                numberPuple = i; 
                return 1;
            }
            sum += 1 / population_fitness[i];
        }
        return 0;       
    }

    //====Mетод оценки вероятности скрещивания для каждой особи====
    void fitness_prob()
    {
        for (int i = 0; i < n; i++) population_prob[i] = 1 / (population_fitness[i] * sum);        
    }

    //====Функция, реализующая алгоритм рулеточной селекции====
    void roulette_wheel_pop()
    {
        count = 0;
        double r = 0;
        double circl;
        for (int i = 0; i < n; i++) chose_individual[i] = -1;

        for (int i = 0; i < number; i++)
        {
            r = rand() % 1000 / 1000.0;
            circl = 0;
            for (int i = 0; i < n; i++)
            {
                circl += population_prob[i];
                if (r <= circl && chose_individual[i] != 1)
                {
                    chose_individual[i] = 1;
                    count++;
                    break;
                }
            }
        }
    }

    //==== Кроссинговер для популяции ====
    void population_crossover()
    {
        int j = 0;
        int power;
        int bit;

        for (int i = 0; i < n; i++)
        {
            if (chose_individual[i] == 1)
            {
                parent[j] = individ[i];
                j++;
            }
        }
        int i = 0;
        j = 0;
        for (int b = 0; b < n; b += 2)
        {
            i = rand() % (count);
            j = rand() % (count);
            double r = rand() % 1000 / 1000.0;

            if (Pc > r)
            {
                bit = rand() % 24;
                power = pow(2, bit % 8);
                if (bit < 8)
                {

                    individ[b] = parent[i];
                    individ[b + 1] = parent[j];
                    individ[b].gen[2] = (parent[i].gen[2] / power) * power + parent[j].gen[2] % power;
                    individ[b + 1].gen[2] = (parent[j].gen[2] / power) * power + parent[i].gen[2] % power;
                }
                else if (bit < 16)
                {

                    individ[b].gen[0] = parent[i].gen[0];
                    individ[b + 1].gen[0] = parent[j].gen[0];
                    individ[b].gen[2] = parent[j].gen[2];
                    individ[b + 1].gen[2] = parent[i].gen[2];
                    individ[b].gen[1] = (parent[i].gen[1] / power) * power + parent[j].gen[1] % power;
                    individ[b + 1].gen[1] = (parent[j].gen[1] / power) * power + parent[i].gen[1] % power;
                }
                else
                {

                    individ[b] = parent[j];
                    individ[b + 1] = parent[i];
                    individ[b].gen[0] = (parent[i].gen[0] / power) * power + parent[j].gen[0] % power;
                    individ[b + 1].gen[0] = (parent[j].gen[0] / power) * power + parent[i].gen[0] % power;
                }
            }
            else
            {

                individ[b] = parent[i];
                individ[b + 1] = parent[j];
            }
        }
    }


    //==== Метод мутации ====   
    void mutation()
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < 3; j++)
                for (int k = 0; k < 8; k++)
                {
                    double r = rand() % 1000 / 1000.0;
                    if (r < Pm)
                    {
                        int power = pow(2, k);
                        int help = (individ[i].gen[j] / power) % 2;
                        if (help == 0)
                        {
                            individ[i].gen[j] += power;
                        }
                        else
                        {
                            individ[i].gen[j] -= power;
                        }
                    }
                }
        }

    }
};


System::Void Lab3::MyForm::drawButton_Click(System::Object^ sender, System::EventArgs^ e) {
    resultFile.open("result.txt");  
    Genes();
    CreatGraph();
    resultFile.close();
    System::Diagnostics::Process::Start("result.txt");
}

void Lab3::MyForm::Genes() {
    if (textBox1->Text == "" || textBox2->Text == "" || textBox3->Text == "") {
        purple.gen[0] = 96;
        purple.gen[1] = 96;
        purple.gen[2] = 159;
        textBox1->Text = Convert::ToString(96);
        textBox2->Text = Convert::ToString(96);
        textBox3->Text = Convert::ToString(159);
    }
    else
    {
        purple.gen[0] = Convert::ToInt32(textBox1->Text);
        purple.gen[1] = Convert::ToInt32(textBox2->Text);
        purple.gen[2] = Convert::ToInt32(textBox3->Text);
    }    
    pictureBox2->BackColor = Color::FromArgb(purple.gen[0], purple.gen[1], purple.gen[2]);
}

void Lab3::MyForm::CreatGraph()
{
   
    Population obj;
    obj.initial_population();//инициализация    
    int Count_Pop = 1;
    bool found = 0; int y = 0;
    found = obj.fit_pop();//оценка
    chart1->ChartAreas[0]->AxisX->Crossing = 0;
    chart1->ChartAreas[0]->AxisY->Crossing = 0;
    this->chart1->Series[0]->Points->Clear();
    this->chart1->Series[1]->Points->Clear();
    int sumMin;
    while (found != 1)
    {
        //obj.Output_pop(Count_Pop);//вывод
        obj.fitness_prob();//вероятность
        obj.roulette_wheel_pop();//селекция
        obj.population_crossover();//кроссенговер
        obj.mutation();//мутация
        sumHelp = 0; sumMin = 100000;
        for (int j = 0; j < n; j++) {
            if (sumMin > obj.population_fitness[j]) sumMin = obj.population_fitness[j];
            sumHelp += obj.population_fitness[j];
        }
        sumHelp /= n;
        this->chart1->Series[0]->Points->AddXY(y, sumHelp);
        this->chart1->Series[1]->Points->AddXY(y, sumMin);
        y++;
        found = obj.fit_pop();//оценка
        Count_Pop++;
    }
    resultFile << "RESULT :" << '\n';
    obj.Output_pop(Count_Pop);//вывод   
    resultTextBox->Text = "Номер популяции: ";
    resultTextBox->Text += Convert::ToString(Count_Pop);
    numberTextBox->Text = "Номер особи: ";
    numberTextBox->Text += Convert::ToString(numberPuple);
    obj.roulette_wheel_pop();
}
