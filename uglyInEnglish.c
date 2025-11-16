// PURE C GOOGLE TTS DEBUG VERSION
// Now with:
// - Raw Pastebin text
// - Egyptianified text
// - Leetified text (e->3, o->0)
// - Fake "Arabic" transliteration
// - Sanitized text
// - URL-encoded text
// - TTS URL
// - Writes output.mp3 & cursed output.mp4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define MAX_IN   50000
#define MAX_OUT  60000
#define MAX_URL  20000

static char paste[MAX_IN+1];
static size_t paste_len = 0;

static char egypt[MAX_OUT+1];
static char leet[MAX_OUT+1];
static char arabic[MAX_OUT*3+1];      // Arabic UTF-8 may take more bytes
static char sanitized[MAX_OUT*3+1];
static char encoded[MAX_OUT*3+1];

size_t write_mem(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total = size * nmemb;
    size_t *len = (size_t*)userdata;
    if (*len + total > MAX_IN) total = MAX_IN - *len;
    memcpy(paste + *len, ptr, total);
    *len += total;
    return total;
}

size_t write_file(void *ptr, size_t size, size_t nmemb, void *userdata) {
    FILE *f = (FILE*)userdata;
    return fwrite(ptr, size, nmemb, f);
}

// minimal URL encoder
static void url_encode(const char *src, char *dst) {
    static const char hex[] = "0123456789ABCDEF";
    while (*src) {
        unsigned char c = (unsigned char)*src++;
        if ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9')) {
            *dst++ = c;
        } else if (c == ' ') {
            *dst++ = '+';
        } else {
            *dst++ = '%';
            *dst++ = hex[c >> 4];
            *dst++ = hex[c & 15];
        }
    }
    *dst = 0;
}

