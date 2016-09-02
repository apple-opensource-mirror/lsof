/*
 * usage.c - usage functions for lsof
 */


/*
 * Copyright 1998 Purdue Research Foundation, West Lafayette, Indiana
 * 47907.  All rights reserved.
 *
 * Written by Victor A. Abell
 *
 * This software is not subject to any license of the American Telephone
 * and Telegraph Company or the Regents of the University of California.
 *
 * Permission is granted to anyone to use this software for any purpose on
 * any computer system, and to alter it and redistribute it freely, subject
 * to the following restrictions:
 *
 * 1. Neither the authors nor Purdue University are responsible for any
 *    consequences of the use of this software.
 *
 * 2. The origin of this software must not be misrepresented, either by
 *    explicit claim or by omission.  Credit to the authors and Purdue
 *    University must appear in documentation and sources.
 *
 * 3. Altered versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 4. This notice may not be removed or altered.
 */

#ifndef lint
static char copyright[] =
"@(#) Copyright 1998 Purdue Research Foundation.\nAll rights reserved.\n";
static char *rcsid = "$Id: usage.c,v 1.17 2001/11/01 20:21:11 abe Exp $";
#endif


#include "lsof.h"
#include "version.h"


/*
 * Local function prototypes
 */

_PROTOTYPE(static char *isnullstr,(char *s));


/*
 * isnullstr() - is it a null string?
 */

static char *
isnullstr(s)
	char *s;			/* string pointer */
{
	if (!s)
		return((char *)NULL);
	while (*s) {
		if (*s != ' ')
			return(s);
		s++;
	}
	return((char *)NULL);
}


/*
 * usage() - display usage and exit
 */

