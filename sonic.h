#ifndef SONIC_H
#define SONIC_H
#include "ellipse.h"
#include "polygon.h"
#include "rect.h"
#include "vec2.h"
#include <vector>
#include <map>

class sonic {
public:
  sonic(double positionX, double positionY, int depthIn , double scale) : depth(depthIn) {
    //body
    vec2 bodyCenter = vec2(150,165);
    position = bodyCenter;
    theEllipses.push_back(make_shared<ellipse>(bodyCenter, 60, 65, 3, sonicC["blue"]));

    //body inside
    theEllipses.push_back(make_shared<ellipse>(bodyCenter, 40, 45, 4, sonicC["skin"]));

    //legs
    vec2 leftLegPos(125,235);
    theEllipses.push_back(make_shared<ellipse>(leftLegPos, 15, 40, 1, sonicC["blue"]));
    vec2 rightLegPos(175,235);
    theEllipses.push_back(make_shared<ellipse>(rightLegPos, 15, 40, 1, sonicC["blue"]));

    //arms
    vec2 leftArmPos(75,175);
    theEllipses.push_back(make_shared<ellipse>(leftArmPos, 40, 10, 1, sonicC["skin"]));
    vec2 rightArmPos(225,140);
    theEllipses.push_back(make_shared<ellipse>(rightArmPos, 40, 10, 1, sonicC["skin"]));

    //head
    vec2 headCenter(150,78);
    theEllipses.push_back(make_shared<ellipse>(headCenter, 45, 30, 1, sonicC["blue"]));

    //mouth
    vec2 mouthCircleCenter(150,95);
    theEllipses.push_back(make_shared<ellipse>(mouthCircleCenter, 25, 20, 2, sonicC["skin"]));

    //eyes
    vec2 leftEye(135,70);
    theEllipses.push_back(make_shared<ellipse>(leftEye, 8, 8, 6, sonicC["white"]));
    theEllipses.push_back(make_shared<ellipse>(leftEye, 2, 2, 7, sonicC["eye"]));
    vec2 rightEye(165,70);
    theEllipses.push_back(make_shared<ellipse>(rightEye, 8, 8, 6, sonicC["white"]));
    theEllipses.push_back(make_shared<ellipse>(rightEye, 2, 2, 7, sonicC["eye"]));

    //mouth
    vec2 mouthPos(150,90);
    theEllipses.push_back(make_shared<ellipse>(mouthPos, 15, 10, 5, sonicC["red"]));
    vec2 mouthCoverPos(133,80);
    theRects.push_back(make_shared<Rect>(mouthCoverPos, 35, 8, sonicC["skin"], 6));

    //ears
    std::vector<vec2> leftEar;
    leftEar.push_back(vec2(125, 55));
    leftEar.push_back(vec2(132, 25));
    leftEar.push_back(vec2(145, 50));
    thePolys.push_back(make_shared<Polygon>(leftEar, 10, sonicC["skin"]));
    std::vector<vec2> rightEar;
    rightEar.push_back(vec2(155, 50));
    rightEar.push_back(vec2(168, 25));
    rightEar.push_back(vec2(175, 55));
    thePolys.push_back(make_shared<Polygon>(rightEar, 10, sonicC["skin"]));

    //shoes
    std::vector<vec2> leftShoe;
    leftShoe.push_back(vec2(114, 265));
    leftShoe.push_back(vec2(140, 265));
    leftShoe.push_back(vec2(135, 285));
    thePolys.push_back(make_shared<Polygon>(leftShoe, 8, sonicC["red"]));
    std::vector<vec2> rightShoe;
    rightShoe.push_back(vec2(164, 265));
    rightShoe.push_back(vec2(190, 265));
    rightShoe.push_back(vec2(185, 285));
    thePolys.push_back(make_shared<Polygon>(rightShoe, 8, sonicC["red"]));

    //white stripes
    theRects.push_back(make_shared<Rect>(vec2(117, 270), 11, 4, sonicC["white"], 10));
    theRects.push_back(make_shared<Rect>(vec2(167, 270), 11, 4, sonicC["white"], 10));

    scale*=1;
    this->scale(scale);
    bodyCenter*=scale;
    this->translate(vec2(positionX - bodyCenter.x(), positionY -bodyCenter.y()));
  }

	//return the color of the top most ellipse or the background color
	color eval(int x, int y, color background) {
		float res;
		color inC;
		bool inTrue = false;
		double curDepth = -1.0;
		for (auto obj : theEllipses) {
		  res = obj->eval(x, y);
		  if (res < 0 && obj->getDepth() > curDepth) {
			inC = obj->getInC();
			inTrue = true;
			curDepth = obj->getDepth();
		  }
		 }

    for (auto poly:thePolys) {
    	if (poly->eval(x, y) && poly->getDepth() > curDepth){
    		inC = poly->getInC();
    		inTrue = true;
    		curDepth = poly->getDepth();
    	}
    }
    for (auto obj:theRects) {
    	if (obj->evalIn(x, y) && obj->getDepth() > curDepth){
    		inC = obj->getInC();
    		inTrue = true;
    		curDepth = obj->getDepth();
    	}
    }
		if (inTrue) {			
			return inC;
		}
		else
			return background;
	}

  void translate(vec2 offset) {
    for(auto obj : theEllipses) {
      obj->translate(offset);
    }
    for(auto obj : theRects) {
      obj->translate(offset);
    }
    for(auto obj : thePolys) {
      obj->translate(offset);
    }
    position += offset;
  }

  int getDepth() { return depth; }
  vec2 getPosition() { return position; }
  void scale(double scalar){
    for (const auto &item : theEllipses) {
        item->scale(scalar);
    }
    for (const auto &item : theRects) {
        item->scale(scalar);
    }
    for (const auto &item : thePolys) {
        item->scale(scalar);
    }
  }

private:
  int depth;
  vec2 position;
	std::vector<shared_ptr<ellipse>> theEllipses;
  std::vector<shared_ptr<Polygon>> thePolys;
  std::vector<shared_ptr<Rect>> theRects;
  //set colors
  std::map<std::string, color> sonicC = {{"skin", color(255, 231, 186)}, {"blue", color(56, 0, 212)},
                                        {"red", color(219, 46, 31)}, {"shoe", color(219, 46, 31)},
                                        {"white", color(255, 255, 255)}, {"eye", color(57, 199, 6)}};
};

#endif
