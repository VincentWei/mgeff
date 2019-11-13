/*
 *   This file is part of mGEff, a component for MiniGUI.
 * 
 *   Copyright (C) 2008~2018, Beijing FMSoft Technologies Co., Ltd.
 * 
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 *   Or,
 * 
 *   As this program is a library, any link to this program must follow
 *   GNU General Public License version 3 (GPLv3). If you cannot accept
 *   GPLv3, you need to be licensed from FMSoft.
 * 
 *   If you have got a commercial license of this program, please use it
 *   under the terms and conditions of the commercial license.
 * 
 *   For more information about the commercial license, please refer to
 *   <http://www.minigui.com/blog/minigui-licensing-policy/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgeff/mgeff-control.h>

#define NUM 12
#define IDC_ICONFLOW 100
#define IDC_LEFT     110
#define IDC_RIGHT    120
#define IDC_CLICK    130
#define TIMER_LEFT   500
#define TIMER_RIGHT  501

static BOOL test_additem(mDialogBox *self);
static BITMAP icon_demos[NUM];
static HITEM icon_items[NUM];
static int last_index;
static HWND btn_hwnds[2];

static const char *icon_files[12] = {
    "res/acroread.png",
    "res/gimp.png",
    "res/icons.png",
    "res/kpilot.png",
    "res/looknfeel.png",
    "res/multimedia.png",
    "res/package_games.png",
    "res/realplayer.png",
    "res/tux.png",
    "res/usb.png",
    "res/xemacs.png",
    "res/xmms.png"
};

static void iconflow_init(mDialogBox *self)
{
    int i;
    /* Load bitmap */
    for (i = 0; i < NUM; i++) {
        LoadBitmap(HDC_SCREEN, &icon_demos[i], icon_files[i]);
    }
}

static BOOL test_seticonsize(mDialogBox *self)
{
#define W 100
#define H 100
    HWND icon_hwnd;
    mIconFlow *iconflowobj;
    NCS_ICONFLOW_ITEMINFO info;

    icon_hwnd = GetDlgItem(self->hwnd, IDC_ICONFLOW);
    iconflowobj = (mIconFlow *)ncsObjFromHandle(icon_hwnd);
    if (!iconflowobj) {
        return FALSE;
    }
    
    _c(iconflowobj)->setIconSize(iconflowobj, W, H);
    int w = _c(iconflowobj)->getProperty(iconflowobj, NCSP_ICONFLOW_DEFICONWIDTH);
    int h = _c(iconflowobj)->getProperty(iconflowobj, NCSP_ICONFLOW_DEFICONHEIGHT);
    assert(w == W);
    assert(h == H);
}

static BOOL test_additem(mDialogBox *self)
{
    int i;
    HWND icon_hwnd;
    mIconFlow *iconflowobj;
    NCS_ICONFLOW_ITEMINFO info;

    icon_hwnd = GetDlgItem(self->hwnd, IDC_ICONFLOW);
    iconflowobj = (mIconFlow *)ncsObjFromHandle(icon_hwnd);
    if (!iconflowobj) {
        return FALSE;
    }

    _c(iconflowobj)->setIconSize(iconflowobj, 100, 100);
    _c(iconflowobj)->setCurSel(iconflowobj, 0);
    last_index = 0;
    
    /* Add items */
    for (i = 0; i < NUM; i++) {
        memset(&info, 0, sizeof(NCS_ICONFLOW_ITEMINFO));
        info.index = i;
        info.bmp = &icon_demos[i];
        info.label = NULL;
        info.addData = 0;
        icon_items[i] = _c(iconflowobj)->addItem(iconflowobj, &info, NULL);
    }
    
    /* Test list if right */
    for (i = 0; i < NUM; i++) {
        int prev_index;
        if (i == 0) {
            prev_index = NUM -1;
        } else {
            prev_index = i - 1;
        }
        //HITEM item_tmp = _c(iconflowobj)->getPrevItemLoop(iconflowobj, icon_items[i]);
        //assert(item_tmp == icon_items[prev_index]);
    }

    for (i = 0; i < NUM; i++) {
        int next_index;
        if (i == (NUM -1)) {
            next_index = 0;
        } else {
            next_index = i + 1;
        }
        //HITEM item_tmp = _c(iconflowobj)->getNextItemLoop(iconflowobj, icon_items[i]);
        //assert(item_tmp == icon_items[next_index]);
    }

    return TRUE;
}

