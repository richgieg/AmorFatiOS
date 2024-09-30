#include <net.h>
#include <console.h>
#include <process.h>
#include <lib/string.h>

#define FRAME_DATA_MAX_SIZE 2048
#define NUM_FRAMES 8

struct frame {
    u8 data[FRAME_DATA_MAX_SIZE];
    size_t length;
};

struct frame_buffer {
    int pid;
    struct frame frames[NUM_FRAMES];
    int head;
    int tail;
    bool is_full;
};

static struct frame_buffer frame_buffer;

void net_init(void) {
    frame_buffer.pid = -1;
}

void net_handle_frame_receive(u8 *buf, size_t length) {
    if (frame_buffer.pid == -1 || frame_buffer.is_full) {
        return;
    }

    memcpy(frame_buffer.frames[frame_buffer.tail].data, buf, length);
    frame_buffer.frames[frame_buffer.tail].length = length;
    frame_buffer.tail = (frame_buffer.tail + 1) % NUM_FRAMES;

    if (frame_buffer.tail == frame_buffer.head) {
        frame_buffer.is_full = true;
    }
}

void net_subscribe(void) {
    if (frame_buffer.pid == -1) {
        frame_buffer.head = 0;
        frame_buffer.tail = 0;
        frame_buffer.is_full = false;
        frame_buffer.pid = process_get_current_pid();
    }
}

void net_unsubscribe(void) {
    int pid = process_get_current_pid();
    net_unsubscribe_for_pid(pid);
}

void net_unsubscribe_for_pid(int pid) {
    if (frame_buffer.pid == process_get_current_pid()) {
        frame_buffer.pid = -1;
    }
}

void net_read_frame(u8 *buf, size_t *length) {
    int pid = process_get_current_pid();
    if (!net_has_frame_for_process(pid)) {
        process_wait_for_net_frame();
    }

    memcpy(buf, frame_buffer.frames[frame_buffer.head].data,
        frame_buffer.frames[frame_buffer.head].length);

    *length = frame_buffer.frames[frame_buffer.head].length;

    frame_buffer.head = (frame_buffer.head + 1) % NUM_FRAMES;
    frame_buffer.is_full = false;
}

bool net_has_frame_for_process(int pid) {
    return frame_buffer.pid == pid && (
        frame_buffer.is_full || frame_buffer.head != frame_buffer.tail);
}
