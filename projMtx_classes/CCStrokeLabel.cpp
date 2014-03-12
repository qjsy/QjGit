#include "CCStrokeLabel.h"  
  
using namespace cocos2d;  
  
CCStrokeLabel::CCStrokeLabel()  
{  
  
}  
  
CCStrokeLabel::~CCStrokeLabel()  
{  
  
}  
  
 CCStrokeLabel* CCStrokeLabel::create( const char *string, const char *fontName, float fontSize, float strokeSize,
 const cocos2d::ccColor3B& colStroke /*= cocos2d::ccc3(0, 0, 0)*/, cocos2d::CCTextAlignment hAlignment/*=cocos2d::kCCTextAlignmentCenter*/, 
 cocos2d::CCVerticalTextAlignment vAlignment/*=cocos2d::kCCVerticalTextAlignmentTop*/ )  
 {  
     CCStrokeLabel *pRet = new CCStrokeLabel();  
     if(pRet && pRet->initWithString(string, fontName, fontSize, strokeSize, colStroke, hAlignment, vAlignment))  
     {  
         pRet->autorelease();  
         return pRet;  
     }  
     CC_SAFE_DELETE(pRet);  
  
     return NULL;  
 }  
  
 CCStrokeLabel * CCStrokeLabel::create( const char *string, const char *fontName, float fontSize )  
 {  
     return create(string,fontName,fontSize,3);  
 }  
  
 bool CCStrokeLabel::initWithString( const char *label, const char *fontName, float fontSize, float fStrokeSize, const cocos2d::ccColor3B& colStroke, cocos2d::CCTextAlignment hAlignment, cocos2d::CCVerticalTextAlignment vAlignment )  
 {  
     m_fStrokeSize = fStrokeSize;  
     m_colStroke = colStroke;  
     m_label = CCLabelTTF::create(label, fontName, fontSize, CCSizeZero,hAlignment,vAlignment);  
     m_label->retain();  
     updateStroke();  
     return true;  
 }  
  
 void CCStrokeLabel::setColor( const cocos2d::ccColor3B& color3 )  
 {  
     if (m_label)  
     {  
         ccColor3B col = m_label->getColor();  
         if(color3.r!=col.r || color3.g!=col.g || color3.b!=col.b)  
         {  
             m_label->setColor(color3);  
             updateStroke();  
         }  
     }  
     else  
     {  
         CCLOG("ERROR:CCLabelTTFStroke::setColor m_label=NULL");  
     }  
 }  
  
 void CCStrokeLabel::updateStroke()  
 {  
     if (m_sprite)  
     {  
         removeChild(m_sprite, true);  
     }  
     CCSize textureSize = m_label->getContentSize();  
     textureSize.width += 2 * m_fStrokeSize;  
     textureSize.height += 2 * m_fStrokeSize;  
     //call to clear error  
     glGetError();  
     CCRenderTexture *rt = CCRenderTexture::create(textureSize.width, textureSize.height);  
     if(!rt)  
     {  
         CCLOG("create render texture failed !!!!");  
         addChild(m_label);  
         return;  
     }  
     ccColor3B col = m_label->getColor();  
     m_label->setColor(m_colStroke);  
     ccBlendFunc originalBlend = m_label->getBlendFunc();  
     ccBlendFunc func = { GL_SRC_ALPHA, GL_ONE};  
     m_label->setBlendFunc(func);  
     m_label->setAnchorPoint(ccp(0.5, 0.5));  
     rt->begin();  
     for(int i = 0; i < 360; i += 15)  
     {  
         float r = CC_DEGREES_TO_RADIANS(i);  
         m_label->setPosition(ccp(  
             textureSize.width * 0.5f + sin(r) * m_fStrokeSize,  
             textureSize.height * 0.5f + cos(r) * m_fStrokeSize));  
         m_label->visit();  
  
     }  
     m_label->setColor(col);  
     m_label->setBlendFunc(originalBlend);  
     m_label->setPosition(ccp(textureSize.width * 0.5f, textureSize.height * 0.5f));  
     m_label->visit();  
     rt->end();  
     CCTexture2D *texture = rt->getSprite()->getTexture();  
     texture->setAliasTexParameters();  
     m_sprite = CCSprite::createWithTexture(rt->getSprite()->getTexture());  
     setContentSize(m_sprite->getContentSize());  
     m_sprite->setAnchorPoint(ccp(0, 0));  
     m_sprite->setPosition(ccp(0, 0));  
     ((CCSprite *)m_sprite)->setFlipY(true);  
     addChild(m_sprite);  
 }  
  
 void CCStrokeLabel::setString( const char *label )  
 {  
     if (m_label)  
     {  
         if(0!=strcmp(label, m_label->getString()))  
         {  
             m_label->setString(label);  
             updateStroke();  
         }  
     }  
     else  
     {  
         CCLOG("ERROR:CCLabelTTFStroke::setString m_label=NULL");  
     }  
 }