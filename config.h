/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* Constants */
#define TERMINAL "st"
#define BROWSER "qutebrowser"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = {
	"mono:pixelsize=14"
};

static const char col_dark[]   = "#222";
static const char col_blue[]   = "#234";
static const char col_light[]  = "#bbb";
static const char col_red[]    = "#f92672";
static const char col_yellow[] = "#fd971f";

static const int colorcount = 2; /* fg and bg */
static const char *colors[][2] = {
	/*                  fg          bg   */
	[SchemeNorm]    = { col_light,  col_dark },
	[SchemeSel]     = { col_light,  col_blue },
	[SchemeWarn]    = { col_yellow, col_dark },
	[SchemeUrgent]  = { col_red,    col_dark },
	[SchemeSelWarn] = { col_yellow, col_blue },
};

/* tagging */
static const char *tags[] = {
	"a", "b", "c", "d", "e", "f", "g", "h", "i",
	"j", "k", "l", "m", "n", "o", "p", "q", "r",
	"s", "t", "u", "v", "w", "x", "y", "z"
};

static const Rule rules[] = {
/* xprop(1):
 *        WM_CLASS(STRING) = instance, class
 *        WM_NAME(STRING) = title
	   class          instance  title            tags mask       isfloating  isterminal  noswallow  monitor */
	{ "KeePassXC",    NULL,     NULL,            (1 << 31),      1,          0,          0,         -1 },
	{ "qutebrowser",  NULL,     "YouTube",       1 << ('y'-'a'), 0,          0,          0,         -1 },
	{ "st-256color",  NULL,     NULL,            0,              0,          1,          0,         -1 },
	{ NULL,           NULL,     "Event Tester",  0,              0,          0,          1,         -1 },
};

/* layout(s) */
static const float mfact        = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster        = 1;    /* number of clients in master area */
static const int resizehints    = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1;    /* 1 will force focus on the fullscreen window */

enum layout_types {
	LAYOUT_COLUMN,
	LAYOUT_TILE,
	LAYOUT_FLOAT,
	LAYOUT_MONOCLE,
	LAYOUT_GRID,
};

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "||",      col },     /* first entry is default */
	{ "|-",      tile },
	{ "[]",      NULL },    /* no layout function means floating behavior */
	{ "mo",      monocle },
	{ "##",      grid },
};

/* key definitions */
#define MODKEY Mod4Mask

#define TAGKEYS(KEY) \
	{ MODKEY,           XK_r, KEY, view,       {.ui = 1 << (KEY - XK_a)} }, \
	{ MODKEY,           XK_e, KEY, tag,        {.ui = 1 << (KEY - XK_a)} }, \

