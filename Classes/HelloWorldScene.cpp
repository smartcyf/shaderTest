#include "HelloWorldScene.h"
#include "EffectSprite.h"

USING_NS_CC;

// Blur
class EffectBlur : public Effect
{
public:
    CREATE_FUNC(EffectBlur);
    virtual void setTarget(EffectSprite *sprite) override;
    void setBlurRadius(float radius);
    void setBlurSampleNum(float num);
    
protected:
    bool init(float blurRadius = 10.0f, float sampleNum = 5.0f);
    
    float _blurRadius;
    float _blurSampleNum;
};

void EffectBlur::setTarget(EffectSprite *sprite)
{
    Size size = sprite->getTexture()->getContentSizeInPixels();
    _glprogramstate->setUniformVec2("resolution", size);
    _glprogramstate->setUniformFloat("blurRadius", _blurRadius);
    _glprogramstate->setUniformFloat("sampleNum", _blurSampleNum);
}

bool EffectBlur::init(float blurRadius, float sampleNum)
{
    initGLProgramState("Shaders/example_Blur.fsh");
    _blurRadius = blurRadius;
    _blurSampleNum = sampleNum;
    
    return true;
}

void EffectBlur::setBlurRadius(float radius)
{
    _blurRadius = radius;
}

void EffectBlur::setBlurSampleNum(float num)
{
    _blurSampleNum = num;
}

// Outline
class EffectOutline : public Effect
{
public:
    CREATE_FUNC(EffectOutline);
    
    bool init()
    {
        initGLProgramState("Shaders/example_outline.fsh");
        
        Vec3 color(1.0f, 1.0f, 1.0f);
        GLfloat radius = 0.01f;
        GLfloat threshold = 1.75;
        
        _glprogramstate->setUniformVec3("u_outlineColor", color);
        _glprogramstate->setUniformFloat("u_radius", radius);
        _glprogramstate->setUniformFloat("u_threshold", threshold);
        return true;
    }
};

// Noise
class EffectNoise : public Effect
{
public:
    CREATE_FUNC(EffectNoise);
    
protected:
    bool init() {
        initGLProgramState("Shaders/example_Noisy.fsh");
        return true;
    }
    
    virtual void setTarget(EffectSprite* sprite) override
    {
        auto s = sprite->getTexture()->getContentSizeInPixels();
        getGLProgramState()->setUniformVec2("resolution", Vec2(s.width, s.height));
    }
};

// Edge Detect
class EffectEdgeDetect : public Effect
{
public:
    CREATE_FUNC(EffectEdgeDetect);
    
protected:
    bool init() {
        initGLProgramState("Shaders/example_edgeDetection.fsh");
        return true;
    }
    
    virtual void setTarget(EffectSprite* sprite) override
    {
        auto s = sprite->getTexture()->getContentSizeInPixels();
        getGLProgramState()->setUniformVec2("resolution", Vec2(s.width, s.height));
    }
};

// Grey
class EffectGreyScale : public Effect
{
public:
    CREATE_FUNC(EffectGreyScale);
    
protected:
    bool init() {
        initGLProgramState("Shaders/example_greyScale.fsh");
        return true;
    }
};

// Sepia
class EffectSepia : public Effect
{
public:
    CREATE_FUNC(EffectSepia);
    
protected:
    bool init() {
        initGLProgramState("Shaders/example_sepia.fsh");
        return true;
    }
};

//test shader
class EffectTest : public Effect
{
public:
    CREATE_FUNC(EffectTest);
    
protected:
    bool init() {
//        initGLProgramState("Shaders/example_edgeDetection.fsh");
        
        auto glprogram = GLProgram::createWithFilenames("test_shader.vsh", "test_shader.fsh");
        
//        auto glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource.c_str());
        
        _glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
        _glprogramstate->retain();
        
        return true;
    }
    
    virtual void setTarget(EffectSprite* sprite) override
    {
        auto s = sprite->getTexture()->getContentSizeInPixels();
        auto pos = sprite->getPosition();
        auto anch = sprite->getAnchorPoint();
        Vec2 center = Vec2(pos.x-s.width*anch.x,pos.y-s.height*anch.y);
        
        getGLProgramState()->setUniformVec2("resolution", Vec2(s.width, s.height));
        getGLProgramState()->setUniformVec2("center", center);
        getGLProgramState()->setUniformInt("direction", 0);
        getGLProgramState()->setUniformFloat("rate", 0.2);
//        getGLProgramState()->setUniformTexture("u_texture1", right->getTexture());
//        getGLProgramState()->setUniformFloat("u_interpolate",0.5);
    }
};

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
//    auto sprite = Sprite::create("HelloWorld.png");
//
//    // position the sprite on the center of the screen
//    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
//
//    // add the sprite as a child to this layer
//    this->addChild(sprite, 0);
    
    //test shader
    auto sprite = EffectSprite::create("testpic.png");
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(sprite, 0);
    sprite->setEffect(EffectTest::create());
    
    auto sprite1 = EffectSprite::create("CloseNormal.png");
    sprite1->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height*0.2 + origin.y));
    this->addChild(sprite1, 0);
    sprite1->setEffect(EffectOutline::create());
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
