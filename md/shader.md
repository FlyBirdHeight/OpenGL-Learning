## 着色器 Shaders

### GLSL
1. 着色器是使用一种叫GLSL的类C语言写成的。GLSL是为图形计算量身定制的，它包含一些针对向量和矩阵操作的有用特性。
着色器的开头总是要声明版本，接着是输入和输出变量、uniform和main函数。每个着色器的入口点都是main函数，在这个函数中我们处理所有的输入变量，并将结果输出到输出变量中。
例子如下：
` ` 
    #version version_number  //申明版本
    in type in_variable_name; //申明输入变量
    in type in_variable_name;

    out type out_variable_name; //申明输出变量

    uniform type uniform_name;

    int main()
    {
      // 处理输入并进行一些图形操作
      ...
      // 输出处理过的结果到输出变量
      out_variable_name = weird_stuff_we_processed;
    }
` `
2. 顶点着色器每个输入变量也叫顶点属性(Vertex Attribute)。我们能声明的*顶点属性是有上限的，它一般由硬件来决定*。
``
    GL_MAX_VERTEX_ATTRIBS可以获取硬件允许的最大顶点属性
    例子： int nrAttributes;
                glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
                std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
``

### GLSL中的数据类型
1. GLSL中包含C等其它语言大部分的默认基础数据类型：int、float、double、uint和bool。GLSL也有两种容器类型，它们会在这个教程中使用很多，分别是*向量(Vector)和矩阵(Matrix)*
> #### GLSL向量
> GLSL中的向量是一个可以包含有1、2、3或者4个分量的容器，分量的类型可以是前面默认基础类型的任意一个
>   | 类型 | 含义 | 
    | :-----: | : ----: |
    | ``vecn`` | 包含``n``个float分量的默认向量 | 
    | ``bvecn`` | 包含``n``个bool分量的向量 | 
    | ``ivecn`` | 包含``n``个int分量的向量| 
    | ``uvecn`` | 包含``n``个unsigned int分量的向量 | 
    | ``dvecn`` | 包含``n``个double分量的向量 | 

2. 一个向量的分量可以通过vec.x这种方式获取，这里x是指这个向量的第一个分量。你可以分别使用.x、.y、.z和.w来获取它们的第1、2、3、4个分量。GLSL也允许你对颜色使用rgba，或是对纹理坐标使用stpq访问相同的分量。

3. 重组
> 向量重组：`` vec2 someVec;
vec4 differentVec = someVec.xyxx;
vec3 anotherVec = differentVec.zyw;
vec4 otherVec = someVec.xxxx + anotherVec.yxzy; ``
> 你可以使用上面4个字母任意组合来创建一个和原来向量一样长的（同类型）新向量，只要原来向量有那些分量即可;然而，不允许在一个vec2向量中去获取.z元素
> 注： 即下一个向量是上一个向量的子元素即可。此处描述的向量是在二维平面中的，顾不可以获取z轴元素
我们也可以把一个向量作为一个参数传给不同的向量构造函数，以减少需求参数的数量：
`` vec2 vect = vec2(0.5, 0.7);
vec4 result = vec4(vect, 0.0, 0.0);
vec4 otherResult = vec4(result.xyz, 1.0);``

### 输入输出
1. GLSL定义了in和out关键字专门来实现这个目的。每个着色器使用这两个关键字设定输入和输出，只要一个输出变量与下一个着色器阶段的输入匹配，它就会传递下去。
2. 顶点着色器与片段着色器中的不同:
> 顶点着色器应该接收的是一种特殊形式的输入，否则就会效率低下。顶点着色器的输入特殊在，它从顶点数据中直接接收输入。
>  `` location `` 作为一元数据指定输入变量。例如：`` layout (location = 0) layout作为一个额外的标识，用于链接到顶点数据 ``
>> 片段着色器，它需要一个vec4颜色输出变量，因为片段着色器需要生成一个最终输出的颜色。如果你在片段着色器没有定义输出颜色，OpenGL会把你的物体渲染为黑色（或白色）。

如果我们打算从一个着色器向另一个着色器发送数据，我们必须在发送方着色器中声明一个输出，在接收方着色器中声明一个类似的输入。当类型和名字都一样的时候，OpenGL就会把两个变量链接到一起，它们之间就能发送数据了（这是在链接程序对象时完成的）。

