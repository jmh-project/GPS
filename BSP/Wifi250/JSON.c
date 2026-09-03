/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"

#define     MEMNAME_POS           10
#define     MEMGENDER_POS        12

#define     MEMWEIGHT_POS         11

#define     MEMAGE_POS              8
#define     CURRSERCERTIME_POS   17

#define     ITEMCODE_POS            10
#define     STDVALUE1_POS           11
#define     STDVALUE2_POS           11
#define     STDVALUE3_POS           11
#define     STDVALUE4_POS           11
#define     AGEGENDERAGE_POS     14

//날짜와 시간을 저장하는 구조체
typedef struct
{
    int     Year;
    int     Month;
    int     Day;
    int     Hour;
    int     Minute;
    int     Second;
} DateTime;

//처방데이터를 저장하는 구조체
typedef struct 
{
    int     itemCode;
    float   stdValue1;
    float   stdValue2;
    float   stdValue3;
    float   stdValue4;
    float   avgGenderAge;    
} __stdValueData;


//서버에서 송신된 결과를 저장하는 구조체
typedef struct
{
    char    Name[12];   //이름
    char    Sex;           //성별
    float    Weight;      //몸무게  
    int       Age;         //나이
    DateTime    ServerTime;
    int     Count_std;   //가지고있는 결과물의 총수
    __stdValueData    stdValueData[20]; //결과물의 배열 최대 20개로지정
} JSON_Result;

const char *Test1 = "{\“jsonrpc\”:\”2.0\”, \”result\”: {\“memAuthKey\”:1,\“memName\”:\” \”,\“memGender\”:\”F\”,\“memWeight\”:\”47.8\”,\“memAge\”:27,\“currServerTime\”:\”20140501170240”\“stdValueList\”:[{\“itemCode\”:13,\“stdValue1\”:10,\“stdValue2\”:15,\“stdValue3\”:20,\“stdValue4\”:25,\“avgGenderAge\”:22.1,},{“itemCode\”:11,“stdValue1\”:30,“stdValue2\”:35.1,“stdValue3\”:45.4,“stdValue4\”:55.1,“avgGenderAge\”:43.2,}]},\“error\”:{\“code\”:\”00000\”,\“message\”:\”\”,\“data\”:null },  }";
JSON_Result     gJSON_Result; //서버에서 데이터를 받은것을 분석해서 여기에 집어넣는다.
const char *Test2 = "{\"jsonrpc\":\"2.0\",\"result\":{\"memAge\":25,\"memName\":\" ,\"memGender\":\"M\",\"currServerTime\":\"20140502161927\",\"memWeight\":78,\"stdValueList\":[{\"avgGenderAge\":37.0,\"itemCode\":13,\"stdValue1\":30.0,\"stdValue3\":40.0,\"stdValue2\":34.0,\"stdValue4\":48.0},{\"avgGenderAge\":284.0,\"itemCode\":11,\"stdValue1\":406.0,\"stdValue3\":248.0,\"stdValue2\":320.0,\"stdValue4\":201.0}],\"memAuthKey\":1},\"error\":{\"code\":0,\"message\":\"\",\"data\":null},\"id\":1}";
const char *Test = "{\"jsonrpc\":\"2.0\",\"result\":{\"memAge\":25,\"memName\":\"?????????\",\"memGender\":\"M\",\"currServerTime\":\"20140502172349\",\"memWeight\":78,\"stdValueList\":[{\"avgGenderAge\":37.0,\"itemCode\":13,\"stdValue1\":30.0,\"stdValue3\":40.0,\"stdValue2\":34.0,\"stdValue4\":48.0},{\"avgGenderAge\":284.0,\"itemCode\":11,\"stdValue1\":406.0,\"stdValue3\":248.0,\"stdValue2\":320.0,\"stdValue4\":201.0}],\"memAuthKey\":1},\"error\":{\"code\":0,\"message\":\"\",\"data\":null},\"id\":1}";

