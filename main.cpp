
//
// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resources, use the helper
// function `resourcePath()` from ResourcePath.hpp
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "ResourcePath.hpp"
#include "vehicle.hpp"
#include "Plotter.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
unsigned long t = 0;
float GLOBAL_SPEED = 15;
sf::Color colors[] = {sf::Color::Red,sf::Color::Blue,sf::Color::Green,sf::Color::Yellow,sf::Color::Magenta,sf::Color::Cyan,sf::Color::Red,sf::Color::Blue,sf::Color::Green,sf::Color::Yellow,sf::Color::Magenta,sf::Color::Cyan,sf::Color::Red,sf::Color::Blue,sf::Color::Green,sf::Color::Yellow,sf::Color::Magenta,sf::Color::Cyan};
void resetList(int num, int per, vehicle * t, bool * ls) {
	for (int l = 0; l < num/per; l++) {
		for (int i = 0; i < per; i++) {
			float r, s, d, rt, brM, acM;
			switch (l/5) {
				case 0:
					r = 2.5+.2*(l%5);
					s = 100;
					d = r*s;
					rt = .2;
					brM = .05;
					acM = .025;
					break;
				case 1:
					r = 3.2;
					s = 100;
					d = r*s;
					rt = .15+.25*(l%5);
					brM = .05;
					acM = .025;
					break;
				case 2:
					r = 3.2;
					s = 100;
					d = r*s;
					rt = .2;
					brM = .04+.01*(l%5);
					acM = .025;
					break;
				case 3:
					r = 3.2;
					s = 80+10*(l%5);
					d = r*s;
					rt = .2;
					brM = .05;
					acM = .025;
					break;
				case 4:
					r = 3.2;
					s = 100;
					d = r*s;
					if (i == 4) {
						rt = .2+2*(l%5);}
					else {
						rt = .2;}
					brM = .05;
					acM = .025;
					break;

			}
			t[l*per+i] = vehicle(sf::Vector2f(-i*(d),25.f*(l+1)), s, rt, brM,r, acM, colors[i]); //speed in fps, time in s
								//position	/	speed / reaction /  / time / braking / brakingMax / followtime / acceleration

		}
		t[l*per].leader = true;
	}
	for (int i = 0; i < num/per; i++) {
		ls[i] = false;}
	
}


int main(int, char const**)
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1024, 1024), "SFML window");

	sf::Font fontMain;
	fontMain.loadFromFile("/Users/connorblake/Documents/SORTOld/TrafficSimulator/TrafficSimulator/arial.ttf");
	sf::Text time(std::to_string(((((int)(t*vehicle::TICK*100)) % 100)/100)), fontMain, 50);
	time.setFillColor(sf::Color::White);
	sf::FloatRect textRect1 = time.getLocalBounds();
	time.setOrigin(textRect1.left + textRect1.width/2.0f, textRect1.top  + textRect1.height/2.0f);
	time.setPosition(200, 680);
	
	
    int numCars = 10;
	int lanes = 25;
	vehicle* traffic = new vehicle[numCars*lanes];
	bool lanesStopped[lanes];
	resetList(numCars*lanes,numCars,traffic, lanesStopped);
    
    //plotter
	Plotter speedGraph("Row 1 Speeds", "Time", "Speed", false, true, numCars, 600, vehicle::WINX/2-200, 650, vehicle::WINX/2, 350);
	speedGraph.setBounds(0,120);
	
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed) {
                switch(event.key.code) {
                    case sf::Keyboard::Escape:
                        window.close();
                        break;
                    case sf::Keyboard::R:
						resetList(numCars*lanes,numCars, traffic, lanesStopped);
						t = 0;
						speedGraph.reset();
						//speedGraph.reset();
                        break;
                    default:
                        break;
                }
            }
        }

        // Clear screen
        window.clear();
        //show background
		t++;//t/TICK = seconds elapsed

		for (int l = 0; l < lanes; l++) {
			if (traffic[l*numCars].getX() > 200 && traffic[l*numCars].getX() < 201) {
				traffic[l*numCars].beginManeuver(t, 3.0);}
			else if (traffic[l*numCars].getX() > 500 && traffic[l*numCars].getX() < 501) {
				traffic[l*numCars].beginManeuver(t, 6.0, vehicle::MAX_ACCR);}
			traffic[l*numCars].modSpeed(new vehicle,t);
			traffic[l*numCars].drive(GLOBAL_SPEED);
			traffic[l*numCars].disp(&window);
			for (int i = 1; i < numCars; i++) {
				traffic[l*numCars+i].modSpeed(&traffic[l*numCars+i-1],t);
				//collision
				if (traffic[l*numCars+i-1].getX()-vehicle::SIZE <= traffic[l*numCars+i].getX()) {
					traffic[l*numCars+i-1].stop = true;
					traffic[l*numCars+i].stop = true;}
				traffic[l*numCars+i].drive(GLOBAL_SPEED);
				traffic[l*numCars+i].disp(&window);
			}
		}

//			//reset cars
//			if (traffic[numCars-1].stop) {
//				resetList(numCars*lanes,numCars,traffic,lanesStopped);
//				t = 0;
//				//speedGraph.reset();
//			}
        // Update the window
		float * speeds = new float[numCars];
		for (int i = 0; i < numCars; i++) {
			speeds[i] = traffic[i].getSpeed();}
		speedGraph.pushData(speeds);
		speedGraph.showPlot(&window);
		std::stringstream stream;
		stream << std::fixed << std::setprecision(2) << t*vehicle::TICK;
		time.setString(stream.str());
		
		
		
		window.draw(time);
		
		vehicle::makeBackground(5, 5,25.0f,10, &window, GLOBAL_SPEED,t);
        window.display();
    }

    return EXIT_SUCCESS;
}


