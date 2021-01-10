#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imagechanger.h"
#include "ConwayMatrix.h"
#include "ConwayGameOfLifeExecutor.h"
#include <thread>
#include <chrono>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString safe= "QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #5500ff,stop: 0.4999 #46a,stop: 0.5 #45a,stop: 1 #238 );}";
    ui->simulationProgessBar->setStyleSheet(safe);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeImage(const QString filename, int iteration){
    QImage image(filename);
    image.scaled(600,600);
    QPixmap pixmap = QPixmap::fromImage(image);
    pixmap = pixmap.scaled(600,600);
    ui->currentIterationImage->setPixmap(pixmap);
    ui->simulationProgessBar->setValue(((double)(iteration+1)/(iterationCount))*100);
    qApp->processEvents();
}

void MainWindow::on_simulateButton_clicked()
{
    ui->simulateButton->setEnabled(false);
    ui->initalizeFromImageButton->setEnabled(false);
    ui->addSubSegmentButton->setEnabled(false);
    ui->simulationProgessBar->setValue(0);

    qApp->processEvents();

    QString iterationCountStringValue =  ui->iterationCountTextEdit->toPlainText();
    iterationCount = iterationCountStringValue.toInt();
    // Configure from file
    std::string fileString = initialFile.toStdString();
    const char* file = fileString.c_str();
    if(file[0] == '\0')
        file = nullptr;
    // Add one segment
    ConwayGameOfLifeExecutor* executor =  new ConwayGameOfLifeExecutor(iterationCount, file);
    if(segmentToAdd)
        executor->addSubSegment(*segmentToAdd);
    else{
        ui->subSegmentLabel->setText("");
        ui->width->clear();
        ui->height->clear();
        ui->positionX->clear();
        ui->positionY->clear();
    }
    connect(executor,&ConwayGameOfLifeExecutor::imageSaved,this,&MainWindow::changeImage);
    executor->simulate();
    ui->simulateButton->setEnabled(true);
    ui->initalizeFromImageButton->setEnabled(true);
    ui->addSubSegmentButton->setEnabled(true);
    ui->fileDisplayLabel->clear();
    this->initialFile = "";
    this->segmentToAdd = nullptr;
}

void MainWindow::on_initalizeFromImageButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("Images (*.pgm *.ppm)"));
    QStringList fileNames;
    if (dialog.exec())
        fileNames = dialog.selectedFiles();
    this->initialFile = fileNames[0];
    ui->fileDisplayLabel->setText(fileNames[0]);
}


void MainWindow::on_addSubSegmentButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("Images (*.pgm *.ppm)"));
    QStringList fileNames;
    if (dialog.exec())
        fileNames = dialog.selectedFiles();
    QString fileNameQString = fileNames[0];
    std::string fileString = fileNameQString.toStdString();
    const char* subSegmentFileName = fileString.c_str();

    segmentToAdd = new ConwayGameOfLifeExecutor::SubSegment();
    segmentToAdd->subSegmentFilePath = subSegmentFileName;
    segmentToAdd->positionX =  ui->positionX->toPlainText().toInt();
    segmentToAdd->positionY =  ui->positionY->toPlainText().toInt();
    segmentToAdd->rowCount = ui->height->toPlainText().toInt();
    segmentToAdd->columnCount = ui->height->toPlainText().toInt();

    QImage image(fileNames[0]);
    image.scaled(100,100);
    QPixmap pixmap = QPixmap::fromImage(image);
    pixmap = pixmap.scaled(100,100);
    ui->subSegmentLabel->setPixmap(pixmap);
    ui->addSubSegmentButton->setEnabled(false);
}
