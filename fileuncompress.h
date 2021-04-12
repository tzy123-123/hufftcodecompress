//
//  fileuncompress.h
//  hufftree_version2
//
//  Created by 汤智焱 on 2020/12/5.
//  Copyright © 2020 汤智焱. All rights reserved.
//

#ifndef fileuncompress_h
#define fileuncompress_h
#include"head.h"
class fileuncompress//解压类
{
public:
    HuffNode<char>* genhufftreebyconfig(char**p,FILE* fp)
    {
        char ch=fgetc(fp);
        //cout<<ch;
        if ((**p == '\0')||((ch!='0')&&(ch!='1')))
            return NULL;
        else
        {
            HuffNode<char> *root;
            if(ch=='0')
            {
                root=new LeafNode<char>(**p,0);
                (*p)++;
                return root;
            }
            else if(ch=='1')
            {
                root=new IntlNode<char>();
            }
            ((IntlNode<char>*)root)->setLeft(genhufftreebyconfig(p,fp));
            ((IntlNode<char>*)root)->setRight(genhufftreebyconfig(p,fp));
            return root;
        }
    }//通过配置文件还原哈夫曼树，p是指向每个叶结点的asc码的二级指针，fp是指向配置文件的指针
    
    bool myfileuncompress(char *filename)//解压的主函数
    {
        int size;//存叶结点个数
        int control=true;
        
        string fileconfig;
        fileconfig=filename;
        for(int i=0;i<=7;i++)
        {
            fileconfig.erase(fileconfig.end()-1);
        }
        fileconfig+="config.txt";
        
        string compressfilename;
        compressfilename=filename;
        
        FILE *fin=fopen(compressfilename.c_str(),"rb");
        if(!fin)
        {
            cout<<"error"<<endl;
            return false;
        }//打开压缩文件，文件指针为fin
        
        FILE *fconfig=fopen(fileconfig.c_str(),"rb");
        if(!fconfig)
        {
            cout<<"error"<<endl;
            return false;
        }//打开配置文件，文件指针为config
        
        fseek(fconfig, 0, SEEK_SET);
        fscanf(fconfig,"%d",&size);//读配置文件，先读个叶结点个数
        int sizeofasc=2*size-1;//通过叶结点个数计算得到全部结点个数
        fseek(fconfig, sizeofasc+4, SEEK_SET);//将文件指针移到配置文件中放asci码的位置
        char *arr=new char[size];//arr用来存放asc码
        int i=0;
        char ch=fgetc(fconfig);
        while(ch != EOF)
        {
            if(i<size)
            {
                arr[i++]=ch;
            }
            ch=fgetc(fconfig);
        }//读配置文件，先读个asc码
        
        fseek(fconfig, 4, SEEK_SET);
        char *p1=arr;
        char **p=&p1;
        HuffNode<char> *root=genhufftreebyconfig(p, fconfig);//还原哈夫曼树
        HuffNode<char> *temp=root;
        
        string uncompressfile;
        uncompressfile=filename;
        for(int i=0;i<=7;i++)
        {
            uncompressfile.erase(uncompressfile.end()-1);
        }
        uncompressfile+="uncompress.txt";
        
        FILE * fout=fopen(uncompressfile.c_str(),"wb");//创建解压文件
        fseek(fin, 0, SEEK_SET);
        int pos=7;
        ch=fgetc(fin);
        while (!feof(fin))
        {
            for (pos = 7; pos >= 0; --pos)
            {
                if (ch & (1 << pos))
                    temp=((IntlNode<char>*)temp)->right();
                else
                    temp=((IntlNode<char>*)temp)->left();
                if (temp->isLeaf())
                {
                    char res;
                    res=((LeafNode<char>*)temp)->val();
                    fputc(res,fout);
                    temp = root;
                }
            }
            ch= fgetc(fin);
        }//通过还原后的哈夫曼树解压文件，遍历树直到叶结点
        delete root;
        delete temp;
        fclose(fin);
        fclose(fout);
        fclose(fconfig);
        return true;
    }
    
};

#endif /* fileuncompress_h */
