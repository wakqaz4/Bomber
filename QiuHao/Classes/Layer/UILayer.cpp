#include "Layer\UILayer.h"
USING_NS_CC;
using namespace cocos2d::ui;

/*
	CC_CALLBACK_2(UILayer::DirSliderEvent, this):
	宏定义如下：
	#define CC_CALLBACK_2(__selector__,__target__, ...)

			std::bind(	&__selector__,
						 __target__, 
						std::placeholders::_1,
						std::placeholders::_2,
						##__VA_ARGS__  )

	std::bind（）:
	是 c++11 新的函数
	调用bind的一般形式为：
	auto newCallback = bind(callback,arg_list);
	首先，
		newCallback是一个可调用对象；
		arg_list是可以用逗号分隔的参数列表；（就是 newCallback函数的参数）
	也就是说，当我们调用newCallback时，newCallback会调用函数callback,并传递参数arg_list给callback.
	其次，
	“_1″是一个占位符对象
	这类占位符都定义在一个名为 std::placeholders 的命名空间中
	最后，
	##_VA_ARGS_是可变参数宏

	http://www.cocoachina.com/bbs/read.php?tid=198291&keyword=cc\_CALLBACK
*/

///////////////////////////////////////////////////////
bool UILayer::init()
{
	if ( !Layer::init() ) return false;

	/*
	cocos studio 做UI

	auto *rootNode = CSLoader::createNode("MyUI1/cocosstudio/UI.csd");//传入Studio2.x的资源路径
	this->addChild(rootNode);//假设this是即将显示的scene
	*/

	/**** 1.添加 label*/
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	LabelTTF* label = LabelTTF::create("******** Test the parabola *********", "Arial", 24);
			  label->setPosition(Point(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label->getContentSize().height));
	this->addChild(label, 1);


	/**** 2.添加 dirSlider*/
	Slider* dirSlider = Slider::create();
			dirSlider->loadBarTexture( "MyUI1/sliderTrack.png" );
			dirSlider->loadSlidBallTextures("MyUI1/sliderThumb.png", "MyUI1/sliderThumb.png", "");
			dirSlider->loadProgressBarTexture("MyUI1/sliderProgress.png");
			dirSlider->setMaxPercent(10000);														//百分比
			dirSlider->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height / 6.0f));		//位置
			dirSlider->addEventListener(CC_CALLBACK_2(UILayer::DirSliderEvent, this));
	this->addChild(dirSlider);


	/**** 3.添加 powerSlider*/
	Slider* powerSlider = Slider::create();
			powerSlider->loadBarTexture("MyUI1/sliderTrack.png");
			powerSlider->loadSlidBallTextures("MyUI1/sliderThumb.png", "MyUI1/sliderThumb.png", "");
			powerSlider->loadProgressBarTexture("MyUI1/sliderProgress1.png");
			powerSlider->setMaxPercent(10000);														//百分比
			powerSlider->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height / 9.0f));	//位置
			powerSlider->addEventListener(CC_CALLBACK_2(UILayer::DirSliderEvent, this));
	this->addChild(powerSlider);


	/****4. dir text*/
	/*位置不对，有问题
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