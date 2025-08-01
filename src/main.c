#include "/usr/include/arcan/shmif/arcan_shmif.h"
#include "/usr/include/arcan/shmif/arcan_shmif_server.h"
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv) {
    /* Initialize connection */
    struct arcan_shmif_cont cont = arcan_shmif_open(
        SEGID_APPLICATION,
        SHMIF_ACQUIRE_FATALFAIL,
        NULL
    );

    if (!cont.addr) {
        fprintf(stderr, "Failed to connect to Arcan.\n");
        return 1;
    }

    /* Set window title (modern API) */
    arcan_shmif_set_title(&cont, "Minimal Arcan Compositor");

    /* Signal ready */
    arcan_shmif_signal(&cont, SHMIF_SIGVID);

    /* Main loop */
    bool running = true;
    while (running) {
        struct arcan_event ev;
        while (arcan_shmif_poll(&cont, &ev) > 0) {
            switch (ev.category) {
                case EVENT_TARGET:
                    if (ev.tgt.kind == TARGET_COMMAND_EXIT)
                        running = false;
                    break;

                case EVENT_IO:
                    /* Modern keyboard event check */
                    if (ev.io.devkind == EVENT_IDEVKBD && ev.io.input.translated.activetype == AT_BUTTON) {
                        if (ev.io.input.translated.keysym == SCANCODE_ESCAPE)
                            running = false;
                    }
                    break;
            }
        }

        /* Fill with red */
        for (size_t i = 0; i < cont.w * cont.h; i++)
            cont.vidp[i] = SHMIF_RGBA(255, 0, 0, 255);

        arcan_shmif_signal(&cont, SHMIF_SIGVID);
        usleep(16000);
    }

    arcan_shmif_drop(&cont);
    return 0;
}
