#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <filesystem> // Для создания директории

namespace fs = std::filesystem;

struct Person {
    std::string first_name;
    std::string last_name;
};

struct Group {
    std::string name;
    std::vector<Person> members;
};

void parse_data(const std::string& input_data, std::vector<Group>& groups) {
    std::istringstream stream(input_data);
    std::string line;
    Group current_group;

    while (std::getline(stream, line)) {
        if (line.find("Group:") == 0) {
            if (!current_group.name.empty()) {
                groups.push_back(current_group);
            }
            current_group.name = line.substr(7);
            current_group.members.clear();
        } else if (line.find("-") == 0) {
            Person person;
            std::istringstream person_stream(line.substr(2));
            person_stream >> person.first_name >> person.last_name;
            current_group.members.push_back(person);
        }
    }
    if (!current_group.name.empty()) {
        groups.push_back(current_group);
    }
}

void generate_svg(const Group& group) {
    std::string directory = "diagrams";
    if (!fs::exists(directory)) {
        fs::create_directory(directory);
    }

    std::string filename = directory + "/" + group.name + ".svg";
    std::ofstream svg_file(filename);

    if (!svg_file.is_open()) {
        std::cerr << "Не удалось создать файл: " << filename << std::endl;
        return;
    }

    svg_file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    svg_file << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"600\" height=\"400\">\n";
    
    int x_offset = 50;
    int y_offset = 50;
    int person_spacing = 30;

    svg_file << "<rect x=\"" << x_offset << "\" y=\"" << y_offset << "\" width=\"150\" height=\""
             << (group.members.size() * person_spacing + 20) << "\" stroke=\"black\" fill=\"none\" stroke-width=\"2\" />\n";
    
    svg_file << "<text x=\"" << x_offset + 10 << "\" y=\"" << y_offset + 20 << "\" font-size=\"14\" fill=\"black\">" 
             << group.name << "</text>\n";

    for (size_t i = 0; i < group.members.size(); ++i) {
        const Person& person = group.members[i];
        svg_file << "<text x=\"" << x_offset + 10 << "\" y=\"" << y_offset + 40 + i * person_spacing 
                 << "\" font-size=\"12\" fill=\"black\">" 
                 << person.first_name << " " << person.last_name << "</text>\n";
    }

    svg_file << "</svg>\n";
}

int main() {
    std::ifstream input_file("data.txt", std::ios::binary | std::ios::ate);
    if (!input_file.is_open()) {
        std::cerr << "Не удалось открыть файл 'data.txt'!" << std::endl;
        return 1;
    }

    std::streamsize input_size = input_file.tellg();
    input_file.seekg(0, std::ios::beg);

    std::stringstream buffer;
    buffer << input_file.rdbuf();
    std::string input_data = buffer.str();

    std::vector<Group> groups;
    parse_data(input_data, groups);

    using namespace std::chrono;
    long long total_generation_time_ns = 0;

    for (const auto& group : groups) {
        auto start = high_resolution_clock::now();
        generate_svg(group);
        auto end = high_resolution_clock::now();
        total_generation_time_ns += duration_cast<nanoseconds>(end - start).count();
    }

    double avg_time_per_diagram_us = (double)total_generation_time_ns / groups.size() / 1000.0;
    double avg_time_per_byte_ns = (double)total_generation_time_ns / input_size;

    std::cout << "Найдено групп: " << groups.size() << "\n";
    std::cout << "Всего сгенерировано " << groups.size() << " диаграмм.\n";
    std::cout << "Объём входных данных: " << input_size << " байт.\n";
    std::cout << "Среднее время генерации одной диаграммы: " << avg_time_per_diagram_us << " микросекунд.\n";
    std::cout << "Среднее время обработки одного байта данных: " << avg_time_per_byte_ns << " наносекунд.\n";

    return 0;
}
