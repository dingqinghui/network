/*
Copyright (C) 2019-2020 JingWeiZhangHuai <jingweizhanghuai@163.com>
Licensed under the Apache License, Version 2.0; you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0 Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
*/

#include "morn_util.h"

#ifdef __MINGW32__
#define gettimeofday mingw_gettimeofday
#endif

#ifdef __GNUC__
#define stricmp strcasecmp
#endif

struct HandleTimer
{
    MList *timer_list;
};
#define HASH_Timer 0x88df05ba
static MList *morn_timer_list=NULL;
void endTimer(struct HandleTimer *handle)
{
    if(handle->timer_list!=NULL) mListRelease(handle->timer_list);
    morn_timer_list=NULL;
}
void _mTimerBegin(const char *name)
{
    #ifdef _MSC_VER
    int offset = sizeof(int);
    #else
    int offset = sizeof(struct timeval);
    #endif
    
    if(morn_timer_list==NULL)
    {
        MHandle *hdl = mHandle(mMornObject(NULL,DFLT),Timer);
        struct HandleTimer *handle = (struct HandleTimer *)(hdl->handle);
        if(handle->timer_list!=NULL) mListRelease(handle->timer_list);
        handle->timer_list = mListCreate();
        morn_timer_list = handle->timer_list;
        hdl->valid = 1;
        mListWrite(morn_timer_list,0,NULL,offset+1);
        *(((char *)(morn_timer_list->data[0]))+offset)=0;
    }
    int n=-1;
    if(name==NULL) n=0;
    else
    {
        for(int i=morn_timer_list->num-1;i>0;i--)
            if(strcmp(name,((char *)(morn_timer_list->data[i]))+offset)==0) 
                {n=i;break;}
    }
    if(n<0)
    {
        n=morn_timer_list->num;
        mListWrite(morn_timer_list,DFLT,NULL,offset+strlen(name));
        strcpy(((char *)(morn_timer_list->data[n]))+offset,name);
    }
    // printf("n=%d\n",n);
    #ifdef _MSC_VER
    *(int *)(morn_timer_list->data[n])=clock();
    #else
    struct timeval *tv=(struct timeval *)(morn_timer_list->data[n]);
    gettimeofday(tv,NULL);
    #endif
}
 
float _mTimerEnd(const char *name,const char *file,int line,const char *function)
{
    #ifdef _MSC_VER
    int cv1 = clock();
    int offset = sizeof(int);
    #else
    struct timeval tv1;gettimeofday(&tv1,NULL);
    int offset = sizeof(struct timeval);
    #endif
    int n=-1;
    if(name==NULL) {name="";n=0;}
    else
    {
        for(int i=morn_timer_list->num-1;i>0;i--)
            if(strcmp(name,((char *)(morn_timer_list->data[i]))+offset)==0) 
                {n=i;break;}
    }
    // printf("n=%d\n",n);
    mException(n<0,EXIT,"invalid timer name %s",name);
    #ifdef _MSC_VER
    int cv0 = *(int *)(morn_timer_list->data[n]);
    float Use = ((float)(cv1-cv0))*1000.0f/((float)CLOCKS_PER_SEC);
    #else
    struct timeval *tv0=(struct timeval *)(morn_timer_list->data[n]);
    float Use = (tv1.tv_sec - tv0->tv_sec)*1000.0f + (tv1.tv_usec - tv0->tv_usec)/1000.0f;
    #endif
    // mLog(MORN_INFO,"[%s,line %d,function %s]Timer: time use %fms\n",__FILE__,__LINE__,__FUNCTION__,Use);
    printf("[%s,line %d,function %s]Timer: %s time use %fms\n",file,line,function,name,Use);
    return Use;
}

char morn_time_now_string[128];
static int64_t morn_time_now=0;
const char *mTimeNowString()
{
    int64_t tv=(int64_t)time(NULL);
    if(tv==morn_time_now) return morn_time_now_string;
    morn_time_now = tv;
    struct tm *t=localtime(&tv);
    snprintf(morn_time_now_string,128,"%04d.%02d.%02d %02d:%02d:%02d",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec); 
    return morn_time_now_string;
}

