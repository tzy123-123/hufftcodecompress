//
//  heap.h
//  hufftree_version2
//
//  Created by 汤智焱 on 2020/12/4.
//  Copyright © 2020 汤智焱. All rights reserved.
//

#ifndef heap_h
#define heap_h
template <typename E,typename Comp> class heap
{
private:
    E *Heap;
    int maxsize;
    int n;
    void swap(E *heap,int pos,int j){
        E temp;
        temp=heap[pos];
        heap[pos]=heap[j];
        heap[j]=temp;
        return;
    }
    void siftdown(int pos){
        while(!isleaf(pos)){
            int j=leftchild(pos);
            int rc=rightchild(pos);
            if((rc<n)&&Comp::prior(Heap[rc],Heap[j]))   j=rc;
            if(Comp::prior(Heap[pos],Heap[j]))  return;
            swap(Heap,pos,j);
            pos=j;
        }
    }
public:
    heap(E *h,int num,int max)
    {
        Heap=h;
        n=num;
        maxsize=max;
        buildheap();
    }
    int size() const
    {
        return n;
    }
    bool isleaf(int pos) const
    {
        return((pos>=n/2)&&(pos<n));
    }
    int leftchild(int pos) const
    {
        return 2*pos+1;
    }
    int rightchild(int pos) const
    {
        return 2*pos+2;
    }
    int parent(int pos) const
    {
        return(pos-1)/2;
    }
    void buildheap()
    {
        for(int i=n/2-1;i>=0;i--)
            siftdown(i);
    }
    void insert(const E &it)
    {
        int curr=n++;
        Heap[curr]=it;
        while((curr!=0)&&(Comp::prior(Heap[curr],Heap[parent(curr)])))
        {
            swap(Heap,curr,parent(curr));
            curr=parent(curr);
        }
    }
    E removefirst()
    {
        swap(Heap,0,--n);
        if(n!=0)
            siftdown(0);
        return Heap[n];
    }
    E remove(int pos)
    {
        if(pos==n-1)
            n--;
        else
        {
            swap(Heap,pos,--n);
            while((pos!=0)&&(Comp::prior(Heap[pos],Heap[parent(pos)])))
            {
                swap(Heap, pos, parent(pos));
                pos=parent(pos);
            }
            if(n!=0)
                siftdown(pos);
        }
        return Heap[n];
    }
};//堆的数据结构（用来建树）
#endif /* heap_h */
