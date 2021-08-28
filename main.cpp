// ----------------------------------------------------- //
// Excercise to gain insight into the use of fixed point // 
// arithemtic in C++                                     //
// Credits: https://www.youtube.com/watch?v=S12qx1DwjVk  //
// https://www.youtube.com/watch?v=npQF28g6s_k           //
// ----------------------------------------------------- //
#include <iostream>

// ===================================================== //
// MEASURING TIME FOR EXECUTION
#include <chrono>
using namespace std::chrono;
// ===================================================== //

using namespace std;

// . is the radix
//                          Precision
// 0110 0100 0101 0101 . 0101 0101 0100 1011
// Boolean and to get only the decimal
// 0000 0000 0000 0000 . 1111 1111 1111 1111
// Whole Mask
// 1111 1111 1111 1111 . 0000 0000 0000 0000

const int scale = 16; // 1/2^16

// The fraction mask is used to get only the decimal
const int FractionMask = 0xffffffff >> (32 - scale);

// The integer part of a fixed point part of a number
const int WholeMask = -1 ^ FractionMask;

// Functions declared using macros
#define DoubleToFixed(x) ((x)*(double)(1<<scale))
#define FixedToDouble(x) ((double)(x)/(double)(1<<scale))
#define IntToFixed(x) ((x) << scale)
#define FixedToInt(x) ((x) >> scale)

#define FractionPart(x) ((x) & FractionMask)
#define WholePart(x) ((x) & WholeMask)

// --- Mulitplication --- //
// Option 1 is to make it 64 bits long - really really slow
// #define MUL(x, y) (((long long)(x)*(long long)(y)) >> scale)

// Option 2 (faster) - shifted init values by 8
// Essential shifting 1s to the precision so that no overflow occurs
#define MUL(x, y) ((((x)>>8)*((y)>>8))>>0)
// 0 shift at the end is the final shift that will result in the correct size scale
// i.e. (8+8 = 16 then need >> 0) (4+4=8 then need >> 8)

// --- Division --- //
// Option 1: Slow, in 64 bits
// #define DIV(x, y) (((long long)(x)<<16)/(y))

// Option 2: Faster
#define DIV(x, y) ((((x)<<8)/(y)<<8)<<0)
// 0 shift at the end is the final shift that will result in the correct size scale
// i.e. (8+8 = 16 then need >> 0) (4+4=8 then need >> 8)


int main() {

    cout.precision(20);

    // ===================================================== //
    auto start = high_resolution_clock::now();
    // ===================================================== //
    
    // Example 1 - Testing Double to Fixed
    int f = DoubleToFixed(6.4);    
    
    cout << "Double to Fixed " << FixedToDouble(f)<<endl;
    cout << "Actual result from doub;le "<< (double)6.4 <<endl;

    // Example 2 - Testing Int to Fixed
    f = IntToFixed(6);    
    
    cout << "Int to Fixed "<<FixedToDouble(f)<<endl;


    // Example 3 - Addition of 2 fixed point numbers
    int f1 = DoubleToFixed(3.8);
    int f2 = DoubleToFixed(4.5);

    f1 += f2;
   
    cout << "Addition: " << FixedToDouble(f1) << endl;

    // Example 4 - Subtraction of 2 fixed point numbers
    f1 = DoubleToFixed(5.6);
    f2 = DoubleToFixed(7.9);

    f1 -= f2;
   
    cout << "Subtraction: " << FixedToDouble(f1) << endl;

    // Example 5 - epsilon
    // epsilon is the smnallest possible increment or decresment
    // that can be used on a fractional number system
    int epsilon = 1;

    // f1++ is equivelant to f1+=epsilon
    cout << "Epsilon "<< FixedToDouble(epsilon) << endl;


    // Example 5: Using fractional mask to get only the fraction part of a number(mod)
    f1 = DoubleToFixed(5.6);

    cout << "Fractional Part: " <<FixedToDouble( FractionPart(f1) )<<endl; 

    // Example 6: Using Whole mask to get only the whole part of a number
    f1 = DoubleToFixed(5.6);

    cout << "Fractional Part: " <<FixedToDouble( WholePart(f1) )<<endl; 

    // Example 7: Multiplication and division
    f = DoubleToFixed(7.0);

    // Doubling F - Shifting left
    f <<= 1;
    // Halving F - Shifting Right
    f >>= 1;

    cout << "Fractional Part: " << FixedToDouble(f)<<endl; 

    // Example 8 - Multiplying 2 numbers together
    int a = DoubleToFixed(7.0);
    int b = DoubleToFixed(3.0);

    int res = MUL(a,b);

    cout << "Result of multiplication is "<< FixedToDouble(res) << endl;

    // Example 9 - Dividing one number by another 
    a = DoubleToFixed(5.8);
    b = DoubleToFixed(4.1);

    res = DIV(a,b);

    cout << "Result of Division is " << FixedToDouble(res) << endl; 
    
    // ===================================================== //
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << duration.count() << " microseconds" << endl;
    // ===================================================== //
    return 0;
}