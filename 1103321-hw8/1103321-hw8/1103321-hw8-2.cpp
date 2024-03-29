// Polynomial division provided that the quotient and remainder have integer coefficients

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <iomanip>
using std::setw;

#include <fstream>
using std::istream;
using std::ifstream;
using std::ios;

struct Term
{
    int coef = 0;  // the coefficient of a term
    int expon = 0; // the exponent of a term
};

// Polynomial struct definition
struct Polynomial
{
    int size = 0;          // the number of terms of the polynomial
    Term* terms = nullptr; // all terms of the polynomial
};

// term1 = term2
void assign(Term& term1, const Term& term2);

// polynomial = 0
void reset(Polynomial& polynomial);

// inputs a polynomial from the file Polynomials.dat
void input(istream& inFile, Polynomial& polynomial);

// outputs the specified polynomial
void output(const Polynomial& polynomial);

// returns true if and only if the specified polynomial has at least a zero term
bool hasZeroTerm(const Polynomial& polynomial);

// returns true if and only if polynomial1 == polynomial2
bool equal(const Polynomial& polynomial1, const Polynomial& polynomial2);

// polynomial1 = -polynomial2
void minus(Polynomial& polynomial1, const Polynomial& polynomial2);

// addend += adder
void addition(Polynomial& addend, const Polynomial adder);

// minuend -= subtrahend
void subtraction(Polynomial& minuend, const Polynomial& subtrahend);

// product = multiplicand * multiplier
void multiplication(const Polynomial& multiplicand, const Polynomial& multiplier, Polynomial& product);

// quotient = dividend / divisor; remainder = dividend % divisor
// provided that the degree of dividend >= the degree of divisor
void division(const Polynomial& dividend, const Polynomial& divisor,
    Polynomial& quotient, Polynomial& remainder);

const int numTestCases = 200; // the number of test cases
const int arraySize = 20;

int main()
{
    ifstream inFile("Polynomials.dat", ios::in | ios::binary);

    // exit program if ifstream could not open file
    if (!inFile)
    {
        cout << "File could not be opened" << endl;
        system("pause");
        exit(1);
    }

    Polynomial dividend;
    Polynomial divisor;
    Polynomial quotient;
    Polynomial remainder;
    Polynomial buffer;

    int numErrors = numTestCases;
    for (int i = 0; i < numTestCases; i++)
    {
        // input dividend and divisor from the file Polynomials.dat
        input(inFile, dividend);
        input(inFile, divisor);
        /**/
        cout << "dividend:  ";
        output(dividend);

        cout << " divisor:  ";
        output(divisor);

        // quotient = dividend / divisor; remainder = dividend % divisor
        // thus, dividend == divisor * quotient + remainder
        division(dividend, divisor, quotient, remainder);

        /**/
        cout << "quotient:  ";
        output(quotient);
        cout << endl;


        if (hasZeroTerm(quotient))
            cout << "quotient has at least a zero term!\n";
        else
        {
            // buffer = divisor * quotient
            multiplication(divisor, quotient, buffer);

            if (hasZeroTerm(buffer))
                cout << "buffer has at least a zero term!\n";
            else
            {
                // buffer = buffer + remainder = divisor * quotient + remainder
                addition(buffer, remainder);

/*                for (int n = 0; n < buffer.size; n++)
                    cout << buffer.terms[n].expon << "  ";
                cout << endl;
                for (int n = 0; n < buffer.size; n++)
                    cout << buffer.terms[n].coef << "  ";
                cout << endl;*/

                if (hasZeroTerm(buffer))
                    cout << "buffer has at least a zero term!\n";
                else
                    // if buffer != dividend, an error occurred!
                    if (equal(buffer, dividend))
                        numErrors--;
            }
        }

        reset(dividend);
        reset(divisor);
        reset(quotient);
        reset(remainder);
        reset(buffer);
    }

    inFile.close();

    cout << "There are " << numErrors << " errors!\n\n";

    system("pause");
}

// term1 = term2
void assign(Term& term1, const Term& term2)
{
    term1.coef = term2.coef;
    term1.expon = term2.expon;
}

// polynomial = 0
void reset(Polynomial& polynomial)
{
    polynomial.size = 0;
    if (polynomial.terms != nullptr)
    {
        delete[] polynomial.terms;
        polynomial.terms = nullptr;
    }
}

// inputs a polynomial from the file Polynomials.dat
void input(istream& inFile, Polynomial& polynomial)
{
    int numericString[arraySize] = {};

    inFile.read(reinterpret_cast<char*>(numericString), 80);

    polynomial.size = arraySize;
    while (polynomial.size > 0 && numericString[polynomial.size - 1] == 0)
        polynomial.size--;

    polynomial.terms = new Term[polynomial.size];
    for (int i = 0; i < polynomial.size; ++i)
        polynomial.terms[i].coef = numericString[i];

    inFile.read(reinterpret_cast<char*>(numericString), 80);

    for (int i = 0; i < polynomial.size; ++i)
        polynomial.terms[i].expon = numericString[i];
}

