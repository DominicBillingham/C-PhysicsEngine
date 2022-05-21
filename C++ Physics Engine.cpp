
#include <SFML\Graphics.hpp>
#include <iostream>
#include <string>
#include <cmath>
#include <math.h>
#include <chrono>
#include <thread>
#include <list>
#include <vector>

#include "PhysicsEngineFunctions.h"

using namespace std;
using namespace sf;

// Defined at bottom due to the size of the function
list<Entity> CreateScenario(int Num);

int main() {

    // Initialise values used to maintain a consistent framerate
    std::chrono::system_clock::time_point StartFrame = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point LastFrame = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point TimeElapsed = std::chrono::system_clock::now();

    // Hardcoded values that can be adjusted
    Vector2f g = Vector2f(0.0f, 9.8f);
    float ResCoeff = 0.5;
    float FrameDelay = 1000 / 20; //Second Value == Desired FPS;
    bool EnableGravity = true;

    // Allows the user to select from a set scenarios
    cout << "Which scenario do you wish to run? \n1) Simple Gravity \n2) Collisions Mid-Air with various speeds \n3) Collisions Mid-Air with various masses \n4) NULL \n5) Mass Collisions Without Gravity \n6) Mass Bouncing Squares \n";
    int UserInput;
    cin >> UserInput;
    list<Entity> TempEntList = CreateScenario(UserInput);

    // Allows user to enable to disable gravity
    cout << "Enable gravity? (0 for no, 1 for yes)" << endl;
    cin >> EnableGravity;

    // Allows the user to set resitution manually.
    cout << "Set restituion (Any value between 1 and 0)" << endl;
    cin >> ResCoeff;

    // Scenario creator returns a linked list that needs to be changed into a vector for running
    vector<Entity> EntList(TempEntList.size());
    int w = 0;
    for (list<Entity>::iterator i = TempEntList.begin(); i != TempEntList.end(); ++i) {
        EntList[w] = (*i);
        w++;
    }

    // Add any constant forces through acceleration
    for (int i = 0; i < EntList.size(); ++i) {
        if (!EntList[i].IsStatic() && EnableGravity) {
            EntList[i].SetAcc(g);
        }
    }

    RenderWindow window(sf::VideoMode(1000, 1000), "Physics Engine Demo");
    Style::Default;
    int FrameCount = 0;

    while (window.isOpen()) {

        StartFrame = std::chrono::system_clock::now();

        //Allows the user to be able to quit either through the X button or pressing escape.
        if (Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
            window.close();
        }
        Event CloseDown;
        while (window.pollEvent(CloseDown)) {
            if (CloseDown.type == CloseDown.Closed) { window.close(); }
        }

        Intergrate(EntList, FrameDelay, ResCoeff);

        // Refresh the window and then draw all of the shapes in their new position
        window.clear(Color(233, 233, 233, 255));
        for (int i = 0; i < EntList.size(); ++i) {
            window.draw(EntList[i]);
        }
        window.display();
    
        // Makes the program sleep if the cycle finishes too quickly, stops hardware for speeding up simulation and caps it at 30fps
        LastFrame = std::chrono::system_clock::now();
        std::chrono::duration<double, std::milli> work_time = LastFrame - StartFrame;
        int sleepfor = trunc(FrameDelay - work_time.count());

        FrameCount++;
        if (FrameCount == 100 ) {
            //cout << "5s has passed" << endl;
            //cout <<  "Square 1 is now at " << EntList[1].getPosition().y;
        }

        if (FrameDelay > work_time.count()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepfor));
        }
        else {
            //cout << "Frames dropped \n";
            cout << "Seconds per frame: " << (work_time.count() / 1000) << endl;
        }

    }

}