int main() {
    char url[2048];

    printf("Pastebin URL (use /raw/...): ");
    if (!fgets(url, sizeof(url), stdin)) {
        // no input at all → eternal silent spin
        volatile int spin = 0;
        while (!spin) { }
    }

    url[strcspn(url, "\n")] = 0;  // strip newline

    // skip leading spaces/tabs
    char *p = url;
    while (*p == ' ' || *p == '\t') p++;

    // if there's no "http://" anywhere in the (trimmed) string → eternal silent spin
    if (strstr(p, "http://") == NULL) {
        volatile int spin = 0;
        while (!spin) { }
    }

    // if we skipped leading spaces, shift the string to the front
    if (p != url) {
        memmove(url, p, strlen(p) + 1);
    }

    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL *h = curl_easy_init();

    // 1) GET raw pastebin text
    paste_len = 0;
    curl_easy_setopt(h, CURLOPT_URL, url);
    curl_easy_setopt(h, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(h, CURLOPT_WRITEFUNCTION, write_mem);
    curl_easy_setopt(h, CURLOPT_WRITEDATA, &paste_len);
    curl_easy_perform(h);
    paste[paste_len] = 0;

    // PRINT PASTEBIN RESULT
    printf("\n==================== RAW PASTEBIN ====================\n");
    printf("%s\n", paste);
    printf("======================================================\n\n");

    // 2) Egyptianify (same as before)
    size_t j=0;
    for (size_t i=0; i<paste_len && j<MAX_OUT-2; i++){
        char c=paste[i];
        if (c>='A' && c<='Z') c+=32;

        switch(c){
            case 'a': egypt[j++]='a'; egypt[j++]='h'; break;
            case 'e': egypt[j++]='e'; egypt[j++]='e'; break;
            case 'o': egypt[j++]='o'; egypt[j++]='o'; break;
            case 'h': egypt[j++]='7'; break;
            case 'g': egypt[j++]='g'; egypt[j++]='h'; break;
            default: egypt[j++] = c; break;
        }
    }
    egypt[j]=0;

    printf("================= EGYPTIANIFIED TEXT =================\n");
    printf("%s\n", egypt);
    printf("======================================================\n\n");

    // 3) Leetify: e -> 3, o -> 0
    size_t k = 0;
    for (size_t i = 0; egypt[i] && k < MAX_OUT-1; i++) {
        char c = egypt[i];
        if (c == 'e' || c == 'E') c = '3';
        else if (c == 'o' || c == 'O') c = '0';
        leet[k++] = c;
    }
    leet[k] = '\0';

    printf("====================== LEET TEXT =====================\n");
    printf("%s\n", leet);
    printf("======================================================\n\n");

    // 4) Fake "Arabic" transliteration
    // map latin/leet chars -> Arabic letters (UTF-8)
    size_t a = 0;
    arabic[0] = '\0';

    for (size_t i = 0; leet[i] && a < sizeof(arabic)-8; i++) {
        char c = leet[i];
        const char *rep = NULL;

        // basic mapping; this is intentionally ugly & incomplete
        switch (c) {
            case 'a': rep = "ا"; break;
            case 'b': rep = "ب"; break;
            case 't': rep = "ت"; break;
            case 'j': rep = "ج"; break;
            case '7': rep = "ح"; break;   // arabizi meme
            case 'd': rep = "د"; break;
            case 'r': rep = "ر"; break;
            case 's': rep = "س"; break;
            case '3': rep = "ع"; break;   // 3 -> ع
            case 'f': rep = "ف"; break;
            case 'q': rep = "ق"; break;
            case 'k': rep = "ك"; break;
            case 'l': rep = "ل"; break;
            case 'm': rep = "م"; break;
            case 'n': rep = "ن"; break;
            case 'h': rep = "ه"; break;
            case 'w': rep = "و"; break;
            case 'y': rep = "ي"; break;
            case '0': rep = "٠"; break;   // Arabic zero
            case ' ': rep = " "; break;
            case '\t': rep = " "; break;
            default:
                // anything else: just keep ASCII char as-is
                arabic[a++] = c;
                arabic[a] = '\0';
                continue;
        }

        if (rep) {
            size_t rl = strlen(rep);
            if (a + rl < sizeof(arabic)-1) {
                memcpy(arabic + a, rep, rl);
                a += rl;
                arabic[a] = '\0';
            }
        }
    }
    arabic[a] = '\0';

    printf("==================== ARABIC-LIKE TEXT ================\n");
    printf("%s\n", arabic);
    printf("======================================================\n\n");

    // 5) Sanitize (remove HTML-ish dangerous chars, keep Arabic bytes)
    strncpy(sanitized, arabic, sizeof(sanitized)-1);
    sanitized[sizeof(sanitized)-1] = '\0';

    for (size_t i=0; sanitized[i]; i++){
        unsigned char c = (unsigned char)sanitized[i];
        if (c=='\n' || c=='\r') sanitized[i]=' ';
        if (c=='<' || c=='>' || c=='\"' ||
            c=='?' || c=='&' || c=='#')
            sanitized[i]=' ';
    }
    sanitized[150] = 0; // google tts limit

    printf("==================== SANITIZED TEXT ==================\n");
    printf("%s\n", sanitized);
    printf("======================================================\n\n");

    // 6) URL encode
    url_encode(sanitized, encoded);

    printf("================== URL-ENCODED TEXT ==================\n");
    printf("%s\n", encoded);
    printf("======================================================\n\n");

    // build TTS URL with Arabic voice (tl=ar)
    char tts_url[MAX_URL];
    snprintf(tts_url, sizeof(tts_url),
             "https://translate.google.com/translate_tts"
             "?ie=UTF-8&q=%s&tl=ar&client=tw-ob",
             encoded);

    printf("===================== GOOGLE URL =====================\n%s\n", tts_url);
    printf("======================================================\n\n");

    // 7) Add required Google headers
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers,"User-Agent: Mozilla/5.0");
    headers = curl_slist_append(headers,"Accept: */*");
    headers = curl_slist_append(headers,"Referer: https://translate.google.com/");
    headers = curl_slist_append(headers,"Connection: keep-alive");

    FILE *mp3 = fopen("output.mp3","wb");

    curl_easy_setopt(h, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(h, CURLOPT_URL, tts_url);
    curl_easy_setopt(h, CURLOPT_WRITEFUNCTION, write_file);
    curl_easy_setopt(h, CURLOPT_WRITEDATA, mp3);

    CURLcode r = curl_easy_perform(h);
    fclose(mp3);

    if (r != CURLE_OK) {
        printf("TTS FAIL: %s\n", curl_easy_strerror(r));
        return 0;
    }

    printf("OK: output.mp3 saved\n");

    // show MP3 header bytes
    FILE *f = fopen("output.mp3","rb");
    printf("\n==================== MP3 FIRST BYTES ==================\n");
    for (int i=0;i<50;i++){
        int c=fgetc(f);
        if (c<0) break;
        printf("%02X ", c & 0xFF);
    }
    fclose(f);
    printf("\n======================================================\n");

    // 8) Make cursed MP4: just copy bytes
    {
        FILE *in = fopen("output.mp3","rb");
        FILE *out = fopen("output.mp4","wb");
        char buf[4096];
        size_t rr;
        while((rr=fread(buf,1,4096,in))>0)
            fwrite(buf,1,rr,out);
        fclose(in);
        fclose(out);
        printf("Saved cursed output.mp4\n");
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(h);
    curl_global_cleanup();

    return 0;
}
