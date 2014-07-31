#ifndef _FCNTL_H_
#define _FCNTL_H_
#include <fcntl.h>
#endif

#ifndef _SYS_TYPES_H_
#define _SYS_TYPES_H_
#include <sys/types.h>
#endif

#ifndef _SYS_IOCTL_H_
#define _SYS_IOCTL_H_
#include <sys/ioctl.h>
#endif

#ifndef _LINUX_SOUNDCARD_H_
#define _LINUX_SOUNDCARD_H_
#include <linux/soundcard.h>
#endif

#ifndef _LINUX_TYPE_H_
#define _LINUX_TYPE_H_
#include <linux/types.h>        // include u8, u16, u32, u64 type
#endif

#define RATE 48000              // the sampling rate
#define SIZE 16         // sample size: 8 or 16 bits
#define CHANNELS 2              // 1 = mono 2 = stereo

#define BUFFER_SIZE 128

