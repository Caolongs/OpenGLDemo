//
//  main.cpp
//  OpenGLDemo
//
//  Created by cao longjian on 2018/2/2.
//  Copyright © 2018年 caolongjian. All rights reserved.
//

/*
 `#include<GLShaderManager.h>` 移入了GLTool 着色器管理器（shader Mananger）类。没有着色器，我们就不能在OpenGL（核心框架）进行着色。着色器管理器不仅允许我们创建并管理着色器，还提供一组“存储着色器”，他们能够进行一些初步䄦基本的渲染操作。
 */
#include "GLShaderManager.h"

/*
 `#include<GLTools.h>`  GLTool.h头文件包含了大部分GLTool中类似C语言的独立函数
 */
#include "GLTools.h"


/*
 在Mac 系统下，`#include<glut/glut.h>`
 在Windows 和 Linux上，我们使用freeglut的静态库版本并且需要添加一个宏
 */
#include <GLUT/GLUT.h>




//定义一个，着色管理器
GLShaderManager shaderManager;

//简单的批次容器，是GLTools的一个简单的容器类。
GLBatch triangleBatch;


//blockSize 1/2边长
GLfloat blockSize = 0.1f;

GLfloat vVerts[] = {
    -blockSize,-blockSize,0.0f,
    blockSize,-blockSize,0.0f,
    blockSize,blockSize,0.0f,
    -blockSize,blockSize,0.0f
};

//x轴上移动的距离
GLfloat xPos = 0.0f;
//y轴上移动的距离
GLfloat yPos = 0.0f;


/*
 在窗口大小改变时，接收新的宽度&高度。
 */
void changeSize(int w,int h)
{
    /*
     x,y 参数代表窗口中视图的左下角坐标，而宽度、高度是像素为表示，通常x,y 都是为0
     */
    glViewport(0, 0, w, h);

}

void RenderScene(void)
{

    //1.清除一个或者一组特定的缓存区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);


    //2.设置一组浮点数来表示红色
    GLfloat vRed[] = {1.0,0.0,0.0,1.0f};

    
    //利用矩阵帮助移动
    //mFinalTransform 结果矩阵
    //mTransformMatrix 平移矩阵
    //mRotationMatrix  旋转矩阵
    M3DMatrix44f mFinalTransform,mTransformMatrix,mRotationMatrix;
    
    //平移 x,y,z,w（缩放因子= 1）
    //3D中平移的原理与矩阵之间关系
    /*
     参数1：矩阵
     参数2、3、4：X,Y,Z上平移距离
     */
    m3dTranslationMatrix44(mTransformMatrix, xPos, yPos, 0.0f);
    
    
    //增加难度! 一边移动，一边旋转
    static float yRot = 0.0f;
    
    /*
     参数1：矩阵
     参数2：弧度
     参数3：X:1，0 1：围绕X轴旋转，0不围绕X轴旋转
     参数4：Y:1，0
     参数5：Z:1，0
     */
    m3dRotationMatrix44(mRotationMatrix, m3dDegToRad(yRot), 0.0f, 0.0f, 1.0f);
    
    //修改旋转度数
    yRot += 5.0f;
    
    //思考：结合2个矩阵的结果 平移矩阵 * 旋转矩阵 = 最终结果矩阵
    m3dMatrixMultiply44(mFinalTransform, mTransformMatrix, mRotationMatrix);
    
    
    //平面着色器
    /*
     0.传递到存储着色器，即GLT_SHADER_IDENTITY着色器，这个着色器只是使用指定颜色以默认笛卡尔坐标第在屏幕上渲染几何图形
     1.平移矩阵 mTransformMatrix 与 每个顶点 相乘 -> 新顶点 （顶点着色器）
     2.将片元着色红色 (片元着色器)
     */
    shaderManager.UseStockShader(GLT_SHADER_FLAT,mFinalTransform,vRed);
    

    //提交着色器
    triangleBatch.Draw();

    //将后台缓冲区进行渲染，然后结束后交换给前台
    glutSwapBuffers();

}

