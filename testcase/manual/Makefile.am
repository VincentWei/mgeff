noinst_PROGRAMS=window_anmation test-effector test-effector-api
#noinst_PROGRAMS=window_anmation test-iconflow test-ctrlfrctmotion

LIBS = @LIBS@ 
MINIGUI_LIBS = @MGEFF_LIBS@ @MGPLUS_LIBS@ -lstdc++

window_anmation_SOURCES=window_animation.c
window_anmation_LDADD=$(MINIGUI_LIBS)

#test_iconflow_SOURCES=test-iconflow.c
#test_iconflow_LDADD=-lmgeff $(MINIGUI_LIBS)

test_effector_SOURCES=test-effector.c
test_effector_LDADD=$(MINIGUI_LIBS)

#test_ctrlfrctmotion_SOURCES=test-ctrlfrctmotion.c
#test_ctrlfrctmotion_LDADD=$(MINIGUI_LIBS)

test_effector_api_SOURCES=effector-api-test.cpp
test_effector_api_LDADD=$(MINIGUI_LIBS) -lgtest
