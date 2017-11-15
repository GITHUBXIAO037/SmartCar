#ifndef CAR_H
#define CAR_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>           
#include <fcntl.h>            
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>        
#include <linux/videodev2.h>

#define CAMERA_DEVICE "/dev/video0"
#define CAPTURE_FILE "frame.jpg"

#define VIDEO_WIDTH 640
#define VIDEO_HEIGHT 480
#define VIDEO_FORMAT V4L2_PIX_FMT_YUYV
#define BUFFER_COUNT 4

typedef struct VideoBuffer {
    void   *start;
    size_t  length;
} VideoBuffer;

struct v4l2_format fmt;
struct v4l2_requestbuffers  req;
VideoBuffer* buffers;
struct v4l2_buffer    buf;//buf为全局变量

int init_camera(int fd);
void startcamera(int fd);
int dqbuf(int fd,void **jpeg, int *size, int *index);
int qbuf(int fd,int index);
void myexit(int fd);

#endif 