static int get_current_index(mWidget *self)
{
    mIconFlow *iconflowobj = (mIconFlow *)self;
    if (!iconflowobj) {
        return -1;
    }

    HITEM item = _c(iconflowobj)->getHilight(iconflowobj);
    int current_index = _c(iconflowobj)->indexOf(iconflowobj, item);

    return current_index;
}

static void test_iconflowevent(mWidget *self, int id, int nc, DWORD add_data)
{
    HITEM hitem = (HITEM)add_data;
    int i;

    if ((nc != NCSN_ICONFLOW_CLICKED) && (nc != NCSN_ICONFLOW_ENTER)) {
        return;
    }
    for (i = 0; i < NUM; i++) {
        if (hitem == icon_items[i]) {
            break;
        }
    }
    int index = get_current_index(self);
    printf("current_index = %d, i = %d\n", index +1, i+1);
    assert(i == index);
}

static NCS_EVENT_HANDLER iconflow_handlers[] = {
    NCS_MAP_NOTIFY(NCSN_ICONFLOW_CLICKED, test_iconflowevent),
    NCS_MAP_NOTIFY(NCSN_ICONFLOW_ENTER, test_iconflowevent),
	{0, NULL }
};

static int timer_flag = 0;

/* Simulate left/right/enter keypress */ 
static void btn_timer(mWidget *self, int id, int nc, DWORD add_data)
{
    if (timer_flag >= 2* NUM) {
        KillTimer(self->hwnd, id);
        timer_flag = 0;
        return;
    }
    
    mIconFlow *iconflowobj = 
        (mIconFlow *)ncsGetChildObj(GetParent(self->hwnd), IDC_ICONFLOW);
    
    switch (id)
    {
        case TIMER_LEFT:
            {
                if (timer_flag % 2 == 0) {
                    SendNotifyMessage(iconflowobj->hwnd, MSG_KEYDOWN, SCANCODE_CURSORBLOCKLEFT, 0);
                    SendNotifyMessage(iconflowobj->hwnd, MSG_KEYUP, SCANCODE_CURSORBLOCKLEFT, 0);
                } else {
                    SendNotifyMessage(iconflowobj->hwnd, MSG_KEYDOWN, SCANCODE_ENTER, 0);
                    SendNotifyMessage(iconflowobj->hwnd, MSG_KEYUP, SCANCODE_ENTER, 0);
                }
            }
            break;
        case TIMER_RIGHT:
            {
                if (timer_flag % 2 == 0) {
                    SendNotifyMessage(iconflowobj->hwnd, MSG_KEYDOWN, SCANCODE_CURSORBLOCKRIGHT, 0);
                    SendNotifyMessage(iconflowobj->hwnd, MSG_KEYUP, SCANCODE_CURSORBLOCKRIGHT, 0);
                } else {
                    SendNotifyMessage(iconflowobj->hwnd, MSG_KEYDOWN, SCANCODE_ENTER, 0);
                    SendNotifyMessage(iconflowobj->hwnd, MSG_KEYUP, SCANCODE_ENTER, 0);
                }
            }
            break;
        default:
            break;
    }
    timer_flag++;
}