char *CHNYearString(int num)
{
    #if defined(__linux__)
    int s=3;
    int dic[10]={0x008780e3,0x0080b8e4,0x008cbae4,0x0089b8e4,0x009b9be5,0x0094bae4,0x00ad85e5,0x0083b8e4,0x00ab85e5,0x009db9e4};
    #elif defined(_WIN64)||defined(_WIN32)
    int s=2;
    int dic[10]={0x000096a9,0x0000bbd2,0x0000feb6,0x0000fdc8,0x0000c4cb,0x0000e5ce,0x0000f9c1,0x0000dfc6,0x0000cbb0,0x0000c5be};
    #else
    int s=0;
    int dic[10]={0,0,0,0,0,0,0,0,0,0};
    #endif
    
    int n;int flag=0;
    char *p=morn_shu;
    n=num/1000;if (n>0)             {flag=1;memcpy(p,dic+n,s);p=p+s;num=num-n*1000;}
    n=num/100 ;if((n>0)||(flag==1)) {flag=1;memcpy(p,dic+n,s);p=p+s;num=num-n*100 ;}
    n=num/10  ;if((n>0)||(flag==1)) {flag=1;memcpy(p,dic+n,s);p=p+s;num=num-n*10  ;}
    n=num     ;if((n>0)||(flag==1)) {flag=1;memcpy(p,dic+n,s);p=p+s;}
    p[0]=0;
    return morn_shu;
}



char morn_time_string[128];
const char *m_TimeString(int64_t time_value,const char *format)
{
    int64_t tv=(time_value==DFLT)?(int64_t)time(NULL):time_value;
    int ty = 0;
    if(tv<0)
    {
        ty=(0-tv)/((366+365*3)*7*24*3600)+1;
        tv+=ty*(366+365*3)*7*24*3600;
        ty=ty*28;
    }
    struct tm *t=localtime(&tv);
    if(format==NULL) format = "%Y.%02M.%02D %02H:%02m:%02S";

    char *wday[7]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
    char *month[12]={"January","February","March","April","May","June","July","August","September","October","November","December"};
    char *awday[7]={"Sun","Mon","Tues","Wed","Thur","Fri","Sat"};
    char *amonth[12]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sept","Oct","Nov","Dec"};

    
    #if defined(__linux__)
    int cwday[7]={0x00a597e6,0x0080b8e4,0x008cbae4,0x0089b8e4,0x009b9be5,0x0094bae4,0x00ad85e5};
    #elif defined(_WIN64)||defined(_WIN32)
    int cwday[7]={0x0000d5c8,0x0000bbd2,0x0000feb6,0x0000fdc8,0x0000c4cb,0x0000e5ce,0x0000f9c1};
    #else
    int cwday[7]={0,0,0,0,0,0,0};
    #endif
    char cy[16],cM[16],cd[8 ],ch[16],cm[16],cs[16];

    char str[256];strcpy(str,format);
    intptr_t d[16];int n=0;
    char *p,*q;
    for(p=str;*p!=0;p++)
    {
        if(n>=16)break;
        if(*p=='/') {p++;continue;}
        if(*p=='%')
        {
            for(q=p+1;*q!=0;q++)
            {
                if((*q>='0')&&(*q<='9')) continue;
                // if(*q=='.') continue;
                     if(*q=='Y') {d[n++]=t->tm_year+1900-ty; *q='d';}
                else if(*q=='M') {d[n++]=t->tm_mon+1;*q='d';}
                else if(*q=='W') {d[n++]=t->tm_wday; *q='d';}
                else if(*q=='D') {d[n++]=t->tm_mday; *q='d';}
                else if(*q=='H') {d[n++]=t->tm_hour; *q='d';}
                else if(*q=='m') {d[n++]=t->tm_min ; *q='d';}
                else if(*q=='S') {d[n++]=t->tm_sec ; *q='d';}
                else if((q[0]=='s')&&(q[1]=='M')) {d[n++]=(intptr_t)( month[t->tm_mon ]);q[0]='h';q[1]='s';q++;}
                else if((q[0]=='a')&&(q[1]=='M')) {d[n++]=(intptr_t)(amonth[t->tm_mon ]);q[0]='h';q[1]='s';q++;}
                else if((q[0]=='s')&&(q[1]=='W')) {d[n++]=(intptr_t)(  wday[t->tm_wday]);q[0]='h';q[1]='s';q++;}
                else if((q[0]=='a')&&(q[1]=='W')) {d[n++]=(intptr_t)( awday[t->tm_wday]);q[0]='h';q[1]='s';q++;}
                else if((q[0]=='C')&&(q[1]=='W')) {d[n++]=(intptr_t)(&cwday[t->tm_wday]);q[0]='h';q[1]='s';q++;}
                else if((q[0]=='C')&&(q[1]=='Y')) {strcpy(cy,CHNYearString(t->tm_year+1900-ty));d[n++]=(intptr_t)cy;q[0]='h';q[1]='s';q++;}
                else if((q[0]=='C')&&(q[1]=='M')) {strcpy(cM,mShu((double)(t->tm_mon+1)));d[n++]=(intptr_t)cM;q[0]='h';q[1]='s';q++;}
                else if((q[0]=='C')&&(q[1]=='D')) {strcpy(cd,mShu((double)(t->tm_mday )));d[n++]=(intptr_t)cd;q[0]='h';q[1]='s';q++;}
                else if((q[0]=='C')&&(q[1]=='H')) {strcpy(ch,mShu((double)(t->tm_hour )));d[n++]=(intptr_t)ch;q[0]='h';q[1]='s';q++;}
                else if((q[0]=='C')&&(q[1]=='m')) {strcpy(cm,mShu((double)(t->tm_min  )));d[n++]=(intptr_t)cm;q[0]='h';q[1]='s';q++;}
                else if((q[0]=='C')&&(q[1]=='S')) {strcpy(cs,mShu((double)(t->tm_sec  )));d[n++]=(intptr_t)cs;q[0]='h';q[1]='s';q++;}
                else mException(1,EXIT,"invalid format");
                break;
            }
            p=q;
        }
    }
    sprintf(morn_time_string,str,d[0],d[1],d[2],d[3],d[4],d[5],d[6],d[7],d[8],d[9],d[10],d[11],d[12],d[13],d[14],d[15]);
    return morn_time_string;
}

