#include "Layer\UILayer.h"
USING_NS_CC;
using namespace cocos2d::ui;

/*
	CC_CALLBACK_2(UILayer::DirSliderEvent, this):
	�궨�����£�
	#define CC_CALLBACK_2(__selector__,__target__, ...)

			std::bind(	&__selector__,
						 __target__, 
						std::placeholders::_1,
						std::placeholders::_2,
						##__VA_ARGS__  )

	std::bind����:
	�� c++11 �µĺ���
	����bind��һ����ʽΪ��
	auto newCallback = bind(callback,arg_list);
	���ȣ�
		newCallback��һ���ɵ��ö���
		arg_list�ǿ����ö��ŷָ��Ĳ����б������� newCallback�����Ĳ�����
	Ҳ����˵�������ǵ���newCallbackʱ��newCallback����ú���callback,�����ݲ���arg_list��callback.
	��Σ�
	��_1����һ��ռλ������
	����ռλ����������һ����Ϊ std::placeholders �������ռ���
	���
	##_VA_ARGS_�ǿɱ������

	http://www.cocoachina.com/bbs/read.php?tid=198291&keyword=cc\_CALLBACK
*/

///////////////////////////////////////////////////////
bool UILayer::init()
{
	if ( !Layer::init() ) return false;

	/*
	cocos studio ��UI

	auto *rootNode = CSLoader::createNode("MyUI1/cocosstudio/UI.csd");//����Studio2.x����Դ·��
	this->addChild(rootNode);//����this�Ǽ�����ʾ��scene
	*/

	/**** 1.��� label*/
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	LabelTTF* label = LabelTTF::create("******** Test the parabola *********", "Arial", 24);
			  label->setPosition(Point(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label->getContentSize().height));
	this->addChild(label, 1);


	/**** 2.��� dirSlider*/
	Slider* dirSlider = Slider::create();
			dirSlider->loadBarTexture( "MyUI1/sliderTrack.png" );
			dirSlider->loadSlidBallTextures("MyUI1/sliderThumb.png", "MyUI1/sliderThumb.png", "");
			dirSlider->loadProgressBarTexture("MyUI1/sliderProgress.png");
			dirSlider->setMaxPercent(10000);														//�ٷֱ�
			dirSlider->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height / 6.0f));		//λ��
			dirSlider->addEventListener(CC_CALLBACK_2(UILayer::DirSliderEvent, this));
	this->addChild(dirSlider);


	/**** 3.��� powerSlider*/
	Slider* powerSlider = Slider::create();
			powerSlider->loadBarTexture("MyUI1/sliderTrack.png");
			powerSlider->loadSlidBallTextures("MyUI1/sliderThumb.png", "MyUI1/sliderThumb.png", "");
			powerSlider->loadProgressBarTexture("MyUI1/sliderProgress1.png");
			powerSlider->setMaxPercent(10000);														//�ٷֱ�
			powerSlider->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height / 9.0f));	//λ��
			powerSlider->addEventListener(CC_CALLBACK_2(UILayer::DirSliderEvent, this));
	this->addChild(powerSlider);


	/****4. dir text*/
	/*λ�ò��ԣ�������
	this->dirValueLabel = Text::create();
	this->dirValueLabel->setFontName("fonts/Marker Felt.ttf");
	this->dirValueLabel->setFontSize(20);
	this->dirValueLabel->setString("No event");
	this->setPosition(Vec2( 0, visibleSize.height / 6.2f));
	this->addChild( dirValueLabel ,20);
	*/
	
	return true;
}


///////////////////////////////////////////////////////
void UILayer::DirSliderEvent(Ref *pSender, Slider::EventType type)
{
	switch ( type )
	{
		case Slider::EventType::ON_PERCENTAGE_CHANGED:
		{
			//Slider* slider = static_cast<Slider*>(pSender);
			//this->dirValueLabel->setString(StringUtils::format(" dir : %d", slider->getPercent()));





		}
			break;

		default:
			break;
	}
}


void UILayer::PowerSliderEvent(Ref *pSender, Slider::EventType type)
{

}