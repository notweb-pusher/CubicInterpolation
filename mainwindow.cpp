#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <Interpolation.cpp>
#include <QVector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_plot_button_clicked()
{
    QVector<double> x, y;
    std::ifstream vhodnoipotok(this->file_path.toStdString());
    std::vector<std::pair<double, double>> dots = read_csv(vhodnoipotok);
    std::vector<double_vec> matrix = get_matrix(dots);
    double_vec answers = get_answers(dots);
    get_rref(matrix, answers);
    std::vector<Poly> polinomi = get_polys(matrix, answers, dots);
    double step = 0.01;
    for (auto el:polinomi)
    {
        for (double x0 = el.from; x0 < el.to; x0+=step)
        {
            x.push_back(x0);
            y.push_back(el.get_value_forx(x0));
        }
    }

    ui->customplot->addGraph();
    ui->customplot->graph(0)->setData(x,y);
    // give the axes some labels:
    ui->customplot->xAxis->setLabel("x");
    ui->customplot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    auto x_scale = std::minmax_element(x.begin(), x.end());
    auto y_scale = std::minmax_element(y.begin(), y.end());

    ui->customplot->xAxis->setRange(*x_scale.first - 1,
                                            *x_scale.second + 1);
    ui->customplot->yAxis->setRange(*y_scale.first - 1,
                                            *y_scale.second + 1);
    ui->customplot->replot();
}

void MainWindow::on_csv_button_clicked()
{
    QFileInfo full_path(
         QFileDialog::getOpenFileName(this, tr("Open file with points")));
    this->file_path = full_path.filePath();
}