int CHNNum(char *str)
{
    #if defined(__linux__)
    int s=3;
    int dic[11]={0x00b69be9,0x0080b8e4,0x008cbae4,0x0089b8e4,0x009b9be5,0x0094bae4,0x00ad85e5,0x0083b8e4,0x00ab85e5,0x009db9e4,0x00818de5};
    #elif defined(_WIN64)||defined(_WIN32)
    int s=2;
    int dic[11]={0x0000e3c1,0x0000bbd2,0x0000feb6,0x0000fdc8,0x0000c4cb,0x0000e5ce,0x0000f9c1,0x0000dfc6,0x0000cbb0,0x0000c5be,0x0000aeca};
    #else
    int s=0;
    int dic[11]={0,0,0,0,0,0,0,0,0,0,0};
    #endif

    int num = 0;
    for(int i=0;str[i]!=0;i+=s)
    {
        if(memcmp(str+i,dic+10,s)==0) {if(num==0) {num=1;}num=num*10;continue;}
        if(memcmp(str+i,dic+ 0,s)==0) continue;
        int j;for(j=1;j<10;j++)
        {
            if(memcmp(str+i,dic+j,s)==0) {num+=j;break;}
        }
        if(j==10) break;
    }
    return num;
}

int CHNYear(char *str)
{
    #if defined(__linux__)
    int s=3;
    int dic[11]={0x00b69be9,0x0080b8e4,0x008cbae4,0x0089b8e4,0x009b9be5,0x0094bae4,0x00ad85e5,0x0083b8e4,0x00ab85e5,0x009db9e4,0x008780e3};
    #elif defined(_WIN64)||defined(_WIN32)
    int s=2;
    int dic[11]={0x0000e3c1,0x0000bbd2,0x0000feb6,0x0000fdc8,0x0000c4cb,0x0000e5ce,0x0000f9c1,0x0000dfc6,0x0000cbb0,0x0000c5be,0x000096a9};
    #else
    int s=0;
    int dic[11]={0,0,0,0,0,0,0,0,0,0,0};
    #endif

    int num = 0;
    for(int i=0;str[i]!=0;i+=s)
    {
        int j;for(j=0;j<11;j++)
        {
            if(memcmp(str+i,dic+j,s)==0){num=num*10+j%10;break;}
        }
        if(j==11) break;
    }
    return num;
}

void _CString(const char *in,char *out)
{
    #if defined(__linux__)
    int s=3;
    int dic[13]={0x0080b8e4,0x008cbae4,0x008780e3,0x0089b8e4,0x00b69be9,0x009b9be5,0x0094bae4,0x00ad85e5,0x0083b8e4,0x00ab85e5,0x009db9e4,0x00818de5,0x00a597e6};
    #elif defined(_WIN64)||defined(_WIN32)
    int s=2;
    int dic[13]={0x0000bbd2,0x0000feb6,0x000096a9,0x0000fdc8,0x0000e3c1,0x0000c4cb,0x0000e5ce,0x0000f9c1,0x0000dfc6,0x0000cbb0,0x0000c5be,0x0000aeca,0x0000d5c8};
    #else
    int s=0;
    int dic[13]={0,0,0,0,0,0,0,0,0,0,0,0,0};
    #endif

    int8_t *p;int n;
    for(p=(int8_t *)in,n=0;*p!=0;p++) 
    {
        // printf("p[%d]=%d,%c\n",n,p[0],p[0]);
        if(p[0]<0)
        {
            int i;for(i=0;i<13;i++)
                if(memcmp(p,dic+i,s)==0)
                    {memcpy(out+n,p,s);n+=s;p+=s-1;break;}
            
            if(i==13) out[n++]=' ';
        }
        else out[n++]=p[0];
        
    }
    out[n++]=' ';out[n++]=0;
}

