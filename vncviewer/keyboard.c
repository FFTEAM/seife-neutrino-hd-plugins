#include <X11/keysym.h>

#include "vncviewer.h"
#include "fbgl.h"
#include "overlay.h"
#include "icons.h"
#include "keyboard.h"

int *fbvnc_keymap;
struct hbtn hbtn;

static int keys_sent_map[N_SCANCODE];

static int fbvnc_keymap_zaurus[N_SCANCODE*4] = {
	0, 0, 0, 0,				/* 0 */
	XK_a, XK_A, XK_bar, 0,
	XK_b, XK_B, 0, 0,
	XK_c, XK_C, 0, 0,
	XK_d, XK_D, XK_numbersign, 0,
	XK_e, XK_E, XK_3, 0,	
	XK_f, XK_F, XK_dollar, 0,
	XK_g, XK_G, XK_percent, 0,
	XK_h, XK_H, XK_underscore, 0,
	XK_i, XK_I, XK_8, 0,
	XK_j, XK_J, XK_ampersand, 0,
	XK_k, XK_K, XK_asterisk, 0,
	XK_l, XK_L, XK_parenleft, 0,
	XK_m, XK_M, XK_equal, 0,
	XK_n, XK_N, XK_plus, 0,
	XK_o, XK_O, XK_odiaeresis, XK_Odiaeresis,
	XK_p, XK_P, XK_0, 0,	
	XK_q, XK_Q, XK_1, 0,	
	XK_r, XK_R, XK_4, 0,	
	XK_s, XK_S, XK_at, 0,
	XK_t, XK_T, XK_5, 0,
	XK_u, XK_U, XK_7, 0,
	XK_v, XK_V, 0, 0,
	XK_w, XK_W, XK_2, 0,
	XK_x, XK_X, 0, 0,
	XK_y, XK_Y, XK_6, 0,
	XK_z, XK_Z, 0, 0,			/* 26 */
	XK_Shift_L, 0, 0, 0,
	XK_Return, 0, XK_greater, 0,
	XK_Next /* menu */, 0, 0, 0,
	0 /* Fn = XK_Alt_R */, 0, 0, 0,		/* 30 */
	XK_BackSpace, XK_Delete, 0, 0,
	XK_bar /* Fn-Space */, XK_backslash, 0, 0,
	0, 0, 0, 0,
	XK_Escape /* cancel =34 */, 0, 0, 0,
	XK_Left, 0, 0, 0,
	XK_Up, 0, 0, 0,
	XK_Down, 0, 0, 0,
	XK_Right, 0, 0, 0,
	XK_Return /* OK =39*/, 0, 0, 0,
	0 /* XK_Home */, 0, 0, 0,				/* 40 */
	XK_1, 0, 0, 0,
	XK_2, 0, 0, 0,
	XK_3, 0, 0, 0,
	XK_4, 0, 0, 0,
	XK_5, 0, 0, 0,
	XK_6, 0, 0, 0,
	XK_7, 0, 0, 0,
	XK_8, 0, 0, 0,
	XK_9, 0, 0, 0,
	XK_0, 0, 0, 0,				/* 50 */
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	XK_minus, 0, 0, 0,
	XK_plus, 0, 0, 0,
	XK_Caps_Lock, 0, 0, 0,				/* 60 */
	XK_at, 0, 0, 0,
	XK_question, 0, 0, 0,
	XK_comma, 0, 0, 0,
	XK_period, 0, XK_less, 0,
	XK_Tab, 0, 0, 0,
	XK_bracketleft /* Fn-x */, XK_grave, 0, 0,
	XK_bracketright /* Fn-c */, 0 /* unused */, 0, 0,
	XK_braceright /* Fn-v */, 0 /* doesn't work? */, 0, 0,
	XK_slash, 0, 0, 0,
	XK_apostrophe, 0, 0, 0,		/* 70 */
	XK_semicolon, 0, 0, 0,
	XK_quotedbl, 0, 0, 0,
	XK_colon, 0, 0, 0,
	XK_numbersign, 0, 0, 0,
	XK_dollar, 0, 0, 0,
	XK_percent, 0, 0, 0,
	XK_underscore, 0, 0, 0,
	XK_ampersand, 0, 0, 0,
	XK_asterisk, 0, 0, 0,
	XK_parenleft, 0, 0, 0,				/* 80 */
	XK_Delete, 0, 0, 0,
	XK_braceleft /* Fn-Z */, XK_asciitilde, 0, 0,
	XK_equal, 0, 0, 0,
	XK_parenright, 0, 0, 0,
	XK_asciitilde, 0, 0, 0,
	XK_less, 0, 0, 0,
	XK_greater, 0, 0, 0,
	XK_Control_L /* calendar */, 0, 0, 0,
	XK_Prior /* contacts */, 0, 0, 0,
	XK_Tab /* mail */, 0, 0, 0,				/* 90 */
	0 /* action, center of cursor pad */, 0, 0, 0,
	XK_space, 0, 0, 0,
	0, 0, 0, 0,
	XK_exclam, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,				/* 100 */
	0, 0, 0, 0,
	0, 0, 0, 0,
	XK_Shift_R, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,				/* 110 */
	0, 0, 0, 0,
	XK_Num_Lock, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
};

