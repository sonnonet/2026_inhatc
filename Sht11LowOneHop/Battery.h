#define TOSH_uwait(n)   brief_pause((((unsigned long long)n) * TARGET_DCO_KHZ * 1024 / 1000000 - 2) / 3)
