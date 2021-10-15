# 1 "../sncProgram.st"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "../sncProgram.st"
# 1 "./../sncExample.stt" 1
program sncExample
double v;
assign v to "{SYSTEM}:master_control_switch";
monitor v;
%{
void loadBoard(int i) {
    printf("in the loadBoard function\n");
}
}%

ss ss1 {
    state init {
        entry {
            v = 0 ;
            pvPut(v);
     printf("sncExample: initializing\n");
        }
 when (delay(10)) {
     printf("sncExample: Startup delay over\n");
            v=1;
            pvPut(v);
 } state waiting
    }
    state low {
 when (v > 5.0) {
     printf("sncExample: Changing to high\n");
 } state high
    }
    state high {
 when (v <= 5.0) {
     printf("sncExample: Changing to low\n");
 } state low
    }
    state waiting {
        entry {
     printf("waiting\n");
        }
 when (v == 6) {
  printf("load all configurations\n");
        } state loading_all
        when (v == 7) {
      printf("load board 1 configurations\n");
        } state loading_board1
        when (v == 8) {
      printf("load board 2 configurations\n");
        } state loading_board2
        when (v == 9) {
      printf("load board 3 configurations\n");
        } state loading_board3
    }
    state loading_all {
        entry {
            loadBoard(1);
            loadBoard(2);
            loadBoard(3);
            v = 1;
            pvPut(v);
        }
        when (1) {
        } state waiting
    }
    state loading_board1 {
        entry {
            loadBoard(1);
            v = 1;
            pvPut(v);
        }
        when (1) {
        } state waiting
    }

    state loading_board2 {
        entry {
            loadBoard(2);
            v = 1;
            pvPut(v);
        }
        when (1) {
        } state waiting
    }

    state loading_board3 {
        entry {
            loadBoard(3);
            v = 1;
            pvPut(v);
        }
        when (1) {
        } state waiting
    }

}
# 1 "../sncProgram.st" 2