//移动图形 -- 修改图形坐标！
void SpeacialKeys(int key,int x,int y) {
    //步长
    GLfloat stepSize = 0.025f;
    
    //计算移动距离
    if (key == GLUT_KEY_UP) {
        yPos += stepSize;
    }
    
    if (key == GLUT_KEY_DOWN) {
        yPos -= stepSize;
    }
    
    if (key == GLUT_KEY_LEFT) {
        xPos -= stepSize;
    }
    
    if (key == GLUT_KEY_RIGHT) {
        xPos += stepSize;
    }
    
    //边界检查
    if (xPos < -1.0f + blockSize) {
        xPos = -1.0f + blockSize;
    }
    
    if (xPos > 1.0f - blockSize) {
        xPos = 1.0f - blockSize;
    }
    
    if (yPos < -1.0f + blockSize) {
        yPos = -1.0f + blockSize;
    }
    
    if (yPos > 1.0f  - blockSize) {
        yPos = 1.0f - blockSize;
    }
    
    glutPostRedisplay();
}




void setupRC()
{
    //设置清屏颜色（背景颜色）
    glClearColor(0.2f, 0.40f, 0.7f, 1);

    //初始化一个渲染管理器。
    shaderManager.InitializeStockShaders();

    /*
     //指定三角形的顶点数据 x,y,z
     GLfloat vVerts[] = {
         -0.5f,0.0f,0.0f,
         0.5f,0.0f,0.0f,
         0.0f,0.5f,0.0f
     };
     
     triangleBatch.Begin(GL_TRIANGLES, 3);
     */
    
    triangleBatch.Begin(GL_TRIANGLE_FAN, 4);
    triangleBatch.CopyVertexData3f(vVerts);
    triangleBatch.End();

}

/*
void draw()
{
    printf("------");
    //1.清屏颜色
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);
    
    //开始渲染 指定填充方式 参考PPT图
    glBegin(GL_LINE_LOOP);
    
    const int n = 5;
    const float R = 0.5f;
    const GLfloat PI = 3.1415926f;
    
    for (int i = 0; i < n; i++) {
        
        //X,Y
        //X,Y (R * cos(2 * PI / n * i), R * sin(2 * PI / n * i)
        glVertex2f(R * cos(2 * PI / n * i), R * sin(2 * PI / n * i));
        
    }
    //结束渲染
    glEnd();
    glFlush();
}
 */



int main(int argc,char *argv[])
{
    //设置当前工作目录，针对MAC OS X
    /*
     `GLTools`函数`glSetWorkingDrectory`用来设置当前工作目录。实际上在Windows中是不必要的，因为工作目录默认就是与程序可执行执行程序相同的目录。但是在Mac OS X中，这个程序将当前工作文件夹改为应用程序捆绑包中的`/Resource`文件夹。`GLUT`的优先设定自动进行了这个中设置，但是这样中方法更加安全。
     */
    gltSetWorkingDirectory(argv[0]);
    
    
    
    //1. 初始化一个GLUT库
    glutInit(&argc, argv);
    
    /*
     GLUT_RGBA:RGBA颜色值模式
     GLUT_DOUBLE:双缓存窗口
     GLUT_DEPTH：
     GLUT_STENCIL：
     */
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
    

    glutInitWindowSize(800, 600);//创建一个窗口
    glutCreateWindow("OpenGL_Demo");//创建一个窗口
    
    
    /*
     GLUT 内部运行一个本地消息循环，拦截适当的消息。然后调用我们不同时间注册的回调函数。我们一共注册2个回调函数：
     1）为窗口改变大小而设置的一个回调函数
     2）包含OpenGL 渲染的回调函数
     
     */
    //注册重塑函数
    glutReshapeFunc(changeSize);
    
    //3. 注册一个绘图函数，操作系统在必要的时刻就会对窗体进行重绘制操作
    //他设置了一个现实回调（display callback）,即GLUT没在每次更新窗口内容时会自动调用改例程
    glutDisplayFunc(RenderScene);
    
    //注册键盘
    glutSpecialFunc(SpeacialKeys);
    
    /*
     初始化一个GLEW库,确保OpenGL API对程序完全可用。
     在试图做任何渲染之前，要检查确定驱动程序的初始化过程中没有任何问题
     */
    GLenum status = glewInit();
    if (GLEW_OK != status) {
        
        printf("GLEW Error:%s\n",glewGetErrorString(status));
        return 1;
        
    }
    
    //设置我们的渲染环境
    setupRC();
    
    //这是一个无限执行的循环，它会负责一直处理窗口和操作系统的用户输入等操作。（注意：不会执行在glutMainLoop()之后的所有命令。）
    glutMainLoop();
    
    return  0;
    
}
