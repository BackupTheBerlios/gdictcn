#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <glib.h>
#include <stdlib.h>

/**
 * get_user_shell: 
 * copyed from libgnome
 * Retrieves the user's preferred shell.
 *
 * Returns: A newly allocated string that is the path to the shell.
 */

char * get_user_shell (void)
{
	struct passwd *pw;
	int i;
	const char *shell;
	static char *shells [] = {
		/* Note that on some systems shells can also
		 * be installed in /usr/bin */
		"/bin/bash", "/usr/bin/bash",
		"/bin/zsh", "/usr/bin/zsh",
		"/bin/tcsh", "/usr/bin/tcsh",
		"/bin/ksh", "/usr/bin/ksh",
		"/bin/csh", "/bin/sh", 0
	};

	if (geteuid () == getuid () &&
	    getegid () == getgid ()) {
		/* only in non-setuid */
		if ((shell = g_getenv ("SHELL"))){
			if (access (shell, X_OK) == 0) {
				return g_strdup (shell);
			}
		}
	}
	pw = getpwuid(getuid());
	if (pw && pw->pw_shell) {
		if (access (pw->pw_shell, X_OK) == 0) {
			return g_strdup (pw->pw_shell);
		}
	}

	for (i = 0; shells [i]; i++) {
		if (access (shells [i], X_OK) == 0) {
			return g_strdup (shells[i]);
		}
	}

	/* If /bin/sh doesn't exist, your system is truly broken.  */
	abort ();

	/* Placate compiler.  */
	return NULL;
}

char * get_home_dir(void)
{
	struct passwd *pw;

	pw = getpwuid(getuid());
	if (pw && pw->pw_dir) {
		return g_strdup (pw->pw_dir);
	}

	/* Placate compiler.  */
	return NULL;
}
