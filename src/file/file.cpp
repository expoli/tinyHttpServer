//
// Created by tcy on 2023/3/23.
//

#include "file.h"

const char * get_file_head_type(const char * file_name){
  const char * ret = "text/html";
  // 返回指向 C 字符串 str 中字符最后一次出现的指针。
  const char * p = strrchr(file_name,'.');
  if (!p)
    return ret;
  p++;
  if (!strcmp(p,"css"))
    ret = "text/css";
  if (!strcmp(p,"jpg"))
    ret = "image/jpeg";
  if (!strcmp(p,"png"))
    ret = "image/png";
  if (!strcmp(p,"js"))
    ret = "application/x-javascript";
  return ret;
}

/*列举一些常见的文件头，可以自行扩展，比如放到某个配置文件中*/
static const char *with_suffix[] = {".gz", ".rar", ".exe", ".bz2",
                                    ".tar", ".xz", ".Z", ".rpm", ".zip",
                                    ".a",   ".so", ".o", ".jar", ".dll",
                                    ".lib", ".deb", ".I", ".png",".jpg",
                                    ".mp3", ".mp4", ".m4a", ".flv", ".mkv",
                                    ".rmvb", ".avi",  ".pcap", ".pdf", ".docx",
                                    ".xlsx", ".pptx", ".ram", ".mid", ".dwg",
                                    NULL};

/*判断某个字符串是否拥有指定后缀*/
static int string_has_suffix(const char *str, const char *suffix) {
  int n, m, i = 0;
  char ch = '\0';
  if (str == NULL || suffix == NULL)
  {
    return 0;
  }
  n = strlen(str);
  m = strlen(suffix);
  if (n < m) {
    return 0;
  }
  for (i = m-1; i >= 0;  i--) {
    if (suffix[i] != str[n - m + i]) {
      return 0;
    }
  }
  return 1;
}

/*判断文件是否具有特殊后缀*/
int file_has_spec_suffix(const char *fname) {
  const char **suffix = NULL;
  suffix = with_suffix;
  while (*suffix)
  {
    if (string_has_suffix(fname, *suffix))
    {
      return 1;
    }
    suffix++;
  }
  return 0;
}