static struct hbtn
hbtn_zaurus = {
	27, 103, 0,
	88, 89, 29,
	40, 91,
};

static int fbvnc_keymap_c700[N_SCANCODE*4] = {
	0, 0, 0, 0,				/* 0 */
	XK_a, XK_A, XK_adiaeresis, XK_Adiaeresis,
	XK_b, XK_B, XK_underscore, 0,
	XK_c, XK_C, 0, 0,
	XK_d, XK_D, XK_grave, 0,
	XK_e, XK_E, XK_equal, 0,	
	XK_f, XK_F, XK_backslash, 0,
	XK_g, XK_G, XK_semicolon, 0,
	XK_h, XK_H, XK_colon, 0,
	XK_i, XK_I, XK_braceright, 0,
	XK_j, XK_J, XK_asterisk, 0,
	XK_k, XK_K, 0, 0,
	XK_l, XK_L, XK_bar, 0,
	XK_m, XK_M, 0, 0,
	XK_n, XK_N, 0, 0,
	XK_o, XK_O, XK_odiaeresis, XK_Odiaeresis,
	XK_p, XK_P, XK_udiaeresis, XK_Udiaeresis,
		/* should be on "u" key, but that's labeled as brackets */	
	XK_q, XK_Q, 0, 0,	
	XK_r, XK_R, XK_plus, 0,	
	XK_s, XK_S, XK_ssharp, 0,
	XK_t, XK_T, XK_bracketleft, 0,
	XK_u, XK_U, XK_braceleft, 0,
	XK_v, XK_V, 0, 0,
	XK_w, XK_W, XK_asciicircum, 0,
	XK_x, XK_X, 0, 0,
	XK_y, XK_Y, XK_bracketright, 0,
	XK_z, XK_Z, 0, 0,			/* 26 */
	XK_Shift_L, 0, 0, 0,
	XK_Return, 0, XK_greater, 0,
	0 /* menu */, 0, 0, 0,
	0 /* Fn = XK_Alt_R */, 0, 0, 0,		/* 30 */
	XK_BackSpace, 0, XK_Delete, 0,
	XK_bar /* Fn-Space */, XK_backslash, 0, 0,
	0, 0, 0, 0,
	XK_Escape /* cancel =34 */, 0, 0, 0,
	XK_Left, 0, 0, 0,
	XK_Up, 0, 0, 0,
	XK_Down, 0, 0, 0,
	XK_Right, 0, 0, 0,
	XK_Return /* OK =39*/, 0, 0, 0,
	XK_End /* XK_Home */, 0, 0, 0,				/* 40 */
	XK_1, XK_exclam, XK_F1, 0,
	XK_2, XK_quotedbl, XK_F2, 0,
	XK_3, XK_numbersign, XK_F3, 0,
	XK_4, XK_dollar, XK_F4, 0,
	XK_5, XK_percent, XK_F5, 0,
	XK_6, XK_ampersand, XK_F6, 0,
	XK_7, XK_apostrophe, XK_F7, 0,
	XK_8, XK_parenleft, XK_F8, 0,
	XK_9, XK_parenright, XK_F9, 0,
	XK_0, XK_asciitilde, XK_F10, 0,				/* 50 */
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	XK_minus, XK_at, XK_at, 0,
	XK_plus, 0, 0, 0,
	XK_Caps_Lock, 0, 0, 0,				/* 60 */
	XK_at, 0, 0, 0,
	XK_question, 0, 0, 0,
	XK_comma, XK_slash, XK_less, 0,
	XK_period, XK_question, XK_greater, 0,
	XK_Tab, 0, 0, 0,
	XK_bracketleft /* Fn-x */, XK_grave, 0, 0,
	XK_bracketright /* Fn-c */, 0 /* unused */, 0, 0,
	XK_braceright /* Fn-v */, 0 /* doesn't work? */, 0, 0,
	XK_slash, 0, 0, 0,
	XK_apostrophe, 0, 0, 0,		/* 70 */
	XK_semicolon, 0, 0, 0,
	XK_quotedbl, 0, 0, 0,
	XK_colon, 0, 0, 0,
	XK_numbersign, 0, 0, 0,
	XK_dollar, 0, 0, 0,
	XK_percent, 0, 0, 0,
	XK_underscore, 0, 0, 0,
	XK_ampersand, 0, 0, 0,
	XK_asterisk, 0, 0, 0,
	XK_parenleft, 0, 0, 0,				/* 80 */
	XK_Delete, 0, 0, 0,
	XK_braceleft /* Fn-Z */, XK_asciitilde, 0, 0,
	XK_equal, 0, 0, 0,
	XK_parenright, 0, 0, 0,
	XK_asciitilde, 0, 0, 0,
	XK_less, 0, 0, 0,
	XK_greater, 0, 0, 0,
	XK_Home /* calendar */, 0, 0, 0,
	XK_Next /* contacts */, 0, 0, 0,
	XK_Prior /* mail */, 0, 0, 0,				/* 90 */
	0 /* action; no key! */, 0, 0, 0,
	XK_space, 0, 0, 0,
	0, 0, 0, 0,
	XK_exclam, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,				/* 100 */
	0, 0, 0, 0,
	0 /* hingemoved */, 0, 0, 0,
	XK_Shift_R, 0, 0, 0,
	XK_Control_L, 0, 0, 0,
	XK_Control_R, 0, 0, 0,
	XK_Alt_L, 0, 0, 0,
	XK_Alt_R, 0, 0, 0,
	0, 0, 0, 0,
	0 /* Power */, 0, 0, 0,
	0, 0, 0, 0,				/* 110 */
	0, 0, 0, 0,
	XK_Num_Lock, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,				/* 120 */
	XK_Control_L /* =Kana/Hira */, 0, 0, 0,
	XK_Alt_L /* =Kanji */, 0, 0, 0,
	XK_Return, 0, 0, 0,
	XK_Escape, 0, 0, 0,
	XK_Prior, 0, 0, 0,
	XK_Next, 0, 0, 0,
};

