#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<curl/curl.h>

#define ح١ 50000
#define ح٢ 60000
#define ح٣ 20000

static char لصق[ح١+1];static size_t طوللصق=0;
static char مصري[ح٢+1],ليت[ح٢+1],عربي[ح٢*3+1],نظيف[ح٢*3+1],مشفّر[ح٢*3+1];

size_t كاتب١(void*بيانات,size_t حجم,size_t عدد,void*مستعمل){
    size_t ك=حجم*عدد,*طول=(size_t*)مستعمل;
    if(*طول+ك>ح١)ك=ح١-*طول;
    memcpy(لصق+*طول,بيانات,ك);
    *طول+=ك;
    return ك;
}

size_t كاتب٢(void*بيانات,size_t حجم,size_t عدد,void*مستعمل){
    return fwrite(بيانات,حجم,عدد,(FILE*)مستعمل);
}

static void ترميز_رابط(const char*دخ,char*خ){
    static const char سداسي[]="0123456789ABCDEF";
    while(*دخ){
        unsigned char حركه=(unsigned char)*دخ++;
        if((حركه>='a'&&حركه<='z')||(حركه>='A'&&حركه<='Z')||(حركه>='0'&&حركه<='9')){
            *خ++=حركه;
        }else if(حركه==' '){
            *خ++='+';
        }else{
            *خ++='%';
            *خ++=سداسي[حركه>>4];
            *خ++=سداسي[حركه&15];
        }
    }
    *خ=0;
}

