#include"head.h"
int controller=0;//控制器
    int main()
{
    char filename[200];//要压缩的文件名
    char filename1[200];//要解压的文件名
        printf("1.compress\n2.uncompress\n3.exit\n");
        while(controller!=3)
     {
            printf("please input your choice:\n");
            scanf("%d",&controller);
            fflush(stdin);
            while((controller!=1&&controller!=2)&&controller!=3)
            {
                printf("error,please input the correct choice\n");
                scanf("%d",&controller);
                fflush(stdin);
            }
            if(controller==1)
            {
                printf("please input the location of file\n");
                scanf("%s",filename);
                fflush(stdin);
                filecompress myconpress;
                controller=myconpress.myfilecompress(filename);//压缩
                if(controller==1)
                printf("start compress..\nfinished!\n");
                if(controller==0)
                printf("error,check file location!\n");
            }
            if(controller==2)
            {
                printf("please input the location of file:(file name must have '.compress')\n");
                scanf("%s",filename1);
                fileuncompress myuncompress;
                controller=myuncompress.myfileuncompress(filename1);//解压
                if(controller==1)
                printf("start uncompress....\nfinished!\n");
                if(controller==0)
                printf("error,check file location\n");
            }
            if(controller==3)
            {
                printf("88!\n");
                return 0;
            }
    }
    return 0;
}