static struct hbtn
hbtn_c700 = {
	27, 103, 30,
	88, 89, 90,
	29, 0 /*122*/,
};

static int fbvnc_keymap_ps2_de[N_SCANCODE*4] = {
	0, 0, 0, 0,				/* 0 */
	XK_Escape, 0, 0, 0,
	XK_1, XK_exclam, 0, 0,
	XK_2, XK_quotedbl, XK_twosuperior, 0,
	XK_3, XK_section, XK_threesuperior, 0,
	XK_4, XK_dollar, 0, 0,
	XK_5, XK_percent, 0, 0,
	XK_6, XK_ampersand, 0, 0,
	XK_7, XK_slash, XK_braceleft, 0,
	XK_8, XK_parenleft, XK_bracketleft, 0,
	XK_9, XK_parenright, XK_bracketright, 0,		/* 10 */
	XK_0, XK_equal, XK_braceright, 0,
	XK_ssharp, XK_question, XK_backslash, 0,
	XK_apostrophe, XK_grave, 0, 0,
	XK_BackSpace, 0, 0, 0,
	XK_Tab, 0, 0, 0,	
	XK_q, XK_Q, XK_at, 0,
	XK_w, XK_W, 0, 0,
	XK_e, XK_E, 0, 0,
	XK_r, XK_R, 0, 0,
	XK_t, XK_T, 0, 0,			/* 20 */
	XK_z, XK_Z, 0, 0,
	XK_u, XK_U, 0, 0,
	XK_i, XK_I, 0, 0,
	XK_o, XK_O, 0, 0,
	XK_p, XK_P, 0, 0,	
	XK_udiaeresis, XK_Udiaeresis, 0, 0,
	XK_plus, XK_asterisk, XK_asciitilde, 0,
	XK_Return, 0, 0, 0,
	XK_Control_L, 0, 0, 0,
	XK_a, XK_A, 0, 0,	/* 30 */
	XK_s, XK_S, 0, 0,
	XK_d, XK_D, 0, 0,
	XK_f, XK_F, 0, 0,
	XK_g, XK_G, 0, 0,
	XK_h, XK_H, 0, 0,
	XK_j, XK_J, 0, 0,
	XK_k, XK_K, 0, 0,
	XK_l, XK_L, 0, 0,
	XK_odiaeresis, XK_Odiaeresis, 0, 0,
	XK_adiaeresis, XK_Adiaeresis, 0, 0,	/* 40 */
	XK_asciicircum, XK_degree, 0, 0,
	XK_Shift_L, 0, 0, 0,
	XK_numbersign, XK_apostrophe, 0, 0,
	XK_y, XK_Y, 0, 0,
	XK_x, XK_X, 0, 0,
	XK_c, XK_C, 0, 0,
	XK_v, XK_V, 0, 0,
	XK_b, XK_B, 0, 0,
	XK_n, XK_N, 0, 0,
	XK_m, XK_M, XK_mu, 0,			/* 50 */
	XK_comma, XK_semicolon, 0, 0,
	XK_period, XK_colon, 0, 0,
	XK_minus, XK_underscore, 0, 0,
	XK_Shift_R, 0, 0, 0,
	XK_KP_Multiply, 0, 0, 0,
	XK_Alt_L, 0, 0, 0,
	XK_space, 0, 0, 0,
	XK_Control_L /* Caps_Lock */, 0, 0, 0,
	XK_F1, 0, 0, 0,
	XK_F2, 0, 0, 0,			/* 60 */
	XK_F3, 0, 0, 0,
	XK_F4, 0, 0, 0,
	XK_F5, 0, 0, 0,
	XK_F6, 0, 0, 0,
	XK_F7, 0, 0, 0,
	XK_F8, 0, 0, 0,
	XK_F9, 0, 0, 0,
	XK_F10, 0, 0, 0,
	XK_Num_Lock, 0, 0, 0,
	XK_Scroll_Lock, 0, 0, 0,	/* 70 */
	XK_KP_7, 0, 0, 0,
	XK_KP_8, 0, 0, 0,
	XK_KP_9, 0, 0, 0,
	XK_KP_Subtract, 0, 0, 0,
	XK_KP_4, 0, 0, 0,
	XK_KP_5, 0, 0, 0,
	XK_KP_6, 0, 0, 0,
	XK_KP_Add, 0, 0, 0,
	XK_KP_1, 0, 0, 0,
	XK_KP_2, 0, 0, 0,		/* 80 */
	XK_KP_3, 0, 0, 0,
	XK_KP_0, 0, 0, 0,
	XK_KP_Separator, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	XK_less, XK_greater, XK_bar, 0,
	XK_F11, 0, 0, 0,
	XK_F12, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,				/* 90 */
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	XK_KP_Enter, 0, 0, 0,
	XK_Control_R, 0, 0, 0,
	XK_KP_Divide, 0, 0, 0,
	XK_Print, 0, 0, 0,
	0 /* XK_Alt_R */, 0, 0, 0,		/* 100 */
	XK_Break, 0, 0, 0,
	XK_Home, 0, 0, 0,
	XK_Up, 0, 0, 0,
	XK_Prior, 0, 0, 0,
	XK_Left, 0, 0, 0,
	XK_Right, 0, 0, 0,
	XK_End, 0, 0, 0,
	XK_Down, 0, 0, 0,
	XK_Next, 0, 0, 0,
	XK_Insert, 0, 0, 0,		/* 110 */
	XK_Delete, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	XK_Pause, 0, 0, 0,
};

