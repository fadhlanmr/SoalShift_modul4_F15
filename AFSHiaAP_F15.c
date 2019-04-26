#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static const char *dirpath = "/home/fdh/Documents";
char list[]={"qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0"};
char h[256],dh[256];

void encode(const char path[]){
    int i,j,k;
    strcpy(h,path);
    for(i=0;i<strlen(h)-1;i++){
        for(j=0;j<strlen(list);j++){
            if(h[i]==list[j])break;
        }
        if(h[i]!=(char)47){
            k = j+17;
            if(k>=94)k=k-94;
            h[i]=list[k];
        }
    }
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
  int res;
	char fpath[1000], final[1000];
    int i,j,k;
    strcpy(h,path);
    if(strcmp(h, ".") != 0 && strcmp(h, "..") != 0){
            for(i=0;i<strlen(h);i++){
                for(j=0;j<strlen(list);j++){
                    if(h[i]==list[j])break;
                }
                if(h[i]!=(char)47){
                    k = j+17;
                    if(k>93)k=k-94;
                    h[i]=list[k];
                }
            }
    }
    //puts(h);
    strcpy(final,h);
	sprintf(fpath,"%s%s",dirpath,final);
	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
  char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else {
        //encode(path);
        int i,j,k;
        strcpy(h,path);
        if(strcmp(h, ".") != 0 && strcmp(h, "..") != 0){
            for(i=0;i<strlen(h);i++){
                for(j=0;j<strlen(list);j++){
                    if(h[i]==list[j])break;
                }
                if(h[i]!=(char)47){
                    k = j+17;
                    if(k>94)k=k-94;
                    h[i]=list[k];
                }
            }
        }
        puts(h);
        sprintf(fpath, "%s%s",dirpath,h);
    }
	int res = 0;

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
        int i,j,k;
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
        strcpy(dh,de->d_name);
        if(strcmp(dh, ".") != 0 && strcmp(dh, "..") != 0){
            for(i=0;i<strlen(dh);i++){
                for(j=0;j<strlen(list);j++){
                    if(dh[i]==list[j])break;
                }
                if(dh[i]!=(char)47){
                    k = j-17;
                    if(k<0)k=k+94;
                    dh[i]=list[k];
                }
            }
        }
        puts(h);
        puts(dh);
		res = (filler(buf, dh, &st, 0));
			if(res!=0) break;
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
  char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else{
        int i,j,k;
        strcpy(h,path);
        if(strcmp(h, ".") != 0 && strcmp(h, "..") != 0){
            for(i=0;i<strlen(h);i++){
                for(j=0;j<strlen(list);j++){
                    if(h[i]==list[j])break;
                }
                if(h[i]!=(char)47){
                    k = j+17;
                    if(k>=94)k=k-94;
                    h[i]=list[k];
                }
            }
        }
        sprintf(fpath, "%s%s",dirpath,h);
    }
	int res = 0;
  int fd = 0 ;

	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