#define STACKKEYS(MOD, CHAIN, ACTION) \
	{ MOD, CHAIN, XK_j,          ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, CHAIN, XK_k,          ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, CHAIN, XK_apostrophe, ACTION##stack, {.i = PREVSEL } }, \
	{ MOD, CHAIN, XK_y,          ACTION##stack, {.i = 0 } }, \
	{ MOD, CHAIN, XK_u,          ACTION##stack, {.i = 1 } }, \
	{ MOD, CHAIN, XK_i,          ACTION##stack, {.i = 2 } }, \
	{ MOD, CHAIN, XK_x,          ACTION##stack, {.i = -1 } },

#define SPAWN(...) {.v = (const char*[]){ __VA_ARGS__, NULL } }

/* commands */
static const char *dmenucmd[] = { "dmenu_run", "-b", "-i", "-p", "$", NULL };
static const char *termcmd[]  = { TERMINAL, NULL };
static const scratchpad keepassxc = {.class = "KeePassXC", .v = (char *[]){"keepassxc", NULL}};

static Key keys[] = {
	/* modifier         chain key                       function        argument */
	/* window management */
	{ MODKEY,           -1,   XK_d,                     spawn,          {.v = dmenucmd } },
	{ MODKEY,           -1,   XK_Return,                spawn,          {.v = termcmd } },
	{ MODKEY,           -1,   XK_o,                     togglescratch,  {.v = &keepassxc } },
	{ MODKEY|ShiftMask, -1,   XK_b,                     togglebar,      {0} },
	{ MODKEY,           -1,   XK_h,                     setmfact,       {.f = -0.05} },
	{ MODKEY,           -1,   XK_l,                     setmfact,       {.f = +0.05} },
	{ MODKEY,           -1,   XK_space,                 zoom,           {0} },
	{ MODKEY,           -1,   XK_Tab,                   view,           {0} },
	{ MODKEY,           -1,   XK_q,                     killclient,     {0} },
	{ MODKEY|ShiftMask, -1,   XK_space,                 togglefloating, {0} },
	{ MODKEY,           -1,   XK_f,                     togglefullscr,  {0} },
	{ MODKEY,           -1,   XK_m,                     focusmon,       {.i = +1 } },
	{ MODKEY,           -1,   XK_n,                     tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask, -1,   XK_q,                     quit,           {0} },
	{ MODKEY,           -1,   XK_0,                     view,           {.ui = ~0 } },

	/* layouts */
	{ MODKEY,           XK_t, XK_t,                     setlayout,      {.v = &layouts[LAYOUT_TILE]} },
	{ MODKEY,           XK_t, XK_f,                     setlayout,      {.v = &layouts[LAYOUT_FLOAT]} },
	{ MODKEY,           XK_t, XK_m,                     setlayout,      {.v = &layouts[LAYOUT_MONOCLE]} },
	{ MODKEY,           XK_t, XK_g,                     setlayout,      {.v = &layouts[LAYOUT_GRID]} },
	{ MODKEY,           XK_t, XK_c,                     setlayout,      {.v = &layouts[LAYOUT_COLUMN]} },

	/* shortcuts */
	{ MODKEY,           -1,   XK_z,                     spawn,          SPAWN("lock-screen") },
	{ MODKEY|ShiftMask, -1,   XK_z,                     spawn,          SPAWN("suspend-lock") },
	{ MODKEY,           -1,   XK_s,                     spawn,          SPAWN("fix-stuff") },
	{ MODKEY,           -1,   XK_w,                     spawn,          SPAWN(BROWSER) },
	{ MODKEY|ShiftMask, -1,   XK_w,                     spawn,          SPAWN(TERMINAL, "-e", "nmtui") },
	{ MODKEY,           -1,   XK_g,                     spawn,          SPAWN("ss", "-s") },
	{ MODKEY|ShiftMask, -1,   XK_g,                     spawn,          SPAWN("ss") },
	{ MODKEY,           -1,   XK_v,                     spawn,          SPAWN("clipboard-control") },
	{ MODKEY,           -1,   XK_p,                     spawn,          SPAWN("media-command", "playpause") },
	{ 0,                -1,   XF86XK_AudioPlay,         spawn,          SPAWN("media-command", "playpause") },
	{ MODKEY,           -1,   XK_bracketleft,           spawn,          SPAWN("media-command", "prev") },
	{ 0,                -1,   XF86XK_AudioPrev,         spawn,          SPAWN("media-command", "prev") },
	{ MODKEY,           -1,   XK_bracketright,          spawn,          SPAWN("media-command", "next") },
	{ 0,                -1,   XF86XK_AudioNext,         spawn,          SPAWN("media-command", "next") },
	{ Mod1Mask,         -1,   XK_comma,                 spawn,          SPAWN("change-volume", "down") },
	{ MODKEY,           -1,   XK_comma,                 spawn,          SPAWN("change-volume", "down") },
	{ 0,                -1,   XF86XK_AudioLowerVolume,  spawn,          SPAWN("change-volume", "down") },
	{ Mod1Mask,         -1,   XK_period,                spawn,          SPAWN("change-volume", "up") },
	{ MODKEY,           -1,   XK_period,                spawn,          SPAWN("change-volume", "up") },
	{ 0,                -1,   XF86XK_AudioRaiseVolume,  spawn,          SPAWN("change-volume", "up") },
	{ Mod1Mask,         -1,   XK_slash,                 spawn,          SPAWN("change-volume", "toggle") },
	{ MODKEY,           -1,   XK_slash,                 spawn,          SPAWN("change-volume", "toggle") },
	{ 0,                -1,   XF86XK_AudioMute,         spawn,          SPAWN("change-volume", "toggle") },
	{ 0,                -1,   XF86XK_MonBrightnessUp,   spawn,          SPAWN("brighten", "up") },
	{ 0,                -1,   XF86XK_MonBrightnessDown, spawn,          SPAWN("brighten", "down") },

	/* defines */
	TAGKEYS(XK_a)
	TAGKEYS(XK_b)
	TAGKEYS(XK_c)
	TAGKEYS(XK_d)
	TAGKEYS(XK_e)
	TAGKEYS(XK_f)
	TAGKEYS(XK_g)
	TAGKEYS(XK_h)
	TAGKEYS(XK_i)
	TAGKEYS(XK_j)
	TAGKEYS(XK_k)
	TAGKEYS(XK_l)
	TAGKEYS(XK_m)
	TAGKEYS(XK_n)
	TAGKEYS(XK_o)
	TAGKEYS(XK_p)
	TAGKEYS(XK_q)
	TAGKEYS(XK_r)
	TAGKEYS(XK_s)
	TAGKEYS(XK_t)
	TAGKEYS(XK_u)
	TAGKEYS(XK_v)
	TAGKEYS(XK_w)
	TAGKEYS(XK_x)
	TAGKEYS(XK_y)
	TAGKEYS(XK_z)

	STACKKEYS(MODKEY,           -1, focus)
	STACKKEYS(MODKEY|ShiftMask, -1, push)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
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
