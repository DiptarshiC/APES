To build BB userspace application:
gcc bb_comms.c bb_logger.c -lpthread -DSPI

To build LKM:
make (inside /BB/src/kernel/)