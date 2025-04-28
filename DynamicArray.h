#ifndef PROJEKT1_DYNAMICARRAY_H
#define PROJEKT1_DYNAMICARRAY_H

#include <stdexcept>

const int DefaultCapacity = 4;
const int CapacityMultiplier = 2;

/// \brief Represents a dynamic array
/// \tparam T Type parameter
template<typename T>
class DynamicArray
{
public:
    /// \brief Constructs an empty array with the capacity of 4 elements
    DynamicArray() : DynamicArray<T>(DefaultCapacity)
    {
    }

    /// \brief Constructs an empty array with the capacity of \p capacity elements.
    /// \param capacity Capacity of the array
    explicit DynamicArray(int capacity) : capacity(capacity), length(0)
    {
        this->items = new T[this->capacity];
    }

    /// \brief Constructs an array containing the elements from the \p initializerList
    /// \param initializerList An \a initializer_list containing elements for the array
    DynamicArray(const std::initializer_list<T> &initializerList) : DynamicArray(initializerList.size())
    {
        for (const auto &item: initializerList)
        {
            this->Add(item);
        }
    }

    /// \brief Constructs an \p array deep copy
    /// \param array An array to copy elements from
    DynamicArray(const DynamicArray<T> &array) : capacity(array.capacity), length(array.length)
    {
        this->items = new T[this->capacity];
        Copy(array.items, 0, this->items, 0, array.length);
    }

    /// \brief Destructs the array freeing up the memory used for storing its elements
    ~DynamicArray()
    {
        delete[] this->items;
    }

    /// \brief Returns the current number of elements in the array
    /// \return Number of elements in the array
    int GetLength() const
    {
        return this->length;
    }

    /// \brief Accesses the element at given \p index position in the array
    /// \param index An zero-based index of an array item
    /// \return An element at the given \p index position
    T operator[](int index) const
    {
        CheckIndex(index);
        return this->items[index];
    }

    /// \brief Accesses the element at given \p index position in the array
    /// \param index An zero-based index of an array item
    /// \return An element at the given \p index position
    T &operator[](int index)
    {
        CheckIndex(index);
        return this->items[index];
    }

    /// \brief Performs a deep copying of the \p array and assigns its contents to the current object
    /// \param array An array to copy from
    /// \return A reference to the edited object
    DynamicArray<T> &operator=(const DynamicArray<T> &array)
    {
        if (&array == this)
        {
            return *this;
        }

        if (array.capacity != this->capacity)
        {
            delete[] this->items;
            this->capacity = array.capacity;
            this->items = new T[this->capacity];
        }

        Copy(array.items, 0, this->items, 0, array.length);
        this->length = array.length;
        return *this;
    }

    /// \brief Performs a deep copying of the \p initializerList and assigns its contents to the current object
    /// \param array An \a initializer_list to copy from
    /// \return A reference to the edited object
    DynamicArray<T> &operator=(const std::initializer_list<T> &initializerList)
    {
        if (this->capacity < initializerList.size())
        {
            delete[] this->items;
            if (initializerList.size() % 4 == 0)
            {
                this->capacity = initializerList.size();
            }
            else
            {
                this->capacity = DefaultCapacity;
                while (this->capacity < initializerList.size())
                {
                    this->capacity *= CapacityMultiplier;
                }
            }

            this->items = new T[this->capacity];
        }

        int i = 0;
        for (const auto &item: initializerList)
        {
            this->items[i++] = item;
        }

        this->length = initializerList.size();
        return *this;
    }

    /// \brief Adds a new \p item to the array
    /// \param item An item to add
    void Add(T item)
    {
        if (this->length == this->capacity)
        {
            IncreaseCapacity();
        }

        this->items[this->length++] = item;
    }