//char    JSON_BUF[1024];

int StrSearch(char *source, char *name)
{
    int     i, j, cnt, t;
    char    tbuf[64];
    char    cbuf[32];
    int src_len = strlen(source);
    int name_len = strlen(name);
    
    Dprintf("String Search TextLen=%d Searchname=%s\n", strlen(source), name);    
    for(i=0; i<src_len; i++)
    {
        cnt=0;
        for(j=i; j<i+name_len; j++)
        {
            tbuf[cnt++]=source[j];
        }
        tbuf[cnt]=0;
        if(!strcmp(tbuf, name))
        {
            for(t=0; t<name_len; t++)
            {
                cbuf[t] = source[i+t];
            }
            cbuf[t]=0;
            Dprintf("\t--->Search OK[%s] = %d\n", cbuf, i);
            return(i);
        }
    }
    Dprintf("\t--->Search Error\n");
    return(-1);
}

int is_JSON_Format(char *Text)
{
    int     result;
    result=  StrSearch(Text, "jsonrpc");
    //Dprintf("JSON Format Compare\n");
    if(result !=-1)
    {
        //Dprintf("\t--->JSON Format OK\n");
        return(1);
    }
    else
    {
        //Dprintf("\t--->This is not JSON Format");
        return(0);
    }
}


int JSON_Get_Name(char *Text, JSON_Result *Data)
{
    int  i;
    int cnt=0;
    char    name[12];
    
    cnt = StrSearch(Text, "memName");
    //Dprintf("JSON_Get_Name:: Start______\n");
    if( cnt != -1)
    {
        //Dprintf("\t--->FirstChar=%c%c%c%c%c\n", Text[cnt], Text[cnt+1], Text[cnt+2], Text[cnt+7], Text[cnt+8] );
        for(i=cnt+MEMNAME_POS; i<cnt+30; i++)
        {
            if(Text[i] == ',') 
            {
                //Dprintf("\t Found End Char i=%d\n", i);
                n=0;
                for(int j=cnt+MEMNAME_POS; j<i; j++)
                {
                    name[n++] = Text[j];
                }
                name[n]=0;
                strcpy(Data->Name, name);
                //Dprintf("\tJSON_Get_Name::Find Name=%s\n", Data->Name);
                return(1);
            }
        }
        //Dprintf("\tJSON_Get_Name::Not Found Name\n");
    }
    return(-1);
}



int JSON_Get_Sex(char *Text, JSON_Result *Data)
{
//    int  i, j;
    int cnt=0;
//    char    name[12];
    
    cnt = StrSearch(Text, "memGender");
    if( cnt != -1)
    {
        //Dprintf("Sex=%c\n", Text[cnt+14]);
        Data->Sex =  Text[cnt+MEMGENDER_POS]; 
        /*
        if(Data->Sex=='F') 
            Dprintf("This is Woman\n");
        else if(Data->Sex=='M') 
            Dprintf("This is Man\n");
        */
        return(1);
    }
    return(0);
}

int JSON_Get_Time(char *Text, JSON_Result *Data)
{
    //int       i, j;
    int      cnt=0;
    char    buf[32];
    int       offset = CURRSERCERTIME_POS;
    
    cnt = StrSearch(Text, "currServerTime");
    if( cnt != -1)
    {
        buf[0] = Text[cnt+offset++];  buf[1] = Text[cnt+offset++];   buf[2] = Text[cnt+offset++];  buf[3] = Text[cnt+offset++];  buf[4] = 0;
        Data->ServerTime.Year = atoi(buf);
        buf[0] = Text[cnt+offset++];  buf[1] = Text[cnt+offset++];   buf[2] = 0;
        Data->ServerTime.Month = atoi(buf);
        buf[0] = Text[cnt+offset++];  buf[1] = Text[cnt+offset++];   buf[2] = 0;
        Data->ServerTime.Day = atoi(buf);
        buf[0] = Text[cnt+offset++];  buf[1] = Text[cnt+offset++];   buf[2] = 0;
        Data->ServerTime.Hour = atoi(buf);
        buf[0] = Text[cnt+offset++];  buf[1] = Text[cnt+offset++];   buf[2] = 0;
        Data->ServerTime.Minute = atoi(buf);
        buf[0] = Text[cnt+offset++];  buf[1] = Text[cnt+offset++];   buf[2] = 0;
        Data->ServerTime.Second= atoi(buf);
        
        //Dprintf("%d %d %d %d %d %d\n", Data->ServerTime.Year, Data->ServerTime.Month ,Data->ServerTime.Day, Data->ServerTime.Hour, Data->ServerTime.Minute, Data->ServerTime.Second);
        return(1);
    }
    return(0);
}