static int fbvnc_keymap_ps2_us[N_SCANCODE*4] = {
	0, 0, 0, 0,				/* 0 */
	XK_Escape, 0, 0, 0,
	XK_1, XK_exclam, 0, 0,
	XK_2, XK_at, 0, 0,
	XK_3, XK_numbersign, 0, 0,
	XK_4, XK_dollar, 0, 0,
	XK_5, XK_percent, 0, 0,
	XK_6, XK_asciicircum, 0, 0,
	XK_7, XK_ampersand, 0, 0,
	XK_8, XK_asterisk, 0, 0,
	XK_9, XK_parenleft, 0, 0,		/* 10 */
	XK_0, XK_parenright, 0, 0,
	XK_minus, XK_underscore, 0, 0,
	XK_equal, XK_plus, 0, 0,
	XK_BackSpace, 0, 0, 0,
	XK_Tab, 0, 0, 0,	
	XK_q, XK_Q, 0, 0,
	XK_w, XK_W, 0, 0,
	XK_e, XK_E, 0, 0,
	XK_r, XK_R, 0, 0,
	XK_t, XK_T, 0, 0,			/* 20 */
	XK_y, XK_Y, 0, 0,
	XK_u, XK_U, XK_udiaeresis, XK_Udiaeresis,
	XK_i, XK_I, 0, 0,
	XK_o, XK_O, XK_odiaeresis, XK_Odiaeresis,
	XK_p, XK_P, 0, 0,	
	XK_bracketleft, XK_braceleft, 0, 0,
	XK_bracketright, XK_braceright, 0, 0,
	XK_Return, 0, 0, 0,
	XK_Control_L, 0, 0, 0,
	XK_a, XK_A, XK_adiaeresis, XK_Adiaeresis,	/* 30 */
	XK_s, XK_S, XK_ssharp, 0,
	XK_d, XK_D, 0, 0,
	XK_f, XK_F, 0, 0,
	XK_g, XK_G, 0, 0,
	XK_h, XK_H, 0, 0,
	XK_j, XK_J, 0, 0,
	XK_k, XK_K, 0, 0,
	XK_l, XK_L, 0, 0,
	XK_semicolon, XK_colon, 0, 0,
	XK_apostrophe, XK_quotedbl, 0, 0,	/* 40 */
	XK_grave, XK_asciitilde, 0, 0,
	XK_Shift_L, 0, 0, 0,
	XK_backslash, XK_bar, 0, 0,
	XK_z, XK_Z, 0, 0,
	XK_x, XK_X, 0, 0,
	XK_c, XK_C, 0, 0,
	XK_v, XK_V, 0, 0,
	XK_b, XK_B, 0, 0,
	XK_n, XK_N, 0, 0,
	XK_m, XK_M, 0, 0,			/* 50 */
	XK_comma, XK_less, 0, 0,
	XK_period, XK_greater, 0, 0,
	XK_slash, XK_question, 0, 0,
	XK_Shift_R, 0, 0, 0,
	XK_KP_Multiply, 0, 0, 0,
	XK_Alt_L, 0, 0, 0,
	XK_space, 0, 0, 0,
	XK_Control_L /* Caps_Lock */, 0, 0, 0,
	XK_F1, 0, 0, 0,
	XK_F2, 0, 0, 0,			/* 60 */
	XK_F3, 0, 0, 0,
	XK_F4, 0, 0, 0,
	XK_F5, 0, 0, 0,
	XK_F6, 0, 0, 0,
	XK_F7, 0, 0, 0,
	XK_F8, 0, 0, 0,
	XK_F9, 0, 0, 0,
	XK_F10, 0, 0, 0,
	XK_Num_Lock, 0, 0, 0,
	XK_Scroll_Lock, 0, 0, 0,	/* 70 */
	XK_KP_7, 0, 0, 0,
	XK_KP_8, 0, 0, 0,
	XK_KP_9, 0, 0, 0,
	XK_KP_Subtract, 0, 0, 0,
	XK_KP_4, 0, 0, 0,
	XK_KP_5, 0, 0, 0,
	XK_KP_6, 0, 0, 0,
	XK_KP_Add, 0, 0, 0,
	XK_KP_1, 0, 0, 0,
	XK_KP_2, 0, 0, 0,		/* 80 */
	XK_KP_3, 0, 0, 0,
	XK_KP_0, 0, 0, 0,
	XK_KP_Separator, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	XK_less, XK_greater, XK_bar, 0,
	XK_F11, 0, 0, 0,
	XK_F12, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,				/* 90 */
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	XK_KP_Enter, 0, 0, 0,
	XK_Control_R, 0, 0, 0,
	XK_KP_Divide, 0, 0, 0,
	XK_Print, 0, 0, 0,
	0 /* XK_Alt_R */, 0, 0, 0,		/* 100 */
	XK_Break, 0, 0, 0,
	XK_Home, 0, 0, 0,
	XK_Up, 0, 0, 0,
	XK_Prior, 0, 0, 0,
	XK_Left, 0, 0, 0,
	XK_Right, 0, 0, 0,
	XK_End, 0, 0, 0,
	XK_Down, 0, 0, 0,
	XK_Next, 0, 0, 0,
	XK_Insert, 0, 0, 0,		/* 110 */
	XK_Delete, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	XK_Pause, 0, 0, 0,
};

