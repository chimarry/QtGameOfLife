#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <CL/cl.hpp>
#include "ConwayMatrix.h"

class ConwayGameOfLifeExecutor:public QObject
{
public:
    struct SubSegment {
        const char* subSegmentFilePath;
        int positionY;
        int positionX;
        int rowCount;
        int columnCount;
    };
    Q_OBJECT
public:
    ConwayGameOfLifeExecutor() = delete;
    explicit ConwayGameOfLifeExecutor(int iterationCount, const char* fromFile = nullptr);
    ConwayGameOfLifeExecutor(const ConwayGameOfLifeExecutor&) = delete;
    ConwayGameOfLifeExecutor(ConwayGameOfLifeExecutor&&) = delete;
    ConwayGameOfLifeExecutor& operator=(const ConwayGameOfLifeExecutor&) = delete;
    ConwayGameOfLifeExecutor& operator=(ConwayGameOfLifeExecutor&&) = delete;
    ~ConwayGameOfLifeExecutor();

    void simulate();
    void addSubSegment(SubSegment subSegmentInfo);
    int* getSubSegment(int positionX, int positionY, int rowCount, int columnCount);
    int* getSubSegment(const ConwayMatrix& initialMatrix, int positionRow, int positionColumn, int rowCount, int columnCount);
    void  saveSubSegment(int positionRow, int positionColumn, int rowCount, int columnCount);
private:
    int* addSubSegment(const ConwayMatrix& initialMatrix, const ConwayMatrix& subSegment);
    int* manipulateSubSegment(const char* kernelName, const ConwayMatrix& initialMatrix, int* subsegment,
                              int positionRow, int positionColumn, int rowCount, int columnCount, int outputSize, bool getSubSegment = false);

public slots:
    void saveImage(const ConwayMatrix& matrix, int iteration) const;

signals:
    void imageSaved(QString imageLocation, int iteration) const;

private:
    int iterationCount;
    const char* initialFile = nullptr;
    ConwayMatrix* segmentToAdd = nullptr;
    ConwayMatrix* gamePlatform = nullptr;
};