int JSON_Get_Weight(char *Text, JSON_Result *Data)
{
    int  i, j;
    int cnt=0, n=0;
    char    name[12];
    
    cnt = StrSearch(Text, "memWeight");
    if( cnt != -1)
    {
        Dprintf("\t--->FirstChar=%c%c%c%c%c\n", Text[cnt], Text[cnt+1], Text[cnt+2], Text[cnt+7], Text[cnt+8] );
        for(i=cnt; i<cnt+30; i++)
        {
            if(Text[i] == ',') 
            {
                Dprintf("\t Found End Char i=%d\n", i);
                n=0;
                for(j=cnt+MEMWEIGHT_POS; j<i; j++)
                {
                    name[n++] = Text[j];
                }
                name[n]=0;
                Data->Weight = atof(name);
                Dprintf("\tWeight=%7.1f %s\n", Data->Weight, name);
                return(1);
            }
        }
    }
    return(-1);
}


int JSON_Get_Age(char *Text, JSON_Result *Data)
{
    int  i, j;
    int cnt=0, n=0;
    char    name[12];
    
    cnt = StrSearch(Text, "memAge");
    if( cnt != -1)
    {
        //Dprintf("\t--->FirstChar=%c%c%c%c%c\n", Text[cnt], Text[cnt+1], Text[cnt+2], Text[cnt+7], Text[cnt+8] );
        for(i=cnt+MEMAGE_POS; i<cnt+30; i++)
        {
            if(Text[i] == ',') 
            {
                Dprintf("\t Found End Char i=%d\n", i);
                n=0;
                for(j=cnt+MEMAGE_POS; j<i; j++)
                {
                    name[n++] = Text[j];
                }
                name[n]=0;
                Data->Age = atoi(name);
                Dprintf("\tAge=%3d %s \n", Data->Age, name);
                return(1);
            }
        }
    }
    return(-1);
}