例子：在顶点着色器中输出一个，然后在片段着色器中接受顶点着色器的输出
` ` 
顶点着色器
#version 330 core
layout (location = 0) in vec3 aPos; // 位置变量的属性位置值为0

out vec4 vertexColor; // 为片段着色器指定一个颜色输出

void main()
{
    gl_Position = vec4(aPos, 1.0); // 注意我们如何把一个vec3作为vec4的构造器的参数
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // 把输出变量设置为暗红色
}
片段着色器

#version 330 core
out vec4 FragColor;

in vec4 vertexColor; // 从顶点着色器传来的输入变量（名称相同、类型相同）

void main()
{
    FragColor = vertexColor;
}
` `

### Uniform
1. Uniform是一种从CPU中的应用向GPU中的*着色器发送数据的方式*，但uniform和顶点属性有些不同。首先，uniform是全局的(Global)。uniform变量必须在每个着色器程序对象中都是唯一的，且它可以被着色器程序的任意着色器在任意阶段访问。第二，无论你把uniform值设置成什么，uniform会一直保存它们的数据，直到它们被重置或更新。
2. 例子如下：
` ` #version 330 core
out vec4 FragColor;

uniform vec4 ourColor; // 在OpenGL程序代码中设定这个变量

void main()
{
    FragColor = ourColor;
}  
` `
> 注：uniform在顶点着色器中不需要声明，uniform是一个全局变量所以可以再任何着色器中被使用
> * 注：如果你声明了一个uniform却在GLSL代码中没用过，编译器会静默移除这个变量，导致最后编译出的版本中并不会包含它，这可能导致几个非常麻烦的错误，记住这点！*

3. 查询uniform地址不要求你之前使用过着色器程序，但是更新一个uniform之前你必须先使用程序（调用glUseProgram)，因为它是在当前激活的着色器程序中设置uniform的。

4. 因为OpenGL在其核心是一个C库，所以它不支持类型重载，在函数参数不同的时候就要为其定义新的函数；glUniform是一个典型例子。这个函数有一个特定的后缀，标识设定的uniform的类型。可能的后缀有：
| 后缀     | 含义 | 
   | :-----: | : ----: |
   | ``f`` |     函数需要一个float作为它的值 | 
   | ``i`` |     函数需要一个int作为它的值 | 
   | ``ui`` |     函数需要一个unsigned作为它的值| 
   | ``3f`` |     函数需要3个float作为它的值 | 
   | ``fv`` |     函数需要一个float向量/数组作为它的值 | 
   
   ### 更多属性
   `` float vertices[] = {
       // 位置              // 颜色
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
       -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
   };以上六个数据为一个顶点数据，前三个代表位置后三个代表算表现的颜色
   ``
` ` #version 330 core
layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0 
layout (location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1

out vec3 ourColor; // 向片段着色器输出一个颜色

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor; // 将ourColor设置为我们从顶点数据那里得到的输入颜色
}  ` `
上方代码为着色器代码，由于现在分为位置与颜色数据，所以location的值分为了1与0

因为我们添加了另一个顶点属性，并且更新了VBO的内存，我们就必须重新配置顶点属性指针。更新后的VBO内存中的数据现在看起来像这样：
![avatar](https://learnopengl-cn.github.io/img/01/05/vertex_attribute_pointer_interleaved.png)

`` // 位置属性
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
// 颜色属性
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
glEnableVertexAttribArray(1); ``
>使用glVertexAttribPointer函数去配置顶点属性，由于我们现在有了两个顶点属性，我们不得不重新计算步长值。为获得数据队列中下一个属性值（比如位置向量的下个x分量）我们必须向右移动6个float，其中3个是位置值，另外3个是颜色值。这使我们的步长值为6乘以float的字节数（=24字节）。
同样，这次我们必须指定一个偏移量。对于每个顶点来说，位置顶点属性在前，所以它的偏移量是0。颜色属性紧随位置数据之后，所以偏移量就是3 * sizeof(float)，用字节来计算就是12字节。

### 自定义着色器类
