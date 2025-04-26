#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Функция для генерации случайного имени или фамилии
string generateRandomName(int minLength = 3, int maxLength = 10) {
    static const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    int length = minLength + rand() % (maxLength - minLength + 1);

    string name;
    name += toupper(alphabet[rand() % 26]); // первая буква большая

    for (int i = 1; i < length; ++i) {
        name += alphabet[rand() % 26];
    }

    return name;
}

int main() {
    srand(time(0)); // Инициализация генератора случайных чисел

    ofstream out("people_data.txt");
    if (!out) {
        cerr << "Не удалось открыть файл для записи.\n";
        return 1;
    }

    const int targetSizeMB = 100;
    const int bytesPerMB = 1024 * 1024;
    const int targetBytes = targetSizeMB * bytesPerMB;

    int totalBytesWritten = 0;
    int groupId = 1;

    while (totalBytesWritten < targetBytes) {
        string groupName = "Group_" + to_string(groupId++);
        out << "Group: " << groupName << "\n";
        totalBytesWritten += groupName.size() + 8; // 8 = "Group: " + "\n"

        int membersInGroup = 5 + rand() % 20; // 5-25 человек в группе

        for (int i = 0; i < membersInGroup; ++i) {
            string firstName = generateRandomName();
            string lastName = generateRandomName();
            string line = "- " + firstName + " " + lastName + "\n";
            out << line;
            totalBytesWritten += line.size();
        }

        out << "\n";
        totalBytesWritten += 1;
    }

    out.close();
    cout << "Файл создан: people_data.txt (~" << totalBytesWritten / bytesPerMB << " MB)\n";

    return 0;
}
