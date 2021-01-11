#include "ConwayGameOfLifeExecutor.h"
#include "ConwayMatrix.h"
#include "OpenCLConfiguration.h"

#include<thread>
#include <vector>
#include <CL/cl.hpp>
#include <QFileInfo>
#include <iostream>
#include <string>

ConwayGameOfLifeExecutor::ConwayGameOfLifeExecutor(int iterationCount, int width, int height, const char* fromFile)
{
    this->iterationCount = iterationCount;
    if(fromFile){
        this->initialFile = fromFile;
    }
    this->width = width;
    this->height = height;
}

ConwayGameOfLifeExecutor::~ConwayGameOfLifeExecutor() {}

void ConwayGameOfLifeExecutor::simulate(bool sleep) {
    gamePlatform = new ConwayMatrix(this->height, this->width);
    if(this->initialFile)
        gamePlatform->fromImage(initialFile);
    else
        gamePlatform->randomInitialize();

    if (segmentToAdd)
    {
        int* newDataWithSegment = addSubSegment(*gamePlatform, *segmentToAdd);
        gamePlatform->fromIntVector(newDataWithSegment);
    }

    int* initialVector = gamePlatform->toIntVector();

    cl_int error;
    cl_context context;
    cl_command_queue commandQueue;
    cl_program kernelProgram;
    cl_kernel kernel;
    cl_mem inputScene;
    cl_mem outputScene;

    size_t gameSceneSizeInBytes = sizeof(int) * this->gamePlatform->getSize();
    OpenCLConfiguration::configure(context, commandQueue, kernelProgram, kernel, SIMULATE_GAME_KERNEL);

    inputScene = clCreateBuffer(context, CL_MEM_READ_WRITE, gameSceneSizeInBytes, NULL, &error);
    outputScene = clCreateBuffer(context, CL_MEM_READ_WRITE, gameSceneSizeInBytes, NULL, &error);
    OpenCLConfiguration::printIfError(error);

    error = clEnqueueWriteBuffer(commandQueue, inputScene, CL_TRUE, 0, gameSceneSizeInBytes, initialVector, 0, NULL, NULL);
    OpenCLConfiguration::printIfError(error);

    size_t localWorkSize[2], globalWorkSize[2];
    localWorkSize[0] = localWorkSize[1] = 4;
    globalWorkSize[0] = this->height;
    globalWorkSize[1] = this->width;
    saveImage(*gamePlatform, 0);
    for (int i = 0; i < this->iterationCount; ++i)
    {
        if(sleep)
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        if (i != 0) {
            cl_mem tmp = inputScene;
            inputScene = outputScene;
            outputScene = tmp;
        }
        clSetKernelArg(kernel, 0, sizeof(cl_mem), &inputScene);
        clSetKernelArg(kernel, 1, sizeof(cl_mem), &outputScene);
        clSetKernelArg(kernel, 2, sizeof(int), (void*)&this->height);
        clSetKernelArg(kernel, 3, sizeof(int), (void*)&this->width);

        error = clEnqueueNDRangeKernel(commandQueue, kernel, 2, NULL, globalWorkSize,
                                       localWorkSize, 0, NULL, NULL);
        OpenCLConfiguration::printIfError(error);

        error = clEnqueueReadBuffer(commandQueue, outputScene, CL_TRUE, 0, gameSceneSizeInBytes, initialVector, 0, NULL, NULL);
        OpenCLConfiguration::printIfError(error);

        gamePlatform->fromIntVector(initialVector);
        saveImage(*gamePlatform, i+1);
    }
    clFinish(commandQueue);

    OpenCLConfiguration::releaseMemory(inputScene, outputScene, kernelProgram, kernel, commandQueue, context);
    delete[](initialVector);
}

void ConwayGameOfLifeExecutor::addSubSegment(SubSegment subSegmentInfo)
{
    segmentToAdd = new ConwayMatrix(subSegmentInfo.rowCount, subSegmentInfo.rowCount);
    segmentToAdd->setPosition(subSegmentInfo.positionX, subSegmentInfo.positionY);
    segmentToAdd->fromImage(subSegmentInfo.subSegmentFilePath);
}

int* ConwayGameOfLifeExecutor::addSubSegment(const ConwayMatrix& original, const ConwayMatrix& subSegment)
{
    int x, y;
    std::tie(x, y) = subSegment.getPosition();
    return manipulateSubSegment(ADD_SUB_SEGMENT, original, subSegment.toIntVector(), x, y, subSegment.getRowCount(), subSegment.getColumnCount(), original.getSize());
}

