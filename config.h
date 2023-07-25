/* See LICENSE file for copyright and license details. */

#define XF86MonBrightnessUp 0x1008ff02
#define XF86MonBrightnessDown 0x1008ff03
#define XF86AudioLowerVolume 0x1008ff11
#define XF86AudioMute 0x1008ff12
#define XF86AudioRaiseVolume 0x1008ff13
#define XF86Display 0x1008ff59

// scratchpad
static Client *scratchpad;

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int gappx     = 10;		/* gap pixel between windows*/
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating	= 0;		/* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
//static const char *fonts[]          = { "NotoSans-Medium:size=11", "NotoSansMonoNerdFont:size=12" };
static const char *fonts[]          = { "monospace:pixelsize=14"/*, "NotoSansMonoNerdFont:pixelsize=16"*/};
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#212121";
static const char col_gray2[]       = "#404040";
static const char col_white[]       = "#ffffff";
static const char col_red[]       = "#ff0000";
static const char col_cyan[]        = "#009eff";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_white, col_gray1, col_gray2 },
	[SchemeSel]  = { col_white, col_gray2,  col_cyan  },
	[SchemeUrg]  = { col_red, col_gray1,  col_gray2  },
	[SchemeBlue] = { col_cyan, col_cyan, col_cyan },
	[SchemeGray] = { col_gray1, col_gray1, col_gray1 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	//{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	//{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "st-256color", NULL, NULL,		   0,		  0,		  1,		   0,		 -1 },
	{ "floating", NULL, NULL,			0,			1,			0,			0,			-1 },
	//{ "URxvt", NULL, NULL,		   0,		  0,		  1,		   0		 -1 },
	//{ "Termite", NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "",      tile },    /* first entry is default */
	{ "",      NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALT Mod1Mask
#define TAGKEYS(KEY,TAG) \
	/*{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },*/

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run -i", NULL };
static const char *termcmd[]  = { "alacritty", NULL };

static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", "-e", "zsh", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ KeyPress, MODKEY,			    XK_Delete, 	   spawn,          SHCMD("setxkbmap fi") },
	{ KeyPress, MODKEY,			    XK_Insert, 	   spawn,          SHCMD("xmodmap ~/.config/keyboard_layouts/layout_rainworld") },
	{ KeyPress, MODKEY|ControlMask,			    XK_j, 	   spawn,          SHCMD("dmenu_run -i -wd 410") },
	{ KeyPress, MODKEY|ControlMask,			    XK_k, 	   spawn,          SHCMD("kill_process") },
	{ KeyPress, MODKEY|ControlMask,			    XK_u, 	   spawn,          SHCMD("dmenu_steam") },
	{ KeyPress, MODKEY|ControlMask,			    XK_i, 	   spawn,          SHCMD("dmenu_wifi") },
	{ KeyPress, MODKEY|ControlMask,			    XK_m, 	   spawn,          SHCMD("dmenu_screens") },
	{ KeyPress, MODKEY|ControlMask,			    XK_l, 	   spawn,          SHCMD("choose_wallpaper") },
	{ KeyPress, MODKEY|ShiftMask, 			    XK_j, 	   spawn,          SHCMD("st -e zsh") },
	{ KeyPress, MODKEY|ShiftMask, 		        XK_k, 	   spawn,          SHCMD("exec env START_LF=true st -e zsh && { eval \"$START_LF\"; unset START_LF; }") }, 
	{ KeyPress, MODKEY|ShiftMask, 		        XK_l, 	   spawn,          SHCMD("firefox") },
	{ KeyPress, MODKEY|ShiftMask, 		        XK_odiaeresis, spawn,          SHCMD("Discord") },
	{ KeyPress, MODKEY|ShiftMask, 		        XK_u, spawn,          SHCMD("steam") },
	{ KeyPress, MODKEY,							XK_y,	togglescratch,	{.v = scratchpadcmd } },
	{ KeyPress, MODKEY|ALT, 				        XK_Return, spawn,          SHCMD("flameshot gui") },
	{ KeyPress, MODKEY,		            XK_Return, spawn,          SHCMD("scrot ~/Pictures/screenshots/%Y-%m-%d-%T.png") },
	{ KeyPress, MODKEY|ALT, 				    XK_u, 	   spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5% \n pkill -RTMIN+1 dwmblocks") },
	{ KeyPress, MODKEY|ALT, 				    XK_i, 	   spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5% \n pkill -RTMIN+1 dwmblocks") },
	{ KeyPress, 0, 				    			XF86AudioLowerVolume, 	   spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5% \n pkill -RTMIN+1 dwmblocks") },
	{ KeyPress, 0, 				    			XF86AudioRaiseVolume, 	   spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5% \n pkill -RTMIN+1 dwmblocks") },
	{ KeyPress, MODKEY|ALT, 				    XK_o, 	   spawn,          SHCMD("toggle_mute") },
//	{ KeyPress, MODKEY|ALT, 				    XK_p, 	   spawn,          SHCMD("xbacklight -inc 5") },
	{ KeyPress, 0,								XF86MonBrightnessUp, spawn,		SHCMD("pkill -RTMIN+6 dwmblocks")},
	{ KeyPress, 0,								XF86MonBrightnessDown, spawn,		SHCMD("pkill -RTMIN+6 dwmblocks")},
	{ KeyRelease, 0,								XF86MonBrightnessUp, spawn,		SHCMD("pkill -RTMIN+6 dwmblocks")},
	{ KeyRelease, 0,								XF86MonBrightnessDown, spawn,		SHCMD("pkill -RTMIN+6 dwmblocks")},
	{ KeyPress, 0,								XF86Display, spawn,		SHCMD("dmenu_screens")},
	{ KeyPress, MODKEY, 				        XK_e, 	   spawn,          SHCMD("power_menu") },
	{ KeyPress, 0,							XK_Caps_Lock,	   spawn,  SHCMD("pkill -RTMIN+5 dwmblocks")},
	{ KeyRelease, 0,							XK_Caps_Lock,	   spawn,  SHCMD("pkill -RTMIN+5 dwmblocks")},
	{ KeyPress, 0,							XF86AudioMute,		spawn,	SHCMD("toggle_mute")},
//	{ KeyPress, MODKEY,                       XK_b,      togglebar,      {0} },
	{ KeyPress, MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ KeyPress, MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ KeyPress, MODKEY,						XK_n,	   focussel,	   {0} },
	//{ MODKEY|ALT,					XK_n,	   movesel,		   {0} },
	{ KeyPress, MODKEY|ALT,  		            XK_j,      movestack,      {.i = +1 } },
	{ KeyPress, MODKEY|ALT,		            XK_k,      movestack,      {.i = -1 } },
	{ KeyPress, MODKEY|ALT,                   XK_h,      incnmasterfocus,     {.i = +1} },
	{ KeyPress, MODKEY|ALT,                   XK_l,      incnmasterfocus,     {.i = -1} },
	{ KeyPress, MODKEY,	             		XK_u,      setcfacts,      {0} },
	{ KeyPress, MODKEY,	             		XK_i,      setcfact,       {.f = -0.25} },
	{ KeyPress, MODKEY,             			XK_o,      setcfact,       {.f = +0.25} },
	{ KeyPress, MODKEY,             			XK_p,      setcfact,       {.f =  0.00} },
	{ KeyPress, MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ KeyPress, MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
//	{ KeyPress, MODKEY,						XK_q,	   test,		   {0} },
//	{ KeyPress, MODKEY,						XK_w,	   restorewin,	   {0} },
	{ KeyPress, MODKEY,						XK_comma,	   toggletile,     {0} },
	{ KeyPress, MODKEY,						XK_period,	   togglestacktile, {0}},
//	{ KeyPress, MODKEY,                       XK_Return, zoom,           {0} },
//	{ KeyPress, MODKEY,                       XK_Tab,    view,           {0} },
	{ KeyPress, MODKEY,      			        XK_r,      killclient,     {0} }, 
//	{ KeyPress, MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} }, 
//	{ KeyPress, MODKEY,                       XK_r,      setlayout,      {.v = &layouts[1]} }, 
//	{ KeyPress, MODKEY,                       XK_BackSpace,      setlayout,      {.v = &layouts[2]} }, 
	{ KeyPress, MODKEY,                       XK_m,      toggletab,      {0} }, 
	{ KeyPress, MODKEY,                       XK_space,  togglefullscr,  {0} }, 
//	{ KeyPress, MODKEY,						XK_BackSpace,  test,	   {0} },
//	{ KeyPress, MODKEY,                       XK_space,  setlayout,      {0} }, 
//	{ KeyPress, MODKEY|ShiftMask,             XK_space,  togglefloating, {0} }, 
//	{ KeyPress, MODKEY,                       XK_0,      view,           {.ui = ~0 } }, 
//	{ KeyPress, MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } }, 
	{ KeyPress, MODKEY,                       XK_odiaeresis,  focusmon,  {.i = +1 } }, 
	{ KeyPress, MODKEY|ALT,		            XK_odiaeresis,  tagmon,    {.i = +1 } }, 
	{ KeyPress, MODKEY,						XK_d,	   viewnext,	   {.i = +1} },		
	{ KeyPress, MODKEY,						XK_s,	   viewnext,	   {.i = -1} },		
	{ KeyPress, MODKEY,						XK_v,	   tagnext,	       {.i = +1} },		
	{ KeyPress, MODKEY,						XK_c,	   tagnext,	       {.i = -1} },
	{ KeyPress, MODKEY|ShiftMask,				XK_e,	   quit,	       {0} }, };
/* button definitions */ /* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */ static Button buttons[] = { 
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
