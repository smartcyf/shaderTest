//
//  EffectSprite.h
//  testShader
//
//  Created by Ares Napoleon on 14/11/9.
//
//

#ifndef __testShader__EffectSprite__
#define __testShader__EffectSprite__

#include <stdio.h>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include <tuple>

USING_NS_CC;
USING_NS_CC_EXT;

//
// Effect
//
class EffectSprite;

class Effect : public Ref
{
public:
    GLProgramState* getGLProgramState() const { return _glprogramstate; }
    virtual void setTarget(EffectSprite *sprite){}
    
protected:
    bool initGLProgramState(const std::string &fragmentFilename);
    Effect();
    virtual ~Effect();
    GLProgramState *_glprogramstate;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string _fragSource;
    EventListenerCustom* _backgroundListener;
#endif
};

//
// EffectSprite
//
static int tuple_sort( const std::tuple<ssize_t,Effect*,QuadCommand> &tuple1, const std::tuple<ssize_t,Effect*,QuadCommand> &tuple2 )
{
    return std::get<0>(tuple1) < std::get<0>(tuple2);
}

class Effect;
class EffectSprite : public Sprite
{
public:
    static EffectSprite *create(const std::string& filename) {
        auto ret = new (std::nothrow) EffectSprite;
        if(ret && ret->initWithFile(filename)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_RELEASE(ret);
        return nullptr;
    }
    
    void setEffect(Effect* effect) {
        if(_defaultEffect != effect) {
            effect->setTarget(this);
            
            CC_SAFE_RELEASE(_defaultEffect);
            _defaultEffect = effect;
            CC_SAFE_RETAIN(_defaultEffect);
            
            setGLProgramState(_defaultEffect->getGLProgramState());
        }
    }
    void addEffect(Effect *effect, ssize_t order) {
        effect->retain();
        effect->setTarget(this);
        
        _effects.push_back(std::make_tuple(order,effect,QuadCommand()));
        
        std::sort(std::begin(_effects), std::end(_effects), tuple_sort);
    }
    
    void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override
    {
        // Don't do calculate the culling if the transform was not updated
        _insideBounds = (flags & FLAGS_TRANSFORM_DIRTY) ? renderer->checkVisibility(transform, _contentSize) : _insideBounds;
        
        if(_insideBounds)
        {
            // negative effects: order < 0
            int idx=0;
            for(auto &effect : _effects) {
                
                if(std::get<0>(effect) >=0)
                    break;
                QuadCommand &q = std::get<2>(effect);
                q.init(_globalZOrder, _texture->getName(), std::get<1>(effect)->getGLProgramState(), _blendFunc, &_quad, 1, transform);
                renderer->addCommand(&q);
                idx++;
                
            }
            
            // normal effect: order == 0
            _quadCommand.init(_globalZOrder, _texture->getName(), getGLProgramState(), _blendFunc, &_quad, 1, transform);
            renderer->addCommand(&_quadCommand);
            
            // postive effects: oder >= 0
            for(auto it = std::begin(_effects)+idx; it != std::end(_effects); ++it) {
                QuadCommand &q = std::get<2>(*it);
                q.init(_globalZOrder, _texture->getName(), std::get<1>(*it)->getGLProgramState(), _blendFunc, &_quad, 1, transform);
                renderer->addCommand(&q);
                idx++;
            }
        }
    }
protected:
    EffectSprite() : _defaultEffect(nullptr)
    {
        _effects.reserve(2);
    }
    ~EffectSprite() {
        for(auto &tuple : _effects) {
            std::get<1>(tuple)->release();
        }
        CC_SAFE_RELEASE(_defaultEffect);
    }
    
    std::vector<std::tuple<ssize_t,Effect*,QuadCommand>> _effects;
    Effect* _defaultEffect;
};




#endif /* defined(__testShader__EffectSprite__) */