void
usage(xv, fh, version)
	int xv;				/* exit value */
	int fh;				/* ``-F ?'' status */
	int version;			/* ``-v'' status */
{
	char buf[MAXPATHLEN+1], *cp, *cp1, *cp2;
	int dx = -2;
	int  i;

	if (Fhelp || xv) {
	    (void) fprintf(stderr, "%s %s\n latest revision: %s\n",
		Pn, LSOF_VERSION, LSOF_URL);
	    (void) fprintf(stderr, " latest FAQ: %sFAQ\n", LSOF_URL);
	    (void) fprintf(stderr, " latest man page: %slsof_man\n", LSOF_URL);
	    (void) fprintf(stderr,
		" usage: [-?ab%shlnNoOP%sstUvV%s]",

#if	defined(HASNCACHE)
		"C",
#else	/* !defined(HASNCACHE) */
		"",
#endif	/* defined(HASNCACHE) */

#if	defined(HASPPID)
		"R",
#else	/* !defined(HASPPID) */
		"",
#endif	/* defined(HASPPID) */

#if	defined(HASXOPT)
# if	defined(HASXOPT_ROOT)
		(Myuid == 0) ? "X" : ""
# else	/* !defined(HASXOPT_ROOT) */
		"X"
# endif	/* defined(HASXOPT_ROOT) */
#else	/* !defined(HASXOPT) */
		""
#endif	/* defined(HASXOPT) */

	    );

#if	defined(HAS_AFS) && defined(HASAOPT)
	    (void) fprintf(stderr, " [-A A]");
#endif	/* defined(HAS_AFS) && defined(HASAOPT) */

	    (void) fprintf(stderr, " [-c c] [+|-d s] [+%sD D]",

#if	defined(HASDCACHE)
		"|-"
#else	/* !defined(HASDCACHE) */
		""
#endif	/* defined(HASDCACHE) */

		);

	    (void) fprintf(stderr, " [+|-f%s]\n [-F [f]] [-g [s]] [-i [i]]",

#if	defined(HASFSTRUCT)
		"[cfgGn]"
#else	/* !defined(HASFSTRUCT) */
		""
#endif	/* defined(HASFSTRUCT) */

		);

#if	defined(HASKOPT)
	    (void) fprintf(stderr, " [-k k]");
#endif	/* defined(HASKOPT) */

	    (void) fprintf(stderr, " [+|-L [l]]");

#if	defined(HASMOPT)
	    (void) fprintf(stderr, " [-m m]");
#endif	/* defined(HASMOPT) */

	    (void) fprintf(stderr,
	    " [+|-M] [-o [o]] [-p s]\n [+|-r [t]] [-S [t]] [-T [t]]");
	    (void) fprintf(stderr,
	    " [-u s] [+|-w] [--] [names]\n");
	}
	if (xv && !Fhelp) {
	    (void) fprintf(stderr,
		"Use the ``-h'' option to get more help information.\n");
	    if (!fh)
    		Exit(xv);
	}
	if (Fhelp) {
	    (void) fprintf(stderr,
		"Defaults in parentheses; comma-separate set (s) items; dash-separate ranges.\n");
	    (void) fprintf(stderr, "  %-23.23s", "-?|-h list help");
	    (void) fprintf(stderr, "  %-25.25s", "-a AND selections (OR)");
	    (void) fprintf(stderr, "  %s\n", "-b avoid kernel blocks");
	    (void) fprintf(stderr, "  %-23.23s", "-c c  cmd c, /c/[bix]");
	    (void) fprintf(stderr, "  %-25.23s",

#if	defined(HASNCACHE)
		"-C no kernel name cache");
#else	/* !defined(HASNCACHE) */
		" ");
#endif	/* defined(HASNCACHE) */

	    (void) fprintf(stderr, "  %s\n", "+d s  dir s files");
	    (void) fprintf(stderr, "  %-23.23s", "-d s  select by FD set");
	    (void) fprintf(stderr, "  %-25.25s", "+D D  dir D tree *SLOW?*");

#if	defined(HASDCACHE)
	    if (Setuidroot)
		cp = "?|i|r";

# if	!defined(WILLDROPGID)
	    else if (Myuid)
		cp = "?|i|r<path>";
# endif	/* !defined(WILLDROPGID) */

	    else
		cp = "?|i|b|r|u[path]";
	    (void) snpf(buf, sizeof(buf), "-D D  %s", cp);
#else	/* !defined(HASDCACHE) */
	    (void) snpf(buf, sizeof(buf), " ");
#endif	/* defined(HASDCACHE) */

	    (void) fprintf(stderr, "  %s\n", buf);
	    (void) snpf(buf, sizeof(buf), "-i select IPv%s files",

#if	defined(HASIPv6)
			  "[46]"
#else	/* !defined(HASIPv6) */
			  "4"
#endif	/* defined(HASIPv6) */

			  );
	    (void) fprintf(stderr, "  %-23.23s", buf);
	    (void) fprintf(stderr, "  %-25.25s", "-l list UID numbers");
	    (void) fprintf(stderr, "  %s\n", "-n no host names");
	    (void) fprintf(stderr, "  %-23.23s", "-N select NFS files");
	    (void) fprintf(stderr, "  %-25.25s", "-o list file offset");
	    (void) fprintf(stderr, "  %s\n", "-O avoid overhead *RISKY*");
	    (void) fprintf(stderr, "  %-23.23s", "-P no port names");
	    (void) fprintf(stderr, "  %-25.25s",

#if	defined(HASPPID)
	 	"-R list paRent PID"
#else	/* !defined(HASPPID) */
		""
#endif	/* defined(HASPPID) */

	    );
	    (void) fprintf(stderr, "  %s\n", "-s list file size");
	    (void) fprintf(stderr, "  %-23.23s", "-t terse listing");
	    (void) fprintf(stderr, "  %-25.25s", "-T disable TCP/TPI info");
	    (void) fprintf(stderr, "  %s\n", "-U select Unix socket");
	    (void) fprintf(stderr, "  %-23.23s", "-v list version info");
	    (void) fprintf(stderr, "  %-25.25s", "-V verbose search");
	    (void) snpf(buf, sizeof(buf), "+|-w  Warnings (%s)",

#if	defined(WARNINGSTATE)
		"-"
#else	/* !defined(WARNINGSTATE) */
		"+"
#endif	/* defined(WARNINGSTATE) */

	    );
	    (void) fprintf(stderr, "  %s\n", buf);

#if	defined(HASXOPT)
# if	defined(HASXOPT_ROOT)
	    if (Myuid == 0)
		(void) snpf(buf, sizeof(buf), "-X %s", HASXOPT);
# else	/* !defined(HASXOPT_ROOT) */
	    (void) snpf(buf, sizeof(buf), "-X %s", HASXOPT);
# endif	/* defined(HASXOPT_ROOT) */
	    (void) fprintf(stderr, "  %-23.23s", buf);
#endif	/* defined(HASXOPT) */

	    (void) fprintf(stderr, "  %s\n", "-- end option scan");
	    (void) fprintf(stderr, "  %-36.36s",
		"+f|-f  +filesystem or -file names");

#if	defined(HASFSTRUCT)
	    (void) fprintf(stderr,
		"  +|-f[cfgGn] Ct,Fstr,flaGs,Node %s%s%s%s%s%s%s\n",
		Fsv ? "(" : "",
		(Fsv & FSV_CT) ? "C" : "",
		(Fsv & FSV_FA) ? "F" : "",
		((Fsv & FSV_FG) && FsvFlagX)  ? "g" : "",
		((Fsv & FSV_FG) && !FsvFlagX) ? "G" : "",
		(Fsv & FSV_NI) ? "N" : "",
		Fsv ? ")" : "");
#else	/* !defined(HASFSTRUCT) */
	    putc('\n', stderr);
#endif	/* defined(HASFSTRUCT) */

	    (void) fprintf(stderr, "  %-36.36s",
		"-F [f] select fields; -F? for help");

#if	defined(HASKOPT)
	    (void) fprintf(stderr,
		"  -k k   kernel symbols (%s)\n",
		Nmlst ? Nmlst
# if	defined(N_UNIX)
		      : N_UNIX
# else	/* !defined(N_UNIX) */
		      : (Nmlst = get_nlist_path(1)) ? Nmlst
						    : "none found"
# endif	/* defined(N_UNIX) */

	    );
#else	/* !defined(HASKOPT) */
	    putc('\n', stderr);
#endif	/* defined(HASKOPT) */

	    (void) fprintf(stderr,
		"  +|-L [l] list (+) suppress (-) link counts < l (0 = all; default = 0)\n");

# if	defined(HASMOPT)
	    (void) snpf(buf, sizeof(buf), "-m m   kernel memory (%s)", KMEM);
#else	/* !defined(HASMOPT) */
	    (void) snpf(buf, sizeof(buf), " ");
#endif	/* defined(HASMOPT) */

	    (void) fprintf(stderr, "  %-36.36s", buf);
	    (void) snpf(buf, sizeof(buf), "+|-M   portMap registration (%s)",

#if	defined(HASPMAPENABLED)
		"+"
#else	/* !defined(HASPMAPENABLED) */
		"-"
#endif	/* defined(HASPMAPENABLED) */

	    );
	    (void) fprintf(stderr, "  %s\n", buf);
	    (void) snpf(buf, sizeof(buf), "-o o   o 0t offset digits (%d)",
		OFFDECDIG);
	    (void) fprintf(stderr, "  %-36.36s", buf);
	    (void) fprintf(stderr, "  -p s   select by PID set\n");
	    (void) snpf(buf, sizeof(buf),
		"-S [t] t second stat timeout (%d)", TMLIMIT);
	    (void) fprintf(stderr, "  %-36.36s", buf);
	    (void) snpf(buf, sizeof(buf), "-T %ss%s TCP/TPI %sSt%s info (s)",

#if 	defined(HASTCPTPIQ)
		"q",
#else	/* !defined(HASTCPTPIQ) */
		" ",
#endif	/* defined(HASTCPTPIQ) */

#if 	defined(HASTCPTPIW)
		"w",
#else	/* !defined(HASTCPTPIW) */
		" ",
#endif	/* defined(HASTCPTPIW) */

#if 	defined(HASTCPTPIQ)
		"Q,",
#else	/* !defined(HASTCPTPIQ) */
		"",
#endif	/* defined(HASTCPTPIQ) */

#if 	defined(HASTCPTPIW)
		",Win"
#else	/* !defined(HASTCPTPIW) */
		""
#endif	/* defined(HASTCPTPIW) */

	    );
	    (void) fprintf(stderr, "  %s\n", buf);

#if	defined(HAS_AFS) && defined(HASAOPT)
	    (void) fprintf(stderr,
		"  -A A   AFS name list file (%s)\n", AFSAPATHDEF);
#endif	/* defined(HAS_AFS) && defined(HASAOPT) */

	    (void) fprintf(stderr,
		"  -g [s] select by process group ID set and print");
	    (void) fprintf(stderr, " process group IDs\n");
	    (void) fprintf(stderr, "  -i i   select by IPv%s address:",

#if	defined(HASIPv6)
			  "[46]"
#else	/* !defined(HASIPv6) */
			  "4"
#endif	/* defined(HASIPv6) */

			  );
	    (void) fprintf(stderr,
		" [%s][proto][@host|addr][:svc_list|port_list]\n",

#if	defined(HASIPv6)
		"46"
#else	/* !defined(HASIPv6) */
		"4"
#endif	/* defined(HASIPv6) */

		);
	    (void) fprintf(stderr,
		"  +|-r [t] repeat every t seconds (%d);", RPTTM);
	    (void) fprintf(stderr, " + until no files, - forever\n");
	    (void) fprintf(stderr,
		"  -u s   exclude(^)|select login|UID set s\n");
	    (void) fprintf(stderr,
		"  names  select named files or files on named file systems\n");
	    (void) fprintf(stderr, "%s can list all files;",

#if	defined(HASSECURITY)
		"Only root"
#else	/* !defined(HASSECURITY) */
		"Anyone"
#endif	/* defined(HASSECURITY) */

	    );
	    (void) fprintf(stderr, " /dev warnings %s;",

#if	defined(WARNDEVACCESS)
		"enabled"
#else	/* !defined(WARNDEVACCESS) */
		"disabled"
#endif	/* defined(WARNDEVACCESS) */

	    );
	    (void) fprintf(stderr, " kernel ID check %s.\n",

#if	defined(HASKERNIDCK)
		"enabled"
#else	/* !defined(HASKERNIDCK) */
		"disabled"
#endif	/* defined(HASKERNIDCK) */

	    );

#if defined(HASDCACHE)
# if	defined(HASENVDC) || defined(HASPERSDC) || defined(HASSYSDC)
	    cp = NULL;
#  if	defined(HASENVDC)
	    if (dx == -2 && (dx = dcpath(1, 0)) >= 0)
		cp = DCpath[1];
#  endif	/* defined(HASENVDC) */
#  if	defined(HASSYSDC)
	    if (!cp)
		cp = HASSYSDC;
#  endif	/* defined(HASSYSDC) */
#  if	defined(HASPERSDC)
	    if (!cp && dx != -1 && (dx = dcpath(1, 0)) >= 0)
		cp = DCpath[3];
#  endif	/* defined(HASPERSDC) */
	    if (cp)
		(void) fprintf(stderr,
		    "%s is the default device cache file read path.\n", cp);
# endif    /* defined(HASENVDC) || defined(HASPERSDC) || defined(HASSYSDC) */
#endif	/* defined(HASDCACHE) */

	}
	if (fh) {
	    (void) fprintf(stderr, "%s:\tID    field description\n", Pn);
	    for (i = 0; FieldSel[i].nm; i++) {

#if	!defined(HASPPID)
		if (FieldSel[i].id == LSOF_FID_PPID)
		    continue;
#endif	/* !defined(HASPPID) */

#if	!defined(HASFSTRUCT)
		if (FieldSel[i].id == LSOF_FID_FA
		||  FieldSel[i].id == LSOF_FID_CT
		||  FieldSel[i].id == LSOF_FID_FG
		||  FieldSel[i].id == LSOF_FID_NI)
		    continue;
#endif	/* !defined(HASFSTRUCT) */

		(void) fprintf(stderr, "\t %c    %s\n",
		    FieldSel[i].id, FieldSel[i].nm);
	    }
	}