// outputs the specified polynomial
void output(const Polynomial& polynomial)
{
    // the specified polynomial is zero polynomial
    if (polynomial.size == 0)
        cout << 0;
    else
    {
        if (polynomial.terms[0].coef < 0)
            cout << "-" << -polynomial.terms[0].coef;
        else if (polynomial.terms[0].coef > 0)
            cout << polynomial.terms[0].coef;

        if (polynomial.terms[0].expon > 0)
        {
            if (polynomial.terms[0].expon == 1)
                cout << "x";
            else
                cout << "x^" << polynomial.terms[0].expon;
        }

        for (int i = 1; i < polynomial.size; i++)
        {
            if (polynomial.terms[i].coef < 0)
                cout << " - " << -polynomial.terms[i].coef;
            else if (polynomial.terms[i].coef > 0)
                cout << " + " << polynomial.terms[i].coef;

            if (polynomial.terms[i].expon > 0)
            {
                if (polynomial.terms[i].expon == 1)
                    cout << "x";
                else
                    cout << "x^" << polynomial.terms[i].expon;
            }
        }
    }

    cout << endl;
}

// returns true if and only if the specified polynomial has at least a zero term
bool hasZeroTerm(const Polynomial& polynomial)
{
    if (polynomial.size != 0)
        for (int i = 0; i < polynomial.size; i++)
            if (polynomial.terms[i].coef == 0)
                return true;

    return false;
}

// returns true if and only if polynomial1 == polynomial2
bool equal(const Polynomial& polynomial1, const Polynomial& polynomial2)
{
    if (polynomial1.size != polynomial2.size)
        return false;

    for (int i = 0; i < polynomial1.size; i++)
        if (polynomial1.terms[i].coef != polynomial2.terms[i].coef ||
            polynomial1.terms[i].expon != polynomial2.terms[i].expon)
            return false;

    return true;
}

// polynomial1 = -polynomial2
void minus(Polynomial& polynomial1, const Polynomial& polynomial2)
{
    if (polynomial1.size != 0)
        delete[] polynomial1.terms;
    polynomial1.terms = new Term[polynomial2.size];

    polynomial1.size = polynomial2.size;
    for (int i = 0; i < polynomial1.size; i++)
    {
        polynomial1.terms[i].coef = -polynomial2.terms[i].coef;
        polynomial1.terms[i].expon = polynomial2.terms[i].expon;
    }
}

// addend += adder
void addition(Polynomial& addend, const Polynomial adder)
{
    Polynomial sum;
    sum.terms = new Term[arraySize];
    int i = 0;
    int j = 0;
    int k = 0;

/*    for (int n = 0; n < addend.size; n++)
        cout << addend.terms[n].expon << "  ";   
    cout << endl;
    for (int n = 0; n < addend.size; n++)
        cout << addend.terms[n].coef << "  ";   
    cout << endl;
    for (int n = 0; n < adder.size; n++)
        cout << adder.terms[n].expon << "  ";
    cout << endl;
    for (int n = 0; n < adder.size; n++)
        cout << adder.terms[n].coef << "  ";
    cout << endl;*/

    if (addend.size == 0)
    {
        for (; k < adder.size; k++)
        {
            sum.terms[k].coef = adder.terms[k].coef;
            sum.terms[k].expon = adder.terms[k].expon;
        }
    }
    else if (adder.size == 0)
    {
        for (; k < addend.size; k++)
        {
            sum.terms[k].coef = addend.terms[k].coef;
            sum.terms[k].expon = addend.terms[k].expon;
        }
    }
    else
    {
        while (1)
        {
            if (i == addend.size && j == adder.size)
                break;
            if (i == addend.size)
            {
                for (; j < adder.size; j++)
                {
                    sum.terms[k].expon = adder.terms[j].expon;
                    sum.terms[k].coef = adder.terms[j].coef;
                    k++;
                }
                break;
            }

            if (j == adder.size)
            {
                for (; i < addend.size; i++)
                {
                    sum.terms[k].expon = addend.terms[i].expon;
                    sum.terms[k].coef = addend.terms[i].coef;
                    k++;
                }
                break;
            }

            if (addend.terms[i].expon > adder.terms[j].expon)
            {
                sum.terms[k].coef = addend.terms[i].coef;
                sum.terms[k].expon = addend.terms[i].expon;
                i++; k++;
            }
            else if (addend.terms[i].expon < adder.terms[j].expon)
            {
                sum.terms[k].coef = adder.terms[j].coef;
                sum.terms[k].expon = adder.terms[j].expon;
                j++; k++;
            }
            else
            {
                if (addend.terms[i].coef + adder.terms[j].coef == 0)
                {
                    i++; j++;
                    continue;
                }
                sum.terms[k].coef = addend.terms[i].coef + adder.terms[j].coef;
                sum.terms[k].expon = addend.terms[i].expon;
                i++; j++; k++;
            }
        }
    }
/*    for (int n = 0; n < k; n++)
        cout << sum.terms[n].expon << "  ";
    cout << endl;
    for (int n = 0; n < k; n++)
        cout << sum.terms[n].coef << "  ";
    cout << endl;*/

    if (addend.size != k)
    {
        if (addend.size != 0)
            delete[] addend.terms;
        addend.terms = new Term[k];
        addend.size = k;
    }

    for (int i = 0; i < addend.size; i++) // addend = sum
        assign(addend.terms[i], sum.terms[i]);
/*    for (int n = 0; n < k; n++)
    {
        addend.terms[n].expon = sum.terms[n].expon;
        addend.terms[n].coef = sum.terms[n].coef;
    }*/

/*   for (int n = 0; n < addend.size; n++)
        cout << addend.terms[n].expon << "  ";
    cout << endl;
    for (int n = 0; n < addend.size; n++)
        cout << addend.terms[n].coef << "  ";
    cout << endl;*/

    delete[] sum.terms;

    if (addend.size > 0 && addend.terms[0].coef == 0)
        cout << "Leading zeroes in addend not allowed!\n";
}

