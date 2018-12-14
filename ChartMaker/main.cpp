#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Chart.hpp"

using namespace std;

void renderChart(sf::RenderWindow& window, const Chart& chart, int offsetX, int offsetY){
    const int minSeparation = 5;

    srand(666);

    window.clear(sf::Color::White);

    sf::VertexArray arr(sf::PrimitiveType::LinesStrip, chart.getData()[0].size());

    int chartLength = chart.getData()[0].size();

    for(int i=0; i<chart.getData().size(); i++){
        unsigned int max = 0;

        for(int j=0; j<chartLength; j++)
            if(chart.getData()[i][j] > max)
                max = chart.getData()[i][j];

        sf::Color color(rand()%200, rand()%200, rand()%200);

        for(int j=0; j<chartLength; j++){
            if(chartLength*minSeparation > 800)
                arr[j].position.x = j*minSeparation - offsetX;
            else
                arr[j].position.x = (j*800)/(chartLength == 1? 1 : chartLength - 1) - offsetX;
            arr[j].position.y = 600-(long long)(chart.getData()[i][j]*600)/(max == 0? 1 : max) - offsetY;
            arr[j].color = color;
        }
        window.draw(arr);
    }
}

int main(int argc, char** argv){
    /*if(argc != 2){
        cout << "First param: chart file" << endl;
        return 0;
    }*/
    Chart* chart;
    /*try{
        chart = new Chart(Chart::loadFromFile(argv[1]));
    }catch(const char* exc){
        cout << exc << endl;
        return 1;
    }*/

    chart = new Chart(2);
    for(int i=0; i<100; i++)
        chart->addValues(unsigned(sin(float(i)*3.14159*4/100.f)*300+300),
                         unsigned(cos(float(i)*3.14159*4/100.f)*300+300));
    chart->saveToFile("test.txt");
    delete chart;
    try{
        chart = new Chart(Chart::loadFromFile("test.txt"));
    }catch(const char* exc){
        cout << exc << endl;
        return 1;
    }

	sf::RenderWindow window;
    window.create(sf::VideoMode(800, 600), "Charts Graphical View");

    int offsetX = 0,
        offsetY = 0;

    map<sf::Keyboard::Key, bool> keys;
    sf::Vector2i mouse;
    bool mousePressed = false;

    while(window.isOpen()){

        sf::Event ev;
        while(window.pollEvent(ev)){
            switch(ev.type){
            case sf::Event::KeyPressed:
                keys[ev.key.code] = true;
                if(ev.key.code == sf::Keyboard::Escape)
                    window.close();
                break;
            case sf::Event::KeyReleased:
                keys[ev.key.code] = false;
                break;

            case sf::Event::MouseMoved:
                if(mousePressed){
                    offsetX += mouse.x - ev.mouseMove.x;
                    offsetY += mouse.y - ev.mouseMove.y;
                    if(offsetX < 0)
                        offsetX = 0;
                    if(offsetY < 0)
                        offsetY = 0;
                }
                mouse = sf::Vector2i(ev.mouseMove.x, ev.mouseMove.y);
                break;
            case sf::Event::MouseButtonPressed:
                if(ev.mouseButton.button == sf::Mouse::Left){
                    mousePressed = true;
                }
                break;
            case sf::Event::MouseButtonReleased:
                if(ev.mouseButton.button == sf::Mouse::Left){
                    mousePressed = false;
                }
                break;

            case sf::Event::Closed:
                window.close();
            }
        }

        renderChart(window, *chart, offsetX, offsetY);

        window.display();
        sf::sleep(sf::milliseconds(10));
    }

    delete chart;
}
