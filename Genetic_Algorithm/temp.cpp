#include <iostream>
using namespace std;
class Complex
{
private:
    double real;
    double imaginary;

public:
    Complex(double r = 0.0, double i = 0.0) : real(r), imaginary(i) {}
    Complex operator+(const Complex &other) const
    {
        Complex result;
        result.real = real + other.real;
        result.imaginary = imaginary + other.imaginary;
        cout << "This: " << this->real << " " << this->imaginary << endl;
        cout << "Other: " << other.real << " " << other.imaginary << endl;
        return result;
    }
    void display() const
    {
        std::cout << real << " + " << imaginary << "i" << std::endl;
    }
};
int main()
{
    Complex c1(3.0, 4.0);
    Complex c2(1.5, 2.5);
    Complex sum = c1 + c2;

    sum.display();

    return 0;
}
