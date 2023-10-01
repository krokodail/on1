#include <iostream>
#include <vector>
#include <algorithm>
#include <future>
#include <thread>
#include <mutex>
#include <random>

const int MIN_PARTITION_SIZE = 100000;

template <typename T>
void quicksort(std::vector<T>& array, int left, int right);

template <typename T>
void parallel_quicksort(std::vector<T>& array, int left, int right, std::mutex& mutex);

template <typename T>
std::future<void> parallel_quicksort_async(std::vector<T>& array, int left, int right, std::mutex& mutex);

template <typename T>
void merge(std::vector<T>& array, int left, int middle, int right);




template <typename T>
void displayVector(const std::vector<T>& data) {
    for (const T& element : data) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}


std::vector<int> generateRandomData(int size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 1000000);

    std::vector<int> data(size);
    for (int i = 0; i < size; ++i) {
        data[i] = dist(gen);
    }

    return data;
}




int main() {
    std::vector<int> data = generateRandomData(100);//задает размер вектора
    std::mutex mutex;

    std::cout << "Перед сортировкой:" << std::endl;
    displayVector(data);

    auto result = parallel_quicksort_async(data, 0, data.size() - 1, mutex);

    // Ждем окончания сортировки
    result.get();
    
    std::cout << std::endl;

    std::cout << "После сортировки:" << std::endl;
    
    displayVector(data);

    return 0;
}




template <typename T>
void quicksort(std::vector<T>& array, int left, int right) {
    if (left < right) {
        int pivot_index = left + (right - left) / 2;
        T pivot_value = array[pivot_index];

        int i = left;
        int j = right;

        while (i <= j) {
            while (array[i] < pivot_value) i++;
            while (array[j] > pivot_value) j--;

            if (i <= j) {
                std::swap(array[i], array[j]);
                i++;
                j--;
            }
        }

        if (left < j) quicksort(array, left, j);
        if (i < right) quicksort(array, i, right);
    }
}

template <typename T>
void parallel_quicksort(std::vector<T>& array, int left, int right, std::mutex& mutex) {
    if (left < right) {
        if (right - left < MIN_PARTITION_SIZE) {
            quicksort(array, left, right);
        } else {
            int pivot_index = left + (right - left) / 2;
            T pivot_value = array[pivot_index];

            int i = left;
            int j = right;

            while (i <= j) {
                while (array[i] < pivot_value) i++;
                while (array[j] > pivot_value) j--;

                if (i <= j) {
                    std::swap(array[i], array[j]);
                    i++;
                    j--;
                }
            }

            std::unique_lock<std::mutex> lock(mutex);

            auto left_future = std::async(std::launch::async, [&array, left, j, &mutex]() {
                parallel_quicksort(array, left, j, mutex);
            });

            lock.unlock();

            if (i < right) {
                parallel_quicksort(array, i, right, mutex);
            }

            left_future.wait(); // ждем завершения левой стороны

            merge(array, left, j, right);
        }
    }
}

template <typename T>
std::future<void> parallel_quicksort_async(std::vector<T>& array, int left, int right, std::mutex& mutex) {
    return std::async(std::launch::async, [&array, left, right, &mutex]() {
        parallel_quicksort(array, left, right, mutex);
    });
}

template <typename T>
void merge(std::vector<T>& array, int left, int middle, int right) {
    std::vector<T> left_part(array.begin() + left, array.begin() + middle + 1);
    std::vector<T> right_part(array.begin() + middle + 1, array.begin() + right + 1);

    int i = 0, j = 0, k = left;

    while (i < left_part.size() && j < right_part.size()) {
        if (left_part[i] <= right_part[j]) {
            array[k] = left_part[i];
            i++;
        } else {
            array[k] = right_part[j];
            j++;
        }
        k++;
    }

    while (i < left_part.size()) {
        array[k] = left_part[i];
        i++;
        k++;
    }

    while (j < right_part.size()) {
        array[k] = right_part[j];
        j++;
        k++;
    }
}