int64_t m_StringTime(char *string,const char *fmt)
{
    if(string == NULL) return time(NULL);
    
    const char *p,*q;
    int n=0,m=0;

    char     in[128]; _CString(string ,in);
    char format[128]; 
    if(fmt==NULL) strcpy(format,"%Y.%02M.%02D %02H:%02m:%02S ");
    else _CString(fmt,format);

    int day=DFLT,month=DFLT,year=DFLT,week=DFLT,hour=0,minute=0,second=0;
    char C_year[32],C_week[32],C_month[32],C_day[32],C_hour[32],C_minute[32],C_second[32];
    char s_week[16],s_month[16];
    int C_year_valid=0,C_month_valid=0,C_day_valid=0,C_hour_valid=0,C_minute_valid=0,C_second_valid=0,s_month_valid=0;
    char *amonth[12]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

    void *ptr[16];
    char str[128];memset(str,0,128);
    
    n=0,m=0;
    for(p=format;*p!=0;p++)
    {
        if((n>=16)||(m>=128))break;
        str[m]=*p;m++;
        if(*p=='/') {p++;continue;}
        if(*p=='%')
        {
            for(q=p+1;*q!=0;q++)
            {
                if((*q>='0')&&(*q<='9')) continue;
                     if(*q=='Y') {ptr[n++]=&year  ;str[m++]='d';}
                else if(*q=='M') {ptr[n++]=&month ;str[m++]='d';}
                else if(*q=='W') {ptr[n++]=&week  ;str[m++]='d';}
                else if(*q=='D') {ptr[n++]=&day   ;str[m++]='d';}
                else if(*q=='H') {ptr[n++]=&hour  ;str[m++]='d';}
                else if(*q=='m') {ptr[n++]=&minute;str[m++]='d';}
                else if(*q=='S') {ptr[n++]=&second;str[m++]='d';}
                else if((q[0]=='s')||(q[0]=='a')||(q[0]=='C'))//&&((q[1]=='M')||(q[1]=='W'))))
                {
                    if((q[0]=='s')||(q[0]=='a')) 
                    {
                        if(q[1]=='M') {ptr[n++]=s_month;s_month_valid=1;}
                        else          {ptr[n++]=s_week;}
                    }
                    else if(q[0]=='C') 
                    {
                             if(q[1]=='Y') {ptr[n++]=C_year;   C_year_valid  =1;}
                        else if(q[1]=='M') {ptr[n++]=C_month;  C_month_valid =1;}
                        else if(q[1]=='D') {ptr[n++]=C_day;    C_day_valid   =1;}
                        else if(q[1]=='H') {ptr[n++]=C_hour;   C_hour_valid  =1;}
                        else if(q[1]=='m') {ptr[n++]=C_minute; C_minute_valid=1;}
                        else if(q[1]=='S') {ptr[n++]=C_second; C_second_valid=1;}
                        else               {ptr[n++]=C_week;}
                    }
                    str[m++]='[';
                    str[m++]='^';str[m++]=q[2];
                    str[m++]=']';
                    q++;
                }
                else mException(1,EXIT,"invalid format");
                break;
            }
            p=q;
        }
    }
    // printf("%s\n",str);
    sscanf(in,str,ptr[0],ptr[1],ptr[2],ptr[3],ptr[4],ptr[5],ptr[6],ptr[7],ptr[8],ptr[9],ptr[10],ptr[11],ptr[12],ptr[13],ptr[14],ptr[15]);
    // printf("C_year=%s,C_month=%s,C_day=%s,C_hour=%s,C_minute=%s\n",C_year,C_month,C_day,C_hour,C_minute);
    if(s_month_valid  ) {s_month[3]=0; for(int i=0;i<12;i++) {if(stricmp(s_month,amonth[i])==0) {month=i+1;break;}}}
    if(C_year_valid   ) year  = CHNYear(C_year );// printf("year  =%d\n",year  );
    if(C_month_valid  ) month = CHNNum(C_month );// printf("month =%d\n",month );
    if(C_day_valid    ) day   = CHNNum(C_day   );// printf("day   =%d\n",day   );
    if(C_hour_valid   ) hour  = CHNNum(C_hour  );// printf("hour  =%d\n",hour  );
    if(C_minute_valid ) minute= CHNNum(C_minute);// printf("minute=%d\n",minute);
    if(C_second_valid ) second= CHNNum(C_second);// printf("second=%d\n",second);
    if((month<0)||(day<0)) return DFLT;
    int64_t td=0; if(year<1972) {td=((1972-year)*365+(1972-year)/4+(((1972-year)%4!=0)&&(month>2)))*24*3600;year=1972;}
    struct tm t;t.tm_year=year-1900; t.tm_mon=month-1;t.tm_mday=day;t.tm_hour=hour;t.tm_min=minute;t.tm_sec=second;
    return (int64_t)mktime(&t)-td;
}
