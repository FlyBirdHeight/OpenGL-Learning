### 纹理学习

1.  纹理环绕方式

| :-----: | :-----: |
| 环绕方式 | 描述 |
| ``GL_REPEAT`` | 纹理贴图重复，default值 |
| ``GL_MIRRORED_REPEAT`` | 纹理重复，但是是镜像模式 |
| ``GL_CLAMP_TO_EDGE`` | 纹理坐标会被约束在0到1之间,超出部分拉伸纹理 |
| ``GL_CLAMP_TO_BORDER`` | 超出的坐标为用户指定的边缘颜色。 |

使用``glTexParameter``这个方法来单独的一个坐标轴设置，其中的参数s->x、t->y、r->z
```
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
```
2. 

