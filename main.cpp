#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "json.hpp"
sf::Clock clk;
sf::Font font;
static float scaleX = 1.75;
static float scaleY = 1.75;
static int wHeight = 768;
static int wWidth = 1024;
static int upgradeGapSize = 5;
static int upgradePosX = wWidth*0.90;
static int upgradePosY = wHeight*0.01;

class Upgrade{
    std::string name;
    int cost;
    float bonus;
    sf::Text label;
    sf::Rect<float> gPos;
public:
    Upgrade(float posX, float posY){
        name = "Kupa";
        cost = 0;
        bonus = 0;
        label.setPosition(posX, posY);
        label.setFont(font);
        label.setCharacterSize(35);
        label.setColor(sf::Color::Blue);
        label.setString(name);
        gPos = label.getGlobalBounds();
    }
    sf::Rect<float> region(){
        return gPos;
    }
    sf::Text* show(){
        return &label;
    }
    bool checkClick(float x, float y){
        if(x >= gPos.left &&  x <= gPos.left + gPos.width && y >= gPos.top && y <= gPos.top+gPos.height){
            return true;
        }else{
            return false;
        }
    }
    void checkHover(float x, float y){
        if(x >= gPos.left &&  x <= gPos.left + gPos.width && y >= gPos.top && y <= gPos.top+gPos.height){
            label.setColor(sf::Color::Green);
        }else{
            label.setColor(sf::Color::Blue);
        }
    }
};

class Points{
        unsigned long long int amount;
        sf::Text label;

    public:
        bool changed;
        Points(){
            label.setFont(font);
            label.setCharacterSize(20);
            label.setColor(sf::Color::Red);
            label.setPosition(10, 10);
            amount = 0;
            label.setString(std::to_string(amount));
            changed = false;
        }
        unsigned long long int addPoint(int howMuch){
            amount += howMuch;
            label.setString(std::to_string(amount));
            changed = true;
            return amount;
        }
        unsigned long long int addPoint(){
            amount++;
            label.setString(std::to_string(amount));
            changed = true;
            return amount;
        }
        unsigned long long int getPoints(){
            return amount;
        }
        sf::Text* show(){
            return &label;
        }
};

class Radek{
        friend class Points;
        sf::Texture rTex;
        sf::Sprite rSpr;
        sf::Sprite* ptrSpr;
        sf::Vector2f pos;
        sf::Rect<float> gPos;
    public:
        Radek(){
            rTex.loadFromFile("radek.png");
            rSpr.setTexture(rTex);
            rSpr.setOrigin(rTex.getSize().x/2, rTex.getSize().y/2);
            rSpr.setScale(scaleX,scaleY);
            // pos.x = wWidth/2 - (rTex.getSize().x*scaleX)/2;
            // pos.y = wHeight/2 - (rTex.getSize().y*scaleY)/2;
            pos.x = wWidth/2;
            pos.y = wHeight/2;
            // std::cout << pos.x << " " << pos.y;
            rSpr.setPosition(pos);
            gPos = rSpr.getGlobalBounds();
            ptrSpr = &rSpr;
        }
        sf::Sprite* getSprite(){
            return ptrSpr;
        }
        void setScale(float x, float y){
            rSpr.setScale(x,y);
        }
        bool checkClick(float x, float y){
            // std::cout << x << " " << gPos.left;
            // std::cout << rSpr.getGlobalBounds().left << " " << rSpr.getGlobalBounds().top << std::endl;
            // std::cout << "width" << rSpr.getGlobalBounds().width << " Height" << rSpr.getGlobalBounds().height << std::endl;
            // std::cout << x << " " << y << "\n";
            if(x >= gPos.left&&  x <= gPos.left + gPos.width && y >= gPos.top && y <= gPos.top+gPos.height){
                return true;
            }else{
                return false;
            }
        }
        void clicked(Points* point){
            point->addPoint();
            //Start Animation if not started
            //Add point
        }
};





int main(){
    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "rClicker");
    font.loadFromFile("AGENCYB.TTF");
    Radek radek;
    Points score;
    std::vector<Upgrade> upgrades;
    float offY = 0;
    for(int i=0;i < 5;i++){
            Upgrade temp(upgradePosX, upgradePosY+i*offY+i*upgradeGapSize);
            upgrades.push_back(temp);
            offY = temp.region().height;
    }
    while (window.isOpen())
    {
        sf::Event event;
        //Reset state
        score.changed = false;


        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::MouseButtonPressed){
                if(event.mouseButton.button == sf::Mouse::Button::Left){
                    float x = event.mouseButton.x;
                    float y = event.mouseButton.y;
                    if(radek.checkClick(x, y)){
                        radek.clicked(&score);
                    }
                    for(unsigned int i=0;i<upgrades.size();i++){
                        if(upgrades[i].checkClick(x, y)){
                            std::cout << "upgraded\n";
                        }
                    }
                }
            }
            if(event.type == sf::Event::MouseMoved){
                float x = event.mouseMove.x;
                float y = event.mouseMove.y;
                // std::cout << x  << y;
                for(unsigned int i=0;i<upgrades.size();i++){
                    upgrades[i].checkHover(x, y);
                }
            }
        }
        // if(score.changed)
        //     std::cout << score.getPoints() << std::endl;
        // if(clk.getElapsedTime().asMilliseconds() %1000 == 0){
        //     radek.setScale(0.5,0.5);
        // }else if(clk.getElapsedTime().asMilliseconds() %1000 == 500)
        //     radek.setScale(1.5,1.5);
        window.clear(sf::Color(29,33,32));
        window.draw(*radek.getSprite());
        window.draw(*score.show());
        for(Upgrade n : upgrades){
            window.draw(*n.show());
        }
        // window.draw(*kupa.show());
        window.display();
    }


}
