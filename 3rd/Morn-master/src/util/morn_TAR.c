/*
Copyright (C) 2019-2020 JingWeiZhangHuai <jingweizhanghuai@163.com>
Licensed under the Apache License, Version 2.0; you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0 Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "morn_util.h"
#define fread(Data,Size,Num,Fl) mException(((int)fread(Data,Size,Num,Fl)!=Num),EXIT,"read file error")

int OctToDec(char *str)
{
    int out = 0;
    for(int i=0;i<11;i++)
        out = (out<<3) + (str[i]-'0');
    return out;
}

struct TARFileInfo
{
    char filename[100];
    char name[60];
    int size;
    int locate;
};
void TARFileList(FILE *f,MList *list)
{
    struct TARFileInfo info;
    int locate = 0;
    
    fseek(f,0,SEEK_END);
    int filesize = ftell(f);
    fseek(f,0,SEEK_SET);
    
    char size[12];
    char magic[8];
    
    while(1)
    {
        if(locate >= filesize)
            break;
        // printf("locate is %d\n",locate);
        
        fseek(f,locate,SEEK_SET);
        fread(info.filename,100,sizeof(int8_t),f);
        // printf("filename is %s\n",info.filename);
        
        if(info.filename[0] == 0)
        {
            locate = locate + 512;
            continue;
        }
        
        fseek(f,locate+124,SEEK_SET);
        fread(size,12,sizeof(int8_t),f);
        info.size = OctToDec(size);
        // printf("size is %s\n",size);
        // printf("info.size is %d\n",info.size);
        
        fseek(f,locate+257,SEEK_SET);
        fread(magic,8,sizeof(int8_t),f);
        // printf("magic is %s\n",magic);
        
        locate = locate + 512;
        if(strcmp(magic,"ustar  ")==0)
        {
            MList *name=mStringSplit(info.filename,"/");
            strncpy(info.name,(char *)(name->data[name->num-1]),59);
            // printf("info->name is %s\n",info.name);
            
            info.locate = locate;
            mListWrite(list,DFLT,&info,sizeof(struct TARFileInfo));
            locate = locate + (((info.size+511)/512)*512);
        }
    }
}

struct HandleTARRead {
    FILE *f;
    MList *filelist;
    char *filedata;
    int filesize;
};
void endTARRead(void *info)
{
    struct HandleTARRead *handle = (struct HandleTARRead *)info;
    if(handle->f != NULL)
        fclose(handle->f);
    handle->f = NULL;
    
    if(handle->filelist != NULL)
        mListRelease(handle->filelist);
    handle->filelist = NULL;
    
    if(handle->filedata != NULL)
        mFree(handle->filedata);
    handle->filedata = NULL;
}
#define HASH_TARRead 0xce148cf8
int TARRead(MFile *file,char *filename,char**out,int *size)
{
    mException(INVALID_POINTER(file),EXIT,"invalid input");
    mException(INVALID_POINTER(filename),EXIT,"invalid input");
    
    MHandle *hdl=mHandle(file,TARRead);
    struct HandleTARRead *handle = (struct HandleTARRead *)(hdl->handle);
    if(hdl->valid == 0)
    {
        if(handle->f != NULL) fclose(handle->f);
        handle->f = fopen(file->filename,"rb");
        mException((handle->f==NULL),EXIT,"file cannot open");
        
        if(handle->filelist != NULL)
            handle->filelist->num = 0;
        else
            handle->filelist = mListCreate(DFLT,NULL);
        
        TARFileList(handle->f,handle->filelist);
        
        int filesize=0;
        for(int i=0;i<handle->filelist->num;i++)
        {
            struct TARFileInfo *fileinfo = (struct TARFileInfo *)(handle->filelist->data[i]);
            filesize = MAX(filesize,fileinfo->size);
        }
        if(filesize > handle->filesize)
        {
            handle->filesize = filesize;
            if(handle->filedata != NULL)
                mFree(handle->filedata);
            handle->filedata = (char *)mMalloc(filesize);
        }
        
        hdl->valid = 1;
    }
    
    MList *list = handle->filelist;
    
    for(int i=0;i<list->num;i++)
    {
        struct TARFileInfo *fileinfo = (struct TARFileInfo *)(list->data[i]);
        int flag = strcmp(filename,fileinfo->filename);
        if(flag!=0) flag = strcmp(filename,fileinfo->name);
        
        // printf("fileinfo->filename is %s\n",fileinfo->filename);
        // printf("filename is %s\n",filename);
        // printf("flag is %d\n",flag);
        
        if(flag == 0)
        {
            if(out!= NULL)
            {
                fseek(handle->f,fileinfo->locate,SEEK_SET);
                fread(handle->filedata,1,fileinfo->size,handle->f);
                *out = handle->filedata;
            }
            if(size != NULL)
                *size = fileinfo->size;
            
            return MORN_SUCCESS;
        }
    }
    
    return MORN_FAIL;
}

// int main()
// {
    // MProc *file = mProcCreate("./test.tar");
    // char *filedata;
    // int filesize;
    // mTARRead(file,"libjpeg.map",&filedata,&filesize);
    
    // printf("filesize is %d\n",filesize);
    // printf("filedata is:\n%s",filedata);
    
    // mProcRelease(file);
// }
    
    
