//#include <Windows.h>
//#include <iostream>
//#include <Gl\GL.h>
//#include <Gl\glut.h>
//#include <ft2build.h>
//#include <freetype/freetype.h>
//#include <freetype/ftglyph.h>
//#include <freetype/ftoutln.h>
//#include <freetype/fttrigon.h>
////#pragma comment(lib , "lib/glut32.lib")
////#pragma comment(lib , "lib/freetype2110.lib")
//using namespace std;
// 
//#define MAX_NO_TEXTURES 1
// 
//#define CUBE_TEXTURE 0
// 
//GLuint texture_id[MAX_NO_TEXTURES];
// 
//struct xCharTexture
//{
//    GLuint  m_texID;
//    wchar_t m_chaID;
//    int     m_Width;
//    int     m_Height;
// 
//    int     m_adv_x;
//    int     m_adv_y;
//    int     m_delta_x;
//    int     m_delta_y;
//public:
//    xCharTexture()
//    {
//        m_texID  = 0;
//        m_chaID  = 0;
//        m_Width  = 0;
//        m_Height = 0;
//    }
//}g_TexID[65536];
// 
// 
//class xFreeTypeLib
//{
//    FT_Library m_FT2Lib;
//    FT_Face    m_FT_Face;
// 
//public:
//    int   m_w;
//    int   m_h;
//    void load(const char* font_file , int _w , int _h);
//    GLuint loadChar(wchar_t ch);
//};
// 
//void xFreeTypeLib::load(const char* font_file , int _w , int _h)
//{
//    FT_Library library;
//    if (FT_Init_FreeType( &library) )
//        exit(0);
//    //加载一个字体,取默认的Face,一般为Regualer
//    if (FT_New_Face( library, font_file, 0, &m_FT_Face ))
//        exit(0);
//    //选择字符表
//    FT_Select_Charmap(m_FT_Face, FT_ENCODING_UNICODE);
//    m_w = _w ; m_h = _h;
//    m_FT_Face->num_fixed_sizes;
//    //大小要乘64.这是规定。照做就可以了。
//    //FT_Set_Char_Size( m_FT_Face , 0 , m_w << 6, 96, 96);
//    //用来存放指定字符宽度和高度的特定数据
//    FT_Set_Pixel_Sizes(m_FT_Face,m_w, m_h);
//}
// 
//GLuint xFreeTypeLib::loadChar(wchar_t ch)
//{
//    if(g_TexID[ch].m_texID)
//    return g_TexID[ch].m_texID;
//    /* 装载字形图像到字形槽（将会抹掉先前的字形图像） */
//    if(FT_Load_Char(m_FT_Face, ch, /*FT_LOAD_RENDER|*/FT_LOAD_FORCE_AUTOHINT|
//        (TRUE ? FT_LOAD_TARGET_NORMAL : FT_LOAD_MONOCHROME | FT_LOAD_TARGET_MONO) )   )
//    {
//        return 0;
//    }
// 
// /*if(FT_Load_Glyph( m_FT_Face, FT_Get_Char_Index( m_FT_Face, ch ), FT_LOAD_FORCE_AUTOHINT ))
//  throw std::runtime_error("FT_Load_Glyph failed");*/
// 
//    xCharTexture& charTex = g_TexID[ch];
// 
//    //得到字模
//    FT_Glyph glyph;
//    //把字形图像从字形槽复制到新的FT_Glyph对象glyph中。这个函数返回一个错误码并且设置glyph。
//    if(FT_Get_Glyph( m_FT_Face->glyph, &glyph ))
//        return 0;
// 
//     //转化成位图
//     FT_Render_Glyph( m_FT_Face->glyph,   FT_RENDER_MODE_LCD );//FT_RENDER_MODE_NORMAL  );
//     FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
//     FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
// 
//    //取道位图数据
//    FT_Bitmap& bitmap=bitmap_glyph->bitmap;
// 
//    //把位图数据拷贝自己定义的数据区里.这样旧可以画到需要的东西上面了。
//    int width  =  bitmap.width;
//    int height =  bitmap.rows;
// 
//    m_FT_Face->size->metrics.y_ppem;        //伸缩距离到设备空间
//    m_FT_Face->glyph->metrics.horiAdvance;  //水平文本排列
// 
// 
//    charTex.m_Width = width;
//    charTex.m_Height = height;
//    charTex.m_adv_x = m_FT_Face->glyph->advance.x / 64.0f;  //步进宽度
//    charTex.m_adv_y = m_FT_Face->size->metrics.y_ppem;        //m_FT_Face->glyph->metrics.horiBearingY / 64.0f;
//    charTex.m_delta_x = (float)bitmap_glyph->left;            //left:字形原点(0,0)到字形位图最左边象素的水平距离.它以整数象素的形式表示。
//    charTex.m_delta_y = (float)bitmap_glyph->top - height;    //Top: 类似于字形槽的bitmap_top字段。
//    glGenTextures(1,&charTex.m_texID);
//    glBindTexture(GL_TEXTURE_2D,charTex.m_texID);
//    char* pBuf = new char[width * height * 4];
//    for(int j=0; j  < height ; j++)
//    {
//        for(int i=0; i < width; i++)
//        {
//               unsigned char _vl =  (i>=bitmap.width || j>=bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width*j];
//              pBuf[(4*i + (height - j - 1) * width * 4)  ] = 0xFF;
//               pBuf[(4*i + (height - j - 1) * width * 4)+1] = 0xFF;
//               pBuf[(4*i + (height - j - 1) * width * 4)+2] = 0xFF;
//               pBuf[(4*i + (height - j - 1) * width * 4)+3] = _vl;
//          }
//     }
// 
//    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width, height,0,GL_RGBA,GL_UNSIGNED_BYTE,pBuf);  //指定一个二维的纹理图片
//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);                               //glTexParameteri():纹理过滤
//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
//    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
//    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
//    glTexEnvi(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_REPLACE);                                //纹理进行混合
// 
//    /*gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pBuf);
//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
//    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
//    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
//    glTexEnvi(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_REPLACE);*/
//    delete[] pBuf;
//    return charTex.m_chaID;
//}
// 
// 
//xFreeTypeLib g_FreeTypeLib;
//float ratio;
// 
//xCharTexture* getTextChar(wchar_t ch)
//{
//    g_FreeTypeLib.loadChar(ch);
//    return &g_TexID[ch];
//}
// 
//LPWSTR AnsiToUnicode(LPCSTR lpcstr)   //参数lpcstr类型也可是char*
//{
//    LPWSTR Pwstr;
//    int  i;
//    i=MultiByteToWideChar(CP_ACP,0,lpcstr,-1,NULL,0);
//    Pwstr=new WCHAR[i];
//    MultiByteToWideChar(CP_ACP,0,lpcstr,-1,Pwstr,i);
// 
//    return (Pwstr);
//}
// 
////wchar_t g_UnicodeString[]=L"aaabb/x4E2D/x6587/x0031/x0032/x0033";
//const char g_UnicodeString[]="aaa VB文件格式：/n/若不明确就标为未知/n/表演者：";
// 
//        
//void drawText(wchar_t* _strText,int x , int y, int maxW , int h)
//{
//    int sx = x;
//    int sy = y;
//    int maxH = h;
//    size_t nLen = wcslen(_strText);
// 
//    for(int i = 0 ; i <nLen ; i ++)
//    {
//        if(_strText[i] =='/n')
//        {
//            sx = x ; sy += maxH + 12;
//            continue;
//        }
//        xCharTexture* pCharTex = getTextChar(_strText[i]);
//        glBindTexture(GL_TEXTURE_2D,pCharTex->m_texID);                            //绑定到目标纹理
//        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
//        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
//        glEnable(GL_BLEND);                                                        //打开或关闭OpenGL的特殊功能
//        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);                        //特殊的像素算法
//        //glDisable(GL_TEXTURE_2D);
//        int w = pCharTex->m_Width;
//        int h = pCharTex->m_Height;
// 
//        int ch_x = sx + pCharTex->m_delta_x;
//        int ch_y = sy - h - pCharTex->m_delta_y;
// 
//        if(maxH < h) maxH = h;
//        glBegin ( GL_QUADS );                                                     // 定义一个或一组原始的顶点
//        {
//            glTexCoord2f(0.0f, 1.0f); glVertex3f(ch_x      , ch_y    ,  1.0f);
//            glTexCoord2f(1.0f, 1.0f); glVertex3f(ch_x +  w, ch_y    ,  1.0f);
//            glTexCoord2f(1.0f, 0.0f); glVertex3f(ch_x +  w, ch_y + h,  1.0f);
//            glTexCoord2f(0.0f, 0.0f); glVertex3f(ch_x     , ch_y + h,  1.0f);
//        }
//        glEnd();
//        sx += pCharTex->m_adv_x;
//        if(sx > x + maxW)
//        {
//            sx = x ; sy += maxH + 12;
//        }
//    }
//}
// 
//void init(void)
//{
//    glShadeModel(GL_SMOOTH|GL_FLAT);                        //选择平直或平滑着色
//    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                    //清除色彩缓冲区
//    glEnable ( GL_COLOR_MATERIAL_FACE );
//    glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );    // 使一个材质色彩指向当前的色彩
// 
//    //g_FreeTypeLib.load("simhei.ttf",14,14);
//    // g_FreeTypeLib.load("c://windows//fonts//simhei.ttf",14,14);
//    g_FreeTypeLib.load("c://windows//fonts//simhei.ttf",12,12);
// 
//    glDisable ( GL_CULL_FACE );
// 
//    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
//}
// 
//void reshape( int w, int h )
//{
// // Prevent a divide by zero, when window is too short
// // (you cant make a window of zero width).
// if(h == 0)
//  h = 1;
// 
// ratio = 1.0f * w / h;
// // Reset the coordinate system before modifying
// glMatrixMode(GL_PROJECTION);                            //定义矩阵
// glLoadIdentity();                                        //用恒等矩阵替换当前矩阵
// 
// // Set the viewport to be the entire window
//    glViewport(0, 0, w, h);                                //设置视窗
//    glOrtho(0,w,h,0,-100,200);                            //用垂直矩阵与当前矩阵相乘
// // Set the clipping volume
// glMatrixMode(GL_MODELVIEW);
// glLoadIdentity();
// gluLookAt(0, 0, 30 ,0 , 0 ,10 , 0.0f , 1.0f , 0.0f);    //设定一个变换视点
//}
// 
//void display( void )
//{
//   glClearColor(0.0f , 0.0f , 0.6f , 1.0f);
//   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
//   /*glLoadIdentity ( );
//   glPushMatrix();
//   glTranslatef ( 0.0, 0.0, -5.0 );
//   */
//   glEnable ( GL_TEXTURE_2D );
//   wchar_t *wstr = AnsiToUnicode(g_UnicodeString);
//   drawText(wstr, 50, 50, 900,25);
//   //glPopMatrix();
//   glutSwapBuffers();
//}
// 
//void keyboard ( unsigned char key, int x, int y )  // Create Keyboard Function
//{
//  switch ( key ) {
//    case 27:        // When Escape Is Pressed...
//      exit ( 0 );   // Exit The Program
//      break;        // Ready For Next Case
//    default:        // Now Wrap It Up
//      break;
//  }
//}
// 
//void arrow_keys ( int a_keys, int x, int y )   // Create Special Function (required for arrow keys)
//{
//  switch ( a_keys ) {
//    case GLUT_KEY_UP:     // When Up Arrow Is Pressed...
//      glutFullScreen ( ); // Go Into Full Screen Mode
//      break;
//    case GLUT_KEY_DOWN:                 // When Down Arrow Is Pressed...
//      glutReshapeWindow ( 900, 500 );   // Go Into A 500 By 500 Window
//      break;
//    default:
//      break;
//  }
//}
// 
// 
//void myDisplay(void)
// 
//{
// 
//    glClear(GL_COLOR_BUFFER_BIT);
// 
//    glColor3f (1.0,0.0,0.0);
//    glBegin (GL_LINES);
//    glVertex2i (180,15);
//    glVertex2i (10,145);
//    glEnd();
// 
//    glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
// 
//    glFlush();
// 
//}
// 
//int main(int argc,char * argv[])
//{
// 
//    //ANSI字符串，内容长度7字节
//    char sz[20] = "中文123";
// 
//    //UNICODE字符串，内容长度5个wchar_t（10字节）
//    wchar_t   wsz[100]   =   L"/x4E2D/x6587/x0031/x0032/x0033";
//    //运行时设定当前ANSI编码，VC格式
//    setlocale(LC_ALL,".936");
// 
//    //GCC中格式
//    setlocale(LC_ALL, "zh_CN.GBK");
// 
//    //VisualC++中使用小写%s，按照setlocale指定编码输出到文件
//    //GCC中使用大写%S
// 
// 
//    //把UNICODE字符串按照setlocale指定的编码转换成字节
//    wcstombs(sz,   wsz,   20);
//    //把字节串按照setlocale指定的编码转换成UNICODE字符串
//    mbstowcs(wsz,   sz,   20);
// 
//    glutInit( &argc, argv );                                       //用于初始化GULT库
//    glutInitDisplayMode ( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );  //设置初始显示模式
//    glutInitWindowPosition (0,0);                                   //确定窗口的位置
//    glutInitWindowSize  ( 900, 500 );                               //设置窗口大小
//    glutCreateWindow    ( "NeHe Lesson 6- Ported by Rustad" );       //创建一个窗口标题
//    init ();
//    //glutFullScreen      ( );          //Put Into Full Screen
//    glutDisplayFunc     ( display );    //注册一个绘图函数
//    glutReshapeFunc     ( reshape );    //设置投影的函数
//    glutKeyboardFunc    ( keyboard );    //键盘回调函数
//    glutSpecialFunc     ( arrow_keys );    //设置当前窗口的特殊键盘回调。
//    glutIdleFunc    ( display );        //程序空闲时调用的函数
//    glutMainLoop        ( );            // 进入GLUT事件处理循环
// 
//    return 0;
// 
//}
