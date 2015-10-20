#ifndef __CAL_ANGLE_H__
#define __CAL_ANGLE_H__
#include "cocos2d.h"
USING_NS_CC;

class CalAngle
{
public:
	static float calangle(const Vec2 &p, const Vec2 &des)
	{
		Point shootVector = p - des;
		Point normalizedVector = ccpNormalize(shootVector);
		float radians = atan2(normalizedVector.y, -normalizedVector.x);
		float degree = CC_RADIANS_TO_DEGREES(radians);
		//右下方
		if ((p.x - des.x) <= 0 && (p.y - des.y) <= 0)
		{
			degree = degree + 90;
			return degree;
		}
		//右上方
		else if ((p.x - des.x) <= 0 && (p.y - des.y) >= 0)
		{
			degree = degree + 90;
			log("right up %f", degree);
		}
		//左上方
		else if ((p.x - des.x) >= 0 && (p.y - des.y) >= 0)
		{
			degree = degree + 90;
			log("left up %f", degree);
		}
		//左下方
		else if ((p.x - des.x) >= 0 && (p.y - des.y) <= 0)
		{
			degree = - degree;
			degree = 450 - degree;
			log("left down %f", degree);
		}
	}
};
#endif