static struct hbtn
hbtn_ps2 = {
	42, 54, 100,
	-1, -1, -1,
	119, 97,
};

static int fbvnc_keymap_ipaq[N_SCANCODE*4] = {
	0, 0, 0, 0,				/* 0 */
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,				/* 10 */
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,				/* 20 */
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,				/* 30 */
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,				/* 40 */
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,				/* 50 */
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,				/* 60 */
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,				/* 70 */
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,				/* 80 */
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,				/* 90 */
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0 /* XK_Return, Action */, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,				/* 100 */
	0, 0, 0, 0,
	0, 0, 0, 0,
	XK_Up, 0, 0, 0,
	0, 0, 0, 0,
	XK_Left, 0, 0, 0,
	XK_Right, 0, 0, 0,
	0, 0, 0, 0,
	XK_Down, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,				/* 110 */
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	XK_Return /* Record */, 0, 0, 0,			/* 120 */
	0, 0, 0, 0,
	XK_Tab /* Calendar */, 0, 0, 0,
	XK_Prior /* Contacts */, 0, 0, 0,
	XK_Next /* Q */, 0, 0, 0,
	0 /* Start */, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
};

static struct hbtn
hbtn_ipaq = {
	-1, -1, -1,
	122, 123, 124,
	125, 96,
};

