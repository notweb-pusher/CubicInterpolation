#include <cstdio>
#include <exception>
#include <iostream>
#include <vector>
#include <fstream>
typedef std::vector<std::pair<double, double>> dots_vec;
typedef std::vector<double> double_vec;


struct Poly
{
private:
    double a,b,c,d;
public:
    double from, to;
    Poly(double a,double b,double c, double d,double from,double to):a(a), b(b), c(c), d(d), from(from), to(to){}
    double get_value_forx(double x)
    {
        double dx = x - from;
        return (a*dx*dx*dx + b*dx*dx + c*dx + d);
    }
    void add_dots(std::vector<double>& x, std::vector<double>& y, double step)
    {
        for (double x0 = from; x0 <= to; x0+=step)
        {
            x.push_back(x0);
            y.push_back(get_value_forx(x0));
        }

    }
};

// we are solving A*x = B
// values (right matrix) -> A
std::vector<double_vec> get_matrix(dots_vec& dots)
{
    std::vector<double_vec> matrix;
    for (int i = 0; i < (dots.size() - 2); i++)
    {
        double_vec ith_row(dots.size() - 3, 0.0);
        ith_row.insert(ith_row.begin()+i, (dots[i+1].first - dots[i].first));
        ith_row.insert(ith_row.begin()+i+1, 2*(dots[i+2].first - dots[i].first));
        ith_row.insert(ith_row.begin()+i+2, (dots[i+2].first - dots[i+1].first));
        ith_row.erase(ith_row.begin());
        ith_row.erase(ith_row.end()-1);
        matrix.push_back(ith_row);
    }
    return matrix;
}

// answers = 6 * (y[i+1] - y[i])/(x[i+1] - x[i]) - 6 * (y[i] - y[i-1])/(x[i] - x[i-1]) -> B
double_vec get_answers(dots_vec& dots)
{
    double_vec answers;

    for (int i = 0; i < (dots.size() - 2); i++)
    {
        answers.push_back(6*(((dots[i+2].second - dots[i+1].second)/(dots[i+2].first - dots[i+1].first)) -
        ((dots[i+1].second - dots[i].second)/(dots[i+1].first - dots[i].first))));
    }

    return answers;
}

void get_rref(std::vector<double_vec>& A, double_vec& B)
{


    for (int i = 0; i <  B.size(); i++)
    {
        for(int j = 0; j < B.size(); j++)
        {
            if (A[j][i] != 0 and j != i)
            {
                B[j] -= A[j][i]/A[i][i] * B[i];
                double mult = A[j][i]/A[i][i];
                for (int c = 0; c < A[0].size(); ++c)
                {
                    A[j][c] -= mult * A[i][c];
                }
            }
        }
    }
}

std::vector<std::pair<double,double>> read_csv(std::ifstream& file)
{
    std::vector<std::pair<double, double>> dots;
    std::string n;
    while(std::getline(file, n, ';'))
    {
        try
        {
            std::pair<double, double> a;
            a.first = std::stod(n);
            std::getline(file, n, ';');
            a.second = std::stod(n);
            dots.push_back(a);
        }
        catch (std::exception)
        {
            break;
        }
    }
    std::sort(dots.begin(), dots.end());
    return dots;
}

std::vector<Poly> get_polys(std::vector<double_vec>& A, double_vec& B, std::vector<std::pair<double, double>>& dots)
{
    double_vec S_coeff;
    std::vector<Poly> answer;
    S_coeff.push_back(0);
    for(int i = 0; i < B.size(); i++)
    {
        S_coeff.push_back((B[i]/A[i][i]));
    }
    S_coeff.push_back(0);

    for(int ind = 0; ind < S_coeff.size() - 1; ind++)
    {
        double dh = (dots[ind+1].first - dots[ind].first);
        double a = (S_coeff[ind+1] - S_coeff[ind])/(6*dh);
        double b = S_coeff[ind]/2;
        double c = (dots[ind+1].second - dots[ind].second)/dh - (2*dh*S_coeff[ind] + dh*S_coeff[ind+1])/6;
        double d = dots[ind].second;
        Poly polynom(a,b,c,d, dots[ind].first, dots[ind+1].first);
        answer.push_back(polynom);
    }
    return answer;
}

