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
#ifndef _ANIMATION_INT_EX_H
#define _ANIMATION_INT_EX_H

extern void test_animation_createex();
extern void test_animation_delete();
extern void test_animation_stop(int keepalive);
extern void test_animation_pauseresume();
#if 0
extern void test_animation_restart();
#endif
extern void test_animation_setgetproperty();
extern void test_animation_setdirection(int keepalive);
extern void test_animation_setduration();
extern void test_animation_setgetvalue();
extern void test_animation_setcurveex();
extern void test_animation_syncrun(int keepalive);
extern void test_animation_asyncrun(int keepalive);
extern void test_animation_runtime ();
extern void test_animation_multiasyncrun();
#if 0
extern void test_animation_loopcount();
extern void test_animation_direction();
#endif
#endif