    /// \brief Inserts a new \p item to the given \p index of the array
    /// \param index A zero-based index in array, where new \p item should be inserted
    /// \param item An item to insert
    void Insert(int index, T item)
    {
        CheckIndex(index);
        if (this->length == this->capacity)
        {
            this->capacity *= CapacityMultiplier;
            T *newItems = new T[this->capacity];
            if (index != 0)
            {
                Copy(this->items, 0, newItems, 0, index);
            }

            newItems[index] = item;
            Copy(this->items, index, newItems, index + 1, this->length - index);
            delete[] this->items;
            this->items = newItems;
        }
        else
        {
            Copy(this->items, index, index + 1, this->length - index);
            this->items[index] = item;
        }

        this->length++;
    }

    /// \brief Verifies if the \p item is present in the array
    /// \param item An item to search for
    /// \return \a true if the item was found, \a false otherwise
    bool Contains(T item) const
    {
        for (int i = 0; i < this->length; i++)
        {
            if (this->items[i] == item)
            {
                return true;
            }
        }

        return false;
    }

    /// \brief Searches the array for the first occurrence of the given \p item
    /// \param item An item to search for
    /// \return A zero-based index of the first occurrence of \p item in the array if found, -1 otherwise
    int IndexOf(T item) const
    {
        for (int i = 0; i < this->length; i++)
        {
            if (this->items[i] == item)
            {
                return i;
            }
        }

        return -1;
    }

    /// \brief Searches the array for the last occurrence of the given \p item
    /// \param item An item to search for
    /// \return A zero-based index of the last occurrence of \p item in the array if found, -1 otherwise
    int LastIndexOf(T item) const
    {
        for (int i = this->length - 1; i >= 0; i--)
        {
            if (this->items[i] == item)
            {
                return i;
            }
        }

        return -1;
    }

    /// \brief Removes all the items from the array
    void Clear()
    {
        this->length = 0;
    }

    /// \brief Removes an array item at the given \p index
    /// \param index A zero-based index of the element to remove
    void RemoveAt(int index)
    {
        CheckIndex(index);
        if (index == this->length - 1)
        {
            return RemoveLast();
        }

        Copy(this->items, index + 1, index, this->length - index - 1);
        --this->length;
    }

    /// \brief Removes the first item of the array.
    void RemoveFirst()
    {
        return RemoveAt(0);
    }

    /// \brief Removes the last item of the array.
    void RemoveLast()
    {
        this->length--;
    }

private:
    T *items;
    int length;
    int capacity;

    void IncreaseCapacity()
    {
        this->capacity *= CapacityMultiplier; // Mnożymy pojemność przez mnożnik (równy 2)
        T *newItems = new T[this->capacity]; // Allokujemy pamięć dla nowej tablicy wewnętrznej
        Copy(this->items, 0, newItems, 0, this->length); // Kopiujemy elementy starej tablicy do nowej
        delete[] this->items; // Zwalniamy pamięć używają przez starą tablicę
        this->items = newItems; // Przepisujemy wskaźnik nowej tablicy do pola obiektu
    }

    void CheckIndex(int index) const
    {
        if (index < 0 || index >= this->length)
        {
            throw std::invalid_argument("Index is outside of the array");
        }
    }

    static void Copy(T *array, int sourceIndex, int destinationIndex, int count)
    {
        if (sourceIndex == destinationIndex)
        {
            return;
        }

        if (sourceIndex < destinationIndex)
        {
            for (int i = count - 1; i >= 0; i--)
            {
                array[destinationIndex + i] = array[sourceIndex + i];
            }
        }
        else
        {
            for (int i = 0; i < count; i++)
            {
                array[destinationIndex + i] = array[sourceIndex + i];
            }
        }
    }

    static void Copy(T *sourceArray, int sourceIndex, T *destinationArray, int destinationIndex, int count)
    {
        if (sourceArray == destinationArray)
        {
            return Copy(sourceArray, sourceIndex, destinationIndex, count);
        }

        for (int i = 0; i < count; i++)
        {
            destinationArray[destinationIndex + i] = sourceArray[sourceIndex + i];
        }
    }
};

#endif //PROJEKT1_DYNAMICARRAY_H
