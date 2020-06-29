/*
 * Filename   : RadixSort.cc
 * Author     : Ryan Haas
 * Course     : CSCI 362-01
 * Assignment : Program #5
 * Description: Write a program to compare the radix sorting algorithm
                against the sorting algorithm that the C++ Standard
                Library provides by timing how long each takes.
 */

/****************************************************/
// System includes

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <queue>
#include <math.h>
#include <iomanip>

/****************************************************/
// Local includes

#include "Timer.hpp"

/****************************************************/
// Using declarations

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::minstd_rand;
using std::sort;
using std::queue;
using std::uniform_int_distribution;
using std::setprecision;
using std::setw;

/****************************************************/
// Function prototypes/global vars/type definitions

size_t
promptSize();

int
promptDigits();

vector<int>
generateVector(size_t& size, int& digits);

void
radixSort(vector<int>& v, int maxDigits);

void
printVector(vector<int>& v);

void
printOutput(const double radixTime, const double stdTime, const bool sorted);

void
performAverages();

/****************************************************/

int
main(int argc, char* argv[])
{
    // Get the input values from user
    size_t size = promptSize();
    // Get the max number of digits from the user
    int digits = promptDigits();

    cout << endl;

    // Generate a vector from the given input
    vector<int> A = generateVector(size, digits);
    vector<int> ACopy(A);

    // Timer for timing the sorts
    Timer<> timer;

    // Time the radix sort
    timer.start();
    radixSort(A, digits);
    timer.stop();
    double radixTime = timer.getElapsedMs();

    // Time the standard library's sort
    timer.start();
    sort(ACopy.begin(), ACopy.end());
    timer.stop();
    double stdTime = timer.getElapsedMs();

    // Neatly print out the information
    printOutput(radixTime, stdTime, A == ACopy);

    return EXIT_SUCCESS;
}

/****************************************************/

// Prompt the user for the size of tinthe vector and return that value
size_t
promptSize()
{
    size_t size;
    cout << "N ==> ";
    cin >> size;
    return size;
}

/****************************************************/

// Prompt the user for the maximum number of digits any number should have
int
promptDigits()
{
    int digits;
    cout << "d ==> ";
    cin >> digits;
    return digits;
}

/****************************************************/

// Create and return of vector of size 'size' filled with numbers with no
//      more than 'digits' number of digits using a uniform int distribution
vector<int>
generateVector(size_t& size, int& digits)
{
    // Allocate the vector
    vector<int> vect(size);

    // Setup random generation
    minstd_rand rand;
    rand.seed(0);

    //default_random_engine generator(rand);
    uniform_int_distribution<int> distribution(0, pow(10, digits) - 1);
    for(size_t i = 0; i < size; ++i)
        vect[i] = distribution(rand);

    return vect;
}

/****************************************************/

// Sort the given vector using a radix sort
void
radixSort(vector<int>& v, int maxDigits)
{
    // Create a vector of queues for each digit
    vector<queue<int>> bins(10);

    // Variable used to determine how much to divide the number by to
    //      get to the proper digit location
    int loc = 1;

    // Loop 'maxDigits' times
    while(maxDigits > 0)
    {
        // For every element in the vector, remove it and place it
        //      in the proper bin
        for(const int& elem : v)
        {
            // Calculate the index by dividing the number enough times so
            //      the last digit is the digit we want, and then break it off
            int index = (elem / loc) % 10;
            bins[index].push(elem);
        }

        // Empty the vector
        v.clear();

        // Go through every bin and insert values back into the vector
        for(queue<int>& bin : bins)
        {
            // Empty each bin into the vector
            while(!bin.empty())
            {
                v.push_back(bin.front());
                bin.pop();
            }
        }

        // Decrease the digit count
        --maxDigits;
        // Multiply the location by 10 to move to the next digit
        loc *= 10;
    }
}

/****************************************************/

// Print the array
void
printVector(vector<int>& v)
{
    cout << "[ ";
    for(const auto& elem : v)
        cout << elem << " ";
    cout << "]" << endl;
}

