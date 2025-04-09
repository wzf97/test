#include<stdio.h>
#include<string>
#include<iostream>
#include<algorithm>
using namespace std;

//test1
typedef struct __tag_ASVL_OFFSCREEN
{
int u32PixelArrayFormat;
int i32Width;
int i32Height;
char* ppu8Plane[2];
int  pi32Pitch[2];
}ASVLOFFSCREEN, *LPASVLOFFSCREEN;


int ReadNV21(const char* fileName,int width,int height,LPASVLOFFSCREEN asvlOffScreen){
    FILE* fp = fopen(fileName,"rb");
    if(fp == NULL){
        printf("open file %s failed\n",fileName);
        return -1;
    }
    int nSize = width * height * 3 / 2;
    char* pData = new char[nSize];
    if (pData == nullptr) {
        cout<<"Memory allocation failed: not enough memory to read the file\n"<<endl;
        fclose(fp);
        return -1;
    }
    int ret = fread(pData,1,nSize,fp);
    if (ret != nSize) {
        cout<<"fread failed: read "<<ret<<" bytes instead of nSize"<<endl;
        fclose(fp);
        delete[] pData;
        return -1;
    }
    
    asvlOffScreen->u32PixelArrayFormat = 0x1000000;
    asvlOffScreen->i32Width = width;
    asvlOffScreen->i32Height = height;
    asvlOffScreen->ppu8Plane[0] = pData;
    asvlOffScreen->ppu8Plane[1] = asvlOffScreen->ppu8Plane[0] + width * height;
    asvlOffScreen->pi32Pitch[0] = width;
    asvlOffScreen->pi32Pitch[1] = width;
    
    fclose(fp);
    return 0;
}

int DrawFaceRect(LPASVLOFFSCREEN asvlOffScreen,int top,int left,int right,int bottom){
    int width = asvlOffScreen->i32Width;
    int height = asvlOffScreen->i32Height;
    int pitch = asvlOffScreen->pi32Pitch[0];
    char* pData = asvlOffScreen->ppu8Plane[0];
    int nSize = width * height;
    
    
    for(int i=top-1;i<top+1;i++){
        for(int j=left-1;j<right-1;j++){
            pData[i*pitch+j] = 0;
        }
    }
    for(int i=bottom-2;i<bottom;i++){
        for(int j=left-1;j<right-1;j++){
            pData[i*pitch+j] = 0;
        }
    }
    for(int i=top-1;i<bottom-1;i++){
        for(int j=left-1;j<left+1;j++){
            pData[i*pitch+j] = 0;
        }
    }
    for(int i=top-1;i<bottom-1;i++){  
        for(int j=right-2;j<right;j++){
            pData[i*pitch+j] = 0;
        }
    }


    return 0;
}

int WriteNV21(const char* pFileName,int width,int height,LPASVLOFFSCREEN asvlOffScreen){
    FILE* fp = fopen(pFileName,"wb");
    if(fp == NULL){
        cout<<"?????????"<<endl;
        return -1;
    }
    int nSize = width * height*3/2;
    char* pData = asvlOffScreen->ppu8Plane[0];
    fwrite(pData,1,nSize,fp);
    fclose(fp);
    return 0;
}

int main(){
    int res = 0;
    int width = 0;
    int height = 0;
    cout<<"input image width"<<endl;
    cin>>width;
    cout<<"input image height"<<endl;
    cin>>height;
    string pFileName;
    cout<<"input image file path"<<endl;
    cin>>pFileName;
    int left=0;
    int top=0;
    int right=0;
    int bottom=0;
    int faceWidth = 0;
    int faceHeight = 0;
    cout<<"input face left top faceWidth and faceHeight"<<endl;
    cin>>left>>top>>faceWidth>>faceHeight;
    right = left + faceWidth;
    bottom = top + faceHeight;
    //read nv21 image
    ASVLOFFSCREEN asvlOffScreen;
    res = ReadNV21(pFileName.c_str(),width,height,&asvlOffScreen);
    cout<<"ReadNV21: "<<res<<endl;
    //draw face rect
    res = DrawFaceRect(&asvlOffScreen,top,left,right,bottom);
    cout<<"DrawFaceRect: "<<res<<endl;
    //save result
    string dstFileName = "./DrawFaceRect_";
    dstFileName.append(to_string(width));
    dstFileName.append("x");
    dstFileName.append(to_string(height));
    dstFileName.append(".nv21");
    res = WriteNV21(dstFileName.c_str(),width,height,&asvlOffScreen);
    cout<<"WriteNV21: "<<res<<endl;
    return 0;
}