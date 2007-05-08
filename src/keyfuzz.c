#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <glob.h>

#include "input.h"
#include "cmdline.h"

#include "keys-from-name.h"
#include "keys-to-name.h"

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

        if (keycode < KEY_MAX && key_names[keycode])
            printf("0x%03x\t%s\n", scancode, key_names[keycode]);
        else
            printf("0x%03x\t0x%03x\n", scancode, keycode);
    }

    if (!r)
        printf("### EOF\n");

fail:
    return r;
}

int merge_table(int fd) {
    int r = 0;
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
            char t[20];
            const struct key *k;
            
            if (sscanf(p, "%i %20s", &scancode, t) != 2) {
                fprintf(stderr, "WARNING: Parse failure at line %i, ignoring.\n", line);
                r = -1;
                continue;
            }

            if (!(k = lookup_key(t, strlen(t)))) {
                fprintf(stderr, "WARNING: Unknown key '%s' at line %i, ignoring.\n", t, line);
                r = -1;
                continue;
            }

            new_keycode = k->id;
        }

        
        
        if ((old_keycode = evdev_get_keycode(fd, scancode, 0)) < 0) {
            r = -1;
            goto fail;
        }
        
        if (evdev_set_keycode(fd, scancode, new_keycode) < 0) {
            r = -1;
            goto fail;
        }
        
        if (args.verbose_flag && new_keycode != old_keycode)
            fprintf(stderr, "Remapped scancode 0x%02x to 0x%02x. (prior: 0x%02x)\n", scancode, new_keycode, old_keycode);
    }

fail:

    return r;
}

static char *find_atkbd_sysfs(void) {
    glob_t globbuf;
    char **p;
    int ret;
    char *r = NULL;

    if ((ret = glob("/sys/class/input/event*", GLOB_ERR, NULL, &globbuf))) {
        fprintf(stderr, "Warning! Failed to access sysfs: %s\n",
                ret == GLOB_NOSPACE ? strerror(ENOMEM) :
                (ret == GLOB_ABORTED ? strerror(errno) :
                 (ret == GLOB_NOMATCH ? strerror(ENOENT) : "Unknown error")));
        return NULL;
    }

    for (p = globbuf.gl_pathv; *p; p++) {
        FILE *f;
        static char fn[PATH_MAX];
        char ln[16];

        snprintf(fn, sizeof(fn), "%s/device/description", *p);

        if (!(f = fopen(fn, "r")))
            continue;
        
        if (fgets(ln, sizeof(ln), f)) {

            if (strcmp(ln, "i8042 KBD port\n") == 0) {
                (fn+17)[strcspn(fn+17, "/")] = 0;
                r = strdup(fn + 17);
                fclose(f);
                goto finish;
            }
        }

        fclose(f);
    }

finish:
    globfree(&globbuf);
    return r;
}

static char *find_atkbd_procfs(void) {
    FILE *f;
    char *r = NULL;
    int found = 0;

    if (!(f = fopen("/proc/bus/input/devices", "r"))) {
        fprintf(stderr, "Warning! Failed to access procfs: %s\n", strerror(errno));
        return NULL;
    }

    while (!feof(f)) {
        char ln[128];

        if (!(fgets(ln, sizeof(ln), f)))
            break;

        if (!found) {
        
            if (strcmp(ln, "N: Name=\"AT Translated Set 2 keyboard\"\n") == 0)
                found = 1;
            
        } else {

            if (strncmp(ln, "H: Handlers=", 12) == 0) {
                char *e;

                if ((e = strstr(ln+12, "event"))) {                    
                    e[strcspn(e, " \n\t")] = 0;
                    r = strdup(e);
                    goto finish;
                }
            }

            if (strcmp(ln, "\n") == 0)
                found = 0;
        }
            
    }

finish:
    fclose(f);
    return r;
}

int main(int argc, char *argv[]) {
    int fd = -1, r = 1;
    
    cmdline_parser(argc, argv, &args);

    if (!args.get_flag && !args.set_flag && !args.info_flag) {
        cmdline_parser_print_help();
        goto fail;
    }

    if (!args.device_arg) 
        args.device_arg = find_atkbd_sysfs(); 

    if (!args.device_arg)
        args.device_arg = find_atkbd_procfs();
    
    if (!args.device_arg)
        args.device_arg  = "event0";
    
    if ((fd = evdev_open(args.device_arg)) < 0)
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
