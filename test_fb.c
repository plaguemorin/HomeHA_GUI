#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/kd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>

#include <linux/fb.h>

#include <ft2build.h>
#include FT_FREETYPE_H

char * fb_dev = "/dev/fb0";
unsigned char * data;
__u32 memsize;

#pragma pack(1)
struct bmpfile_header {
    unsigned char magic[2];
    uint32_t filesz;
    uint16_t creator1;
    uint16_t creator2;
    uint32_t bmp_offset;
};

struct bmpfile_info {
    uint32_t size; /* Header size in bytes      */
    uint32_t width, height; /* Width and height of image */
    uint16_t planes; /* Number of colour planes   */
    uint16_t bits; /* Bits per pixel            */
    uint32_t compression; /* Compression type          */
    uint32_t imagesize; /* Image size in bytes       */
    uint32_t xresolution, yresolution; /* Pixels per meter          */
    uint32_t ncolours; /* Number of colours         */
    uint32_t importantcolours; /* Important colours         */
};
#pragma pack(0)

int fbfd;
struct fb_fix_screeninfo finfo;
struct fb_var_screeninfo vinfo;

static void print_fixed_info(const struct fb_fix_screeninfo *fixed, const char *s) {
    static const char *visuals[] = { "MONO01", "MONO10", "TRUECOLOR", "PSEUDOCOLOR", "DIRECTCOLOR", "STATIC_PSEUDOCOLOR" };

    printf("%s info -----------------------\n", s);
    printf("id              = %16s\n", fixed->id);
    printf("smem_start      = 0x%x\n", fixed->smem_start);
    printf("smem_len        = %d (0x%x)\n", fixed->smem_len, fixed->smem_len);
    printf("type            = 0x%x\n", fixed->type);
    printf("type_aux        = 0x%x\n", fixed->type_aux);
    printf("visual          = 0x%x (%s)\n", fixed->visual, visuals[fixed->visual]);
    printf("xpanstep        = %d\n", fixed->xpanstep);
    printf("ypanstep        = %d\n", fixed->ypanstep);
    printf("ywrapstep       = %d\n", fixed->ywrapstep);
    printf("line_length     = %d\n", fixed->line_length);
    printf("mmio_start      = 0x%x\n", fixed->mmio_start);
    printf("mmio_len        = %d (0x%x)\n", fixed->mmio_len, fixed->mmio_len);
    printf("accel           = 0x%x\n\n", fixed->accel);
}

static void print_var_info(const struct fb_var_screeninfo *var, const char *s) {
    printf("%s info -----------------------\n", s);
    printf("xres            = %d\n", var->xres);
    printf("yres            = %d\n", var->yres);
    printf("xres_virtual    = %d\n", var->xres_virtual);
    printf("yres_virtual    = %d\n", var->yres_virtual);
    printf("xoffset         = %d\n", var->xoffset);
    printf("yoffset         = %d\n", var->yoffset);
    printf("bits_per_pixel  = %d\n", var->bits_per_pixel);
    printf("grayscale       = %d\n", var->grayscale);

    printf("red.offset      = %d  length = %d  msb_right = %d\n", var->red.offset, var->red.length, var->red.msb_right);
    printf("green.offset    = %d  length = %d  msb_right = %d\n", var->green.offset, var->green.length, var->green.msb_right);
    printf("blue.offset     = %d  length = %d  msb_right = %d\n", var->blue.offset, var->blue.length, var->blue.msb_right);
    printf("transp.offset   = %d  length = %d  msb_right = %d\n", var->transp.offset, var->transp.length, var->transp.msb_right);

    printf("nonstd          = %d\n", var->nonstd);
    printf("activate        = %d\n", var->activate);
    printf("height          = %d mm\n", var->height);
    printf("width           = %d mm\n", var->width);
    printf("accel_flags     = 0x%x\n", var->accel_flags);
    printf("pixclock        = %d\n", var->pixclock);
    printf("left_margin     = %d\n", var->left_margin);
    printf("right_margin    = %d\n", var->right_margin);
    printf("upper_margin    = %d\n", var->upper_margin);
    printf("lower_margin    = %d\n", var->lower_margin);
    printf("hsync_len       = %d\n", var->hsync_len);
    printf("vsync_len       = %d\n", var->vsync_len);
    printf("sync            = %d\n", var->sync);
    printf("vmode           = %d\n\n", var->vmode);
}

int open_fb(char * fbdev) {
    data = (unsigned char *) MAP_FAILED;
    memsize = 0;

    fbfd = open(fb_dev, O_RDWR);

    if (fbfd < 0) {
        printf("Unable to open '%s': %s\n", fbdev, strerror(errno));
        return 1;
    }

    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
        printf("IOCTL FBIOGET_FSCREENINFO Failed\n");
        return 1;
    }

    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
        printf("IOCTL FBIOGET_VSCREENINFO Failed\n");
        return 1;
    }

    print_fixed_info(&finfo, fbdev);
    print_var_info(&vinfo, fbdev);

    memsize = finfo.smem_len;
    data = (unsigned char *) mmap(0, memsize, PROT_WRITE, MAP_SHARED, fbfd, 0);

    if (data == MAP_FAILED) {
        printf("mmap failed\n");
        return 1;
    }

    return 0;
}

void close_fb() {

    if (data != MAP_FAILED && memsize > 0) {
        munmap((char*) data, memsize);
    }

    if (fbfd > 0) {
        close(fbfd);
    }
}

void show_bmp(const char *bmp) {
    int bmpfd;
    struct bmpfile_header hrd;
    struct bmpfile_info bmi;

    bmpfd = open(bmp, O_RDONLY);
    read(bmpfd, &hrd, sizeof(struct bmpfile_header));
    read(bmpfd, &bmi, sizeof(struct bmpfile_info));
    lseek(bmpfd, hrd.bmp_offset, SEEK_SET);

    if (bmi.width != vinfo.xres || bmi.height != vinfo.yres || bmi.bits != vinfo.bits_per_pixel) {
        printf("Unsupported bitmap: %s is %dx%d (%d) but %dx%d (%d) was expected\n", bmp, bmi.width, bmi.height, bmi.bits, vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
    } else {
        read(bmpfd, data, memsize);
    }
    close(bmpfd);
}

int main(int argc, char * argv[]) {

    if (open_fb("/dev/fb0")) {
        close_fb();
        return 1;
    }

    show_bmp("background.bmp");

    close_fb();

    return 0;

}