//주어진 문자열에서 처방항목이 몇개인지를 검사한후 지정된 항목의 데이터를 읽어낸다.
int JSON_Get_Item(char *source, char *name, JSON_Result *Data)
{
    int     i, j, cnt, t, loop;
    char    tbuf[64];
    char    cbuf[32];
    char    buf[12];    
    int     start_cnt=0;
    int     search_cnt=0;
    int     k, h, n;
    
    int src_len = strlen(source);
    int name_len = strlen(name);
    loop=0;
    
    for(i=loop; i<src_len; i++)
    {
        cnt=0;
        for(j=i; j<i+name_len; j++)
        {
            tbuf[cnt++]=source[j];
        }
        tbuf[cnt]=0;
        if(!strcmp(tbuf, name))
        {
            for(t=0; t<name_len; t++)
            {
                cbuf[t] = source[i+t];
            }
            cbuf[t]=0;
            //Dprintf("\t--->Search OK[%s] = %d\n", cbuf, i);

            search_cnt = i;
            
            for(k=search_cnt; k<search_cnt+30; k++)
            {
                if(source[k] == ',') 
                {
                    if(!strcmp(name, "itemCode"))
                    {
                        n=0;
                        for(h=search_cnt+ITEMCODE_POS; h<k; h++)
                        {
                            buf[n++] = source[h];
                        }
                        buf[n]=0;                        
                        Data->stdValueData[start_cnt].itemCode = atoi(buf);
                        //Dprintf("%s=%d\n", name, Data->stdValueData[start_cnt].itemCode);
                    }
                    else if(!strcmp(name, "stdValue1"))
                    {
                        n=0;
                        for(h=search_cnt+STDVALUE1_POS; h<k; h++)
                        {
                            buf[n++] = source[h];
                        }
                        buf[n]=0;                        
                        Data->stdValueData[start_cnt].stdValue1 = atof(buf);
                        //Dprintf("%s=%f\n", name, Data->stdValueData[start_cnt].stdValue1);
                    }
                    else if(!strcmp(name, "stdValue2"))
                    {
                        n=0;
                        for(h=search_cnt+STDVALUE2_POS; h<k; h++)
                        {
                            buf[n++] = source[h];
                        }
                        buf[n]=0;                        
                        Data->stdValueData[start_cnt].stdValue2 = atof(buf);
                        //Dprintf("%s=%f\n", name, Data->stdValueData[start_cnt].stdValue2);
                    }
                    else if(!strcmp(name, "stdValue3"))
                    {
                        n=0;
                        for(h=search_cnt+STDVALUE3_POS; h<k; h++)
                        {
                            buf[n++] = source[h];
                        }
                        buf[n]=0;                        
                        Data->stdValueData[start_cnt].stdValue3 = atof(buf);
                        //Dprintf("%s=%f\n", name, Data->stdValueData[start_cnt].stdValue3);
                    }
                    else if(!strcmp(name, "stdValue4"))
                    {
                        n=0;
                        for(h=search_cnt+STDVALUE4_POS; h<k; h++)
                        {
                            buf[n++] = source[h];
                        }
                        buf[n]=0;                        
                        Data->stdValueData[start_cnt].stdValue4 = atof(buf);
                        //Dprintf("%s=%f\n", name, Data->stdValueData[start_cnt].stdValue4);
                    }
                    else if(!strcmp(name, "avgGenderAge"))
                    {
                        n=0;
                        for(h=search_cnt+AGEGENDERAGE_POS; h<k; h++)
                        {
                            buf[n++] = source[h];
                        }
                        buf[n]=0;                        
                        Data->stdValueData[start_cnt].avgGenderAge = atof(buf);
                        //Dprintf("%s=%7.1f\n", name, Data->stdValueData[start_cnt].avgGenderAge);
                    }
                    break;
                }
            }
            loop = i+1;
            start_cnt++;
        }
    }
    Data->Count_std = start_cnt;
    //Dprintf("Total=%d\n", start_cnt);
    return(start_cnt);
}

char    JSON_Buf[20][50];

int JSON_Read_Item(char *source, char *name)
{
    int     i, j, cnt, t, loop;
    char    tbuf[64];
    char    cbuf[32];
    char    buf[64];    
    int     start_cnt=0;
    int     search_cnt=0;
    int     k, h, n;
    int     flag=0;
    int     Done=0;
    
    int src_len = strlen(source);
    int name_len = strlen(name);
    loop=0;
    Dprintf("--->JSON_Read_Item::Entry\n");
    for(i=loop; i<src_len; i++)
    {
        cnt=0;
        for(j=i; j<i+name_len; j++)
        {
            tbuf[cnt++]=source[j];
        }
        tbuf[cnt]=0;
        if(!strcmp(tbuf, name))
        {
            for(t=0; t<name_len; t++)
            {
                cbuf[t] = source[i+t];
            }
            cbuf[t]=0;
            Dprintf("\t--->Search OK[%s] = %d\n", cbuf, i);

            search_cnt = i;
            
            for(k=search_cnt; k<search_cnt+50; k++)
            {
                if(source[k] == ':') 
                {
                    Dprintf("\t--->Found collon=%d\n", k);
                    n=0;
                    h = k+1;
                    flag=0;
                    Done=0;
                    while(!Done)
                    {
                        switch(source[h])
                        {
                            case '"': Dprintf("\t--->Found Double\n");  if(flag==0) flag=1;  else Done=1;  break;
                            case ',': Dprintf("\t--->Found comma\n"); Done=1; break;
                            default: buf[n++] =source[h]; break;
                        }                        
                        h++;
                    }
                    buf[n]=0;                        
                    strcpy(JSON_Buf[start_cnt], buf);
                    Dprintf("\t--->%d. Found <%s>Data =>[%s]\n", start_cnt, name, JSON_Buf[start_cnt]);
                    break;
                }
            }
            loop = i+1;
            start_cnt++;
        }
    }
    Dprintf("--->Total %d Found\n", start_cnt);
    return(start_cnt);
}