static void btn_notify(mWidget *self, int id, int nc, DWORD add_data)
{
    int i;
    mIconFlow *iconflowobj = 
        (mIconFlow *)ncsGetChildObj(GetParent(self->hwnd), IDC_ICONFLOW);

    if (!iconflowobj)
        return;

    switch (id)
    {
        case IDC_LEFT:
            {
                static int left_flag = 1;
                if (left_flag) {
                    btn_hwnds[0] = self->hwnd;
                    left_flag = 0;
                }
                if (btn_hwnds[1]) {
                    KillTimer(btn_hwnds[1], TIMER_RIGHT);
                    timer_flag = 0;
                }
                SetTimer(self->hwnd, TIMER_LEFT, 50);
            }
            break;
        case IDC_RIGHT:
            {
                static int right_flag = 1;
                if (right_flag) {
                    btn_hwnds[1] = self->hwnd;
                    right_flag = 0;
                }
                if (btn_hwnds[0]) {
                    KillTimer(btn_hwnds[0], TIMER_LEFT);
                    timer_flag = 0;
                }
                SetTimer(self->hwnd, TIMER_RIGHT, 50);
            }
            break;
        /* Simulate mouse click event */ 
        case IDC_CLICK:
            {
                if (btn_hwnds[0]) {
                    KillTimer(btn_hwnds[0], TIMER_LEFT);
                    timer_flag = 0;
                }
                if (btn_hwnds[1]) {
                    KillTimer(btn_hwnds[1], TIMER_RIGHT);
                    timer_flag = 0;
                }
                DWORD x = iconflowobj->visWidth /2;
                DWORD y = iconflowobj->visHeight /2;
                DWORD lparam = (DWORD)((y << 16) | x);
                SendNotifyMessage(iconflowobj->hwnd, MSG_LBUTTONDOWN, 0, lparam);
                SendNotifyMessage(iconflowobj->hwnd, MSG_LBUTTONUP, 0, lparam);
            }
            break;
        default:
            break;
    }
}

static NCS_EVENT_HANDLER btn_handlers[] = {
    NCS_MAP_NOTIFY(NCSN_BUTTON_PUSHED, btn_notify),
    {MSG_TIMER, btn_timer},
	{0, NULL }
};

static NCS_WND_TEMPLATE _ctrl_tmpl[] = {
    {
        NCSCTRL_ICONFLOW, 
        IDC_ICONFLOW,
        15, 10, 600, 200,
        WS_BORDER | WS_CHILD | WS_VISIBLE | NCSS_NOTIFY | NCSS_ICONFLOW_LOOP,
        WS_EX_NONE,
        "",
        NULL,
        NULL,
        iconflow_handlers,
        NULL,
        0,
        0
    },
    {
        NCSCTRL_BUTTON,
        IDC_LEFT,
        160, 220, 80, 30,
        WS_VISIBLE | NCSS_NOTIFY,
        WS_EX_NONE,
        "left",
        NULL,
        NULL,
        btn_handlers,
        NULL,
        0,
        0
    },
    {
        NCSCTRL_BUTTON,
        IDC_RIGHT,
        260, 220, 80, 30,
        WS_VISIBLE | NCSS_NOTIFY,
        WS_EX_NONE,
        "right",
        NULL,
        NULL,
        btn_handlers,
        NULL,
        0,
        0
    },
    {
        NCSCTRL_BUTTON,
        IDC_CLICK,
        360, 220, 80, 30,
        WS_VISIBLE | NCSS_NOTIFY,
        WS_EX_NONE,
        "click",
        NULL,
        NULL,
        btn_handlers,
        NULL,
        0,
        0
    }
};

static NCS_MNWND_TEMPLATE mainwnd_tmpl = {
	NCSCTRL_DIALOGBOX, 
	7,
	0, 0, 640, 300,
	WS_CAPTION | WS_BORDER | WS_VISIBLE,
	WS_EX_NONE,
    "IconView Demo",
	NULL,
	NULL,
	NULL,
	_ctrl_tmpl,
	sizeof(_ctrl_tmpl) / sizeof(_ctrl_tmpl[0]),
	0,
	0, 0,
};

int MiniGUIMain(int argc, const char *argv[])
{
	ncsInitialize();
    MGNCS_REGISTER_COMPONENT(mAnimation);
    MGNCS_REGISTER_COMPONENT_EX(mIconFlow, WS_VSCROLL,
            WS_EX_NONE, IDC_ARROW, NCS_BGC_WINDOW);
	mDialogBox* mydlg = (mDialogBox *)ncsCreateMainWindowIndirect 
                                (&mainwnd_tmpl, HWND_DESKTOP);

    iconflow_init(mydlg);
    test_additem(mydlg);
    test_seticonsize(mydlg);
	_c(mydlg)->doModal(mydlg, TRUE);

	MainWindowThreadCleanup(mydlg->hwnd);
	ncsUninitialize();

	return 0;
}
