/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include <sys/stat.h>

#include "fileio-label.h"
#include "fileio.h"
#include "selinux-util.h"

int create_shutdown_run_nologin_or_warn(void) {
        int r;

        /* This is used twice: once in systemd-user-sessions.service, in order to block logins when we
         * actually go down, and once in systemd-logind.service when shutdowns are scheduled, and logins are
         * to be turned off a bit in advance. We use the same wording of the message in both cases.
         *
         * Traditionally, there was only /etc/nologin, and we managed that. Then, in PAM 1.1
         * support for /run/nologin was added as alternative
         * (https://github.com/linux-pam/linux-pam/commit/e9e593f6ddeaf975b7fe8446d184e6bc387d450b).
         * 13 years later we stopped managing /etc/nologin, leaving it for the administrator to manage.
         */

        r = write_string_file("/run/nologin",
                              "System is going down. Unprivileged users are not permitted to log in anymore. "
                              "For technical details, see pam_nologin(8).",
                              WRITE_STRING_FILE_CREATE|WRITE_STRING_FILE_ATOMIC|WRITE_STRING_FILE_LABEL);
        if (r < 0)
                return log_error_errno(r, "Failed to create /run/nologin: %m");

        return 0;
}
