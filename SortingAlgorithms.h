#ifndef PROJEKT1_SORTINGALGORITHMS_H
#define PROJEKT1_SORTINGALGORITHMS_H

#include <cmath>

template<typename T>
void Swap(T &first, T &second)
{
    T temp = first;
    first = second;
    second = temp;
}

template<typename T>
void Copy(const T *source, int sourceIndex, T *destination, int destinationIndex, int count)
{
    for (int i = 0; i < count; i++)
    {
        destination[destinationIndex + i] = source[sourceIndex + i];
    }
}

template<typename T>
void BubbleSort(T *array, int size)
{
    bool swapped;
    int i = 0;
    do
    {
        swapped = false;
        for (int j = 0; j < size - i - 1; j++)
        {
            if (array[j] > array[j + 1])
            {
                Swap(array[j], array[j + 1]);
                swapped = true;
            }
        }
    }
    while (swapped && i++ < size);
}

template<typename T>
void InsertionSort(T *array, int size)
{
    for (int i = 1; i < size; i++)
    {
        for (int j = i; j > 0 && array[j - 1] > array[j]; j--)
        {
            Swap(array[j], array[j - 1]);
        }
    }
}

template<typename T>
void SelectionSort(T *array, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < size; j++)
        {
            if (array[j] < array[minIndex])
            {
                minIndex = j;
            }
        }

        if (minIndex != i)
        {
            Swap(array[i], array[minIndex]);
        }
    }
}

template<typename T>
void SiftDown(T *array, int root, int end)
{
    auto GetLeftChildIndex = [](int index)
    { return 2 * index + 1; };
    int child;
    while ((child = GetLeftChildIndex(root)) < end)
    {
        if (child + 1 < end && array[child] < array[child + 1])
        {
            child++;
        }

        if (array[root] < array[child])
        {
            Swap(array[root], array[child]);
            root = child;
        }
        else
        {
            return;
        }
    }
}

template<typename T>
void Heapify(T *array, int size)
{
    auto GetParentIndex = [](int index)
    { return (int)((index - 1) / 2); };
    for (int i = GetParentIndex(size - 1); i >= 0; i--)
    {
        SiftDown(array, i, size);
    }
}

template<typename T>
void HeapSort(T *array, int size)
{
    Heapify(array, size);
    for (int i = size - 1; i > 0; i--)
    {
        Swap(array[0], array[i]);
        SiftDown(array, 0, i);
    }
}

template<typename T>
void Merge(T *array, int startIndex, int middleIndex, int endIndex, T *buffer)
{
    int leftIndex = startIndex;
    int rightIndex = middleIndex;
    for (int i = startIndex; i < endIndex; i++)
    {
        if (leftIndex < middleIndex && (rightIndex >= endIndex || buffer[leftIndex] < buffer[rightIndex]))
        {
            array[i] = buffer[leftIndex++];
        }
        else
        {
            array[i] = buffer[rightIndex++];
        }
    }
}

template<typename T>
void MergeSortHelper(T *array, int startIndex, int endIndex, T *buffer) // [startIndex, endIndex)
{
    if (startIndex >= endIndex - 1)
    {
        return;
    }

    int middleIndex = (startIndex + endIndex) / 2;
    MergeSortHelper(buffer, startIndex, middleIndex, array);
    MergeSortHelper(buffer, middleIndex, endIndex, array);
    Merge(array, startIndex, middleIndex, endIndex, buffer);
}

template<typename T>
void MergeSort(T *array, int size)
{
    T *buffer = new T[size];
    Copy(array, 0, buffer, 0, size);
    MergeSortHelper(array, 0, size, buffer);
    delete[] buffer;
}

template<typename T>
int Partition(T *array, int startIndex, int endIndex)
{
    int midIndex = startIndex + (endIndex - startIndex) / 2;

    // Median-of-three pivot
    if (array[startIndex] > array[midIndex])
        Swap(array[startIndex], array[midIndex]);
    if (array[startIndex] > array[endIndex - 1])
        Swap(array[startIndex], array[endIndex - 1]);
    if (array[midIndex] > array[endIndex - 1])
        Swap(array[midIndex], array[endIndex - 1]);

    // Use median value (now in array[midIndex]) as pivot
    Swap(array[midIndex], array[endIndex - 1]); // Move pivot to end - 1
    T pivot = array[endIndex - 1];

    int pivotIndex = startIndex;
    for (int i = startIndex; i < endIndex - 1; i++)
    {
        if (array[i] <= pivot)
        {
            Swap(array[i], array[pivotIndex++]);
        }
    }

    Swap(array[pivotIndex], array[endIndex - 1]);
    return pivotIndex;
}


template<typename T>
void QuickSortHelper(T *array, int startIndex, int endIndex) // [startIndex, endIndex)
{
    if (startIndex >= endIndex - 1)
    {
        return;
    }

    int pivot = Partition(array, startIndex, endIndex);
    QuickSortHelper(array, startIndex, pivot);
    QuickSortHelper(array, pivot + 1, endIndex);
}

template<typename T>
void QuickSort(T *array, int size)
{
    QuickSortHelper(array, 0, size);
}

template<typename T>
void IntroSortHelper(T *array, int startIndex, int endIndex, int maxDepth)
{
    int size = endIndex - startIndex;
    if (size < 16)
    {
        InsertionSort(array + startIndex, size);
    }
    else if (maxDepth == 0)
    {
        HeapSort(array + startIndex, size);
    }
    else
    {
        int pivot = Partition(array, startIndex, endIndex);
        IntroSortHelper(array, startIndex, pivot, maxDepth - 1);
        IntroSortHelper(array, pivot + 1, endIndex, maxDepth - 1);
    }
}

template<typename T>
void IntroSort(T *array, int size)
{
    if (size <= 1)
    {
        return;
    }

    int maxDepth = (int)log2(size) * 2;
    IntroSortHelper(array, 0, size, maxDepth);
}

#endif //PROJEKT1_SORTINGALGORITHMS_H
