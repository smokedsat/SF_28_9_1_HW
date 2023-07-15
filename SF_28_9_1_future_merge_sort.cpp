#include <future>
#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>

void merge(std::vector<int> & arr, int l, int m, int r)
{
    int nl = m - l + 1;
    int nr = r - m;

    // Создаем временные массивы
    int left[nl], right[nr];

    // копируем данные во временные массивы
    for(int i = 0; i < nl; i++)
    {
        left[i] = arr[l + i];
    }

    for(int j = 0; j < nr; j++)
    {
        right[j] = arr[m + 1 + j];
    }

    int i =0, j = 0;
    int k = l; // Начало левой части

    while(i < nl && j < nr)
    {
        // Записываем минимальные элементы обратно во входной массив
        if(left[i] <= right[j])
        {
            arr[k] = left[i];
            i++;
        }
        else
        {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    // Записываем оставшиеся элементы левой части
    while(i < nl)
    {
        arr[k] = left[i];
        i++;
        k++;
    }

    // Записываем оставшиеся элементы правой части
    while(j < nr)
    {
        arr[k] = right[j];
        j++;
        k++;
    }
}

void mergeSortParallel(std::vector<int> &arr, int l, int r)
{
    if(l >= r)
    {
        return;
    }

    const int threshold = 100; // Пороговое значение для последовательной сортировки

    if( r - l < threshold)
    {
        std::sort(arr.begin() + l, arr.begin() + r + 1);
        return;
    }

   
    int m = l + (r - l) / 2;

    std::future<void> leftFuture = std::async(std::launch::async, mergeSortParallel, std::ref(arr),l,m);
    std::future<void> rightFuture = std::async(std::launch::async, mergeSortParallel, std::ref(arr), m + 1, r);

    leftFuture.wait(); 
    rightFuture.wait();

    merge(arr, l, m, r);

}


void mergerSort(std::vector<int> & arr, int l, int r)
{
    if(l >= r)
    {
        return;
    }

    int m = (l + r - 1) / 2;

    mergerSort(arr, l, m);

    mergerSort(arr, m + 1, r);

    merge(arr, l, m, r);
}



int main()
{
    std::vector<int> arr;

    for(auto i =0 ; i < 1000000; i++)
    {
        arr.push_back(i%10000);
    }

    std::cout << "Unsorted: " << std::endl;
    for(auto & it : arr)
    {
        std::cout << " "<< it; 
    }

    std::cout << std::endl;

    mergeSortParallel(arr, 0, arr.size() - 1);


    std::cout << "Sorted: " << std::endl;
    for(auto & it : arr)
    {
        std::cout << " "<< it; 
    }

    std::cout << std::endl;

    return 0;
}