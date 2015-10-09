#ifndef __UILAYER_H__
#define __UILAYER_H__

#include <cocos2d.h>
//#include "extensions/cocos-ext.h"//����ͷ�ļ��ļ��ϰ���һЩ��չ( extension )�궨��
#include "ui\UISlider.h"
#include "ui\UIText.h"

class UILayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(UILayer);
	UILayer(  ):dirValue(0), powerValue(0) {}
	virtual bool init();

	//dir & power slider events
	void DirSliderEvent(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type);
	void PowerSliderEvent(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type);
	
	/*
	���������Ķ���Ա����������ǿɼ���, �����������ſ����ڵ��õ��������չ���ú����Ĵ���.
	����ṩ��ͷ�ļ���,ֻ�к���ԭ��, ��Ȼ�ǲ�����, Ҫ��inline�����ĺ�����Ҳ�Ž�ͷ�ļ�.
	inline���滻���ڱ����ʱ��ͱ�ִ�е�(�����������ⲽ), �����ں�,
	*/
	inline unsigned int getDirValue() { return this->dirValue; }
	inline void setDirValue(int dirValue){ this->dirValue = dirValue; }
	inline unsigned int getPowerValue(){ return this->powerValue; }
	inline void setPowerValue(int powerValue){ this->powerValue = powerValue; }
	
private:
	int dirValue;
	int powerValue;
};

#endif