// minuend -= subtrahend
void subtraction(Polynomial& minuend, const Polynomial& subtrahend)
{
    Polynomial buffer;
    buffer.size = subtrahend.size;
    buffer.terms = new Term[buffer.size]();

    // buffer = -subtrahend
    minus(buffer, subtrahend);

    // minuend = minuend + buffer = minuend - subtrahend
    addition(minuend, buffer);

    delete[] buffer.terms;

    if (minuend.size > 0 && minuend.terms[0].coef == 0)
        cout << "Leading zeroes in minuend not allowed!\n";
}

// product = multiplicand * multiplier
void multiplication(const Polynomial& multiplicand, const Polynomial& multiplier, Polynomial& product)
{
    // product = 0;
    if (product.size != 0)
    {
        product.size = 0;
        delete[] product.terms;
    }

    Polynomial buffer;
    buffer.size = multiplicand.size;
    buffer.terms = new Term[buffer.size];

    // if( multiplicand != 0 && multiplier != 0 )
    if (multiplicand.size != 0 && multiplier.size != 0)
    {
        for (int j = 0; j < multiplier.size; j++)
        {
            for (int i = 0; i < multiplicand.size; i++)
            {
                buffer.terms[i].expon = multiplicand.terms[i].expon + multiplier.terms[j].expon;
                buffer.terms[i].coef = multiplicand.terms[i].coef * multiplier.terms[j].coef;
            }

            addition(product, buffer);

/*            for (int i = 0; i < product.size; i++)
            {
                cout << product.terms[i].expon << "  ";
            }
            cout << endl;
            for (int i = 0; i < product.size; i++)
            {
                cout << product.terms[i].coef << "  ";
            }
            cout << endl;*/
        }
    }

    if (product.size > 0 && product.terms[0].coef == 0)
        cout << "Leading zeroes in product not allowed!\n";
}

// quotient = dividend / divisor; remainder = dividend % divisor
// provided that the degree of dividend >= the degree of divisor
void division(const Polynomial& dividend, const Polynomial& divisor,
    Polynomial& quotient, Polynomial& remainder)
{
    // remainder = dividend
    remainder.size = dividend.size;
    remainder.terms = new Term[remainder.size]();
    for (int i = 0; i < remainder.size; i++)
        assign(remainder.terms[i], dividend.terms[i]);

    Polynomial monomial;
    monomial.size = 1;
    monomial.terms = new Term[1]();

    Polynomial buffer;

    Polynomial temp;
    temp.size = 0;
    temp.terms = new Term[arraySize]();

    while (remainder.size != 0 && remainder.terms[0].expon >= divisor.terms[0].expon)
    {
        temp.terms[temp.size].coef = remainder.terms[0].coef / divisor.terms[0].coef;
        temp.terms[temp.size].expon = remainder.terms[0].expon - divisor.terms[0].expon;

/*        cout << temp.terms[quotient.size].expon << endl;
        cout << temp.terms[quotient.size].coef << endl;*/

        monomial.terms[0].coef = temp.terms[temp.size].coef;
        monomial.terms[0].expon = temp.terms[temp.size].expon;

        multiplication(divisor, monomial, buffer);

        subtraction(remainder, buffer);

        temp.size++;
    }


    // quotient = temp
    if (quotient.size != 0)
        delete[] quotient.terms;
    quotient.size = temp.size;
    quotient.terms = new Term[quotient.size];

    for (int i = 0; i < quotient.size; ++i)
        assign(quotient.terms[i], temp.terms[i]);

/*    for (int n = 0; n < temp.size; n++)
        cout << quotient.terms[n].expon << "  ";
    cout << endl;
    for (int n = 0; n < temp.size; n++)
        cout << quotient.terms[n].coef << "  ";
    cout << endl;*/

    delete[] monomial.terms;
    delete[] buffer.terms;
    delete[] temp.terms;

    if (quotient.size > 0 && quotient.terms[0].coef == 0)
        cout << "Leading zeroes in quotient not allowed!\n";

    if (remainder.size > 0 && remainder.terms[0].coef == 0)
        cout << "Leading zeroes in remainder not allowed!\n";
}