void AllocationTest(void)
{
    char    *buf;
    buf = (char *)malloc(strlen("TEST"));
    if(buf!=NULL)
    {
        strcpy(buf,"TEST");
        Dprintf("Test->%s\n", buf);
        free(buf);
    }
    else
    {
        Dprintf("malloc error\n");
    }
}

void JSON_Test(void)
{
    int     i;
//    int     result;
    //JSON_Result     *pgJSON_Result = &gJSON_Result;
    //JSON_Result     *pgJSON_Result = (JSON_Result *)malloc(sizeof(JSON_Result));
    
    Dprintf("\n\n\nJSON Parser Test Start\n");

    
    JSON_Read_Item((char *)Test, "itemCode");
    AllocationTest();
    while(1);
    
    //주어진 문자열이 JSON Format 인지 확인한다.    
    if(is_JSON_Format((char *)Test))
    {
        //주어진 문자열에서 이름을 찾아서 기록한다.
        JSON_Get_Name((char *)Test, &gJSON_Result);
        Dprintf("Name=%s\n", gJSON_Result.Name); //찾은이름 재확인
        
        JSON_Get_Sex((char *)Test, &gJSON_Result);
        Dprintf("Sex=%c\n", gJSON_Result.Sex);
        
        JSON_Get_Weight((char *)Test, &gJSON_Result);    
        Dprintf("Weight=%7.1f\n", gJSON_Result.Weight);
        
        JSON_Get_Age((char *)Test, &gJSON_Result);    
        JSON_Get_Time((char *)Test, &gJSON_Result);    
        Dprintf("%d %d %d %d %d %d\n", gJSON_Result.ServerTime.Year, gJSON_Result.ServerTime.Month, gJSON_Result.ServerTime.Day, gJSON_Result.ServerTime.Hour, gJSON_Result.ServerTime.Minute, gJSON_Result.ServerTime.Second);
        
        JSON_Get_Item((char *)Test, "itemCode", &gJSON_Result);        
        for(i=0; i<gJSON_Result.Count_std; i++)
            Dprintf("itemcode=%d\n", gJSON_Result.stdValueData[i].itemCode);        
        
        JSON_Get_Item((char *)Test, "stdValue1", &gJSON_Result);
        for(i=0; i<gJSON_Result.Count_std; i++)
            Dprintf("itemcode=%7.1f\n", gJSON_Result.stdValueData[i].stdValue1);        

        JSON_Get_Item((char *)Test, "stdValue2", &gJSON_Result);
        JSON_Get_Item((char *)Test, "stdValue3", &gJSON_Result);
        JSON_Get_Item((char *)Test, "stdValue4", &gJSON_Result);
        JSON_Get_Item((char *)Test, "avgGenderAge", &gJSON_Result);     
        Dprintf("Total Result=%d\n", gJSON_Result.Count_std);
    }
    else
    {
        Dprintf("JSON FORMAT Error\n");
    }
        
    while(1);
}
