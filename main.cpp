#include <iostream>
#include <cmath>
#include <chrono>
#include <fstream>
#include <direct.h>
#include "DynamicArray.h"
#include "SortingAlgorithms.h"

void Display(int *array, int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << array[i] << ' ';
    }

    std::cout << std::endl;
}

void Display(int *array, int startIndex, int endIndex)
{
    for (int i = startIndex; i < endIndex; i++)
    {
        std::cout << array[i] << ' ';
    }

    std::cout << std::endl;
}

void FillRandom(int *array, int size, int rangeStart = -100, int rangeEnd = 100)
{
    srand(time(0));
    for (int i = 0; i < size; i++)
    {
        array[i] = rand() % (rangeEnd - rangeStart + 1) + rangeStart;
    }
}

void Fill(int *array, int size, int startValue = 1, int step = 1)
{
    for (int i = 0; i < size; i++)
    {
        array[i] = startValue;
        startValue += step;
    }
}

bool IsSorted(int *array, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        if (array[i] > array[i + 1])
        {
            return false;
        }
    }

    return true;
}

void TestSort(int *array, int size)
{
    int sortedCount = 1;
    for (int i = 0; i < size - 1; i++)
    {
        if (array[i] <= array[i + 1])
        {
            sortedCount++;
        }
        else
        {
            break;
        }
    }

    std::cout << "Array is " << ((double)sortedCount / size * 100) << "% (" << sortedCount << ") sorted." << std::endl;
}

template<typename T, typename U>
struct Pair
{
    T first;
    U second;
};

int main()
{
    DynamicArray<Pair<const char*, void(*)(int*, int)>> algorithms = { Pair<const char*, void(*)(int*, int)>("MergeSort", MergeSort),
                                                                       Pair<const char*, void(*)(int*, int)>("QuickSort", QuickSort),
                                                                       Pair<const char*, void(*)(int*, int)>("IntroSort", IntroSort)};
    DynamicArray<int> arraySizes = {100, 500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000};
    DynamicArray<double> arraySortedPercentages = {0, 25, 50, 75, 95, 99, 99.7};
    int samplesCount = 100;

    for (int i = 0; i < algorithms.GetLength(); i++)
    {
        auto directoryName = algorithms[i].first;
        _mkdir(directoryName);
        for (int j = 0; j < samplesCount; j++)
        {
            std::string filename = std::string(directoryName) + "/sample" + std::to_string(j + 1) + ".csv";
            std::ofstream file(filename);
            file << "Size,Sorted percentage(%),Time(s)" << std::endl;
            for (int k = 0; k < arraySizes.GetLength(); k++)
            {
                int size = arraySizes[k];
                int *array = new int[size];
                for (int l = 0; l < arraySortedPercentages.GetLength(); l++)
                {
                    int sortedCount = (int)(size * arraySortedPercentages[l] / 100);
                    int unsortedCount = size - sortedCount;
                    Fill(array, sortedCount);
                    FillRandom(array + sortedCount, unsortedCount, sortedCount, sortedCount + unsortedCount);

                    auto start = std::chrono::high_resolution_clock::now();
                    algorithms[i].second(array, size);
                    auto end = std::chrono::high_resolution_clock::now();
                    double time = std::chrono::duration<double>(end-start).count();
                    file << size << "," << arraySortedPercentages[l] << "," << time << std::endl;
                }

                Fill(array, size, size, -1);

                auto start = std::chrono::high_resolution_clock::now();
                algorithms[i].second(array, size);
                auto end = std::chrono::high_resolution_clock::now();
                double time = std::chrono::duration<double>(end-start).count();
                file << size << "," << -1 << "," << time << std::endl;

                delete[] array;
            }

            file.close();
        }
    }


    return 0;
}