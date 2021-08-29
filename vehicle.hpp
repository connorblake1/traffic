//
//  vehicle.hpp
//  TrafficSimulator
//
//  Created by Connor Blake on 7/17/21.
//  Copyright © 2021 Connor Blake. All rights reserved.
//

#ifndef vehicle_hpp
#define vehicle_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>

class vehicle {
private:
	unsigned long holdTime;
	//int maneuverCode; //0 is idle, 1 is hard break, 2 is left turn, 3 is right turn
	
    float speed, followTime, followDist, reactionTime, brakingMax, accelerationMax, maneuverIntensity;
    sf::Vector2f position;
    sf::RectangleShape show;
	float * reactions;
public:
	int reacting = 0;
    const static int SIZE = 20;
    static constexpr float TICK = .002;
	static constexpr float DEFAULT_SPEED = 100;
	static constexpr float SPEED_LIMIT = 130;
	static constexpr float MAX_BRAC = .05;
	static constexpr float MAX_ACCR = .025;
	const static int WINX = 1200;
	const static int WINY = 800;
	bool leader = false;
	bool lState = 0;
	bool stop = false;
	sf::Color color;
    vehicle() {}
    vehicle(sf::Vector2f startPos, float speedIn, float reactionIn,float brakingIn, float followIn, float accIn, sf::Color cIn) {
        position = startPos;
        speed = speedIn;
        reactionTime = reactionIn;
		brakingMax = brakingIn;
		accelerationMax = accIn;
        followTime = followIn;
		color = cIn;
		show.setSize(sf::Vector2f(SIZE, 12));
        show.setFillColor(color);
		reactions = new float[int(reactionTime/TICK)];
    }
	void computeFollow(vehicle * ahead);
    float getX();
	float getSpeed();
    void drive(float globalSpeed);
	void beginManeuver(unsigned long time, float duration, float intensity);
	void beginManeuver(unsigned long time, float duration);
	void accelerate();
	void pushReaction(float in);
    void modSpeed(vehicle * ahead, unsigned long clock);
    void disp(sf::RenderWindow * wind);
	static void makeBackground(int lanes, int splits, float width, int hashLenght, sf::RenderWindow * disp, float globalSpeed, unsigned long time);
};
    
#endif /* vehicle_hpp */