int main(){
    char رابط[2048];
    printf("Pastebin URL (استعمل /raw/...): ");
    fgets(رابط,sizeof(رابط),stdin);
    رابط[strcspn(رابط,"\n")]=0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL*مقبض=curl_easy_init();

    طوللصق=0;
    curl_easy_setopt(مقبض,CURLOPT_URL,رابط);
    curl_easy_setopt(مقبض,CURLOPT_FOLLOWLOCATION,1L);
    curl_easy_setopt(مقبض,CURLOPT_WRITEFUNCTION,كاتب١);
    curl_easy_setopt(مقبض,CURLOPT_WRITEDATA,&طوللصق);
    curl_easy_perform(مقبض);
    لصق[طوللصق]=0;

    printf("\n==================== RAW PASTEBIN ====================\n%s\n======================================================\n\n",لصق);

    size_t ف=0,س=0;
    for(س=0;س<طوللصق&&ف<ح٢-2;س++){
        char ح=لصق[س];
        if(ح>='A'&&ح<='Z')ح+=32;
        switch(ح){
            case 'a':مصري[ف++]='a';مصري[ف++]='h';break;
            case 'e':مصري[ف++]='e';مصري[ف++]='e';break;
            case 'o':مصري[ف++]='o';مصري[ف++]='o';break;
            case 'h':مصري[ف++]='7';break;
            case 'g':مصري[ف++]='g';مصري[ف++]='h';break;
            default:مصري[ف++]=ح;break;
        }
    }
    مصري[ف]=0;
    printf("================= EGYPTIANIFIED TEXT =================\n%s\n======================================================\n\n",مصري);

    size_t ل=0;
    for(س=0;مصري[س]&&ل<ح٢-1;س++){
        char ح=مصري[س];
        if(ح=='e'||ح=='E')ح='3';
        else if(ح=='o'||ح=='O')ح='0';
        ليت[ل++]=ح;
    }
    ليت[ل]=0;
    printf("====================== LEET TEXT =====================\n%s\n======================================================\n\n",ليت);

    size_t ع=0;
    عربي[0]=0;
    for(س=0;ليت[س]&&ع<sizeof(عربي)-8;س++){
        char ح=ليت[س];
        const char*بديل=0;
        switch(ح){
            case 'a':بديل="ا";break;
            case 'b':بديل="ب";break;
            case 't':بديل="ت";break;
            case 'j':بديل="ج";break;
            case '7':بديل="ح";break;
            case 'd':بديل="د";break;
            case 'r':بديل="ر";break;
            case 's':بديل="س";break;
            case '3':بديل="ع";break;
            case 'f':بديل="ف";break;
            case 'q':بديل="ق";break;
            case 'k':بديل="ك";break;
            case 'l':بديل="ل";break;
            case 'm':بديل="م";break;
            case 'n':بديل="ن";break;
            case 'h':بديل="ه";break;
            case 'w':بديل="و";break;
            case 'y':بديل="ي";break;
            case '0':بديل="٠";break;
            case ' ':بديل=" ";break;
            case '\t':بديل=" ";break;
            default:
                عربي[ع++]=ح;
                عربي[ع]=0;
                continue;
        }
        if(بديل){
            size_t طبديل=strlen(بديل);
            if(ع+طبديل<sizeof(عربي)-1){
                memcpy(عربي+ع,بديل,طبديل);
                ع+=طبديل;
                عربي[ع]=0;
            }
        }
    }
    عربي[ع]=0;
    printf("==================== ARABIC-LIKE TEXT ================\n%s\n======================================================\n\n",عربي);

    strncpy(نظيف,عربي,sizeof(نظيف)-1);
    نظيف[sizeof(نظيف)-1]=0;
    for(س=0;نظيف[س];س++){
        unsigned char ح=(unsigned char)نظيف[س];
        if(ح=='\n'||ح=='\r')نظيف[س]=' ';
        if(ح=='<'||ح=='>'||ح=='\"'||ح=='?'||ح=='&'||ح=='#')نظيف[س]=' ';
    }
    if(strlen(نظيف)>150)نظيف[150]=0;
    printf("==================== SANITIZED TEXT ==================\n%s\n======================================================\n\n",نظيف);

    ترميز_رابط(نظيف,مشفّر);
    printf("================== URL-ENCODED TEXT ==================\n%s\n======================================================\n\n",مشفّر);

    char رابط_صوت[ح٣];
    snprintf(رابط_صوت,sizeof(رابط_صوت),
        "https://translate.google.com/translate_tts?ie=UTF-8&q=%s&tl=ar&client=tw-ob",
        مشفّر);
    printf("===================== GOOGLE URL =====================\n%s\n======================================================\n\n",رابط_صوت);

    struct curl_slist*رؤوس=NULL;
    رؤوس=curl_slist_append(رؤوس,"User-Agent: Mozilla/5.0");
    رؤوس=curl_slist_append(رؤوس,"Accept: */*");
    رؤوس=curl_slist_append(رؤوس,"Referer: https://translate.google.com/");
    رؤوس=curl_slist_append(رؤوس,"Connection: keep-alive");

    FILE*ملفmp3=fopen("output.mp3","wb");
    curl_easy_setopt(مقبض,CURLOPT_HTTPHEADER,رؤوس);
    curl_easy_setopt(مقبض,CURLOPT_URL,رابط_صوت);
    curl_easy_setopt(مقبض,CURLOPT_WRITEFUNCTION,كاتب٢);
    curl_easy_setopt(مقبض,CURLOPT_WRITEDATA,ملفmp3);
    CURLcode نتيجه=curl_easy_perform(مقبض);
    fclose(ملفmp3);

    if(نتيجه!=CURLE_OK){
        printf("TTS FAIL: %s\n",curl_easy_strerror(نتيجه));
        return 0;
    }
    printf("OK: output.mp3 saved\n");

    FILE*فmp3=fopen("output.mp3","rb");
    printf("\n==================== MP3 FIRST BYTES ==================\n");
    for(س=0;س<50;س++){
        int ح=fgetc(فmp3);
        if(ح<0)break;
        printf("%02X ",ح&0xFF);
    }
    fclose(فmp3);
    printf("\n======================================================\n");

    {
        FILE*دخول=fopen("output.mp3","rb");
        FILE*خروج=fopen("output.mp4","wb");
        char عازل[4096];size_t ق;
        while((ق=fread(عازل,1,sizeof(عازل),دخول))>0)fwrite(عازل,1,ق,خروج);
        fclose(دخول);fclose(خروج);
        printf("Saved cursed output.mp4\n");
    }

    curl_slist_free_all(رؤوس);
    curl_easy_cleanup(مقبض);
    curl_global_cleanup();
    return 0;
}
