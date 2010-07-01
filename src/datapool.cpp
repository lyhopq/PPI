#include <stdio.h>
#include <stdlib.h>
#include "datapool.h"

/*!
*    \brief 构造函数
*
*    从文件中加载数据，并初始化 FrameBuffer 设备。
*/
DataPool::DataPool()
{
    //从文件中读取地址表
    if(initDataTable()!=0)//初始化各种坐标表格分配内存空间
    {
        printf("load addr table error\n");
        exit(-1);//退出程序
    }

    //init frame buffer
    if(initFbDev()!=0)//打开fb设备，分配内存
    {
        printf("init frame buffer error\n");
        exit(-1);//退出程序
    }

    //init frame buffer layer
    /*
    if(initFbLayer()!=0)//初始化fb控制层
    {
        printf("init frame buffer layer error\n");
        exit(-1);//退出程序
    }


    //用于解析时标和视频信息的数据结构和变量
    aziBuf = new s_ppi_azi[maxazisize];
    MEMALLOCCHECK(aziBuf)//where is this macro defined??-->defined in "debug.h" --> #define MEMALLOCCHECK(p) if(p == NULL){ printf("allocate mem failed and 												exit\n"); exit(0); }
    videoBuf = new s_ppi_video[maxvideosize];
    MEMALLOCCHECK(videoBuf)

    //用于显示的数据
    dispbuf = new s_ppi_video_for_disp[maxvideosize];
    MEMALLOCCHECK(dispbuf)
    dispnum = 0;

    */
}

//! 析构函数
DataPool::~DataPool()
{
    //vecvwa.clear();
    //delete[] dispbuf;
    //delete[] videoBuf;
    //delete[] aziBuf;
    //freeFbLayer();
    freeFbDev();
    freeDataTable();
}

/*!
*    加载数据表
*/
int DataPool::initDataTable()
{

    /*为近区地址表和远区地址表分配空间*/

    //近区
    addr_near_x = new int*[ADDR_NEAR_AZI_LEN];    //defined in "def.h"
    addr_near_y = new int*[ADDR_NEAR_AZI_LEN];
    //建立
    for( int i=0; i<ADDR_NEAR_AZI_LEN; i++ )
    {
        addr_near_x[i] = new int[ADDR_NEAR_SMP_LEN];
        addr_near_y[i] = new int[ADDR_NEAR_SMP_LEN];
    }

    /*
    //远区
    addr_far_x = new int*[ADDR_FAR_AZI_LEN];
    addr_far_y = new int*[ADDR_FAR_AZI_LEN];
    for( int i=0; i<ADDR_FAR_AZI_LEN; i++ )
    {
        addr_far_x[i] = new int[ADDR_FAR_SMP_LEN];
        addr_far_y[i] = new int[ADDR_FAR_SMP_LEN];
    }
    */

    /*
    //余辉随机数表
    randomx = new int[RND_LEN];
    MEMALLOCCHECK(randomx)
    randomy = new int[RND_LEN];
    MEMALLOCCHECK(randomy)

    colortable = new MFB_COLORTYPE[256];//颜色表
    MEMALLOCCHECK(colortable)
    */

    //从文件中读取
    FILE *fp;

    //近区初始化
    if( (fp=fopen("./data/Rad_4096_512_xb","r"))==NULL )
    {
        printf("read ./data/Rad_4096_512_xb error!\n");
        return -1;
    }
    else
    {
        for( int i=0; i<ADDR_NEAR_AZI_LEN; i++ )
            fread(addr_near_x[i],sizeof(int),ADDR_NEAR_SMP_LEN,fp);
        fclose(fp);
    }

    if( (fp=fopen("./data/Rad_4096_512_yb","r"))==NULL )
    {
        printf("read ./data/Rad_4096_512_yb error!\n");
        return -1;
    }
    else
    {
        for( int i=0; i<ADDR_NEAR_AZI_LEN; i++ )
            fread(addr_near_y[i],sizeof(int),ADDR_NEAR_SMP_LEN,fp);
        fclose(fp);
    }

    /*
    //远区初始化
    if( (fp=fopen("./data/Rad_8192_512_xb","r"))==NULL )
    {
        printf("read ./data/Rad_8192_512_xb error!\n");
        return -1;
    }
    else
    {
        for( int i=0; i<ADDR_FAR_AZI_LEN; i++ )
            fread(addr_far_x[i],sizeof(int),ADDR_FAR_SMP_LEN,fp);
        fclose(fp);
    }

    if( (fp=fopen("./data/Rad_8192_512_yb","r"))==NULL )
    {
        printf("read ./data/Rad_8192_512_yb error!\n");
        return -1;
    }
    else
    {
        for( int i=0; i<ADDR_FAR_AZI_LEN; i++ )
            fread(addr_far_y[i],sizeof(int),ADDR_FAR_SMP_LEN,fp);
        fclose(fp);
    }
    */

    /*
    //余辉初始化
    if( (fp=fopen("./dat/Rnd_xb","r"))==NULL )
    {
        printf("read ./dat/Rnd_xb error!\n");
        return -1;
    }
    else
    {
        fread(randomx,sizeof(int),RND_LEN,fp);
        fclose(fp);
    }

    if( (fp=fopen("./dat/Rnd_yb","r"))==NULL )
    {
        printf("read ./dat/Rnd_yb error!\n");
        return -1;
    }
    else
    {
        fread(randomy,sizeof(int),RND_LEN,fp);
        fclose(fp);
    }

    // 颜色表初始化
    if( (fp=fopen(MFB_COLORTABLE_FILE,"r"))==NULL )//defined in "fbinclude.h" #define MFB_COLORTABLE_FILE     "./dat/colortable16" or "colortable32"
    {
        printf("read %s error!\n",MFB_COLORTABLE_FILE);
        return -1;
    }
    else
    {
        fread(colortable,sizeof(MFB_COLORTYPE),256,fp);
        fclose(fp);
    }
    */

    return 0;
}