void
init_keyboard(void)
{
	if (!strcmp(hwType, "zaurus")) {
		fbvnc_keymap = fbvnc_keymap_zaurus;
		hbtn = hbtn_zaurus;
	} else if (!strcmp(hwType, "ipaq")) {
		fbvnc_keymap = fbvnc_keymap_ipaq;
		hbtn = hbtn_ipaq;
	} else if (!strcmp(hwType, "ps2de")) {
		fbvnc_keymap = fbvnc_keymap_ps2_de;
		hbtn = hbtn_ps2;
	} else if (!strcmp(hwType, "ps2us") || !strcmp(hwType, "ps2")) {
		fbvnc_keymap = fbvnc_keymap_ps2_us;
		hbtn = hbtn_ps2;
	} else if (!strcmp(hwType, "c700") || !strcmp(hwType, "C700")) {
		fbvnc_keymap = fbvnc_keymap_c700;
		hbtn = hbtn_c700;
	} else {
		cleanup_and_exit("unknown hardware type", EXIT_ERROR);
	}
}

int rep_key = 0;

char btn_state[N_SCANCODE];
int fn_action = 0;
int pan_toggle_count = 0;

static bool
repeating_key(int key)
{
	if (key == XK_Shift_L
	 || key == XK_Shift_R
	 || key == XK_Control_L 
	 || key == XK_Control_R
	 || key == XK_Alt_L 
	 || key == XK_Alt_R
	 || key == XK_Scroll_Lock 
	 || key == XK_Num_Lock
	 || key == XK_Caps_Lock
	) {
	 	return 0;
	} else {
	 	return 1;
	}
}

