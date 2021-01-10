#pragma once
#include <iostream>
#include <tuple>
#include <QObject>
#include <windows.h>

class ConwayMatrix
{
private:
    struct Index
    {
        int i;
        int j;
    };
public:
    enum  Cell { DEAD, ALIVE };
    explicit ConwayMatrix(int, int = 0);
    ConwayMatrix() = delete;
    ConwayMatrix(const ConwayMatrix&);
    ConwayMatrix(ConwayMatrix&&) noexcept;
    ConwayMatrix& operator=(const ConwayMatrix&);
    ConwayMatrix& operator=(ConwayMatrix&&) noexcept;
    ~ConwayMatrix();

    const Cell& operator[](const Index&) const noexcept(false);

    void randomInitialize();
    void fromIntVector(int* originalVector);
    int* toIntVector() const;
    void fromImage(const char* filename);
    void writeToImage(const char* filename) const;
    friend std::ostream& operator<<(std::ostream& s, const ConwayMatrix& matrix)
    {
        system("cls");
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        for (int i = 0; i < matrix.n; ++i, s << std::endl)
            for (int j = 0; j < matrix.m; ++j) {
                int value = (matrix.mat[i][j] == ConwayMatrix::Cell::DEAD) ? 7 : 254;
                SetConsoleTextAttribute(hConsole, value);
                s << "  ";
                SetConsoleTextAttribute(hConsole, 7);
            }
        return s;
    }
    size_t getSize() const;
    size_t getColumnCount() const;
    size_t getRowCount() const;
    void setPosition(int, int);
    std::tuple<int,int> getPosition() const;
private:
    int n;
    int m;
    Cell** mat;
    void copy(const ConwayMatrix&);
    void move(ConwayMatrix&&);
    bool wrongLocation(int, int) const;
    int positionX;
    int positionY;
};

