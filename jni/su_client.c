#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <android/log.h>
#include "su_log.h"
#include <su_client.h>

/*
 * Receive a file descriptor from a Unix socket.
 * Contributed by @mkasick
 *
 * Returns the file descriptor on success, or -1 if a file
 * descriptor was not actually included in the message
 *
 * On error the function terminates by calling exit(-1)
 */
static int recv_fd(int sockfd) {
    // Need to receive data from the message, otherwise don't care about it.
    char iovbuf;

    struct iovec iov = {
        .iov_base = &iovbuf,
        .iov_len  = 1,
    };

    char cmsgbuf[CMSG_SPACE(sizeof(int))];

    struct msghdr msg = {
        .msg_iov        = &iov,
        .msg_iovlen     = 1,
        .msg_control    = cmsgbuf,
        .msg_controllen = sizeof(cmsgbuf),
    };

    if (recvmsg(sockfd, &msg, MSG_WAITALL) != 1) {
        goto error;
    }

    // Was a control message actually sent?
    switch (msg.msg_controllen) {
    case 0:
        // No, so the file descriptor was closed and won't be used.
        return -1;
    case sizeof(cmsgbuf):
        // Yes, grab the file descriptor from it.
        break;
    default:
        goto error;
    }

    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);

    if (cmsg             == NULL                  ||
        cmsg->cmsg_len   != CMSG_LEN(sizeof(int)) ||
        cmsg->cmsg_level != SOL_SOCKET            ||
        cmsg->cmsg_type  != SCM_RIGHTS) {
error:
        LOGE("unable to read fd");
        exit(-1);
    }

    return *(int *)CMSG_DATA(cmsg);
}

/*
 * Send a file descriptor through a Unix socket.
 * Contributed by @mkasick
 *
 * On error the function terminates by calling exit(-1)
 *
 * fd may be -1, in which case the dummy data is sent,
 * but no control message with the FD is sent.
 */
static void send_fd(int sockfd, int fd) {
    // Need to send some data in the message, this will do.
    struct iovec iov = {
        .iov_base = "",
        .iov_len  = 1,
    };

    struct msghdr msg = {
        .msg_iov        = &iov,
        .msg_iovlen     = 1,
    };

    char cmsgbuf[CMSG_SPACE(sizeof(int))];

    if (fd != -1) {
        // Is the file descriptor actually open?
        if (fcntl(fd, F_GETFD) == -1) {
            if (errno != EBADF) {
                goto error;
            }
            // It's closed, don't send a control message or sendmsg will EBADF.
        } else {
            // It's open, send the file descriptor in a control message.
            msg.msg_control    = cmsgbuf;
            msg.msg_controllen = sizeof(cmsgbuf);

            struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);

            cmsg->cmsg_len   = CMSG_LEN(sizeof(int));
            cmsg->cmsg_level = SOL_SOCKET;
            cmsg->cmsg_type  = SCM_RIGHTS;

            *(int *)CMSG_DATA(cmsg) = fd;
        }
    }

    if (sendmsg(sockfd, &msg, 0) != 1) {
error:
        PLOGE("unable to send fd");
        exit(-1);
    }
}

static int read_int(int fd) {
    int val;
    int len = read(fd, &val, sizeof(int));
    if (len != sizeof(int)) {
        LOGE("unable to read int: %d", len);
        exit(-1);
    }
    return val;
}

static void write_int(int fd, int val) {
    int written = write(fd, &val, sizeof(int));
    if (written != sizeof(int)) {
        PLOGE("unable to write int");
        exit(-1);
    }
}

static char* read_string(int fd) {
    int len = read_int(fd);
    if (len > PATH_MAX || len < 0) {
        LOGE("invalid string length %d", len);
        exit(-1);
    }
    char* val = malloc(sizeof(char) * (len + 1));
    if (val == NULL) {
        LOGE("unable to malloc string");
        exit(-1);
    }
    val[len] = '\0';
    int amount = read(fd, val, len);
    if (amount != len) {
        LOGE("unable to read string");
        exit(-1);
    }
    return val;
}

static void write_string(int fd, const char* val) {
    int len = strlen(val);
    write_int(fd, len);
    int written = write(fd, val, len);
    if (written != len) {
        PLOGE("unable to write string");
        exit(-1);
    }
}

int connect_daemon(int argc, const char *argv[], int ppid)
{
    int sockfd;
    struct sockaddr_un servaddr;
    char buf;

    // argv[0] xdsu
    // argv[1] passnum
    // argv[2] ... argv[argc - 1] parameter deliver to sudaemon
    if (argc < 3)
    {
        write(STDOUT_FILENO, "number of paremeter must greater than 2\n", sizeof("number of paremeter must greater than 2\n"));
        return -1;
    }
    
    sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, SOCKET_PATH);
    {
        int i;
        for (i = 0; i < argc; i++)
        {
            LOGD("prepare for deamon: argv[%d]:%s", i, argv[i]);
        }
    }

    if ( 0 != connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) )
    {
        LOGD("===>>> connect fail\n");
        close(sockfd);
        return -1;
    }
    else
    {
        LOGD("===>>> connect succeed\n");
    }

    write_int(sockfd, getpid());
    write_int(sockfd, getuid());
    write_int(sockfd, ppid);

    send_fd(sockfd, STDOUT_FILENO);
    send_fd(sockfd, STDERR_FILENO);

    // transfer para num
    write_int(sockfd, argc - 2);

    int i;
    for (i = 2; i < argc; i++)
    {
        write_string(sockfd, argv[i]);
    }

    // wait for ack from daemon
    read_int(sockfd);


    // Get the exit code
    int code = read_int(sockfd);
#if 0
    while(read(sockfd, &buf, 1) > 0)
        write(STDOUT_FILENO, &buf, 1);
#endif

    close(sockfd);

    return code;
}

