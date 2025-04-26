#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Структура для блока
struct Block {
    string text;
    int x, y;  // координаты центра
};

// Функция для записи блока
void drawBlock(ofstream& out, const Block& block) {
    int width = 140;
    int height = 40;
    int x = block.x - width/2;
    int y = block.y - height/2;

    // Прямоугольник
    out << "<rect x=\"" << x << "\" y=\"" << y
        << "\" width=\"" << width << "\" height=\"" << height
        << "\" fill=\"white\" stroke=\"black\" />\n";

    // Текст
    out << "<text x=\"" << block.x << "\" y=\"" << block.y
        << "\" font-size=\"12\" text-anchor=\"middle\" dominant-baseline=\"middle\">"
        << block.text << "</text>\n";
}

// Функция для стрелки вниз
void drawArrowDown(ofstream& out, int x, int y1, int y2) {
    out << "<line x1=\"" << x << "\" y1=\"" << y1 << "\" x2=\"" << x << "\" y2=\"" << y2
        << "\" stroke=\"black\" marker-end=\"url(#arrow)\" />\n";
}

// Функция для стрелки налево
void drawArrowLeft(ofstream& out, int x1, int x2, int y) {
    out << "<line x1=\"" << x1 << "\" y1=\"" << y << "\" x2=\"" << x2 << "\" y2=\"" << y
        << "\" stroke=\"black\" marker-end=\"url(#arrow)\" />\n";
}

int main() {
    ofstream out("diagram.svg");
    if (!out) {
        cerr << "Не удалось открыть файл для записи.\n";
        return 1;
    }

    // Начало SVG
    out << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"800\" height=\"600\">\n";

    // Определение маркера для стрелок
    out << "<defs>\n"
        << "<marker id=\"arrow\" markerWidth=\"10\" markerHeight=\"10\" refX=\"5\" refY=\"3\" orient=\"auto\" markerUnits=\"strokeWidth\">\n"
        << "<path d=\"M0,0 L0,6 L9,3 z\" fill=\"black\" />\n"
        << "</marker>\n"
        << "</defs>\n";

    // Основные блоки
    vector<Block> mainBlocks = {
        {"Зашли на сайт 100%", 400, 50},
        {"Оставили заявку 80%", 400, 150},
        {"Приехали в салон 50%", 400, 250},
        {"Прошли тест-драйв 40%", 400, 350},
        {"Купили авто 35%", 400, 450}
    };

    // Потери клиентов
    vector<Block> lossBlocks = {
        {"Ушли сразу 20%", 200, 50},
        {"Не пришли в салон 30%", 200, 150},
        {"Отказались от тест-драйва 10%", 200, 250},
        {"Отказались от покупки 5%", 200, 350}
    };

    // Рисуем блоки
    for (auto& block : mainBlocks) {
        drawBlock(out, block);
    }
    for (auto& block : lossBlocks) {
        drawBlock(out, block);
    }

    // Стрелки вниз
    for (size_t i = 0; i < mainBlocks.size() - 1; ++i) {
        drawArrowDown(out, mainBlocks[i].x, mainBlocks[i].y + 20, mainBlocks[i+1].y - 20);
    }

    // Стрелки налево
    for (size_t i = 0; i < lossBlocks.size(); ++i) {
        drawArrowLeft(out, mainBlocks[i].x - 70, lossBlocks[i].x + 70, lossBlocks[i].y);
    }

    // Конец SVG
    out << "</svg>\n";

    out.close();
    cout << "SVG-файл успешно создан: diagram.svg\n";
    return 0;
}