/****************************************************/

// Print out the results
void
printOutput(const double radixTime, const double stdTime, const bool sorted)
{
    cout << "Radix time:     " << setprecision(4) << radixTime << "ms" << endl;
    cout << "std::sort time: " << setprecision(4) << stdTime << "ms" << endl << endl;
    cout << "Sorted? " << (sorted ? "yes" : "no") << endl;
}

/****************************************************/

// // Function to get out and print the averages of the functions
// void
// performAverages()
// {
//     // To add spacing at the top
//     cout << endl << endl << endl;
//
//     // Print heading information
//     printf("%-5s %-7s %-7s %-7s %-10s\n", "N", "d=2", "d=4", "d=8", "std (d=8)");
//     printf("%-5s %7s %7s %7s %10s\n", "=====", "======", "======", "======", "==========");
//
//     // How many times to run the sorting algorithm
//     const int timesToRun = 5;
//
//     // Timer to time each operation
//     Timer<> timer;
//
//     // Size of array to create
//     size_t N = 100 * pow(10, 6);
//
//     for(int i = 0; i < 3; ++i)
//     {
//         // The max number of digits (will be multiplied by 2 each time)
//         int d = 2;
//
//         // Array of avg times for each d value
//         double avgTimes[3];
//
//         // Get average times for each d value
//         for(double& time : avgTimes)
//         {
//             // The total time it takes to run the sorting algorithm N times
//             double totalTime = 0;
//
//             // Perform radixSort 'timesToRun' times to get an average
//             // This is just so the compiler makes an executable that will utilize
//             //      multiple cores which can make the code finish executing faster
//             //      because using only 1 core for all these calculations on so much
//             //      data takes quite a while
//             #pragma omp parallel for
//             for(int i = 0; i <  timesToRun; ++i)
//             {
//                 vector<int> A = generateVector(N, d);
//                 timer.start();
//                 radixSort(A, d);
//                 timer.stop();
//                 totalTime += timer.getElapsedMs();
//             }
//
//             time = totalTime / timesToRun;
//             d *= 2;
//         }
//
//         // Do the same for std::sort, except only for d=8 which d should already be 8
//         double totalTime = 0;
//         #pragma omp parallel for
//         for(int i = 0; i <  timesToRun; ++i)
//         {
//             vector<int> A = generateVector(N, d);
//             timer.start();
//             sort(A.begin(), A.end());
//             timer.stop();
//             totalTime += timer.getElapsedMs();
//         }
//         double stdTime = totalTime / timesToRun;
//
//         // Print the info
//         cout << setprecision(5) << N << " " << setprecision(6) << avgTimes[0] << " " << avgTimes[1] << " " << avgTimes[2] << " " << stdTime << endl;
//
//         // Double N
//         N *= 2;
//     }
// }

/****************************************************/

/*

N      d=2      d=4        d=8       std (d=8)
=====  =======  ========   =======   ==========
100e6  2289.70  3238.51   6404.34   7791.39
200e6  3350.33  6502.20   12881.70  16206.40
400e6  6816.44  12888.30  25589.10  33678.70

Discussion
==========
Given that radix sort's complexity is O(d * N), the times obtained above
are definitely consistent with that complexity. For all 3 different Ns, it can
be easily seen that a d of 4 took approximately 2x as long as a d of 2, and that
a d of 8 took approximately 2x as long as a d of 4. It can also be seen if you keep
d constant and change just N instead. For all columns of d's, the N of 200e6 took
approximately half the time of N of 400e6.

As the N sizes got larger radix sort began doing much better than std::sort. When
N was lower, at 100e6, they performed very close to the same time with radix sort
only being one second faster. However by the time N was 400e6 radix sort was almost
10 seconds faster than std::sort indicating that radix sort performs much better
on larger sizes than std::sort. It is likely much faster because there aren't any
comparisons happening. Radix sort does all of it's shuffling of data through
calculations as opposed to comparisons which end up being much faster.

*/
