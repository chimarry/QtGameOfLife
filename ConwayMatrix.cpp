#include "ConwayMatrix.h"
#include<exception>
#include<stdexcept>

ConwayMatrix::ConwayMatrix(int n, int mm) :rowCount(n), columnCount(mm ? mm : n), mat(new ConwayMatrix::Cell* [n])
{
    for (int i = 0; i < n; ++i) {
        mat[i] = new ConwayMatrix::Cell[columnCount];
        std::fill_n(mat[i], columnCount, ConwayMatrix::Cell::DEAD);
    }
}

ConwayMatrix::ConwayMatrix(const ConwayMatrix& matrix)
{
    copy(matrix);
}

ConwayMatrix::ConwayMatrix(ConwayMatrix&& matrix) noexcept
{
    move(std::move(matrix));
}

ConwayMatrix& ConwayMatrix::operator=(const ConwayMatrix& matrix)
{
    if (this != &matrix) {
        this->~ConwayMatrix();
        copy(matrix);
    }
    return *this;
}

ConwayMatrix& ConwayMatrix::operator=(ConwayMatrix&& matrix) noexcept
{
    if (this != &matrix) {
        this->~ConwayMatrix();
        move(std::move(matrix));
    }
    return *this;
}

ConwayMatrix::~ConwayMatrix()
{
    for (int i = 0; i < rowCount; ++i) {
        delete[] mat[i];
        mat[i] = nullptr;
    }
    delete[] mat;
    mat = nullptr;
    columnCount = rowCount = 0;
}

size_t ConwayMatrix::getSize() const
{
    return (size_t)rowCount * columnCount;
}

const ConwayMatrix::Cell& ConwayMatrix::operator[](const Index& index) const noexcept(false)
{
    return wrongLocation(index.i, index.j) ? throw(std::out_of_range("Wrong")) : mat[index.i][index.j];
}

size_t ConwayMatrix::getColumnCount() const {
    return (size_t)columnCount;
}

size_t ConwayMatrix::getRowCount() const
{
    return (size_t)rowCount;
}

void ConwayMatrix::setPosition(int x, int y)
{
    positionX = x;
    positionY = y;
}

std::tuple<int, int> ConwayMatrix::getPosition() const
{
    return std::tuple<int, int>(positionX,positionY);
}

void ConwayMatrix::fromIntVector(int* vector)
{
    for (int i = 0; i < rowCount; ++i)
        for (int j = 0; j < columnCount; ++j)
            mat[i][j] = static_cast<ConwayMatrix::Cell>(vector[i * columnCount + j]);
}

void ConwayMatrix::randomInitialize()
{
    for (int i = 0; i < rowCount; ++i)
        for (int j = 0; j < columnCount; ++j)
            if (i == 0 || i == rowCount - 1 || columnCount == 0 || j == columnCount - 1)
                mat[i][j] = ConwayMatrix::Cell::DEAD;
            else
                mat[i][j] = (rand() % 100) > 50 ? ConwayMatrix::Cell::DEAD : ConwayMatrix::Cell::ALIVE;
}

int* ConwayMatrix::toIntVector() const
{
    int* vector = new int[getSize()];
    for (int i = 0; i < rowCount; ++i)
        for (int j = 0; j < columnCount; ++j)
            vector[i * columnCount + j] = mat[i][j];
    return vector;
}

void ConwayMatrix::fromImage(const char* filename)
{
    FILE* fp = fopen(filename, "rb");
    int w, h;
    if (!fscanf(fp, "P5\n%d %d\n255\n", &w, &h))
    {
        throw "error";
    }
    unsigned char* image = new unsigned char[(size_t)rowCount * columnCount];
    fread(image, sizeof(unsigned char), (size_t)rowCount * (size_t)columnCount, fp);
    fclose(fp);
    for (int i = 0; i < rowCount; ++i)
        for (int j = 0; j < columnCount; ++j)
            mat[i][j] = image[i * columnCount + j] == 255 ? ConwayMatrix::ALIVE : ConwayMatrix::DEAD;
}

void ConwayMatrix::writeToImage(const char* filename) const
{
    FILE* fp = fopen(filename, "wb");
    fprintf(fp, "P5\n%d %d\n255\n", columnCount, rowCount);
    int* intVector = toIntVector();
    unsigned char* charVector = new unsigned char[rowCount * columnCount];
    for (int i = 0; i < rowCount * columnCount; ++i)
        charVector[i] = intVector[i] == 0 ? 0 : 255;
    fwrite(charVector, sizeof(unsigned char), (size_t)rowCount * (size_t)columnCount, fp);
    fclose(fp);
}

void ConwayMatrix::copy(const ConwayMatrix& matrix)
{
    rowCount = matrix.rowCount;
    columnCount = matrix.columnCount;
    mat = new ConwayMatrix::Cell * [rowCount];
    for (int i = 0; i < rowCount; i++)
    {
        mat[i] = new ConwayMatrix::Cell[columnCount];
        std::copy(matrix.mat[i], matrix.mat[i] + columnCount, mat[i]);
    }
}

void ConwayMatrix::move(ConwayMatrix&& matrix)
{
    rowCount = matrix.rowCount;
    columnCount = matrix.columnCount;
    mat = matrix.mat;
    matrix.mat = nullptr;
}

bool ConwayMatrix::wrongLocation(int i, int j) const
{
    return (i < 0 || i >= rowCount || j < 0 || j >= columnCount) ? true : false;
}
