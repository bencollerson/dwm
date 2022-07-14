/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* Constants */
#define TERMINAL "/usr/local/bin/st"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = {
	"mono:pixelsize=14"
};
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_cyan2[]        = "#0077ff";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
/* xprop(1):
 *        WM_CLASS(STRING) = instance, class
 *        WM_NAME(STRING) = title
	   class          instance  title  tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",         NULL,     NULL,  0,         1,          0,          0,         -1 },
	{ "KeePassXC",    NULL,     NULL,  0,         1,          0,          0,         -1 },
	{ "st-256color",  NULL,     NULL,  0,         0,          1,          0,         -1 },
	{ "Notepadqq",    NULL,     NULL,  0,         0,          0,          0,         -1 },
};


/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "|t|",      tile },    /* first entry is default */
	{ "|f|",      NULL },    /* no layout function means floating behavior */
	{ "|m|",      monocle },
	{ "|g|",      grid },
	{ "|c|",      col },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(CHAIN,KEY,TAG) \
	{ MODKEY,                       CHAIN,    KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           CHAIN,    KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             CHAIN,    KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, CHAIN,    KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD, CHAIN, ACTION) \
	{ MOD, CHAIN, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, CHAIN, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, CHAIN, XK_grave, ACTION##stack, {.i = PREVSEL } }, \
	{ MOD, CHAIN, XK_c,     ACTION##stack, {.i = 0 } }, \
	{ MOD, CHAIN, XK_a,     ACTION##stack, {.i = 1 } }, \
	{ MOD, CHAIN, XK_z,     ACTION##stack, {.i = 2 } }, \
	{ MOD, CHAIN, XK_x,     ACTION##stack, {.i = -1 } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-b", "-i", "-p", "$", NULL };
static const char *termcmd[]  = { TERMINAL, NULL };
static const scratchpad keepassxc = {.class = "KeePassXC", .v = (char *[]){"keepassxc", NULL}};

static Key keys[] = {
	/* modifier                     chain key   key        function        argument */
	TAGKEYS(                        -1,         XK_1,                      0)
	TAGKEYS(                        -1,         XK_2,                      1)
	TAGKEYS(                        -1,         XK_3,                      2)
	TAGKEYS(                        -1,         XK_4,                      3)
	TAGKEYS(                        -1,         XK_5,                      4)
	TAGKEYS(                        -1,         XK_6,                      5)
	TAGKEYS(                        -1,         XK_7,                      6)
	TAGKEYS(                        -1,         XK_8,                      7)
	TAGKEYS(                        -1,         XK_9,                      8)

	STACKKEYS(MODKEY,               -1,           focus)
	STACKKEYS(MODKEY|ShiftMask,     -1,           push)

	{ MODKEY,                       XK_a,       XK_a,      view,           {.ui = 1 << 0} },
	{ MODKEY,                       XK_a,       XK_s,      view,           {.ui = 1 << 1} },
	{ MODKEY,                       XK_a,       XK_d,      view,           {.ui = 1 << 2} },
	{ MODKEY,                       XK_a,       XK_f,      view,           {.ui = 1 << 3} },
	{ MODKEY,                       XK_a,       XK_g,      view,           {.ui = 1 << 4} },
	{ MODKEY,                       XK_a,       XK_h,      view,           {.ui = 1 << 5} },
	{ MODKEY,                       XK_a,       XK_j,      view,           {.ui = 1 << 6} },
	{ MODKEY,                       XK_a,       XK_k,      view,           {.ui = 1 << 7} },
	{ MODKEY,                       XK_a,       XK_l,      view,           {.ui = 1 << 8} },

	{ MODKEY,                       XK_s,       XK_a,      tag,            {.ui = 1 << 0} },
	{ MODKEY,                       XK_s,       XK_s,      tag,            {.ui = 1 << 1} },
	{ MODKEY,                       XK_s,       XK_d,      tag,            {.ui = 1 << 2} },
	{ MODKEY,                       XK_s,       XK_f,      tag,            {.ui = 1 << 3} },
	{ MODKEY,                       XK_s,       XK_g,      tag,            {.ui = 1 << 4} },
	{ MODKEY,                       XK_s,       XK_h,      tag,            {.ui = 1 << 5} },
	{ MODKEY,                       XK_s,       XK_j,      tag,            {.ui = 1 << 6} },
	{ MODKEY,                       XK_s,       XK_k,      tag,            {.ui = 1 << 7} },
	{ MODKEY,                       XK_s,       XK_l,      tag,            {.ui = 1 << 8} },

	{ MODKEY|ShiftMask,             -1,         XK_q,      quit,           {0} },
	{ MODKEY,                       -1,         XK_minus,  togglescratch,  {.v = &keepassxc } },
	{ MODKEY,                       -1,         XK_o,      togglescratch,  {.v = &keepassxc } },
	{ MODKEY,                       -1,         XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       -1,         XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       -1,         XK_b,      togglebar,      {0} },
	{ MODKEY,                       -1,         XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       -1,         XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             -1,         XK_Return, zoom,           {0} },
	{ MODKEY,                       -1,         XK_Tab,    view,           {0} },
	{ MODKEY,                       -1,         XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,       XK_t,      setlayout,      {.v = &layouts[0]} }, /* tile */
	{ MODKEY,                       XK_t,       XK_f,      setlayout,      {.v = &layouts[1]} }, /* floating */
	{ MODKEY,                       XK_t,       XK_m,      setlayout,      {.v = &layouts[2]} }, /* monocle */
	{ MODKEY,                       XK_t,       XK_g,      setlayout,      {.v = &layouts[3]} }, /* grid */
	{ MODKEY,                       XK_t,       XK_c,      setlayout,      {.v = &layouts[4]} }, /* col */
	{ MODKEY,                       -1,         XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             -1,         XK_space,  togglefloating, {0} },

	{ MODKEY,                       -1,         XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       -1,         XK_m,      focusmon,       {.i = +1 } },
	{ MODKEY,                       -1,         XK_n,      tagmon,         {.i = +1 } },

	{ MODKEY,                       -1,   XK_w,            spawn,         SHCMD("$BROWSER") },
	{ MODKEY|ShiftMask,             -1,   XK_w,            spawn,         SHCMD(TERMINAL " -e sudo nmtui") },
	{ MODKEY,                       -1,   XK_p,            spawn,         SHCMD("media-command playpause") },
	{ 0,                            -1,   XF86XK_AudioPlay,spawn,         SHCMD("media-command playpause") },
	{ MODKEY,                       -1,   XK_bracketleft,  spawn,         SHCMD("media-command prev") },
	{ 0,                            -1,   XF86XK_AudioPrev,spawn,         SHCMD("media-command prev") },
	{ MODKEY,                       -1,   XK_bracketright, spawn,         SHCMD("media-command next") },
	{ 0,                            -1,   XF86XK_AudioNext,spawn,         SHCMD("media-command next") },
	{ MODKEY,                       -1,   XK_backslash,    view,          {0} },
	{ MODKEY,                       -1,   XK_g,            spawn,         SHCMD("ss -s") },
	{ MODKEY|ShiftMask,             -1,   XK_g,            spawn,         SHCMD("ss") },
	{ MODKEY,                       -1,   XK_z,            spawn,         SHCMD("lock-screen") },
	{ MODKEY|ShiftMask,             -1,   XK_z,            spawn,         SHCMD("suspend-lock") },
	{ MODKEY,                       -1,   XK_x,            spawn,         SHCMD("keepmenu") },
	{ MODKEY,                       -1,   XK_v,            spawn,         SHCMD("diodon-in-the-corner") },
	{ MODKEY,                       -1,   XK_e,            spawn,         SHCMD("fix-stuff") },
	{ Mod1Mask,                     -1,   XK_comma,        spawn,         SHCMD("change-volume down") },
	{ MODKEY,                       -1,   XK_comma,        spawn,         SHCMD("change-volume down") },
	{ 0,                            -1,   XF86XK_AudioLowerVolume, spawn, SHCMD("change-volume down") },
	{ Mod1Mask,                     -1,   XK_period,       spawn,         SHCMD("change-volume up") },
	{ MODKEY,                       -1,   XK_period,       spawn,         SHCMD("change-volume up") },
	{ 0,                            -1,   XF86XK_AudioRaiseVolume, spawn, SHCMD("change-volume up") },
	{ Mod1Mask,                     -1,   XK_slash,        spawn,         SHCMD("change-volume toggle") },
	{ MODKEY,                       -1,   XK_slash,        spawn,         SHCMD("change-volume toggle") },
	{ 0,                            -1,   XF86XK_AudioMute,spawn,         SHCMD("change-volume toggle") },
	{ 0,                            -1,   XF86XK_MonBrightnessUp,spawn,   SHCMD("brighten up") },
	{ 0,                            -1,   XF86XK_MonBrightnessDown,spawn, SHCMD("brighten down") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

