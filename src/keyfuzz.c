#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/input.h>

#include "cmdline.h"

#define MAX_SCANCODES 1024

static struct gengetopt_args_info args;

int evdev_open(const char *dev) {
    int fd;
    char fn[PATH_MAX];
    
    if (!strchr(dev, '/')) {
        snprintf(fn, sizeof(fn), "/dev/input/%s", dev);
        dev = fn;
    }
    
    if ((fd = open(dev, O_RDWR)) < 0) {
        fprintf(stderr, "open('%s'): %s\n", dev, strerror(errno));
        return -1;
    }
    
    return fd;
}

int evdev_driver_version(int fd, char *v, size_t l) {
    int version;
    
    if (ioctl(fd, EVIOCGVERSION, &version)) {
        fprintf(stderr, "EVIOCGVERSION: %s\n", strerror(errno));
        return -1;
    }
    
    snprintf(v, l,
             "%i.%i.%i.",
             version >> 16,
             (version >> 8) & 0xff,
             version & 0xff);
    
    return 0;
}

int evdev_device_name(int fd, char *n, size_t l) {
    
    if (ioctl(fd, EVIOCGNAME(l), n) < 0) {
        fprintf(stderr, "EVIOCGNAME: %s\n", strerror(errno));
        return -1;
    }
    
    return 0;
}

int evdev_get_keycode(int fd, int scancode, int e) {
    int codes[2];

    codes[0] = scancode;
    if (ioctl(fd, EVIOCGKEYCODE, codes) < 0) {
        if (e && errno == EINVAL)
            return -2;
        else {
            fprintf(stderr, "EVIOCGKEYCODE: %s\n", strerror(errno));
            return -1;
        }
    }

    return codes[1];
}

int evdev_set_keycode(int fd, int scancode, int keycode) {
    int codes[2];

    codes[0] = scancode;
    codes[1] = keycode;
    
    if (ioctl(fd, EVIOCSKEYCODE, codes) < 0) {
        fprintf(stderr, "EVIOCSKEYCODE: %s\n", strerror(errno));
        return -1;
    }

    return 0;
}

int print_info(int fd) {
    char version[256], name[256];
    int r = -1;
    
    if (evdev_driver_version(fd, version, sizeof(version)) < 0)
        goto fail;

    if (evdev_device_name(fd, name, sizeof(name)) < 0)
        goto fail;

    printf("evdev driver version %s\n"
           "device name: %s\n",
           version, name);
    
    r = 0;
    
fail:
    return r;
}

int dump_table(int fd) {
    char version[256], name[256];
    int scancode, r = -1;

    if (evdev_driver_version(fd, version, sizeof(version)) < 0)
        goto fail;

    if (evdev_device_name(fd, name, sizeof(name)) < 0)
        goto fail;

    printf("### evdev %s, driver '%s'\n", version, name);

    r = 0;
    
    for (scancode = 0; scancode < MAX_SCANCODES; scancode++) {
        int keycode;

        if ((keycode = evdev_get_keycode(fd, scancode, 1)) < 0) {
            if (keycode != -2)
                r = -1;
            
            break;
        }

        printf("0x%03x\t0x%03x\n", scancode, keycode);
    }

    if (!r)
        printf("### EOF\n");

fail:
    return r;
}

int merge_table(int fd) {
    int r = -1;
    int line = 0;
    
    while (!feof(stdin)) {
        char s[256], *p;
        int scancode, new_keycode, old_keycode;

        if (!fgets(s, sizeof(s), stdin))
            break;

        line++;

        p = s+strspn(s, "\t ");

        if (*p == '#' || *p == '\n')
            continue;

        if (sscanf(p, "%i %i", &scancode, &new_keycode) != 2) {
            fprintf(stderr, "Parse failure at line %i\n", line);
            goto fail;
        }

        if ((old_keycode = evdev_get_keycode(fd, scancode, 0)) < 0)
            goto fail;
        
        if (evdev_set_keycode(fd, scancode, new_keycode) < 0) 
            goto fail; 
        
        if (args.verbose_flag && new_keycode != old_keycode)
            fprintf(stderr, "Remapped scancode 0x%02x to 0x%02x. (prior: 0x%02x)\n", scancode, new_keycode, old_keycode);
    }

    r = 0;

fail:

    return r;
}

int main(int argc, char *argv[]) {
    int fd = -1, r = 1;
    
    cmdline_parser(argc, argv, &args);

    if (!args.get_flag && !args.set_flag && !args.info_flag) {
        cmdline_parser_print_help();
        goto fail;
    }
    
    if ((fd = evdev_open(args.device_arg ? args.device_arg : "event0")) < 0)
        goto fail;

    if (args.get_flag) {
        
        if (dump_table(fd) < 0)
            goto fail;
        
    } else if (args.set_flag) {

        if (merge_table(fd) < 0)
            goto fail;
        
    } else if (args.info_flag) {

        if (print_info(fd) < 0)
            goto fail;
    }
        
    r = 0;
    
fail:
    if (fd >= 0)
        close(fd);
    
    return r;
}
