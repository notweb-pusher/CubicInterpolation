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
     QString file_path;

private slots:
    void on_plot_button_clicked();
    void on_csv_button_clicked();

private:
    std::vector<std::vector<double>> polys;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