int* ConwayGameOfLifeExecutor::getSubSegment(int positionX, int positionY, int rowCount, int columnCount)
{
    return getSubSegment(*gamePlatform, positionX, positionY, rowCount, columnCount);
}

int* ConwayGameOfLifeExecutor::getSubSegment(const ConwayMatrix& original, int positionX, int positionY, int rowCount, int columnCount)
{
    size_t subSegmentSize = (size_t)rowCount * columnCount;
    return manipulateSubSegment(GET_SUB_SEGMENT, original, new int[subSegmentSize], positionX, positionY, rowCount, columnCount, subSegmentSize, true);
}

void ConwayGameOfLifeExecutor::saveSubSegment(int positionX, int positionY, int rowCount, int columnCount)
{
    int* segment = getSubSegment(positionX,positionY,rowCount,columnCount);
    ConwayMatrix segmentMatrix = ConwayMatrix(rowCount,columnCount);
    segmentMatrix.fromIntVector(segment);
    segmentMatrix.writeToImage("C:\\Users\\Marija\\Desktop\\test\\segment.pgm");
    delete(segment);
}

int* ConwayGameOfLifeExecutor::manipulateSubSegment(const char* kernelName, const ConwayMatrix& original, int* subSegmentData,
                                                    int positionRow, int positionColumn, int rowCount, int columnCount, int outputSize, bool getSubSegment)
{
    cl_int error;
    cl_context context;
    cl_command_queue commandQueue;
    cl_program kernelProgram;
    cl_kernel kernel;
    cl_mem inputScene;
    cl_mem subSegment;
    OpenCLConfiguration::configure(context, commandQueue, kernelProgram, kernel, kernelName);


    size_t subSegmentSize = (size_t)rowCount * columnCount;
    size_t originalSize = original.getSize();
    size_t originalSizeInBytes = sizeof(int) * originalSize;
    size_t subSegmentSizeInBytes = sizeof(int) * subSegmentSize;
    size_t outputSizeInBytes = outputSize * sizeof(int);
    int* result = new int[outputSize];

    size_t localWorkSize[2], globalWorkSize[2];
    localWorkSize[0] = localWorkSize[1] = 4;
    globalWorkSize[0] = original.getSize() / original.getColumnCount();
    globalWorkSize[1] = original.getColumnCount();

    inputScene = clCreateBuffer(context, CL_MEM_READ_WRITE, originalSizeInBytes, NULL, &error);
    subSegment = clCreateBuffer(context, CL_MEM_READ_WRITE, subSegmentSizeInBytes, NULL, &error);
    OpenCLConfiguration::printIfError(error);

    int* originalData = original.toIntVector();
    error = clEnqueueWriteBuffer(commandQueue, inputScene, CL_TRUE, 0, originalSizeInBytes, originalData, 0, NULL, NULL);
    error = clEnqueueWriteBuffer(commandQueue, subSegment, CL_TRUE, 0, subSegmentSizeInBytes, subSegmentData, 0, NULL, NULL);
    OpenCLConfiguration::printIfError(error);

    int originalColumnCount = original.getColumnCount();
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &inputScene);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &subSegment);
    clSetKernelArg(kernel, 2, sizeof(int), (void*)&originalColumnCount);
    clSetKernelArg(kernel, 3, sizeof(int), (void*)&columnCount);
    clSetKernelArg(kernel, 4, sizeof(int), (void*)&rowCount);
    clSetKernelArg(kernel, 5, sizeof(int), (void*)&positionRow);
    clSetKernelArg(kernel, 6, sizeof(int), (void*)&positionColumn);

    error = clEnqueueNDRangeKernel(commandQueue, kernel, 2, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);
    OpenCLConfiguration::printIfError(error);

    if (getSubSegment)
        error = clEnqueueReadBuffer(commandQueue, subSegment, CL_TRUE, 0, outputSizeInBytes, result, 0, NULL, NULL);
    else
        error = clEnqueueReadBuffer(commandQueue, inputScene, CL_TRUE, 0, outputSizeInBytes, result, 0, NULL, NULL);
    OpenCLConfiguration::printIfError(error);

    clFinish(commandQueue);

    OpenCLConfiguration::releaseMemory(inputScene, subSegment, kernelProgram, kernel, commandQueue, context);
    return result;
}
void ConwayGameOfLifeExecutor::saveImage(const ConwayMatrix& matrix,int iteration) const
{
    std::string fileNameString = "C:\\Users\\Marija\\Desktop\\test\\" + std::to_string(iteration)+".pgm";
    const char* fileName = fileNameString.c_str();
    matrix.writeToImage(fileName);
    emit imageSaved(fileName,iteration);
}
