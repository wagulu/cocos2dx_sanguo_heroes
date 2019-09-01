//
//  CCMyShader.h
//  Game
//
//  Created by fu.chenhao on 3/23/15.
//
//

#ifndef __Game__CCMyShader__
#define __Game__CCMyShader__

using namespace cocos2d;

class CCMyShader
{
public:
    
    static void initSharder()
    {
        initSharder_normal();
        initSharder_alpha();
    }
    
private:
    static void initSharder_normal()
    {
        const char* frag1 =
        "                                   \n\
        #ifdef GL_ES                        \n\
        precision lowp float;               \n\
        #endif                              \n\
        varying vec4 v_fragmentColor;       \n\
        varying vec2 v_texCoord;            \n\
        uniform vec4 u_color;               \n\
        void main()                         \n\
        {                                   \n\
        gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord) * u_color;    \n\
        }";
        
        auto p1 = new GLProgram();
        p1->initWithByteArrays(ccPositionTextureColor_vert, frag1);
        //p1->initWithByteArrays(ccPositionTextureColor_vert, ccPositionTextureColor_frag);
        
        GLProgramCache::getInstance()->addGLProgram(p1, "animation_shader");
        
        
        CHECK_GL_ERROR_DEBUG();
        
        p1->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
        p1->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
        p1->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
        
        CHECK_GL_ERROR_DEBUG();
        
        p1->link();
        
        CHECK_GL_ERROR_DEBUG();
        
        p1->updateUniforms();
        
        CHECK_GL_ERROR_DEBUG();
    }
    
    static void initSharder_alpha()
    {
        const char* frag2 =
        "                                   \n\
        #ifdef GL_ES                        \n\
        precision lowp float;               \n\
        #endif                              \n\
        varying vec4 v_fragmentColor;       \n\
        varying vec2 v_texCoord;            \n\
        uniform vec4 u_color;               \n\
        void main()                         \n\
        {                                   \n\
        gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord) * u_color * v_fragmentColor.a;    \n\
        }";
        
        auto p2 = new GLProgram();
        p2->initWithByteArrays(ccPositionTextureColor_vert, frag2);
        GLProgramCache::getInstance()->addGLProgram(p2, "animation_shader_alpha");
        
        
        CHECK_GL_ERROR_DEBUG();
        
        p2->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
        p2->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
        p2->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
        
        CHECK_GL_ERROR_DEBUG();
        
        p2->link();
        
        CHECK_GL_ERROR_DEBUG();
        
        p2->updateUniforms();
        
        CHECK_GL_ERROR_DEBUG();
        
    }
    
    static void initSharder_lighten()
    {
        const char* frag2 =
        "                                   \n\
        #ifdef GL_ES                        \n\
        precision lowp float;               \n\
        #endif                              \n\
        varying vec4 v_fragmentColor;       \n\
        varying vec2 v_texCoord;            \n\
        uniform vec4 u_color;               \n\
        void main()                         \n\
        {                                   \n\
        gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord) * u_color * v_fragmentColor.a;    \n\
        }";
        
        auto p2 = new GLProgram();
        p2->initWithByteArrays(ccPositionTextureColor_vert, frag2);
        GLProgramCache::getInstance()->addGLProgram(p2, "animation_shader_lighten");
        
        
        CHECK_GL_ERROR_DEBUG();
        
        p2->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
        p2->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
        p2->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
        
        CHECK_GL_ERROR_DEBUG();
        
        p2->link();
        
        CHECK_GL_ERROR_DEBUG();
        
        p2->updateUniforms();
        
        CHECK_GL_ERROR_DEBUG();
        
    }
    
};

#endif /* defined(__Game__CCMyShader__) */
