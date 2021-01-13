/*
Copyright (C) 2019-2020 JingWeiZhangHuai <jingweizhanghuai@163.com>
Licensed under the Apache License, Version 2.0; you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0 Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
*/
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "morn_tensor.h"
#include "morn_tensor_caculate.h"

float Detect(MLayer *layer,MLayer *prev,float *error)
{
    MTensor *tv = layer->tns;MTensor *in = prev->tns;
    struct TensorOutputPara *para = (struct TensorOutputPara *)(layer->para);
    int group = in->channel/atoi(para->argv[0]);
    float thresh = atof(para->argv[1]);
    
    mException((INVALID_TENSOR(in)||INVALID_TENSOR(tv)),EXIT,"invalid input");
    
    int size = in->height*in->width;
    mException((tv->batch!=in->batch)||(tv->height*tv->width*tv->channel!=size*in->channel),EXIT,"invalid tensor");
    
    float sum=0.0;
    for(int b =0;b<in->batch;b++)
    {
        float err = 0.0;
        for(int c=0;c<in->channel;c++)
        {
            int l = (c/group)*group;
            
            float *ldata = tv->data[b]+l*size;
            float *idata = in->data[b]+c*size;
            float *tdata = tv->data[b]+c*size;
            
            for(int i=0;i<size;i++)
                if((c%group==0)||(ldata[i]>thresh))
                {
                    float e = (idata[i] - tdata[i]);
                    err += e*e;
                }
        }
        err = err/2.0f;
        
        sum += err;
        if(error!=NULL) error[b]=err;
    }
    return (sum/(float)(in->batch));
}

void D_Detect(MLayer *layer,MLayer *prev)
{
    MTensor *tv = layer->tns;MTensor *in = prev->tns;MTensor *res=prev->res;
    struct TensorOutputPara *para = (struct TensorOutputPara *)(layer->para);
    int group = in->channel/atoi(para->argv[0]);
    float thresh = atof(para->argv[1]);
    
    int size = in->height*in->width;
    
    for(int b = 0;b<in->batch;b++)for(int c=0;c<in->channel;c++)
    {
        int l = (c/group)*group;
        
        float *ldata = tv->data[b]+l*size;
        float *idata = in->data[b]+c*size;
        float *tdata = tv->data[b]+c*size;
        float *rdata =res->data[b]+c*size;
        
        for(int i=0;i<size;i++)
        {
            if((c%group==0)||(ldata[i]>thresh))
            {
                float data = (idata[i]-tdata[i]);
                rdata[i] = (prev->state==MORN_FORWARD)?data:(rdata[i]+data);
            }
            else if(prev->state==MORN_FORWARD)
                rdata[i]=0.0f;
        }
    }
}

/*
float Split(MLayer *layer,MLayer *prev,float *error)
{
    MTensor *tv = layer->tns;MTensor *in = prev->tns;
    mException((INVALID_TENSOR(in)||INVALID_TENSOR(tv)),EXIT,"invalid input");
    
    int size = in->height*in->width*in->channel;
    mException((tv->batch!=in->batch)||(tv->height*tv->width*tv->channel!=size),EXIT,"invalid tensor");
    
    // printf("layer is %s,%p,tv is %p\n",layer->name,layer,tv);
    // printf("tv->data[2] is %p\n",tv->data[2]);
    
    float err=0.0;
    for(int b =0;b<in->batch;b++)
    {
        int sum1=0;int sum2=0;
        int err1=0;int err2=0;
        for(int i=0;i<size;i++)
        {
            if(tv->data[b][i]<0.5f) {sum1+=1;if(in->data[b][i]>0.5f) err1+=1;}
            else                      {sum2+=1;if(in->data[b][i]<0.5f) err2+=1;}
        }
        if(sum1==0)      err+=(float)err2/(float)sum2;
        else if(sum2==0) err+=(float)err1/(float)sum1;
        else err += (((float)err1/(float)sum1+(float)err2/(float)sum2)/2.0f);
        printf("err1 is %f,err2 is %f\n",(float)err1/(float)sum1,(float)err2/(float)sum2);
    }
    
    return err/(in->batch);
}

void D_Split(MLayer *layer,MLayer *prev)
{
    MTensor *tv = layer->tns;MTensor *in = prev->tns;MTensor *res=prev->res;
    
    int size = in->height*in->width*in->channel;
    for(int b =0;b<in->batch;b++)
    {
        int sum1=0,sum2=0;
        for(int i=0;i<size;i++)
        {
            if(tv->data[b][i]>0.5f) sum1+=1;
            else                      sum2+=1;
        }
        float k=(sum2>0)?((float)sum1/(float)sum2):1.0f;
        // k=k*k;
        // printf("sum1 is %d,sum2 is %d,k is %f\n",sum1,sum2,k);
        
        
        for(int i=0;i<size;i++)
        {
            float data;
            if(tv->data[b][i]>0.5f) data=(in->data[b][i]-1.0f);
            else                      data=(in->data[b][i]-0.0f)*k;
            // if(i<10)printf("%f(%f),",in->data[b][i],data);
                
            res->data[b][i] = (prev->state==MORN_FORWARD)?data:(res->data[b][i]+data);
        }
        // printf("\n");
    }
}
*/
