//
//  filecompress.h
//  hufftree_version2
//
//  Created by 汤智焱 on 2020/12/4.
//  Copyright © 2020 汤智焱. All rights reserved.
//

#ifndef filecompress_h
#define filecompress_h
#include<string>
#include<iostream>
#include<cstdio>
struct fileinfo
{
    unsigned char ch;
    int cnt=0;
    string code;

};//info这个结构体存放从文件获取的信息：ch存放字符，cnt存放频率，code存放字符编码
class filecompress//压缩类
{
    private:
    fileinfo info[256];//256个info对应256个asc码对应的字符
    public:
    filecompress()
    {
        for(int i=0;i<256;++i)
        {
            info[i].ch= i;
            info[i].cnt=0;
        }
    }//初始化每个info，如info[14].ch就是14这个asc码对应的字符。cnt表示出现的频率，初始化为0
    void buildcode(HuffNode<char> *root,string code)
    {
    if(root->isLeaf())
    {   char ch=((LeafNode<char>*)root)->val();
        info[(unsigned char)ch].code=code;
        return;
    }
    buildcode(((IntlNode<char>*)root)->left(),code + '0');
    buildcode(((IntlNode<char>*)root)->right(),code + '1');
        code.empty();
    }//通过哈夫曼树构建info的code（之前要先建树）
    void getconfiginfo(HuffNode<char> *root,string &configinfo)
    {
        if(!root->isLeaf())
        {
            configinfo+='1';
        }
        else
        {
            configinfo+='0';
            return;
        }
        getconfiginfo(((IntlNode<char>*)root)->left(),configinfo);
        getconfiginfo(((IntlNode<char>*)root)->right(),configinfo);
    }//通过哈夫曼树获取配置文件的信息，配置文件格式为一个int+一串二进制编码+每个叶结点对应的asc码
    //二进制编码是按前序遍历获得的，若访问到了leaf就添1，访问到了intlnode就添0
    //这个函数是获得二进制编码的
    void getconfigasc(HuffNode<char> *root,string &configinfo)
    {
        if(root->isLeaf())
        {
            configinfo+=((LeafNode<char>*)root)->val();
            return;
        }
        getconfigasc(((IntlNode<char>*)root)->left(),configinfo);
        getconfigasc(((IntlNode<char>*)root)->right(),configinfo);
        
    }//这个函数是获得配置文件里最后面的asc码的
    
    bool myfilecompress(const char* filename)//压缩的主函数，若压缩成功返回1失败返回0
       {
           FILE* fin = fopen(filename, "rb");
           if (fin == NULL)
           {
               //cout<<"打开错误，请输入正确的文件路径！"<<endl;
               return false;
           }
           char ch = fgetc(fin);
           while (ch != EOF)
           {
               info[(unsigned char)ch].cnt++;
               ch = fgetc(fin);
           }
           int size=0;
           int k=0;
           string code;
           for(int i=0;i<256;i++)
           {
               if(info[i].cnt!=0)
                   size++;
           }//读文件，初始化info即统计每个字符出现次数，并计算叶结点个数size
           HuffTree<char> **treearry=new HuffTree<char>*[size];
           for(int i=0;i<256;i++)
           {
               if(info[i].cnt!=0)
               {
                   char tempch=info[i].ch;
                   int tempcnt=info[i].cnt;
                   treearry[k++]=new HuffTree<char>(tempch,tempcnt);
               }
           }//通过info创建treearry，treearry是用来建树的
           HuffTree<char> *my_hufftree=buildHuff<char>(treearry,size);//通过treearry建树
           delete[] treearry;
           buildcode(my_hufftree->root(),code);//用buildcode函数生成每个字符的哈夫曼编码，存在info里
           
           string compressfile=filename;
           compressfile+=".compress";
           
           FILE* fout = fopen(compressfile.c_str(), "wb");//生成压缩文件
           if(!fout)
           {
                    cout<<"error!"<<endl;
                    return false;
           }
                  fseek(fout, 0, SEEK_SET);
                  fseek(fin, 0 ,SEEK_SET);
                  string code2;
                  int index = 0;
                  char mych = 0;
                  ch = fgetc(fin);
                  while (!feof(fin))
                  {
                      string& code2 = info[(unsigned char)ch].code;
                      for (size_t i = 0; i < code2.size(); ++i)
                      {
                          mych<<=1;
                          if(code2[i] == '1')
                          {
                              mych|=1;
                          }
                          else if(code2[i] == '0')
                          {
                              mych|=0;
                          }
                          ++index;
                          if(index==8)
                          {
                              fputc(mych, fout);
                              //cout<<mych;
                              index=0;
                              mych=0;
                          }
                      }
                      ch=fgetc(fin);
                  }
           if(index>0)
           {
               mych<<=(8-index);
               fputc(mych, fout);
           }//以上是将info里的code写入压缩文件的过程，具体就是对ch这个字符依据我们生成的哈夫曼码进行位操作，然后每满8位（即一个字节），写入文件一次。
           string config=filename;
           config+=".config.txt";
           FILE* myconfig = fopen(config.c_str(),"wb");//创建配置文件
           
           if(!myconfig)
           {
               cout<<"error!"<<endl;
               return false;
           }
           fprintf(myconfig,"%d",size);//写入叶结点个数
           string configinfo;
           getconfiginfo(my_hufftree->root(),configinfo);//获得叶结点位置信息
           getconfigasc(my_hufftree->root(),configinfo);//获得叶结点对应的asc码
           fseek(myconfig, 4, SEEK_SET);//空4个字节写入，因为前4个字节存的是叶结点个数
           fputs(configinfo.c_str(), myconfig);
           fclose(fin);
           fclose(fout);
           fclose(myconfig);
           return true;//压缩结束
       }
    };
#endif /* filecompress_h */
