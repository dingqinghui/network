## Morn：MImage图像

图像处理算法是Morn的一个非常非常重要组成部分。这里先介绍一下Morn里面，图像的数据结构的定义和一些最基本的操作。



### MImage的定义

```c
typedef struct MImage {
    int channel;
    int height;
    int width;
    unsigned char **data[MORN_MAX_IMAGE_CN];
    MArray *border;
    Morn;
    void *reserve;
}MImage;
```

这其中，channel就是图像的通道数，通道数最小是1，最大是4（也就是MORN_MAX_IMAGE_CN），这里面，channel=1（二值图像、灰度图像）和channel=3（RGB图像、YUV图像）最为常用。

height和width是图像的分辨率，即图像的高和宽。

data是数据索引，指向了图像像素点的数据。例如data\[0]\[100][200]就是图像上第0通道，纵坐标为100，横坐标为200的像素点的值。重申一遍，MImage里像素点的存储不一定是连续的，所以要访问像素点的值，请使用此索引。

这里特别指出，MImage对图像数据的存储，顺序是：**通道-高-宽**，也就是说先把图像分成若干个通道，同一通道的在一起。在一个通道里再分成若干行，同一行数据是连续的。这可能与其它库（例如著名的OpenCV）是不一样的。

border是图像的感兴趣边界，以后再讲。



### MImage基本操作

这里说的基本操作是最最基本的操作，不涉及图像处理算法。



#### MImage的创建

```c
MImage *mImageCreate();
MImage *mImageCreate(int height,int width);
MImage *mImageCreate(int channel,int height,int width);
MImage *mImageCreate(int channel,int height,int width,unsigned char **data[]);
```

这是典型Morn风格的Create函数，包括四种形式。其中channel是所创建的通道数，height是所创建的图像高度，width是所创建的图像宽度。channel、height、width都可以取默认值DFLT，它们的默认值都是0，当取默认值时，只是创建了一个MImage结构体，不会为图像数据分配内存空间。

data是图像像素的索引，如果创建时尚没有数据，就把它设置为NULL（通常都是把它设置为NULL）。

当对图像的通道和尺寸都未知时，通常使用`mImageCreate()`来创建图像。例如：

```c
MImage *img = mImageCreate();	//在载入图像之前，图像尺寸和类型未知。
mImageLoad(img,"./test.bmp");
```

当只知道图像尺寸，时使用`mImageCreate(height,width)`来创建图像。例如：

```c
MImage *dst=mImageCreate(480,640);
mImageResize(src,dst,DFLT,DFLT,DFLT);	//将图像src缩放到640*480像素，dst的通道数与src相同
```

创建一个已知尺寸和类型的图像，使用`MImageCreate(cn,height,width)`。例如：

```c
MImage *img = mImageCreate(3,480,640);	//创建一个3通道，640*480像素的图像
```

最后一种`MImageCreate(cn,height,width,data)`用于为已有的数据创建图像。例如：

```c
unsigned char data[480][640];
unsigned char **p=mMalloc(480*sizeof(unsigned char *));
for(int i=0;i<480;i++)p[i]=&(data[i][0]);
MImage *img1 = mImageCreate(1,480,640,&p);	//创建一幅图像，图像数据为data。

MImage *img = mImageCreate(3,480,640);
MImage *img2 = mImageCreate(1,480,640,img->data);	//img2是img的浅拷贝
```





#### MImage的释放

```c
void mImageRelease(MImage *img);
```

使用`MImageCreate`创建的MImage，必须且只能用MImageRelease释放掉。



#### MImage重定义

```c
void mImageRedefine(MImage *img,int channel,int height,int width);
```

这也是一个典型Morn风格的Redefine函数。当你需要改变MImage的通道数或者高度或者宽度的时候，你就需要使用此函数。（强烈不建议对img->cn、img->height、img->width直接赋值，尤其是如果你对MImage的实现细节不是非常非常清楚的话）。

img是需要重定义的图像，channel、height、width是重新定义后的通道数、高度和宽度。



#### MImage拷贝

```c
void mImageCopy(MImage *src,MImage *dst);
```

Morn里（不限于MImage）的拷贝都是深拷贝。如果你想浅拷贝的话，可以使用

`dst=mImageCreate(src->cn,src->height,src->width,src->data);`



#### MImage裁剪

```c
void mImageCut(MImage *img,MImage *dst,int x1,int x2,int y1,int y2);
```

就是把img上的，从x1列到x2列，从第y1行到底y2行（左闭右开，不包含第x2行和第y2列）的图像裁剪下来，保存到dst里。

x1的默认值是0，x2的默认值是img->width，y1的默认值是0，y2的默认值是img->height。

dst的默认值是src。

这个函数的参数中，并不要求x1>0，y1>0，也不要求x2\<img->width，y2\<img->height。甚至不要求x1<x2，y1<y2。所以，这个函数用起来很灵活，可以实现很多功能。



#### MImage像素运算

```c
void mImageDataAdd(MImage *src1,MImage *src2,MImage *dst);
void mImageDataSub(MImage *src1,MImage *src2,MImage *dst);
void mImageDataAnd(MImage *src1,MImage *src2,MImage *dst);
void mImageDataOr (MImage *src1,MImage *src2,MImage *dst);
void mImageDiff   (MImage *src1,MImage *src2,MImage *dst)；
```

这是图像像素的加法运算、减法运算、位与运算、位或运算。

这里面src1、src2是输入，dst是输出。要求src1和src2的高度和宽度相同。dst可以设置为DFLT，dst的默认值是src1，也就是说如果不指定输出的话，计算结果会保存在src1里，这是Morn的惯例。

特别说明，这里会出现两种情况：第一种，src1和src2的通道数相同，这个好理解，就是各个相对应的通道分别运算。第二种，src1是多通道的，src2是单通道的，这种也是允许的，这时src1的各个通道会分别和src2的第0通道进行运算。除此以外（src1单通道src2多通道，或者src1和src2都是多通道，但是通道数不相同）都是不允许的，会报错的。



#### MImage差值绝对值

```c
void mImageDiff(MImage *src1,MImage *src2,MImage *diff);
```

差值绝对值常常被用来计算两张图像的差异。

这里面src1、src2是输入，dst是输出。要求src1和src2的通道数、高度和宽度都相同。dst的默认值是src1。



#### MImage反色

```c
void mImageInvert(MImage *src,MImage *dst);
```

原来像素如果是50，那么反色后像素值就变成了255-50=205，原来像素如果是(50,100,150)，反色后就是(205,155,105)。

dst的默认值是src。



#### MImage对比度线性拉伸

```c
void mImageLinearMap(MImage *src,MImage *dst,float k,float b);
```

这个实现的是dst->data\[cn]\[j][i]=src->data\[cn]\[j][i]*k+b。常常用来调整图像的对比度。

dst的默认值是src。



#### MImage像素遍历

```c
void mImageOperate(MImage *src,MImage *dst,void (*func)(unsigned char *,unsigned char *,void *),void *para);
```

这是对图像的像素逐个遍历，进行自定义的操作，这个操作被定义在func函数里面。

例如，对图像进行阈值操作，可以这么写：

```c
{
    int thresh = 128;
	void thresh_func(unsigned char *in,unsigned char *out,void *para)
	{
    	 *out=((data[0]>thresh)&&(data[1]>thresh)&&(data[2]>thresh))?255:0;
	}
	mImageOperate(src,dst,thresh_func,NULL);
}
```











