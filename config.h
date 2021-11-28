#include <X11/XF86keysym.h>

/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const Bool viewontag         = True;     /* Switch view on tag switch */
static const char *fonts[]          = { "Source Code Pro Nerd Font Mono:size=12" }; //source-code-pro
static const char dmenufont[]       = "Source Code Pro Nerd Font Mono:size=12";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#ffffff";
static const char col_cyan[]        = "#37474F";
static const char col_border[]      = "#42A5F5";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_border  },
	[SchemeHid]  = { col_cyan,  col_gray1, col_border  },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
//需要 nerd-fonts 字体
// static const char *tags[] = { "一", "二", "三", "四", "五", "六", "七", "八", "九" };
static const char *tags[] = { "\uf120", "\uf7ae", "\uf121", "\uf04b", "\ue62e", "\uf251", "\ue727", "\uf537", "\uf684" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                instance    title       tags mask     isfloating   monitor */
	{ "Gimp",               NULL,       NULL,       0,            1,           -1 },
	{ "Android Emulator",   NULL,       NULL,       0,            1,           -1 },
	{ "Emulator", 			NULL,       NULL,       0,            1,           -1 },
	{ "quemu-system-i386",  NULL,       NULL,       0,            1,           -1 },
	{ "chromium",           NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "Tile",      tile },    /* first entry is default */
	{ " ◰ ",       NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle }, //windows nums?
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *browsercmd[]  = { "chromium", NULL };
static const char *thunarcmd[]  = { "thunar", NULL };
static const char *rofidrun[]  = { "rofi", "-show", "drun", NULL };
static const char *thunderbirdcmd[]  = { "thunderbird", NULL };
static const char *steamcmd[]  = { "steam", NULL };
static const char *music[] = {"netease-cloud-music", NULL};
static const char *screenshoot[] = {"flameshot", "gui", NULL};

static const char *upvol[]   = { "/home/july/scripts/vol-up.sh",  NULL };
static const char *downvol[] = { "/home/july/scripts/vol-down.sh",  NULL };
static const char *togglevol[] = { "/home/july/scripts/vol-toggle.sh", NULL};

static const char *wpcmd[]  = { "/home/july/scripts/wp-change.sh", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "80x24", NULL };

static const char *suspendcmd[]  = { "/home/july/scripts/suspend.sh", NULL };
// 不能用sh执行i3lock， 我不知道为什么？
//static const char *screenLock[]  = { "/home/july/scripts/screen-lock.sh", NULL };
static const char *screenLock[]  = { "i3lock", "-i", "/home/july/lock.png", NULL };

static Key keys[] = {
	/* modifier            key                      function        argument */
	//exe
	{ MODKEY,              XK_d,                    spawn,          {.v = dmenucmd } },
	{ MODKEY,              XK_Return,               spawn,          {.v = termcmd } },
	{ MODKEY|ControlMask,  XK_Return,       	    togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY|ShiftMask,    XK_Return,               zoom,           {0} },
	{ MODKEY,			   XK_c,                    spawn,          {.v = browsercmd } },
	{ Mod1Mask,            XK_b,                    spawn,          {.v = thunderbirdcmd } },
	{ Mod1Mask,            XK_s,                    spawn,          {.v = steamcmd } },
	{ Mod1Mask,            XK_t,                    spawn,          {.v = thunarcmd } },
	{ Mod1Mask,            XK_f,                    spawn,          {.v = rofidrun } },
    { Mod1Mask,            XK_m,                    spawn,          {.v = music } },
    { Mod1Mask|ShiftMask,  XK_c,                    spawn,          {.v = screenshoot } },
	//volume
	{ MODKEY,              XK_Down,          		spawn,          {.v = downvol } },
	{ MODKEY,              XK_Up,         			spawn,          {.v = upvol   } },
    { MODKEY,              XK_F9,                   spawn,          {.v = togglevol} },
	//wallpaper
	{ MODKEY,              XK_a,                    spawn,          {.v = wpcmd } },
	//window switch
	{ MODKEY|ShiftMask,    XK_j,                    rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_k,                    rotatestack,    {.i = -1 } },
	{ MODKEY,              XK_j,                    focusstack,     {.i = +1 } },
	{ MODKEY,              XK_k,                    focusstack,     {.i = -1 } },
	//windwos 向 左||右 移动到其他 workspace
	{ MODKEY|ControlMask,  XK_h,                    tagtoleft,      {0} },
	{ MODKEY|ControlMask,  XK_l,                    tagtoright,     {0} },
	//windwos 向 左||右 堆叠
	{ MODKEY|ShiftMask,    XK_h,                    incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_l,                    incnmaster,     {.i = -1 } },
	//windwos 边界向 左||右 移动 
	{ MODKEY,              XK_h,                    setmfact,       {.f = -0.05} },
	{ MODKEY,              XK_l,                    setmfact,       {.f = +0.05} },
	//windwos 全屏
	{ MODKEY, 			   XK_f,                    fullscreen,     {0} },
	//hide and restore windows
	{ MODKEY,              XK_n,                    hidewin,        {0} },
	{ MODKEY|ShiftMask,    XK_n,                    restorewin,     {0} },
	{ MODKEY,              XK_m,                    hideotherwins,  {0}},
	{ MODKEY|ShiftMask,    XK_m,                    restoreotherwins, {0}},
	//关闭选中的窗口
	{ MODKEY|ShiftMask,    XK_q,                    killclient,     {0} },
	//改变窗口布局
	{ MODKEY,              XK_z,                    setlayout,      {.v = &layouts[0]} },
	{ MODKEY,              XK_x,                    setlayout,      {.v = &layouts[2]} },
	//切换窗口布局
	{ MODKEY,              XK_space,                setlayout,      {0} },
	// 讲变形的窗口恢复原状
	{ MODKEY|ShiftMask,    XK_space,                togglefloating, {0} },

	// 未发现的功能
	{ MODKEY,              XK_comma,                focusmon,       {.i = -1 } },
	{ MODKEY,              XK_period,               focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_comma,                tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,    XK_period,               tagmon,         {.i = +1 } },

	//workspace swicth
	TAGKEYS(               XK_1,                      0)
	TAGKEYS(               XK_2,                      1)
	TAGKEYS(               XK_3,                      2)
	TAGKEYS(               XK_4,                      3)
	TAGKEYS(               XK_5,                      4)
	TAGKEYS(               XK_6,                      5)
	TAGKEYS(               XK_7,                      6)
	TAGKEYS(               XK_8,                      7)
	TAGKEYS(               XK_9,                      8)

	//合并在一起观看
	//向 左||右 浏览 workspace,一一浏览，包括未使用的workspace
	{ MODKEY,              XK_0,                    view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,    XK_0,                    tag,            {.ui = ~0 } },

	//在最近 2 个workspace 切换 
	{ MODKEY,              XK_i,                    viewtoleft,     {0} },
	{ MODKEY,              XK_o,                    viewtoright,    {0} },
	{ MODKEY,              XK_Tab,                  view,           {0} },

	//解决了，但没完全解决
	{ Mod1Mask|ShiftMask,  XK_l,                    spawn,          {.v = screenLock } },
	//system sleep
	{ MODKEY|ShiftMask,    XK_p,                    spawn,          {.v = suspendcmd } },
	//system exit
    //MODKEY + TAB + j : 也能退出系统，不知道为什么...
	{ MODKEY|ControlMask,  XK_e,                    quit,           {0} },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button1,        togglewin,      {0} },
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

