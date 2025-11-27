/* See LICENSE file for copyright and license details. */
#define TERMINAL "st"
#define TERMCLASS "St"

#include <X11/XF86keysym.h>
#include "/home/jannes/.cache/wal/colors-wal-dwm.h"

/* appearance */
static unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int gappx   = 10;        /* gap pixel between windows */
static const Gap default_gap        = {.isgap = 1, .realgap = 10, .gappx = 10};
static unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 0;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 0;     /* 0 means no systray */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static const int vertpad            = 10;       /* vertical padding of bar */
static const int sidepad            = 10;       /* horizontal padding of bar */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const char *fonts[]          = {
    "SauceCodePro Nerd Font:size=10",
    "Noto Color Emoji:size=10"
};
static const char dmenufont[]       = "SauceCodePro Nerd Font:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
// Uncomment to user colors from Xresources
//static char *colors[][3] = {
       /*               fg           bg           border   */
       //[SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       //[SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
//};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title             tags mask     isfloating  isterminal  noswallow   monitor */
	{ TERMCLASS,  NULL,       NULL,             0,            0,          1,          0,           -1 },
	{ NULL,       NULL,       "ranger",         0,            0,          1,          0,           -1 },
	{ NULL,       NULL,       "lf",             0,            0,          1,          0,           -1 },
	{ NULL,       NULL,       "pulsemixer",     0,            1,          0,          0,           -1 },
	{ "qutebrowser",  NULL,   NULL,             0,            0,          1,          0,           -1 },
    { "keepassxc",    NULL,   NULL,             9,            0,          0,          0,           -1 },
	{ NULL,      NULL,        "Event Tester",   0,            0,          0,          1,           -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[ ]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define PrintScreenWDM 0x0000ff61

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, NULL };
static const char *termcmd[] = { TERMINAL, NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };
static const char *brupcmd[] = { "brightnessctl", "set", "10%+", NULL };
static const char *brdowncmd[] = { "brightnessctl", "set", "10%-", NULL };
/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          		INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",      	 	FLOAT,   &mfact },
};

#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_o,      togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                       XK_l,      spawn,          SHCMD("pulsemixer") },
	{ MODKEY,                       XK_b,      spawn,          SHCMD("firefox") },
	{ MODKEY|ShiftMask,             XK_b,      spawn,          SHCMD("rofi-bluetooth") },
	{ MODKEY,                       XK_e,      spawn,          SHCMD(TERMINAL " -e lf") },
	{ MODKEY|ShiftMask,             XK_e,      spawn,          SHCMD("thunar") },
    { MODKEY,                       XK_a,      spawn,          SHCMD(TERMINAL " -e scrcpy") },
    { MODKEY|ShiftMask,             XK_a,      spawn,          SHCMD("$HOME/Dev/suckless/dwm/scripts/sndcpy.sh") },
	{ MODKEY,                       XK_u,      spawn,          SHCMD("dmenu_mount.sh") },
	{ MODKEY|ShiftMask,             XK_u,      spawn,          SHCMD("dmenu_unmount.sh") },
	{ MODKEY|ShiftMask,             XK_d,      spawn,          SHCMD("$HOME/Dev/suckless/dwm/scripts/dock.sh") },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          SHCMD("dmenu_shutdown.sh") },
	{ MODKEY|ShiftMask,             XK_t,      spawn,          SHCMD("dmenu_screen_temp.sh") },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          SHCMD("thunderbird") },
	{ MODKEY|ShiftMask,             XK_n,      spawn,          SHCMD(TERMINAL " -e ncspot") },
	{ MODKEY|ShiftMask,             XK_p,      spawn,          SHCMD("keepassxc") },
	{ MODKEY|ShiftMask,             XK_x,      spawn,          SHCMD("slock") },
	{ MODKEY,                       XK_x,      spawn,          SHCMD("systemctl suspend") },
	{ MODKEY,                       XK_s,      spawn,          SHCMD("z /home/jannes/Documents/Studium/SS25/ATII.pdf") },
	{ MODKEY,                       XK_w,      spawn,          SHCMD("$HOME/Dev/suckless/dwm/scripts/connect.sh") },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
    { MODKEY,                       XK_g,      spawn,          SHCMD("/home/jannes/Dev/suckless/dwm/scripts/todo.py") },
    { MODKEY,                       XK_n,      spawn,          SHCMD("dmenu_emojis.sh") },
    { MODKEY|ShiftMask,             XK_i,      spawn,          SHCMD("networkmanager_dmenu") },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
    { 0,                            XF86XK_AudioLowerVolume, spawn, SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -2% && pkill -RTMIN+11 dwmblocks") },
    { 0,                            XF86XK_AudioRaiseVolume, spawn, SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +2% && pkill -RTMIN+11 dwmblocks") },
    { 0,                            XF86XK_AudioMute,        spawn, SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle && pkill -RTMIN+11 dwmblocks") },
    { 0,                            XF86XK_AudioMicMute,     spawn, SHCMD("amixer set Capture toggle && pkill -RTMIN+10 dwmblocks") },
    { 0,                            XF86XK_AudioPlay, spawn, SHCMD("playerctl play") },
    { 0,                            XF86XK_AudioStop, spawn, SHCMD("playerctl pause") },
    { 0,                            XF86XK_AudioPrev, spawn, SHCMD("playerctl previous") },
    { 0,                            XF86XK_AudioNext, spawn, SHCMD("playerctl next") },
    { 0,                            XF86XK_Bluetooth, spawn, SHCMD("rofi-bluetooth") },
    { 0,                            XF86XK_Display,   spawn, SHCMD("dmenu_display.sh") },
    { 0,                            XF86XK_RFKill,    spawn, SHCMD("rfkill toggle wlan && pkill -RTMIN+20 dwmblocks") },
    { 0,                            XF86XK_Favorites, spawn, SHCMD("dmenu_bookmarks.sh") },
    { 0,                            XF86XK_Keyboard,  spawn, SHCMD("/home/jannes/Dev/suckless/dwm/scripts/fix_touchpad.sh") },
    { 0,                            XF86XK_MonBrightnessUp,   spawn,    {.v = brupcmd } },
    { 0,                            XF86XK_MonBrightnessDown, spawn,    {.v = brdowncmd } },
    { 0,                            PrintScreenWDM,   spawn, SHCMD("import $(date +'%d.%m.%Y_%H:%m:%s').jpg") },
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