// Just used for initialising scenarios, everything below can be safely ignored
list<Entity> CreateScenario(int Num) {

    list<Entity> TempEntityList;
    //Entity(Vector2f position, Vector2f velocity, Vector2f size, float mass, bool stat, Color colour)
    Entity TempEnt(Vector2f(0, 0), Vector2f(0, 0), Vector2f(0, 0), 0, true, Color::Black);


    if (Num == 1) {

        TempEnt = Entity(Vector2f(0, 900), Vector2f(0, 0), Vector2f(1000, 150), 0, true, Color::Green);
        TempEntityList.push_back(TempEnt);

        TempEnt = Entity(Vector2f(150, 500), Vector2f(0, 0), Vector2f(50, 50), 0, false, Color::Cyan);
        TempEntityList.push_back(TempEnt);

        TempEnt = Entity(Vector2f(250, 400), Vector2f(0, 0), Vector2f(50, 50), 0, false, Color::Cyan);
        TempEntityList.push_back(TempEnt);

        TempEnt = Entity(Vector2f(350, 300), Vector2f(0, 0), Vector2f(50, 50), 0, false, Color::Cyan);
        TempEntityList.push_back(TempEnt);

    }

    if (Num == 2) {

        //Pair 1
        TempEnt = Entity(Vector2f(300, 200), Vector2f(100, 0), Vector2f(50, 50), 50, false, Color::Cyan);
        TempEntityList.push_back(TempEnt);

        TempEnt = Entity(Vector2f(500, 200), Vector2f(0, 0), Vector2f(50, 50), 50, false, Color::Blue);
        TempEntityList.push_back(TempEnt);

        //Pair 2
        TempEnt = Entity(Vector2f(300, 300), Vector2f(100, 0), Vector2f(50, 50), 50, false, Color::Cyan);
        TempEntityList.push_back(TempEnt);

        TempEnt = Entity(Vector2f(500, 300), Vector2f(-100, 0), Vector2f(50, 50),50, false, Color::Blue);
        TempEntityList.push_back(TempEnt);

        //Pair 3
        TempEnt = Entity(Vector2f(300, 400), Vector2f(200, 0), Vector2f(50, 50), 50, false, Color::Cyan);
        TempEntityList.push_back(TempEnt);

        TempEnt = Entity(Vector2f(500, 400), Vector2f(-100, 0), Vector2f(50, 50), 50, false, Color::Blue);
        TempEntityList.push_back(TempEnt);
    
    }

    if (Num == 3) {

        //Pair 1
        TempEnt = Entity(Vector2f(300, 200), Vector2f(100, 0), Vector2f(50, 50), 50, false, Color::Cyan);
        TempEntityList.push_back(TempEnt);

        TempEnt = Entity(Vector2f(500, 200), Vector2f(-100, 0), Vector2f(50, 50), 50, false, Color::Blue);
        TempEntityList.push_back(TempEnt);

        //Pair 2
        TempEnt = Entity(Vector2f(300, 300), Vector2f(100, 0), Vector2f(50, 50), 100, false, Color::Cyan);
        TempEntityList.push_back(TempEnt);

        TempEnt = Entity(Vector2f(500, 300), Vector2f(-100, 0), Vector2f(50, 50), 50, false, Color::Blue);
        TempEntityList.push_back(TempEnt);

        //Pair 3
        TempEnt = Entity(Vector2f(300, 400), Vector2f(100, 0), Vector2f(50, 50), 10000, false, Color::Cyan);
        TempEntityList.push_back(TempEnt);

        TempEnt = Entity(Vector2f(500, 400), Vector2f(-100, 0), Vector2f(50, 50), 50, false, Color::Blue);
        TempEntityList.push_back(TempEnt);

    }

    // Mass collisions without gravity
    if (Num == 5) {

        TempEnt = Entity(Vector2f(150, 100.0), Vector2f(30.0, 150.0), Vector2f(50, 50), 50, false, Color::Magenta);
        TempEntityList.push_back(TempEnt);

        TempEnt = Entity(Vector2f(350, 100.0), Vector2f(50.0, 150.0), Vector2f(50, 50), 50, false, Color::Magenta);
        TempEntityList.push_back(TempEnt);

        TempEnt = Entity(Vector2f(450, 100.0), Vector2f(50, 200.0), Vector2f(50, 50), 50, false, Color::Magenta);
        TempEntityList.push_back(TempEnt);

        TempEnt = Entity(Vector2f(250, 800.0), Vector2f(-20.0, -150.0), Vector2f(50, 50), 50, false, Color::Magenta);
        TempEntityList.push_back(TempEnt);

        TempEnt = Entity(Vector2f(350, 800.0), Vector2f(20.0, -150.0), Vector2f(50, 50), 50, false, Color::Magenta);
        TempEntityList.push_back(TempEnt);

        int temp;

        for (int i = 0; i < 10; i++) {


            for (int j = 0; j < 15; j++) {

                if (i % 2 == 1) {
                    TempEnt = Entity(Vector2f(50 + j * 50, 200 + i * 50), Vector2f(0, 0), Vector2f(20.0, 20.0), 20.0, false, Color::Cyan);
                    TempEntityList.push_back(TempEnt);
                }
                else {
                    TempEnt = Entity(Vector2f(75 + j * 50, 200 + i * 50), Vector2f(0, 0), Vector2f(20.0, 20.0), 20.0, false, Color::Cyan);
                    TempEntityList.push_back(TempEnt);
                }

            }

        }



    }

    // Mass Chaos
    if (Num == 6) {

        Vector2f TempVel;

        for (int i = 0; i < 10; i++) {


            for (int j = 0; j < 20; j++) {

                TempVel.x = (rand() % 100) - 50;
                TempVel.y = (rand() % 100) - 50;

                if (i % 2 == 1) {
                    TempEnt = Entity(Vector2f(50 + j * 20, 200 + i * 20), TempVel, Vector2f(10.0, 10.0), 20.0, false, Color::Cyan);
                    TempEntityList.push_back(TempEnt);
                }
                else {
                    TempEnt = Entity(Vector2f(75 + j * 20, 200 + i * 20), TempVel, Vector2f(10.0, 10.0), 20.0, false, Color::Cyan);
                    TempEntityList.push_back(TempEnt);
                }

         
            }
        }
    
    }

    return TempEntityList;

}
