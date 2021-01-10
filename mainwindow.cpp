#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imagechanger.h"
#include "ConwayMatrix.h"
#include "ConwayGameOfLifeExecutor.h"
#include <thread>
#include <chrono>
#include <QFileDialog>

using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

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
    ui->simulationProgessBar->setValue(0);

    qApp->processEvents();

    QString iterationCountStringValue =  ui->iterationCountTextEdit->toPlainText();
    iterationCount = iterationCountStringValue.toInt();
    std::string fileString = initialFile.toStdString();
    const char* file = fileString.c_str();
    if(file[0] == '\0')
        file = nullptr;
    ConwayGameOfLifeExecutor* executor =  new ConwayGameOfLifeExecutor(iterationCount, file);
    connect(executor,&ConwayGameOfLifeExecutor::imageSaved,this,&MainWindow::changeImage);
    executor->simulate();
    ui->simulateButton->setEnabled(true);
    ui->initalizeFromImageButton->setEnabled(true);
    ui->fileDisplayLabel->setText("");
    this->initialFile = "";
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
