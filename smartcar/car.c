#include "car.h"

int init_camera(int fd)
{
	/*设置视频捕捉格式*/
	//struct v4l2_format fmt;
	memset ( &fmt, 0, sizeof(fmt) );
	fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width       = 640;
	fmt.fmt.pix.height      = 480;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;/*待修改，小车直接采集的图片格式是 V4L2_PIX_FMT_MJPEG...*/
	fmt.fmt.pix.field       = V4L2_FIELD_ANY;
	if (ioctl(fd, VIDIOC_S_FMT, &fmt) == -1) {
		return -1;/*设置失败*/
		printf("设置失败\n");
	}
	if (ioctl(fd, VIDIOC_G_FMT, &fmt) == -1) {
		return -1;/*设置失败*/
		printf("设置失败\n");
	}
	
	//struct v4l2_requestbuffers  req;
	req.count = 4;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
					//•	VIDIOC_QUERYBUF：把VIDIOC_REQBUFS中分配的数据缓存转换成物理地址
	if (ioctl(fd, VIDIOC_REQBUFS, &req) == -1) {  
		printf("VIDIOC_REQBUFS error");
		return -1;
	}
	
	/*VideoBuffer* */ 
	buffers = calloc( req.count, sizeof(*buffers) );
	
	//struct v4l2_buffer    buf;//buf 局部变量
	
	int numBufs;
	for (numBufs = 0; numBufs < req.count; numBufs++) {
		memset( &buf, 0, sizeof(buf) );
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = numBufs;
		// 读取缓存     询问Buffer状态：
		if (ioctl(fd, VIDIOC_QUERYBUF, &buf) == -1) {
			printf("读取缓存error");
			return -1;
		}
				//在调用ioctl-VIDIOC_QUERYBUF后，Driver会填充 v4l2_buffer 结构体内所有信息供用户使用。
		
		buffers[numBufs].length = buf.length;
		// 转换成相对地址
		buffers[numBufs].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE,
			MAP_SHARED,fd, buf.m.offset);
	
		if (buffers[numBufs].start == MAP_FAILED) {
			printf("转换成相对地址error");
			return -1;
		}
		// 放入缓存队列
		if (ioctl(fd, VIDIOC_QBUF, &buf) == -1) {
			printf("放入缓存队列error");
			return -1;
		}
	}
}

void startcamera(int fd)
{
	// 开始录制
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int ret = ioctl(fd, VIDIOC_STREAMON, &type);//on 一次
    if (ret < 0) {
        printf("VIDIOC_STREAMON failed (%d)\n", ret);
        //return ret;
    }
}

int dqbuf(int fd,void **jpeg, int *size, int *index)
{
	//struct v4l2_buffer    buf;
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	
	
	int ret = ioctl(fd, VIDIOC_DQBUF, &buf);
	
	
    if (ret < 0) {
        printf("VIDIOC_DQBUF failed (%d)\n", ret);
        return -1;
    }
	*index = buf.index;
	*size = buf.length;
	*jpeg = buffers[buf.index].start;
	
	return 0;
}

int qbuf(int fd,int index)
{
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	buf.index = index;
	int ret = ioctl(fd, VIDIOC_QBUF, &buf);
    if (ret < 0) {
        printf("VIDIOC_QBUF failed (%d)\n", ret);
        return -1;
    }
	return 0;
}

void myexit(int fd)
{
	int i;
    for (i=0; i< 4; i++) 
    {
        munmap(buffers[i].start, buffers[i].length);
    }

    close(fd);
    printf("Camera test Done.\n");
}