/*!
*    退出是释放一加载的内存
*/
void DataPool::freeDataTable()
{
    //delete[] colortable;
    //delete[] randomy;
    //delete[] randomx;

    /*
    for( int i=0; i<ADDR_FAR_AZI_LEN; i++ )
    {
        delete[] addr_far_x[i];
        delete[] addr_far_y[i];
    }
    delete[] addr_far_x;
    delete[] addr_far_y;
    */

    for( int i=0; i<ADDR_NEAR_AZI_LEN; i++ )
    {
       delete[] addr_near_x[i];
       delete[] addr_near_y[i];
    }
    delete[] addr_near_x;
    delete[] addr_near_y;
}

/*!
*    初始化并设置 FrameBuffer
*/
int DataPool::initFbDev()
{
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;

    fp=open("/dev/fb0",O_RDWR);//打开framebuffer
    if(fp<0)
    {
        printf("Error:Can not open framebuffer device\n");
        return -1;
    }

    if(ioctl(fp,FBIOGET_FSCREENINFO,&finfo)) //<linux/fb.h>获得屏幕固定参数
    {
        printf("Error reading fixed information\n");
        return -1;
    }

    if(ioctl(fp,FBIOGET_VSCREENINFO,&vinfo))//获得屏幕当前参数
    {
        printf("Error reading variable information\n");
        return -1;
    }

    printf("The mem is:%dK\n",finfo.smem_len/1024);        //fb缓存长度
    printf("The line_len is:%d\n",finfo.line_length);      //1行的字节数
    printf("The res is:%d*%d\n",vinfo.xres,vinfo.yres);    //可见解析度
    printf("The bits_per_pixel is:%d\n",vinfo.bits_per_pixel);//bpp

    bytes_per_pixel = BYTES_PER_PIXEL ;//defined in "def.h"-->16bit=2;32bit=4.
    fbmemsize = vinfo.xres*vinfo.yres*bytes_per_pixel;//开辟大小为屏幕分辨率大小

    fbplinear = (char *)mmap(NULL, fbmemsize, PROT_READ | PROT_WRITE, MAP_SHARED, fp, 0);//获得映射后起始地址
    if((int)fbplinear == -1L)
    {
        printf("Error: failed to map framebuffer device to memory.\n");
        return -1;
    }

    //从一维线性地址中分离出需要作图的区域,转换为二维地址空间
    //要注意这样映射后,fbp[x][y]在屏幕系奈恢盟轿獃,竖直为x
    /*
    fbp = new FB_COLORTYPE*[PPI_R];
    for( int i=0; i<PPI_R; i++ )
    {
        fbp[i] = (FB_COLORTYPE *)(fbplinear+(FB_XSTART+(FB_YSTART+i)*vinfo.xres)*bytes_per_pixel);
    }
    */
    fbp = new FB_COLORTYPE*[vinfo.yres];
    for( int i=0; i<vinfo.yres; i++ )
    {
        fbp[i] = (FB_COLORTYPE *)(fbplinear+i*vinfo.xres*bytes_per_pixel);
    }
    return 0;
}

/*!
*    退出是卸载 FrameBuffer
*/
void DataPool::freeFbDev()
{
    munmap(fbplinear,fbmemsize);//删除映射
    close(fp);

    delete[] fbp;
}

/*
int DataPool::initFbLayer()//初始化fblayer
{
    layer = new s_layer_unit*[MFB_HEIGHT];//defined in "ppidef.h"
    MEMALLOCCHECK(layer)

    for( int i=0; i<MFB_HEIGHT; i++ )//fbp转化后y与x交换，所以这里也交换
    {
        layer[i] = new s_layer_unit[MFB_WIDTH];
        MEMALLOCCHECK(layer[i])
        memset(layer[i],0,MFB_WIDTH*sizeof(s_layer_unit));//初始化为零
    }
    TRACE("--fb layer inited");
    return 0;
}

void DataPool::freeFbLayer()
{
    for( int i=0; i<MFB_HEIGHT; i++ )
    {
        delete[] layer[i];
    }
    delete[] layer;
    TRACE("--fb layer freed");
}
*/
