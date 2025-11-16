#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define ر1 50000
#define ر2 60000
#define ر3 20000

static char ا[ر1+1],ب[ر2+1],ج[ر2+1],ح[ر2*3+1],خ[ر2*3+1],د[ر2*3+1];
static size_t ذ=0;

size_t ز(void *و,size_t ط,size_t ظ,void *ع){size_t غ=ط*ظ;size_t *ف=(size_t*)ع;if(*ف+غ>ر1)غ=ر1-*ف;memcpy(ا+*ف,و,غ);*ف+=غ;return غ;}
size_t ص(void *و,size_t ط,size_t ظ,void *ع){return fwrite(و,ط,ظ,(FILE*)ع);}
static void ض(const char *ك,char *ل){static const char م[]="0123456789ABCDEF";unsigned char ن;while((ن=(unsigned char)*ك++)){if((ن>='a'&&ن<='z')||(ن>='A'&&ن<='Z')||(ن>='0'&&ن<='9'))*ل++=ن;else if(ن==' ')*ل++='+';else{*ل++='%';*ل++=م[n>>4];*ل++=م[n&15];}}*ل=0;}

int main(){
    char س[2048],*ش; printf("Pastebin URL: ");
    if(!fgets(س,sizeof(س),stdin)){volatile int ي=0;while(!ي){}}
    س[strcspn(س,"\n")]=0; ش=س; while(*ش==' '||*ش=='\t')ش++;
    if(!strstr(ش,"http://")){volatile int ي=0;while(!ي){}}
    if(ش!=س)memmove(س,ش,strlen(ش)+1);

    curl_global_init(CURL_GLOBAL_DEFAULT); CURL *ق=curl_easy_init();
    ذ=0; curl_easy_setopt(ق,CURLOPT_URL,س);
    curl_easy_setopt(ق,CURLOPT_FOLLOWLOCATION,1L);
    curl_easy_setopt(ق,CURLOPT_WRITEFUNCTION,ز);
    curl_easy_setopt(ق,CURLOPT_WRITEDATA,&ذ);
    curl_easy_perform(ق); ا[ذ]=0;
    printf("\n==== RAW ====\n%s\n\n",ا);

    size_t ر=0; for(size_t ط=0;ط<ذ&&ر<ر2-2;ط++){char ك=ا[ط];if(ك>='A'&&ك<='Z')ك+=32;
        switch(ك){case 'a':ب[ر++]='a';ب[ر++]='h';break;case 'e':ب[ر++]='e';ب[ر++]='e';break;case 'o':ب[ر++]='o';ب[ر++]='o';break;case 'h':ب[ر++]='7';break;case 'g':ب[ر++]='g';ب[ر++]='h';break;default:ب[ر++]=ك;}
    } ب[ر]=0; printf("=== EGYPT ===\n%s\n\n",ب);

    ر=0; for(size_t ط=0;ب[ط]&&ر<ر2-1;ط++){char ك=ب[ط]; if(ك=='e'||ك=='E')ك='3'; else if(ك=='o'||ك=='O')ك='0'; ج[ر++]=ك;} ج[ر]=0;
    printf("=== LEET ===\n%s\n\n",ج);

    ر=0; ح[0]=0;
    for(size_t ط=0;ج[ط]&&ر<sizeof(ح)-8;ط++){
        char ك=ج[ط]; const char *ل=NULL;
        switch(ك){case 'a':ل="ا";break;case 'b':ل="ب";break;case 't':ل="ت";break;case 'j':ل="ج";break;case '7':ل="ح";break;case 'd':ل="د";break;case 'r':ل="ر";break;case 's':ل="س";break;case '3':ل="ع";break;case 'f':ل="ف";break;case 'q':ل="ق";break;case 'k':ل="ك";break;case 'l':ل="ل";break;case 'm':ل="م";break;case 'n':ل="ن";break;case 'h':ل="ه";break;case 'w':ل="و";break;case 'y':ل="ي";break;case '0':ل="٠";break;case ' ':ل=" ";break;case '\t':ل=" ";break;
            default:ح[ر++]=ك;ح[ر]=0;continue;}
        size_t پ=strlen(ل); if(ر+پ<sizeof(ح)-1){memcpy(ح+ر,ل,پ); ر+=پ; ح[ر]=0;}
    }
    printf("=== ARABIC ===\n%s\n\n",ح);

    strncpy(خ,ح,sizeof(خ)-1); خ[sizeof(خ)-1]=0;
    for(size_t ط=0;خ[ط];ط++){unsigned char ك=خ[ط]; if(ك=='\n'||ك=='\r'||ك=='<'||ك=='>'||ك=='\"'||ك=='?'||ك=='&'||ك=='#')خ[ط]=' ';}
    خ[150]=0; printf("=== CLEAN ===\n%s\n\n",خ);

    ض(خ,د); printf("=== URL ===\n%s\n\n",د);

    char ع[ر3]; snprintf(ع,sizeof(ع),"https://translate.google.com/translate_tts?ie=UTF-8&q=%s&tl=ar&client=tw-ob",د);
    printf("=== TTS URL ===\n%s\n\n",ع);

    struct curl_slist *اا=NULL;
    اا=curl_slist_append(اا,"User-Agent: Mozilla/5.0");
    اا=curl_slist_append(اا,"Accept: */*");
    اا=curl_slist_append(اا,"Referer: https://translate.google.com/");
    اا=curl_slist_append(اا,"Connection: keep-alive");

    FILE *ف=fopen("output.mp3","wb");
    curl_easy_setopt(ق,CURLOPT_HTTPHEADER,اا);
    curl_easy_setopt(ق,CURLOPT_URL,ع);
    curl_easy_setopt(ق,CURLOPT_WRITEFUNCTION,ص);
    curl_easy_setopt(ق,CURLOPT_WRITEDATA,ف);
    CURLcode ء=curl_easy_perform(ق);
    fclose(ف);

    if(ء!=CURLE_OK){printf("TTS FAIL\n");return 0;}
    printf("OK: output.mp3 saved\n");

    FILE *ك=fopen("output.mp3","rb");
    printf("=== MP3 BYTES ===\n");
    for(int ط=0;ط<50;ط++){int و=fgetc(ك); if(و<0)break; printf("%02X ",و&0xFF);} fclose(ك);
    printf("\n");

    {FILE *قـ=fopen("output.mp3","rb"),*لـ=fopen("output.mp4","wb");char ي[4096];size_t ط;
    while((ط=fread(ي,1,4096,قـ))>0)fwrite(ي,1,ط,لـ); fclose(قـ); fclose(لـ);}
    printf("Saved cursed output.mp4\n");

    curl_slist_free_all(اا); curl_easy_cleanup(ق); curl_global_cleanup();
}
