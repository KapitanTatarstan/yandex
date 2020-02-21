#include <iostream>
#include <ctime>
#include <windows.h>
#include <fstream>
#include <thread>
#include <string>

class Matrix {
public:
    int row = 4;
    int col = 4;

    int* array;
    Matrix() = delete;

    Matrix(const int row, const int col) {
        this->row = row;
        this->col = col;
        this->array = new int[row * col];
    } 

    ~Matrix() {
        delete[] array;
    }

    friend std::ostream& operator<< (std::ostream& output, const Matrix &obj) {
        for (int i = 0; i < obj.row; ++i) {
            for (int j = 0; j < obj.col; ++j)
                output << obj.array[i * obj.row + j] << " ";
            output << "\n";
        }
        return output;
    }
};


// std::thread second(&MyTimer, time, std::ref(*matrix));
void MyTimer(int time, Matrix& matrix) {
    while (1) {
        Sleep(time);
        system("cls");
        std::cout << matrix;
    }
}


// main.exe command text.txt M N seconds radius
int main(int argc, char** argv) {
    setlocale(LC_ALL, "Russian");
    srand(time(0));
    int M = 1, N = 1;
    int radius = 1;
    int time = 1;
    
    // Проверка на колличество аргументов командной строки и их значение.
    if (argc != 7) {
        std::cout << "Недостаточно входных параметров.\n";
        return -1;
    }
    else {
        M = std::stoi(argv[3]);
        N = std::stoi(argv[4]);
        time = std::stoi(argv[5]);
        radius = std::stoi(argv[6]);

        if (N < 1 || M < 1) {
            std::cout << "Неверно задан размер доски.\n";
            return -2;
        }
        if (radius < 1) {
            std::cout << "Неверно задан параметр \"радиус\".\n";
            return -3;
        }
        if (time < 1) {
            std::cout << "Неверно задан параметр \"секунды\".\n";
            return -4;
        }
        else
            time *= 1000;
        
    }
    int command = std::stoi(argv[1]);

   
    Matrix *matrix = new Matrix(M, N);
    std::ifstream myfile;
    

    switch (command) {
        case 1: 
            myfile.open(argv[2]);
            if (!myfile) {
                std::cout << "Файл " << argv[2] << " не найден.\n";
                return -5;
            }
            for (int i = 0; i < M; ++i)
                for (int j = 0; j < N; ++j)
                    myfile >> matrix->array[M * i + j];

            myfile.close();
            break;

        case 2:
            for (int i = 0; i < M; ++i)
                for (int j = 0; j < N; ++j)
                    matrix->array[M * i + j] = rand() % 2;
            break;

        default:
            std::cout << "Неверно выбранная команда обработки\n";
            return -6;
            break;
    }
    
    
    std::thread second(&MyTimer, time, std::ref(*matrix));
    second.detach();

    int row = 0;
    int col = 0;

    int xr1, yr1, xr2, yr2;
    
    while (1) {
        // Определение границ области проверки соседей
        xr1 = row - radius;
        yr1 = col - radius;
        xr2 = row + radius;
        yr2 = col + radius;
        int neighbor = 0;

        // Если область проверки соседей выходит за границу доски, то граница проверки обрезается
        if (xr1 < 0)
            xr1 = 0;
        if (yr1 < 0)
            yr1 = 0;
        if (xr2 >= M)
            xr2 = M;
        if (yr2 >= N)
            yr2 = N;

        // Суммирование всех соседей.
        for (int i = xr1; i <= xr2; ++i) {
            for (int j = yr1; j <= yr2; ++j)
                neighbor += matrix->array[i * M + j];
        }

        int currentCell = row * M + col;
        neighbor -= matrix -> array[currentCell];


        
        if (matrix -> array[currentCell] == 1) {
            if ((neighbor < 2) || (neighbor > 3)) {
                matrix -> array[currentCell] = 0;
            }
        }
        else {
            if (neighbor == 3)
                matrix -> array[currentCell] = 1;
        }

        
        ++col;
        if (col == N) {
            col = 0;
            ++row;
        }
        if (row == M)
            row = 0;
    }

}