#if	defined(HASDCACHE)
	if (DChelp) {

	/*
	 * Display device cache file read-only and write paths.
	 */
	    (void) fprintf(stderr, "%s: device cache file read-only paths:\n",
		Pn);
	    if ((dx = dcpath(1, 0)) < 0)
		(void) fprintf(stderr, "\tnone\n");
	    else {
		(void) fprintf(stderr, "\tNamed via -D: %s\n",
		    DCpath[0] ? DCpath[0] : "none");

# if	defined(HASENVDC)
		(void) fprintf(stderr,
		    "\tNamed in environment variable %s: %s\n",
		    HASENVDC, DCpath[1] ? DCpath[1] : "none");
# endif	/* defined(HASENVDC) */

# if	defined(HASSYSDC)
		if (DCpath[2])
		    (void) fprintf(stderr,
			"\tSystem-wide device cache: %s\n", DCpath[2]);
# endif	/* defined(HASSYSDC) */

# if	defined(HASPERSDC)
		(void) fprintf(stderr,
		    "\tPersonal path format (HASPERSDC): \"%s\"\n",
		    HASPERSDC);
#  if	defined(HASPERSDCPATH)
		(void) fprintf(stderr,
		    "\tModified personal path environment variable: %s\n",
		    HASPERSDCPATH);
		cp = getenv(HASPERSDCPATH);
		(void) fprintf(stderr, "\t%s value: %s\n",
			HASPERSDCPATH, cp ? cp : "none");
#  endif	/* defined(HASPERSDCPATH) */
		(void) fprintf(stderr, "\tPersonal path: %s\n",
		    DCpath[3] ? DCpath[3] : "none");
# endif	/* defined(HASPERSDC) */
	    }
	    (void) fprintf(stderr, "%s: device cache file write paths:\n", Pn);
	    if ((dx = dcpath(2, 0)) < 0)
		(void) fprintf(stderr, "\tnone\n");
	    else {
		(void) fprintf(stderr, "\tNamed via -D: %s\n",
		    DCstate == 2 ? "none"
				 : DCpath[0] ? DCpath[0] : "none");

# if	defined(HASENVDC)
		(void) fprintf(stderr,
		    "\tNamed in environment variable %s: %s\n",
		    HASENVDC, DCpath[1] ? DCpath[1] : "none");
# endif	/* defined(HASENVDC) */

# if	defined(HASPERSDC)
		(void) fprintf(stderr,
		    "\tPersonal path format (HASPERSDC): \"%s\"\n",
		    HASPERSDC);
#  if	defined(HASPERSDCPATH)
		(void) fprintf(stderr,
		    "\tModified personal path environment variable: %s\n",
		    HASPERSDCPATH);
		cp = getenv(HASPERSDCPATH);
		(void) fprintf(stderr, "\t%s value: %s\n",
			HASPERSDCPATH, cp ? cp : "none");
#  endif	/* defined(HASPERSDCPATH) */
		 (void) fprintf(stderr, "\tPersonal path: %s\n",
		    DCpath[3] ? DCpath[3] : "none");
# endif	/* defined(HASPERSDC) */
	    }
	}
