#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void changeImage(const QString fileName, int iteration);
private slots:
    void on_simulateButton_clicked();

    void on_initalizeFromImageButton_clicked();

private:
    Ui::MainWindow *ui;
    int iterationCount;
    QString initialFile;
};
#endif // MAINWINDOW_H