bool
key_special_action(int key)
{
	if (debug) {
		fprintf(stderr, "fn_action=%d, btn_state[hbtn.action]=%d\n",
			fn_action, btn_state[hbtn.action]);
	}
	if (! (fn_action || btn_state[hbtn.action])) return 0;

	if (key==XK_bracketleft || key==XK_Next) {
		ev_zoom_out(0, 0);
	} else if (key==XK_bracketright || key==XK_Prior) {
		ev_zoom_in(0, 0);
	} else if (key==XK_Escape) {
		system("zsuspend");
	} else if (key==XK_Return) {
		toggle_light();
	} else if (key==XK_Tab || key==XK_End) {
		flip_orientation();
	} else if ((fn_action && key==XK_Home) || (!fn_action && key==XK_Control_L)) {
		set_mouse_state((mouse_multibutton_mode+1) % 3);
	} else if (key==XK_f) {
		extern void refresh_framerate(void);
		refresh_framerate();
	} else if (key==XK_Down) {
		grid_pan(0, 1);
	} else if (key==XK_Up) {
		grid_pan(0, -1);
	} else if (key==XK_Left) {
		grid_pan(-1, 0);
	} else if (key==XK_Right) {
		grid_pan(1, 0);
	} else if (key==XK_Q) {
		cleanup_and_exit("Bye.", EXIT_OK);
	} else if (key==XK_z && img_saved) {
		show_pnm_image();
	}
	return 1;
}

static int
landscape_cursor_remap (int key) {
	bool landscape = global_framebuffer.p_landscape;

	if (landscape_is_native) {
		if (landscape) return key;

		if (key == XK_Up) {
			key = XK_Left;
		} else if (key == XK_Left) {
			key = XK_Down;
		} else if (key == XK_Right) {
			key = XK_Up;
		} else if (key == XK_Down) {
			key = XK_Right;
		}
	} else {
		if (!landscape) return key;

		if (key == XK_Up) {
			key = XK_Right;
		} else if (key == XK_Left) {
			key = XK_Up;
		} else if (key == XK_Right) {
			key = XK_Down;
		} else if (key == XK_Down) {
			key = XK_Left;
		}
	}
	return key;
}

int  /* keysym */
key_map(int hwkey)
{
	int pos;
	int key;

	if (hwkey < 0 || hwkey >= N_SCANCODE) return 0;

	pos=hwkey*4;
	if (!fbvnc_keymap[pos]) return 0;
	
	if (btn_state[hbtn.altgr]) {
		if (fbvnc_keymap[pos+2]) {
			pos+=2;
		} else {
			fn_action = 1;
		}
	}
	if (btn_state[hbtn.shift_l] || btn_state[hbtn.shift_r]) {
		if (fbvnc_keymap[pos+1]) pos++;
	}
	key = fbvnc_keymap[pos];
	key = landscape_cursor_remap(key);

	return key;
}

void
key_press(int hwkey) {
	int key;

	fn_action = 0;
	key = key_map(hwkey);
	if (!key) return;

	if (key_special_action(key)) return;

	if (repeating_key(key)) {
		rep_key = key;
		schedule_add(sched, kbdDelay, FBVNC_EVENT_KEYREPEAT);
	}

	keys_sent_map[hwkey] = key;
	if (debug) fprintf(stderr, "key_press: hwkey=%d, keysym=%d\n", hwkey, key);
	SendKeyEvent(key, 1);
}

void
key_release(int hwkey)
{
	int key;

	if (fn_action) return;
	if (hwkey < 0 || hwkey >= N_SCANCODE) return;

	key = keys_sent_map[hwkey];
	if (debug) fprintf(stderr, "key_release: hwkey=%d, keysym=%d\n", hwkey, key);
	SendKeyEvent(key, 0);

	pan_toggle_count = 0;
}
