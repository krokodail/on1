#include <iostream>
#include <thread>
#include <vector>
#include <numeric> // для accumulate
#include <random> // для рандома

std::random_device rd;
std::mt19937 gen(rd());

const int range_min = 1; // мин.знач. для рандома
const int range_max = 100; // макс. знач. для рандома

int* generateRandomArray(int size) {
    int* array = new int[size];
    std::uniform_int_distribution<> dis(range_min, range_max);

    for (int i = 0; i < size; ++i) {
        array[i] = dis(gen);
	std::cout << array[i] << " ";
    }
    std::cout << std::endl;
    return array;
}

void partialSum(int* array, int start, int end, int &result) {
    int sum = 0;
    for (int i = start; i < end; ++i) {
        sum += array[i];
    }
    result = sum;
}

int main() {
    int array_size, num_pieces;

    std::cout << "Размер массива: ";
    std::cin >> array_size;

    std::cout << "Количество частей: ";
    std::cin >> num_pieces;

    int* array = generateRandomArray(array_size);

    // для хранения промежуточных результатов
    std::vector<int> partial_results(num_pieces);

    // создание потоков и вычисление целой и дробной части
    std::vector<std::thread> threads;
    int chunk_size = array_size / num_pieces;
    int remainder = array_size % num_pieces;

    int start = 0;
    for (int i = 0; i < num_pieces; ++i) {
        int end = start + chunk_size + (i < remainder ? 1 : 0);
        threads.emplace_back(partialSum, array, start, end, std::ref(partial_results[i]));
        start = end;
    }

    // Ждем пока все потоки закончат
    for (std::thread &t : threads) {
        t.join();
    }

    // вычисляем конечную сумму
    int total_sum = std::accumulate(partial_results.begin(), partial_results.end(), 0);

    std::cout << "Результат: " << total_sum << std::endl;

    delete[] array;

    return 0;
}

