//  
//  main.c  
//  Fourer1D  
//  
//  Created by Tony on 14/11/16.  
//  Copyright (c) 2017年 Yuhao Wang. All rights reserved.  
//  
#include "fft.h"
  
////////////////////////////////////////////////////////////////////  
//定义一个复数结构体  
///////////////////////////////////////////////////////////////////  

////////////////////////////////////////////////////////////////////  
//定义一个复数计算，包括乘法，加法，减法  
///////////////////////////////////////////////////////////////////  
void Add_Complex(Complex * src1,Complex *src2,Complex *dst){  
    dst->imagin=src1->imagin+src2->imagin;  
    dst->real=src1->real+src2->real;  
}  
void Sub_Complex(Complex * src1,Complex *src2,Complex *dst){  
    dst->imagin=src1->imagin-src2->imagin;  
    dst->real=src1->real-src2->real;  
}  
void Multy_Complex(Complex * src1,Complex *src2,Complex *dst){  
    double r1=0.0,r2=0.0;  
    double i1=0.0,i2=0.0;  
    r1=src1->real;  
    r2=src2->real;  
    i1=src1->imagin;  
    i2=src2->imagin;  
    dst->imagin=r1*i2+r2*i1;  
    dst->real=r1*r2-i1*i2;  
}  
////////////////////////////////////////////////////////////////////  
//在FFT中有一个WN的n次方项，在迭代中会不断用到，具体见算法说明  
///////////////////////////////////////////////////////////////////  
void getWN(double n,double size_n,Complex * dst){  
    double x=2.0*M_PI*n/size_n;  
    dst->imagin=-sin(x);  
    dst->real=cos(x);  
}  

////////////////////////////////////////////////////////////////////  
//定义FFT的初始化数据，因为FFT的数据经过重新映射，递归结构  
///////////////////////////////////////////////////////////////////  
int FFT_remap(double * src,int size_n){  
  
    if(size_n==1)  
        return 0;  
    double * temp=(double *)malloc(sizeof(double)*size_n);  
    for(int i=0;i<size_n;i++)  
        if(i%2==0)  
            temp[i/2]=src[i];  
        else  
            temp[(size_n+i)/2]=src[i];  
    for(int i=0;i<size_n;i++)  
        src[i]=temp[i];  
    free(temp);  
    FFT_remap(src, size_n/2);  
    FFT_remap(src+size_n/2, size_n/2);  
    return 1;  
  
  
}  
////////////////////////////////////////////////////////////////////  
//定义FFT，具体见算法说明，注释掉的显示部分为数据显示，可以观察结果  
///////////////////////////////////////////////////////////////////  
void FFT(double * src,Complex * dst,int size_n){  
      
    FFT_remap(src, size_n); 
    printf("fft start \n"); 
   // for(int i=0;i<size_n;i++)  
    //    printf("%lf\n",src[i]);  
    clock_t start,end;  
    start=clock();  
    int k=size_n;  
    int z=0;  
    while (k/=2) {  
        z++;  
    }  
    k=z;  
    if(size_n!=(1<<k))  
        exit(0);  
    Complex * src_com=(Complex*)malloc(sizeof(Complex)*size_n);  
    if(src_com==NULL)  
        exit(0);  
    for(int i=0;i<size_n;i++){  
        src_com[i].real=src[i];  
        src_com[i].imagin=0;  
    }  
    for(int i=0;i<k;i++){  
        z=0;  
        for(int j=0;j<size_n;j++){  
            if((j/(1<<i))%2==1){  
                Complex wn;  
                getWN(z, size_n, &wn);  
                Multy_Complex(&src_com[j], &wn,&src_com[j]);  
                z+=1<<(k-i-1);  
                Complex temp;  
                int neighbour=j-(1<<(i));  
                temp.real=src_com[neighbour].real;  
                temp.imagin=src_com[neighbour].imagin;  
                Add_Complex(&temp, &src_com[j], &src_com[neighbour]);  
                Sub_Complex(&temp, &src_com[j], &src_com[j]);  
            }  
            else  
                z=0;  
        }  
      
    }  
      
   for(int i=0;i<size_n;i++)
   {
        if(src_com[i].imagin>=0.0){ 
            printf("%lf+%lfj\n",src_com[i].real,src_com[i].imagin); 
        } 
        else 
            printf("%lf%lfj\n",src_com[i].real,src_com[i].imagin);  
   }
        
    for(int i=0;i<size_n;i++){  
        dst[i].imagin=src_com[i].imagin;  
        dst[i].real=src_com[i].real;  
    }  
    end=clock();  
    printf("FFT use time :%lfs for Datasize of:%d\n",(double)(end-start)/CLOCKS_PER_SEC,size_n);  
      
}  
