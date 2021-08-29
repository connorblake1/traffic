//
//  vehicle.cpp
//  TrafficSimulator
//
//  Created by Connor Blake on 7/17/21.
//  Copyright © 2021 Connor Blake. All rights reserved.
//

#include "vehicle.hpp"

void vehicle::drive(float globalSpeed) {
	if (!stop) {
		position.x += (speed-globalSpeed)*TICK;}
	else {
		position.x -= globalSpeed*TICK;
	}
}

void vehicle::beginManeuver(unsigned long time,float duration, float intensity) {
	if (leader) {
		holdTime = int(duration/TICK)+time;
		maneuverIntensity = intensity;
		lState = 1;
	}}
void vehicle::beginManeuver(unsigned long time, float duration) {
	beginManeuver(time, duration, -brakingMax);}


void vehicle::modSpeed(vehicle * ahead, unsigned long inTime) {
	if ((*ahead).stop) {
		if (speed > 0) {
			speed -= fmin(brakingMax,speed);
		}
		if (speed < 0) {
			stop = true;
		}
		return;
	}
	if (leader) {
		if (inTime > holdTime) {
			lState = 0;}
		if (lState == 1) {
			if (maneuverIntensity < 0) {
				if (speed > DEFAULT_SPEED*.7) {
					speed += maneuverIntensity;
				}
				else {
					lState=0;
				}
			}
			}}
	else {
		computeFollow(ahead);
		float gap = (*ahead).getX()-this->getX();
//		float gapSpeed =speed-ahead->getSpeed();
//		float gapTime = gap/(gapSpeed+.0001); //time until collision
		float band = (gap-followDist)/followDist;
		pushReaction(band);
		float laggedBand = reactions[int(reactionTime/TICK)-2];
//		std::cout << "m" << std::endl;
//		std::cout << band << std::endl;
//		std::cout << laggedBand << std::endl;
		
//		switch(reacting) {
//			case 0:
//				if (laggedBand < -.3) {
//					holdTime = inTime;
//					reacting = 1;} //move foot to the brake
//				break;
//			case 1:
//				if (laggedBand > -.3) {
//					reacting = 0;
//					break;} //no go
//				if (inTime - reactionTime/TICK > holdTime) {
//					reacting = 2;} //go
//			case 2:
//				if (laggedBand > -.3) {
//					reacting = 0;}
//				else {
//					speed += fmax(-brakingMax,brakingPower*laggedBand);
//					return;}
//				break;
//		}
		if (reactions[int(reactionTime/TICK)-2] < 0) {
			speed += fmax(-brakingMax,brakingMax*laggedBand);}
		else if (band > 0) {
			if (speed < SPEED_LIMIT) {
				speed += fmin(accelerationMax,band*accelerationMax);}}
	//use information from 100 ticks back (200 ms) to mimic reaction times DONE
	//needs functionality to decelerate with minimal delay (lifting foot) DONE
	//needs functionality to brake with 1 second foot delay DONE
	//needs to cap hard braking by car limit DONE
	//needs to cap acceleration too DONE
	}}
void vehicle::accelerate() {
	if (speed < DEFAULT_SPEED) {
		speed += accelerationMax;}}
float vehicle::getX(){
    return this->position.x;}
float vehicle::getSpeed() {
	return this->speed;}
void vehicle::disp(sf::RenderWindow * disp) {
	if (position.x < WINX) {
		show.setPosition(position);
		disp->draw(show);}}
void vehicle::computeFollow(vehicle * ahead) {
	followDist = followTime*this->speed;}
void vehicle::pushReaction(float in) {
	for (int r = int(reactionTime/TICK)-1; r > 0; r--)  {
		reactions[r] = reactions[r-1];}
	reactions[0] = in;}
void vehicle::makeBackground(int lanes, int splits, float width, int hashLength, sf::RenderWindow * disp, float globalSpeed, unsigned long time) {
	float offset = globalSpeed*time*TICK;
	while (offset > 2*hashLength) {
		offset -= 2*hashLength;}
	for (int s = 0; s < splits + 1; s++) {
		sf::Vertex line1[] = { sf::Vertex(sf::Vector2f(0,6+s*lanes*width+width/2),sf::Color::Yellow), sf::Vertex(sf::Vector2f(WINX,6+s*lanes*width+width/2),sf::Color::Yellow)};
		(*disp).draw(line1, 2, sf::Lines);}
	for (int s = 0; s < splits; s++) {
		for (int l = 0; l < lanes+2; l++) {
			for (int w = -offset; w < WINX; w+=2*hashLength) {
				sf::Vertex line1[] = { sf::Vertex(sf::Vector2f(w,6+(s*lanes+l)*width-width/2),sf::Color::White), sf::Vertex(sf::Vector2f(w+hashLength,6+(s*lanes+l)*width-width/2),sf::Color::White)};
				(*disp).draw(line1, 2, sf::Lines);}}}}
