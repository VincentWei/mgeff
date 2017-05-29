# This is a sample inc file. if you don't want to run 
# with this inc file. Please modify the ".inc" suffix

URI="svn+ssh://devsrv/home/projects/svn/mgeff/trunk"
PREFIX="$PWD/target"
CONFIGURE_OPTS="--prefix=${PREFIX}  --enable-mgeffctrl=no"
MG_CFG_PATH="${PREFIX}/etc"
MG_RES_PATH="/usr/local/share/minigui/res"
LD_LIBRARY_PATH="${PREFIX}/lib"