#endif	/* defined(HASDCACHE) */

	if (version) {

	/*
	 * Display version information in reponse to ``-v''.
	 */
	    (void) fprintf(stderr, "%s version information:\n", Pn);
	    (void) fprintf(stderr, "    revision: %s\n", LSOF_VERSION);
	    (void) fprintf(stderr, "    latest revision: %s\n", LSOF_URL);
	    (void) fprintf(stderr, "    latest FAQ: %sFAQ\n",
		LSOF_URL);
	    (void) fprintf(stderr, "    latest man page: %slsof_man\n",
		LSOF_URL);

#if	defined(LSOF_CINFO)
	    if ((cp = isnullstr(LSOF_CINFO)))
		(void) fprintf(stderr, "    configuration info: %s\n", cp);
#endif	/* defined(LSOF_CINFO) */

	    if ((cp = isnullstr(LSOF_CCDATE)))
		(void) fprintf(stderr, "    constructed: %s\n", cp);
	    cp = isnullstr(LSOF_HOST);
	    if (!(cp1 = isnullstr(LSOF_LOGNAME)))
		cp1 = isnullstr(LSOF_USER);
	    if (cp || cp1) {
		if (cp && cp1)
		    cp2 = "by and on";
		else if (cp)
		    cp2 = "on";
		else
		    cp2 = "by";
		(void) fprintf(stderr, "    constructed %s: %s%s%s\n",
		    cp2,
		    cp1 ? cp1 : "",
		    (cp && cp1) ? "@" : "",
		    cp  ? cp  : ""
		);
	    }

#if	defined(LSOF_BLDCMT)
	    if ((cp = isnullstr(LSOF_BLDCMT)))
		(void) fprintf(stderr, "    builder's comment: %s\n", cp);
#endif	/* defined(LSOF_BLDCMT) */

	    if ((cp = isnullstr(LSOF_CC)))
		(void) fprintf(stderr, "    compiler: %s\n", cp);
	    if ((cp = isnullstr(LSOF_CCV)))
		(void) fprintf(stderr, "    compiler version: %s\n", cp);
	    if ((cp = isnullstr(LSOF_CCFLAGS)))
		(void) fprintf(stderr, "    compiler flags: %s\n", cp);
	    if ((cp = isnullstr(LSOF_LDFLAGS)))
		(void) fprintf(stderr, "    loader flags: %s\n", cp);
	    if ((cp = isnullstr(LSOF_SYSINFO)))
		(void) fprintf(stderr, "    system info: %s\n", cp);
	}
	Exit(xv);
}